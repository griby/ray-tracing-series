/**
 * MIT License
 * Copyright (c) 2019 Guillaume Riby <guillaumeriby@gmail.com>
 *
 * GitHub repository - https://github.com/griby/ray-tracing-series
 *
 * A ray tracer implementation based on the Ray Tracing in One Weekend Book Series by Peter Shirley - https://raytracing.github.io/
 */

#pragma once

#include "ray.h"

namespace rts // for ray tracing series
{
    class Camera final
    {
    public:
        Camera(vec3 lookFrom, vec3 lookAt, vec3 vUp, float vFov, float aspectRatio);

        // Return the ray starting at the camera position and oriented towards a specific point in space
        // this point is determined by applying the given offset to the point corresponding to the lower-left corner
        Ray getRay(float u, float v) const { return Ray(m_origin, m_lowerLeftCorner + u * m_horizontal + v * m_vertical - m_origin); }

    private:
        vec3 m_origin;
        vec3 m_lowerLeftCorner;
        vec3 m_horizontal;
        vec3 m_vertical;
    };
}
