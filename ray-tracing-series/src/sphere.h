/**
 * MIT License
 * Copyright (c) 2019 Guillaume Riby <guillaumeriby@gmail.com>
 *
 * GitHub repository - https://github.com/griby/ray-tracing-series
 *
 * A ray tracer implementation based on the Ray Tracing in One Weekend Book Series by Peter Shirley - https://raytracing.github.io/
 */

#pragma once

#include <memory>

#include "hitable.h"

namespace rts // for ray tracing series
{
    class Sphere final : public Hitable
    {
    public:
        Sphere() : m_center(vec3()), m_radius(0.f) {}
        Sphere(vec3 center, float radius, std::shared_ptr<Material> material)
            : m_center(center)
            , m_radius(radius)
            , m_material(material)
        {
        }

        virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const override;

    private:
        inline void setHitRecord(HitRecord& rec, float t, const Ray& r, const Material* material) const;

        vec3 m_center;
        float m_radius;
        std::shared_ptr<Material> m_material; // a material may be shared by multiple spheres
    };
}
