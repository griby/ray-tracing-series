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
    class Material;
    class Ray;

    struct HitRecord
    {
        float t;
        vec3 p;
        vec3 normal;
        const Material* matPtr;
    };

    class Hitable
    {
    public:
        virtual ~Hitable() {}

        virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const = 0;
    };
}
