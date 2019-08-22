/**
 * MIT License
 * Copyright (c) 2019 Guillaume Riby <guillaumeriby@gmail.com>
 *
 * GitHub repository - https://github.com/griby/ray-tracing-series
 *
 * A ray tracer implementation based on the Ray Tracing in One Weekend Book Series by Peter Shirley - https://raytracing.github.io/
 */

#include <fstream>
#include <future>
#include <iostream>
#include <memory>
#include <tuple>
#include <utility>

#include "camera.h"
#include "config.h"
#include "defines.h"
#include "dielectric.h"
#include "hitableList.h"
#include "lambertian.h"
#include "metal.h"
#include "random.h"
#include "raytracer.h"
#include "sphere.h"
#include "timer.h"
#include "vec3.h"

namespace rts // for ray tracing series
{
    void generateCustomWorld(HitableList& world)
    {
        auto lambertianMat1 = std::make_shared<Lambertian>(vec3(0.1f, 0.2f, 0.5f));
        auto lambertianMat2 = std::make_shared<Lambertian>(vec3(0.8f, 0.8f, 0.f));
        auto metallicMat = std::make_shared<Metal>(vec3(0.8f, 0.6f, 0.2f), 0.3f);
        auto dielectricMat = std::make_shared<Dielectric>(1.5f);

        world.reserve(5);
        world.add(std::make_unique<Sphere>(vec3(0.f, 0.f, -1.f), 0.5f, lambertianMat1));        // diffuse sphere at the center of the screen
        world.add(std::make_unique<Sphere>(vec3(0.f, -100.5f, -1.f), 100.f, lambertianMat2));   // diffuse sphere representing the ground
        world.add(std::make_unique<Sphere>(vec3(1.f, 0.f, -1.f), 0.5f, metallicMat));           // metallic sphere on the right side of the diffuse one
        world.add(std::make_unique<Sphere>(vec3(-1.f, 0.f, -1.f), 0.5f, dielectricMat));        // glass sphere on the left side of the diffuse one
        world.add(std::make_unique<Sphere>(vec3(-1.f, 0.f, -1.f), -0.45f, dielectricMat));      // activate this to make the glass sphere hollow (negative radius)
    }

    void generateSimpleCustomWorld(HitableList& world)
    {
        float R = cos(static_cast<int>(M_PI) / 4.f);
        world.reserve(2);
        world.add(std::make_unique<Sphere>(vec3(-R, 0.f, -1.f), R, std::make_shared<Lambertian>(vec3(0.f, 0.f, 1.f))));
        world.add(std::make_unique<Sphere>(vec3(R, 0.f, -1.f), R, std::make_shared<Lambertian>(vec3(1.f, 0.f, 0.f))));
    }

    void generateRandomWorld(HitableList& world)
    {
        world.reserve(500);
        world.add(std::make_unique<Sphere>(vec3(0.f, -1000.f, 0.f), 1000.f, std::make_shared<Lambertian>(vec3(0.5f, 0.5f, 0.5f))));

        Random random;
        for (int a = -11; a < 11; ++a)
        {
            for (int b = -11; b < 11; ++b)
            {
                float chooseMat = random.get();
                vec3 center(a + 0.9f * random.get(), 0.2f, b + 0.9f * random.get());

                if ((center - vec3(4.f, 0.2f, 0.f)).length() > 0.9f)
                {
                    if (chooseMat < 0.8f) // diffuse
                    {
                        world.add(std::make_unique<Sphere>(center, 0.2f,
                            std::make_shared<Lambertian>(vec3(random.get() * random.get(), random.get() * random.get(), random.get() * random.get()))));
                    }
                    else if (chooseMat < 0.95f) // metal
                    {
                        world.add(std::make_unique<Sphere>(center, 0.2f,
                            std::make_shared<Metal>(vec3(0.5f * (1.f + random.get()), 0.5f * (1.f + random.get()), 0.5f * (1.f + random.get())), 0.5f * random.get())));
                    }
                    else // glass
                    {
                        world.add(std::make_unique<Sphere>(center, 0.2f, std::make_shared<Dielectric>(1.5f)));
                    }
                }
            }
        }

        world.add(std::make_unique<Sphere>(vec3(0.f, 1.f, 0.f), 1.f, std::make_shared<Dielectric>(1.5f)));
        world.add(std::make_unique<Sphere>(vec3(-4.f, 1.f, 0.f), 1.f, std::make_shared<Lambertian>(vec3(0.4f, 0.2f, 0.1f))));
        world.add(std::make_unique<Sphere>(vec3(4.f, 1.f, 0.f), 1.f, std::make_shared<Metal>(vec3(0.7f, 0.6f, 0.5f), 0.f)));
    }

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

    std::cout << "A ray tracer implementation based on the Ray Tracing in One Weekend Book Series by Peter Shirley - https://raytracing.github.io/\n\n";
    Timer globalTimer;
    globalTimer.setStartTime();

    ////////////////////////////////////////////////////////////////////////////////
    std::cout << "Setting up the world..." << std::endl;
    Timer stepTimer;
    stepTimer.setStartTime();

    HitableList world;
    std::unique_ptr<Camera> camera;
    if (WORLD_GEN_RANDOM)
    {
        generateRandomWorld(world);

        vec3 lookFrom(6.f, 1.5f, -2.f);
        vec3 lookAt(4.f, 1.1667f, -1.333f);
        float distToFocus = (lookFrom - lookAt).length();
        float aperture = 0.02f;
        camera = std::make_unique<Camera>(lookFrom, lookAt, vec3(0.f, 1.f, 0.f), CAMERA_FOV, CAMERA_ASPECT_RATIO, aperture, distToFocus);
    }
    else
    {
        generateCustomWorld(world);
        //generateSimpleCustomWorld(world);

        vec3 lookFrom(3.f, 3.f, 2.f);
        vec3 lookAt(0.f, 0.f, -1.f);
        float distToFocus = (lookFrom - lookAt).length();
        float aperture = 2.f;
        camera = std::make_unique<Camera>(lookFrom, lookAt, vec3(0.f, 1.f, 0.f), 20.f, CAMERA_ASPECT_RATIO, aperture, distToFocus);
    }

    std::cout << "Done! (" << stepTimer.getElapsedTime() << "s)\n\n";

    ////////////////////////////////////////////////////////////////////////////////
    std::cout << "Performing ray tracing..." << std::endl;
    stepTimer.setStartTime();

    // Start the ray tracing main task
    auto imageData = std::make_unique<ImageData>();
    auto mainTask = std::async(std::launch::async,
        [&]() { rayTracingMainTask(*camera.get(), world, imageData.get()); });

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
