/**
 * MIT License
 * Copyright (c) 2019 Guillaume Riby <guillaumeriby@gmail.com>
 *
 * GitHub repository - https://github.com/griby/ray-tracing-series
 *
 * A ray tracer implementation based on the Ray Tracing in One Weekend Book Series by Peter Shirley - https://raytracing.github.io/
 */

#include "camera.h"

#include "defines.h"

namespace rts
{
    Camera::Camera(vec3 lookFrom, vec3 lookAt, vec3 vUp, float vFov, float aspectRatio)
        : m_origin(lookFrom)
    {
        float theta = vFov * static_cast<int>(M_PI) / 180.f; // the FOV converted in radian
        float halfHeight = tan(theta / 2.f);
        float halfWidth = aspectRatio * halfHeight;

        // Compute the orthonormal basis (u, v, w)
        // where w is aligned with the camera direction and the plane (u, v) is orthogonal to it
        vec3 w = unitVector(lookFrom - lookAt);
        vec3 u = unitVector(cross(vUp, w));
        vec3 v = cross(w, u);

        // Determine the lower-left corner of the focus plane along with its horizontal and vertical axes
        m_lowerLeftCorner = vec3(-halfWidth, -halfHeight, -1.f);
        m_lowerLeftCorner = m_origin - halfWidth * u - halfHeight * v - w;
        m_horizontal = 2.f * halfWidth * u;
        m_vertical = 2.f * halfHeight * v;
    }
}
