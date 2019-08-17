// Ray Tracing in One Weekend Book Series by Peter Shirley

#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>

#include "camera.h"
#include "config.h"
#include "hitable.h"
#include "hitableList.h"
#include "random.h"
#include "ray.h"
#include "sphere.h"
#include "vec3.h"

namespace rts // for ray tracing series
{
    // Initialize a global random value generator
    Random random;

    vec3 randomPointInUnitSphere()
    {
        vec3 p;
        do
        {
            // Generate a random point in a unit cube ie its components fall between -1 and +1
            p = 2.f * vec3(random.get(), random.get(), random.get()) - vec3(1.f, 1.f, 1.f);

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
}

int main()
{
    using namespace rts;

    std::cout << "Ray Tracing in One Weekend Book Series by Peter Shirley" << std::endl;

    // Save the time before ray tracing
    auto startTime = std::chrono::system_clock::now();

    // Set up the world and camera
    HitableList<2> world;
    world[0] = std::move(std::make_unique<Sphere>(vec3(0.f, 0.f, -1.f), 0.5f));         // sphere at the center of the screen
    world[1] = std::move(std::make_unique<Sphere>(vec3(0.f, -100.5f, -1.f), 100.f));    // sphere representing the ground
    Camera cam(IMAGE_ASPECT_RATIO);

    // The array which will store the colors output by the ray tracer
    using Color = std::tuple<int, int, int>;
    using ImageData = std::array<Color, IMAGE_WIDTH* IMAGE_HEIGHT>;
    auto imageData = std::make_unique<ImageData>();

    // Run the ray tracer on each pixel to determine its color
    // from left to right and top to bottom
    for (int j = IMAGE_HEIGHT - 1; j >= 0; --j)
    {
        for (int i = 0; i < IMAGE_WIDTH; ++i)
        {
            vec3 col(0.f, 0.f, 0.f); // the accumulated color

            // Sample multiple times randomly within the current pixel
            for (int s = 0; s < SAMPLES_PER_PIXEL; ++s)
            {
                float u = float(i + random.get()) / float(IMAGE_WIDTH);
                float v = float(j + random.get()) / float(IMAGE_HEIGHT);
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

            // Store the resulting color in the array
            (*imageData)[i + j * IMAGE_WIDTH] = std::make_tuple(ir, ig, ib);
        }
    }

    // Write the ray tracer output to the image file
    std::ofstream imageFile(IMAGE_FILE_PATH);
    if (imageFile.is_open())
    {
        // Write the image file header
        imageFile << "P3" << std::endl;
        imageFile << IMAGE_WIDTH << " " << IMAGE_HEIGHT << std::endl;
        imageFile << "255" << std::endl;

        // Iterate over the ray tracer output
        for (int j = IMAGE_HEIGHT - 1; j >= 0; --j)
        {
            for (int i = 0; i < IMAGE_WIDTH; ++i)
            {
                // Output the color to the image file
                auto col = (*imageData)[i + j * IMAGE_WIDTH];
                imageFile << std::get<0>(col) << " " << std::get<1>(col) << " " << std::get<2>(col) << std::endl;
            }
        }

        imageFile.close();
    }

    // Compute and display the elapsed time
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedSeconds = endTime - startTime;
    std::cout << "The process took " << elapsedSeconds.count() << "s to complete" << std::endl;

    return 0;
}
