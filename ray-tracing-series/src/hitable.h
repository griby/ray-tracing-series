#pragma once

#include "vec3.h"

namespace rts // for ray tracing series
{
    class Ray;

    struct HitRecord
    {
        float t;
        vec3 p;
        vec3 normal;
    };

    class Hitable
    {
    public:
        virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const = 0;
    };
}
