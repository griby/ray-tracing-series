/**
 * MIT License
 * Copyright (c) 2019 Guillaume Riby <guillaumeriby@gmail.com>
 *
 * GitHub repository - https://github.com/griby/ray-tracing-series
 *
 * A ray tracer implementation based on the Ray Tracing in One Weekend Book Series by Peter Shirley - https://raytracing.github.io/
 */

#include "rayTracer.h"

#include <assert.h>
#include <cmath>
#include <future>
#include <mutex>
#include <utility>
#include <vector>

#include "camera.h"
#include "config.h"
#include "hitable.h"
#include "hitableList.h"
#include "material.h"
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

    vec3 getReflected(const vec3& v, const vec3& n)
    {
        // Compute the reflected vector as described in chapter 8
        return v - 2 * dot(v, n) * n;
    }

    // TODO Add more information from http://psgraphics.blogspot.com/2015/06/ray-tracing-refraction.html
    bool getRefracted(const vec3& v, const vec3& n, float niOverNt, vec3& refracted)
    {
        vec3 uv = unitVector(v);
        float dt = dot(uv, n);
        float discriminant = 1.f - niOverNt * niOverNt * (1.f - dt * dt);
        if (discriminant > 0.f)
        {
            refracted = niOverNt * (uv - n * dt) - n * sqrt(discriminant);
            return true;
        }
        return false;
    }

    float getSchlick(float cosine, float refIdx)
    {
        float r0 = pow((1.f - refIdx) / (1.f + refIdx), 2);
        return r0 + (1.f - r0) * pow(1.f - cosine, 5);
    }

    vec3 getColor(const Ray& r, const HitableList& world, int depth, Random& random)
    {
        // Check if the ray hits any object
        HitRecord rec;
        if (world.hit(r, T_MIN, T_MAX, rec))
        {
#ifdef RENDER_NORMAL_MAP
            // The normal is a unit vector ie its components fall between -1 and +1
            // map those components between 0 and +1 before returning the value
            return 0.5f * vec3(rec.normal.x() + 1.f, rec.normal.y() + 1.f, rec.normal.z() + 1.f);
#elif defined RENDER_NO_MATERIAL
            // The ray hit a surface, determine a new target to bounce off of it
            // also check the depth to avoid infinite recursions, it can happen with spheres of negative radius
            // rays end up being trapped inside and there's no refraction possible since we ignore the material
            if (depth < RAY_MAX_DEPTH)
            {
                vec3 target = rec.p + rec.normal + getRandomPointInUnitSphere(random);
                return 0.5f * getColor(Ray(rec.p, target - rec.p), world, depth + 1, random);
            }
            else
            {
                // TODO This ray shouldn't contribute to the overall pixel's color
                // have the getColor function return a boolean and the color passed-by-ref instead
                // and just ignore the result in the ray tracing sub task when it's false
                return vec3(0.f, 0.f, 0.f);
            }
#else
            // The surface must have a material
            assert(rec.matPtr != nullptr);
            
            Ray scattered;
            vec3 attenuation;

            // The ray hit a surface, get the attenuation and scattered information from its material
            if (depth < RAY_MAX_DEPTH && rec.matPtr->scatter(r, rec, attenuation, scattered, random))
            {
                return attenuation * getColor(scattered, world, depth + 1, random);
            }
            else
            {
                // TODO This ray shouldn't contribute to the overall pixel's color
                // have the getColor function return a boolean and the color passed-by-ref instead
                // and just ignore the result in the ray tracing sub task when it's false
                return vec3(0.f, 0.f, 0.f);
            }
#endif // RENDER_NORMAL_MAP, RENDER_NO_MATERIAL
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

#ifdef MULTITHREADING_LOGS
    // Mutex used to display debug logs
    static std::mutex ioMutex;
#endif // MULTITHREADING_LOGS

    void rayTracingSubTask(const Camera& camera, const HitableList& world, ImageData* imageData, int startLine, int endLine, int taskId)
    {
#ifdef MULTITHREADING_LOGS
        // Display some debug log
        {
            std::lock_guard<std::mutex> lock(ioMutex);
            std::cout << "    START | RT sub task ID[" << taskId << "] to update lines in the range [" << startLine << ", " << endLine << ")" << std::endl;
        }
#endif // MULTITHREADING_LOGS

        // Initialize a random value generator for this specific sub task
        // give it a unique seed based on the taskId
#ifdef MULTITHREADING_ON
        Random random(taskId);
#else
        Random random;
#endif // MULTITHREADING_ON

        // Run the ray tracer on each pixel in the range [startLine, endLine) to determine its color
        // from left to right and bottom to top
        for (int j = startLine; j < endLine; ++j)
        {
            for (int i = 0; i < IMAGE_WIDTH; ++i)
            {
                vec3 col(0.f, 0.f, 0.f); // the accumulated color

                // Sample multiple times randomly within the current pixel
                for (int s = 0; s < RAY_COUNT_PER_PIXEL; ++s)
                {
                    float u = float(i + random.get()) / float(IMAGE_WIDTH);
                    float v = float(j + random.get()) / float(IMAGE_HEIGHT);
                    Ray r = camera.getRay(u, v);

                    // Accumulate the sample
                    col += getColor(r, world, 0, random);
                }

                // Average the color
                col /= static_cast<float>(RAY_COUNT_PER_PIXEL);

#ifdef RENDER_GRAYSCALE
                // Colorimetric conversion to grayscale https://en.wikipedia.org/wiki/Grayscale
                // apply it before gamma correction
                float lum = 0.2126f * col[0] + 0.7152f * col[1] + 0.0722f * col[2]; // compute the luminance

                // Apply a gamma to brighten the color
                lum = sqrt(lum);

                // Scale the color between 0 and 255
                int il = int(255.99f * lum);

                auto finalColor = std::make_tuple(il, il, il);
#else
                // Apply a gamma to brighten the color
                col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

                // Scale the color between 0 and 255
                int ir = int(255.99f * col[0]);
                int ig = int(255.99f * col[1]);
                int ib = int(255.99f * col[2]);

                auto finalColor = std::make_tuple(ir, ig, ib);
#endif // RENDER_GRAYSCALE

                // Store the resulting color in the array
                (*imageData)[i + j * IMAGE_WIDTH] = finalColor;
            }
        }
    }

    void rayTracingMainTask(const Camera& camera, const HitableList& world, ImageData* imageData)
    {
#ifdef MULTITHREADING_ON
        std::vector<std::future<void>> subTaskFutures;

        // The number of lines that each task will take care of
        int linesPerTask = IMAGE_HEIGHT / SUBTASK_COUNT;

        // Initialize each of the tasks
        for (int taskId = 0; taskId < SUBTASK_COUNT; ++taskId)
        {
            // The last task takes care of whatever is left
            int startLine = taskId * linesPerTask;
            int endLine = (taskId == SUBTASK_COUNT - 1) ? IMAGE_HEIGHT : startLine + linesPerTask;

#ifdef MULTITHREADING_LOGS
            // Display some debug log
            {
                std::lock_guard<std::mutex> lock(ioMutex);
                std::cout << "  CREATE | RT sub task ID[" << taskId << "] to update the range [" << startLine << ", " << endLine << ")" << std::endl;
            }
#endif // MULTITHREADING_LOGS
            
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
#endif // MULTITHREADING_ON
    }
}
