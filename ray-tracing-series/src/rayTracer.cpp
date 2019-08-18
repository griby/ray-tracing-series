#include "rayTracer.h"

#include <future>
#include <mutex>
#include <utility>
#include <vector>

#include "camera.h"
#include "config.h"
#include "hitable.h"
#include "hitableList.h"
#include "random.h"
#include "ray.h"

namespace rts
{
    vec3 getRandomPointInUnitSphere(Random& random)
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

    vec3 getColor(const Ray& r, const HitableList& world, Random& random)
    {
        // Check if the ray hits any object
        HitRecord rec;
        if (world.hit(r, T_MIN, T_MAX, rec))
        {
#if NORMAL_MAP_COLOR
            // The normal is a unit vector ie its components fall between -1 and +1
            // map those components between 0 and +1 before returning the value
            return 0.5f * vec3(rec.normal.x() + 1.f, rec.normal.y() + 1.f, rec.normal.z() + 1.f);
#else
            // The ray hit a surface, determine a new target to bounce off the surface
            vec3 target = rec.p + rec.normal + getRandomPointInUnitSphere(random);
            return 0.5f * getColor(Ray(rec.p, target - rec.p), world, random);
#endif // NORMAL_MAP_COLOR
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

#if RAY_TRACING_LOG
    // Mutex used to display debug logs
    static std::mutex ioMutex;
#endif // RAY_TRACING_LOG

    void rayTracingSubTask(const Camera& camera, const HitableList& world, ImageData* imageData, int startLine, int endLine, int taskId)
    {
#if RAY_TRACING_LOG
        // Display some debug log
        {
            std::lock_guard<std::mutex> lock(ioMutex);
            std::cout << "    START | RT sub task ID[" << taskId << "] to update lines in the range [" << startLine << ", " << endLine << ")" << std::endl;
        }
#endif // RAY_TRACING_LOG

        // Initialize a random value generator for this specific sub task
        // give it a unique seed based on the taskId
#if THREADING_ON
        Random random(taskId);
#else
        Random random;
#endif // THREADING_ON

        // Run the ray tracer on each pixel in the range [startLine, endLine) to determine its color
        // from left to right and bottom to top
        for (int j = startLine; j < endLine; ++j)
        {
            for (int i = 0; i < IMAGE_WIDTH; ++i)
            {
                vec3 col(0.f, 0.f, 0.f); // the accumulated color

                // Sample multiple times randomly within the current pixel
                for (int s = 0; s < SAMPLES_PER_PIXEL; ++s)
                {
                    float u = float(i + random.get()) / float(IMAGE_WIDTH);
                    float v = float(j + random.get()) / float(IMAGE_HEIGHT);
                    Ray r = camera.getRay(u, v);

                    // Accumulate the sample
                    col += getColor(r, world, random);
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
    }

    void rayTracingMainTask(const Camera& camera, const HitableList& world, ImageData* imageData)
    {
#if THREADING_ON
        std::vector<std::future<void>> subTaskFutures;

        // The number of lines that each task will take care of
        int linesPerTask = IMAGE_HEIGHT / NUMBER_OF_TASKS;

        // Initialize each of the tasks
        for (int taskId = 0; taskId < NUMBER_OF_TASKS; ++taskId)
        {
            // The last task takes care of whatever is left
            int startLine = taskId * linesPerTask;
            int endLine = (taskId == NUMBER_OF_TASKS - 1) ? IMAGE_HEIGHT : startLine + linesPerTask;

#if RAY_TRACING_LOG
            // Display some debug log
            {
                std::lock_guard<std::mutex> lock(ioMutex);
                std::cout << "  CREATE | RT sub task ID[" << taskId << "] to update the range [" << startLine << ", " << endLine << ")" << std::endl;
            }
#endif // RAY_TRACING_LOG
            
            // Create the async sub task
            auto subTask = std::async(std::launch::async,
                [&, startLine, endLine, taskId]() { rayTracingSubTask(camera, world, imageData, startLine, endLine, taskId); });
            
            // Store it in a vector
            subTaskFutures.push_back(std::move(subTask));
        }
        // Leaving the scope will destroy the vector, which will in turn wait for the sub tasks to complete before destroying those
#else
        // Multithreading is disabled, just call the function directly to update the entire image
        rayTracingSubTask(camera, world, imageData, 0, IMAGE_HEIGHT, -1);
#endif // THREADING_ON
    }
}
