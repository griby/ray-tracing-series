// Ray Tracing in One Weekend Book Series by Peter Shirley

#include <iostream>
#include <fstream>
#include <limits>
#include <random>

#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "hitableList.h"
#include "camera.h"

using namespace rts;


#define DETERMINISTIC_EXECUTION 1
#define NORMAL_MAP_COLOR 0

const std::string IMAGE_FILE_PATH = "output/image.ppm";

// Image resolution
const int IMAGE_WIDTH = 200;
const int IMAGE_HEIGHT = 100;
const float IMAGE_ASPECT_RATIO = static_cast<float>(IMAGE_WIDTH) / IMAGE_HEIGHT;

// Sampling per pixel
const int SAMPLES_PER_PIXEL = 100;

// The shortest and longest ray length
const float T_MIN = 0.001f;
const float T_MAX = std::numeric_limits<float>::max();


// Initialize the random number generator
#if DETERMINISTIC_EXECUTION
    std::mt19937 gen;           // initialize the mersenne twister engine without a seed so that it falls back on a default constant
#else
    std::random_device rd;      // create a random device to seed the pseudo-random generator
    std::mt19937 gen(rd());     // initialize the mersenne twister engine with a random seed (we could also use the clock)
#endif
    std::uniform_real_distribution<float> dist(0.f, 1.f); // distribute the results in [0, 1)


// Return a random float in [0, 1)
inline float getRandom()
{
    return dist(gen);
}

vec3 randomPointInUnitSphere()
{
    vec3 p;
    do
    {
        // Generate a random point in a unit cube ie its components fall between -1 and +1
        p = 2.f * vec3(getRandom(), getRandom(), getRandom()) - vec3(1.f, 1.f, 1.f);

        // Until we find one that is contained in the unit sphere
    } while (p.squaredLength() >= 1.f);

    return p;
}

template <int N>
vec3 color(const Ray& r, const HitableList<N>& world)
{
    // Check if the ray hits a sphere at the center of the screen
    HitRecord rec;
    if (world.hit(r, T_MIN, T_MAX, rec))
    {
#if NORMAL_MAP_COLOR
        // The normal is a unit vector ie its components fall between -1 and +1
        // map those components between 0 and +1 before returning the value
        return 0.5f * vec3(rec.normal.x() + 1.f, rec.normal.y() + 1.f, rec.normal.z() + 1.f);
#else
        // The ray hit a surface, determine a new target to bounce off the surface
        vec3 target = rec.p + rec.normal + randomPointInUnitSphere();
        return 0.5f * color(Ray(rec.p, target - rec.p), world);
#endif
    }
    else
    {
        // Nothing has been hit, determine the background's color
        vec3 unitDirection = unitVector(r.direction());     // unitDirection Y is between -1 and +1
        float t = 0.5f * (unitDirection.y() + 1.f);         // scale unitDirection Y between 0 and +1

        // Blend between white and blue depending on the direction Y
        return (1.f - t) * vec3(1.f, 1.f, 1.f) + t * vec3(0.5f, 0.7f, 1.f);
    }
}

int main()
{
    std::ofstream imageFile(IMAGE_FILE_PATH);
    if (imageFile.is_open())
    {
        // Write the image file header
        imageFile << "P3" << std::endl;
        imageFile << IMAGE_WIDTH << " " << IMAGE_HEIGHT << std::endl;
        imageFile << "255" << std::endl;

        // Set up the world and camera
        HitableList<2> world;
        world[0] = std::move(std::make_unique<Sphere>(vec3(0.f, 0.f, -1.f), 0.5f));         // sphere at the center of the screen
        world[1] = std::move(std::make_unique<Sphere>(vec3(0.f, -100.5f, -1.f), 100.f));    // sphere representing the ground
        Camera cam(IMAGE_ASPECT_RATIO);

        // Determine each pixel's color from left to right and top to bottom
        for (int j = IMAGE_HEIGHT - 1; j >= 0; --j)
        {
            for (int i = 0; i < IMAGE_WIDTH; ++i)
            {
                vec3 col(0.f, 0.f, 0.f); // the accumulated color

                // Sample multiple times randomly within the current pixel
                for (int s = 0; s < SAMPLES_PER_PIXEL; ++s)
                {
                    float u = float(i + getRandom()) / float(IMAGE_WIDTH);
                    float v = float(j + getRandom()) / float(IMAGE_HEIGHT);
                    Ray r = cam.getRay(u, v);

                    // Accumulate the sample
                    col += color(r, world);
                }

                // Average the color
                col /= static_cast<float>(SAMPLES_PER_PIXEL);

                // Apply a gamma to brighten the color
                col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

                // Scale the color between 0 and 255
                int ir = int(255.99f * col[0]);
                int ig = int(255.99f * col[1]);
                int ib = int(255.99f * col[2]);

                // Output the color to the image file
                imageFile << ir << " " << ig << " " << ib << std::endl;
            }
        }

        imageFile.close();
    }

    return 0;
}
