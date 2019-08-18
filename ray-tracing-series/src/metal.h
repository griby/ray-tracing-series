#pragma once

#include <algorithm>

#include "material.h"
#include "vec3.h"

namespace rts // for ray tracing series
{
    class Metal : public Material
    {
    public:
        Metal(const vec3& a, float f) : albedo(a), fuzz(std::min(f, 1.f)) {}

        virtual bool scatter(const Ray& rIn, const HitRecord& rec, vec3& attenuation, Ray& scattered, Random& random) const;

    private:
        vec3 albedo;
        float fuzz;
    };
}
