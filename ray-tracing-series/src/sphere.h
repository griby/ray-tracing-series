#pragma once

#include "hitable.h"

namespace rts // for ray tracing series
{
    class sphere : public hitable
    {
    public:
        sphere() : center(vec3()), radius(0.f) {}
        sphere(vec3 _center, float _radius) : center(_center), radius(_radius) {}

        virtual bool hit(const ray& r, float tMin, float tMax, hitRecord& rec) const;

    private:
        inline void setHitRecord(hitRecord& rec, float t, const ray& r) const;

        vec3 center;
        float radius;
    };
}
