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
#include "random.h"
#include "utils.h"

namespace rts
{
    Camera::Camera(vec3 lookFrom, vec3 lookAt, vec3 vUp, float vFov, float aspectRatio, float aperture, float focusDist)
        : m_origin(lookFrom)
        , m_lensRadius(aperture / 2.f)
    {
        float theta = vFov * static_cast<int>(M_PI) / 180.f; // the FOV converted in radian
        float halfHeight = tan(theta / 2.f);
        float halfWidth = aspectRatio * halfHeight;

        // Compute the orthonormal basis (u, v, w)
        // where w is aligned with the camera direction and the plane (u, v) is orthogonal to it
        w = unitVector(lookFrom - lookAt);
        u = unitVector(cross(vUp, w));
        v = cross(w, u);

        // Determine a point in space which corresponds to the lower-left corner of our screen
        // along with the screen's horizontal and vertical axes
        m_lowerLeftCorner = m_origin - halfWidth * focusDist * u - halfHeight * focusDist * v - focusDist * w;
        m_horizontal = 2.f * halfWidth * focusDist * u;
        m_vertical = 2.f * halfHeight * focusDist * v;
    }

    Ray Camera::getRay(float s, float t, Random& random) const
    {
        vec3 rd = m_lensRadius * getRandomPointInUnitDisk(random);
        vec3 offset = u * rd.x() + v * rd.y();
        return Ray(m_origin + offset, m_lowerLeftCorner + s * m_horizontal + t * m_vertical - m_origin - offset);
    }
}
