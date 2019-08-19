#pragma once

#include "material.h"

namespace rts // for ray tracing series
{
    class Dielectric : public Material
    {
    public:
        Dielectric(float ri) : refIdx(ri) {}

        virtual bool scatter(const Ray& rIn, const HitRecord& rec, vec3& attenuation, Ray& scattered, Random& random) const;

    private:
        float refIdx;
    };
}
