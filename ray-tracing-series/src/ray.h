/**
 * MIT License
 * Copyright (c) 2019 Guillaume Riby <guillaumeriby@gmail.com>
 *
 * GitHub repository - https://github.com/griby/ray-tracing-series
 *
 * A ray tracer implementation based on the Ray Tracing in One Weekend Book Series by Peter Shirley - https://raytracing.github.io/
 */

#pragma once

#include "vec3.h"

namespace rts // for ray tracing series
{
    class Ray final
    {
    public:
        Ray() : m_origin(), m_direction() {}
        Ray(const vec3& origin, const vec3& direction) : m_origin(origin), m_direction(direction) {}

        vec3 origin() const { return m_origin; }
        vec3 direction() const { return m_direction; }

        vec3 pointAtParameter(float t) const { return m_origin + t * m_direction; }

    private:
        vec3 m_origin;
        vec3 m_direction;
    };
}
