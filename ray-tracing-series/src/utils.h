/**
 * MIT License
 * Copyright (c) 2019 Guillaume Riby <guillaumeriby@gmail.com>
 *
 * GitHub repository - https://github.com/griby/ray-tracing-series
 *
 * A ray tracer implementation based on the Ray Tracing in One Weekend Book Series by Peter Shirley - https://raytracing.github.io/
 */

#pragma once

#include "vec3.h"

namespace rts // for ray tracing series
{
    class Random;

    // Generate a random point in a unit disk
    vec3 getRandomPointInUnitDisk(Random& random);

    // Generate a random point in a unit sphere
    vec3 getRandomPointInUnitSphere(Random& random);

    // Compute the reflected vector for the given vector and normal
    vec3 getReflectedVector(const vec3& v, const vec3& n);

    // Compute the refracted vector for the given vector, normal and a refraction ratio
    bool getRefractedVector(const vec3& v, const vec3& n, float niOverNt, vec3& refracted);

    // Compute Schlick's approximation for the given values
    float getSchlickApproximation(float cosine, float refIdx);
}
