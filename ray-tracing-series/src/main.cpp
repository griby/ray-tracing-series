// Ray Tracing in One Weekend Book Series by Peter Shirley

#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <tuple>
#include <utility>

#include "camera.h"
#include "config.h"
#include "hitable.h"
#include "hitableList.h"
#include "rayTracer.h"
#include "sphere.h"
#include "vec3.h"

namespace rts // for ray tracing series
{
    void writeImageFile(const ImageData* imageData)
    {
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
    Camera camera(IMAGE_ASPECT_RATIO);

    // Ray tracing
    auto imageData = std::make_unique<ImageData>();
    performRayTracing(camera, world, imageData.get());

    // Output to file
    writeImageFile(imageData.get());

    // Compute and display the elapsed time
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedSeconds = endTime - startTime;
    std::cout << "The process took " << elapsedSeconds.count() << "s to complete" << std::endl;

    return 0;
}
