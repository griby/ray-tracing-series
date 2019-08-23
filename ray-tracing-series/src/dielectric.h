/**
 * MIT License
 * Copyright (c) 2019 Guillaume Riby <guillaumeriby@gmail.com>
 *
 * GitHub repository - https://github.com/griby/ray-tracing-series
 *
 * A ray tracer implementation based on the Ray Tracing in One Weekend Book Series by Peter Shirley - https://raytracing.github.io/
 */

#pragma once

#include "material.h"
#include "vec3.h"

namespace rts // for ray tracing series
{
    class Dielectric final : public Material
    {
    public:
        Dielectric(float refIdx) : m_albedo(1.f, 1.f, 1.f), m_refIdx(refIdx) {}
        Dielectric(const vec3& albedo, float refIdx) : m_albedo(albedo), m_refIdx(refIdx) {}

        virtual bool scatter(const Ray& rIn, const HitRecord& rec, vec3& attenuation, Ray& scattered, Random& random) const override;

    private:
        vec3 m_albedo;
        float m_refIdx; // the refraction index
    };
}
