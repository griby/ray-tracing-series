/**
 * MIT License
 * Copyright (c) 2019 Guillaume Riby <guillaumeriby@gmail.com>
 *
 * GitHub repository - https://github.com/griby/ray-tracing-series
 *
 * A ray tracer implementation based on the Ray Tracing in One Weekend Book Series by Peter Shirley - https://raytracing.github.io/
 */

#include "utils.h"

#include <cmath>

#include "random.h"

namespace rts
{
    vec3 getRandomPointInUnitDisk(Random& random)
    {
        vec3 p;
        do
        {
            p = 2.f * vec3(random.get(), random.get(), 0.f) - vec3(1.f, 1.f, 0.f);
        } while (p.squaredLength() >= 1.f);

        return p;
    }

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

    vec3 getReflectedVector(const vec3& v, const vec3& n)
    {
        // Compute the reflected vector as described in chapter 8
        return v - 2 * dot(v, n) * n;
    }

    // TODO Add more information from http://psgraphics.blogspot.com/2015/06/ray-tracing-refraction.html
    // n sin(t) = n' sin(t')
    // D = -cos(t) N + sin(t) Q
    // T = -cos(t') N + sin(t') Q
    // where D is the normalized incoming ray, T is the exiting one
    // T = -cos(t') N + sin(t') * (D + cos(t) N) / sin(t)
    // cos(t) = -dot(D, N)
    // sin(t) = sqrt(1 - cos(t)^2) = sqrt(1, dot(D, N)^2)
    // sin(t') = (n / n') * sin(t) = (n / n') * sqrt(1 - dot(D, N)^2)
    // cos(t') = sqrt(1 - sin(t')^2) = sqrt(1 - (n / n')^2 * (1 - dot(D, N)^2))
    // injecting cos(t), sin(t), sin(t') and cos(t') in T we get:
    // T = -sqrt(1 - (n / n')^2 * (1 - dot(D, N)^2)) N + (n / n') * (D + -dot(D, N) N)
    // T = (n / n') * D + (-dot(D, N) * (n / n') - sqrt(1 - (n / n')^2 * (1 - dot(D, N)^2))) N
    bool getRefractedVector(const vec3& v, const vec3& n, float refIdxRatio, vec3& refracted)
    {
        vec3 uv = unitVector(v);
        float dt = dot(uv, n);
        float discriminant = 1.f - refIdxRatio * refIdxRatio * (1.f - dt * dt);
        if (discriminant > 0.f)
        {
            refracted = refIdxRatio * (uv - n * dt) - n * sqrt(discriminant);
            return true;
        }
        return false;
    }

    float getSchlickApproximation(float cosine, float refIdx)
    {
        float r0 = pow((1.f - refIdx) / (1.f + refIdx), 2);
        return r0 + (1.f - r0) * pow(1.f - cosine, 5);
    }
}
