#pragma once

#include <memory>

#include "hitable.h"

namespace rts // for ray tracing series
{
    class Sphere : public Hitable
    {
    public:
        Sphere() : center(vec3()), radius(0.f) {}
        Sphere(vec3 _center, float _radius, std::shared_ptr<Material> _material)
            : center(_center)
            , radius(_radius)
            , material(_material)
        {
        }

        virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const;

    private:
        inline void setHitRecord(HitRecord& rec, float t, const Ray& r, const Material* material) const;

        vec3 center;
        float radius;
        std::shared_ptr<Material> material; // a material may be shared by multiple spheres
    };
}
