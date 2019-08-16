#pragma once

#include "hitable.h"

namespace rts // for ray tracing series
{
    class Sphere : public Hitable
    {
    public:
        Sphere() : center(vec3()), radius(0.f) {}
        Sphere(vec3 _center, float _radius) : center(_center), radius(_radius) {}

        virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const;

    private:
        inline void setHitRecord(HitRecord& rec, float t, const Ray& r) const;

        vec3 center;
        float radius;
    };
}
