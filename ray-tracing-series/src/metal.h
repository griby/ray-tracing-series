/**
 * MIT License
 * Copyright (c) 2019 Guillaume Riby <guillaumeriby@gmail.com>
 *
 * GitHub repository - https://github.com/griby/ray-tracing-series
 *
 * A ray tracer implementation based on the Ray Tracing in One Weekend Book Series by Peter Shirley - https://raytracing.github.io/
 */

#pragma once

#include <algorithm>

#include "material.h"
#include "vec3.h"

namespace rts // for ray tracing series
{
    class Metal final : public Material
    {
    public:
        Metal(const vec3& albedo, float fuzz) : m_albedo(albedo), m_fuzz(std::min(fuzz, 1.f)) {}

        virtual bool scatter(const Ray& rIn, const HitRecord& rec, vec3& attenuation, Ray& scattered, Random& random) const override;

    private:
        vec3 m_albedo;
        float m_fuzz;
    };
}
