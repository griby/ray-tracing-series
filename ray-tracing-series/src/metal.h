#pragma once

#include <algorithm>

#include "material.h"
#include "vec3.h"

namespace rts // for ray tracing series
{
    class Metal : public Material
    {
    public:
        Metal(const vec3& albedo, float fuzz) : m_albedo(albedo), m_fuzz(std::min(fuzz, 1.f)) {}

        virtual bool scatter(const Ray& rIn, const HitRecord& rec, vec3& attenuation, Ray& scattered, Random& random) const override;

    private:
        vec3 m_albedo;
        float m_fuzz;
    };
}
