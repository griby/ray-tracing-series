// Ray Tracing in One Weekend Book Series by Peter Shirley

#include <fstream>
#include <future>
#include <iostream>
#include <memory>
#include <tuple>
#include <utility>

#include "camera.h"
#include "config.h"
#include "hitableList.h"
#include "lambertian.h"
#include "metal.h"
#include "rayTracer.h"
#include "sphere.h"
#include "timer.h"
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

    std::cout << "Ray Tracing in One Weekend Book Series by Peter Shirley\n\n";
    Timer globalTimer;
    globalTimer.setStartTime();

    ////////////////////////////////////////////////////////////////////////////////
    std::cout << "Setting up the world..." << std::endl;
    Timer stepTimer;
    stepTimer.setStartTime();

    HitableList world;
    world.reserve(4);
    world.push_back(std::make_unique<Sphere>(vec3(0.f, 0.f, -1.f), 0.5f, std::make_shared<Lambertian>(vec3(0.8f, 0.3f, 0.3f))));        // diffuse sphere at the center of the screen
    world.push_back(std::make_unique<Sphere>(vec3(0.f, -100.5f, -1.f), 100.f, std::make_shared<Lambertian>(vec3(0.8f, 0.8f, 0.f))));    // diffuse sphere representing the ground
    world.push_back(std::make_unique<Sphere>(vec3(1.f, 0.f, -1.f), 0.5f, std::make_shared<Metal>(vec3(0.8f, 0.6f, 0.2f), 0.3f)));       // one metallic sphere on each side of the diffuse one
    world.push_back(std::make_unique<Sphere>(vec3(-1.f, 0.f, -1.f), 0.5f, std::make_shared<Metal>(vec3(0.8f, 0.8f, 0.8f), 1.f)));
    Camera camera(IMAGE_ASPECT_RATIO);

    std::cout << "Done! (" << stepTimer.getElapsedTime() << "s)\n\n";

    ////////////////////////////////////////////////////////////////////////////////
    std::cout << "Performing ray tracing..." << std::endl;
    stepTimer.setStartTime();

    auto imageData = std::make_unique<ImageData>();
    auto mainTask = std::async(std::launch::async,
        [&]() { rayTracingMainTask(camera, world, imageData.get()); });

    // Check periodically if the main task is completed
    while (mainTask.wait_for(std::chrono::milliseconds(500)) != std::future_status::ready)
    {
        std::cout << ".";
    }
    std::cout << std::endl;

    std::cout << "Done! (" << stepTimer.getElapsedTime() << "s)\n\n";

    ////////////////////////////////////////////////////////////////////////////////
    std::cout << "Writing the image file..." << std::endl;
    stepTimer.setStartTime();

    writeImageFile(imageData.get());

    std::cout << "Done! (" << stepTimer.getElapsedTime() << "s)\n\n";

    ////////////////////////////////////////////////////////////////////////////////
    std::cout << "All done! (" << globalTimer.getElapsedTime() << "s)" << std::endl;

    return 0;
}
