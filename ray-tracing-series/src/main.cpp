
// Ray Tracing in One Weekend Book Series by Peter Shirley

#include <iostream>
#include <fstream>

#include "vec3.h"
#include "ray.h"

using namespace rts;

const std::string IMAGE_FILE_PATH = "output/image.ppm";

vec3 color(const ray& r)
{
    // unitDirection Y is between -1 and +1
    vec3 unitDirection = unitVector(r.direction());

    // scale unitDirection Y between 0 and +1
    float t = 0.5f * (unitDirection.y() + 1.f);

    // blend between white and blue depending on the direction Y
    return (1.f - t) * vec3(1.f, 1.f, 1.f) + t * vec3(0.5f, 0.7f, 1.f);
}

int main()
{
    std::ofstream imageFile(IMAGE_FILE_PATH);
    if (imageFile.is_open())
    {
        int nx = 200;
        int ny = 100;

        imageFile << "P3" << std::endl;
        imageFile << nx << " " << ny << std::endl;
        imageFile << "255" << std::endl;

        vec3 lowerLeftCorner(-2.f, -1.f, -1.f);
        vec3 horizontal(4.f, 0.f, 0.f);
        vec3 vertical(0.f, 2.f, 0.f);
        vec3 origin(0.f, 0.f, 0.f);

        // From top to bottom
        for (int j = ny - 1; j >= 0; --j)
        {
            // From left to right
            for (int i = 0; i < nx; ++i)
            {
                float u = float(i) / float(nx);
                float v = float(j) / float(ny);

                // Create a ray between the camera (origin) and the current pixel (offset from the lower-left corner)
                ray r(origin, lowerLeftCorner + u * horizontal + v * vertical);
                vec3 c = color(r);

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
