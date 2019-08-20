#pragma once

#include "material.h"
#include "vec3.h"

namespace rts // for ray tracing series
{
    class Lambertian final : public Material
    {
    public:
        Lambertian(const vec3& albedo) : m_albedo(albedo) {}

        virtual bool scatter(const Ray& rIn, const HitRecord& rec, vec3& attenuation, Ray& scattered, Random& random) const override;

    private:
        vec3 m_albedo;
    };
}
