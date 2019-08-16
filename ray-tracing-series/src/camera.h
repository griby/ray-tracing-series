#pragma once

#include "ray.h"

namespace rts // for ray tracing series
{
    class Camera
    {
    public:
        Camera()
            : origin(0.f, 0.f, 0.f)
            , lowerLeftCorner(-2.f, -1.f, -1.f)
            , horizontal(4.f, 0.f, 0.f)
            , vertical(0.f, 2.f, 0.f)
        {}

        // Return the ray starting at the camera position and oriented towards the point on the plane corresponding to the given offset
        // note that the offset is applied from the lower-left corner of the plane
        Ray getRay(float u, float v) const { return Ray(origin, lowerLeftCorner + u * horizontal + v * vertical - origin); }

    private:
        vec3 origin;
        vec3 lowerLeftCorner;
        vec3 horizontal;
        vec3 vertical;
    };
}
