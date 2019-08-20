#pragma once

#include "ray.h"

namespace rts // for ray tracing series
{
    class Camera final
    {
    public:
        Camera(float aspectRatio)
            : m_origin(0.f, 0.f, 0.f)
            , m_lowerLeftCorner(-2.f, -2.f/aspectRatio, -1.f)
            , m_horizontal(4.f, 0.f, 0.f)
            , m_vertical(0.f, 4.f/aspectRatio, 0.f)
        {
        }

        // Return the ray starting at the camera position and oriented towards a specific point in space
        // this point is determined by applying the given offset to the arbitrary position lowerLeftCorner
        Ray getRay(float u, float v) const { return Ray(m_origin, m_lowerLeftCorner + u * m_horizontal + v * m_vertical - m_origin); }

    private:
        vec3 m_origin;
        vec3 m_lowerLeftCorner;
        vec3 m_horizontal;
        vec3 m_vertical;
    };
}
