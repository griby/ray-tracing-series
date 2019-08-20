#pragma once

#include "material.h"

namespace rts // for ray tracing series
{
    class Dielectric final : public Material
    {
    public:
        Dielectric(float refIdx) : m_refIdx(refIdx) {}

        virtual bool scatter(const Ray& rIn, const HitRecord& rec, vec3& attenuation, Ray& scattered, Random& random) const override;

    private:
        float m_refIdx; // the refraction index
    };
}
