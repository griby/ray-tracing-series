#pragma once

#include "material.h"
#include "vec3.h"

namespace rts // for ray tracing series
{
    class Lambertian : public Material
    {
    public:
        Lambertian(const vec3& a) : albedo(a) {}

        virtual bool scatter(const Ray& rIn, const HitRecord& rec, vec3& attenuation, Ray& scattered, Random& random) const;

    private:
        vec3 albedo;
    };
}
