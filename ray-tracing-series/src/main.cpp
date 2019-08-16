// Ray Tracing in One Weekend Book Series by Peter Shirley

#include <iostream>
#include <fstream>
#include <limits>

#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "hitableList.h"

using namespace rts;

const std::string IMAGE_FILE_PATH = "output/image.ppm";

template <int N>
vec3 color(const ray& r, const hitableList<N>& world)
{
    // Check if the ray hits a sphere at the center of the screen
    hitRecord rec;
    if (world.hit(r, 0.f, std::numeric_limits<float>::max(), rec))
    {
        // The normal is a unit vector ie its components fall between -1 and +1
        // map those components between 0 and +1 before returning the value
        return 0.5f * vec3(rec.normal.x() + 1.f, rec.normal.y() + 1.f, rec.normal.z() + 1.f);
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
        int nx = 200;
        int ny = 100;

        // Write the image file header
        imageFile << "P3" << std::endl;
        imageFile << nx << " " << ny << std::endl;
        imageFile << "255" << std::endl;

        vec3 lowerLeftCorner(-2.f, -1.f, -1.f);
        vec3 horizontal(4.f, 0.f, 0.f);
        vec3 vertical(0.f, 2.f, 0.f);
        vec3 origin(0.f, 0.f, 0.f);

        // Create the list of hitable objects
        hitableList<2> world;
        world[0] = std::move(std::make_unique<sphere>(vec3(0.f, 0.f, -1.f), 0.5f));         // sphere at the center of the screen
        world[1] = std::move(std::make_unique<sphere>(vec3(0.f, -100.5f, -1.f), 100.f));    // sphere representing the ground

        // Determine each pixel's color from left to right and top to bottom
        for (int j = ny - 1; j >= 0; --j)
        {
            for (int i = 0; i < nx; ++i)
            {
                float u = float(i) / float(nx);
                float v = float(j) / float(ny);

                // Ray trace between the camera (origin) and the current pixel (offset from the lower-left corner)
                ray r(origin, lowerLeftCorner + u * horizontal + v * vertical);
                vec3 c = color(r, world);

                int ir = int(255.99f * c[0]);
                int ig = int(255.99f * c[1]);
                int ib = int(255.99f * c[2]);

                imageFile << ir << " " << ig << " " << ib << std::endl;
            }
        }

        imageFile.close();
    }

    return 0;
}
