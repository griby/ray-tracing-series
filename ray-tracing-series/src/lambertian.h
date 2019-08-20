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
    class Lambertian final : public Material
    {
    public:
        Lambertian(const vec3& albedo) : m_albedo(albedo) {}

        virtual bool scatter(const Ray& rIn, const HitRecord& rec, vec3& attenuation, Ray& scattered, Random& random) const override;

    private:
        vec3 m_albedo;
    };
}
