#pragma once

#include "ray.h"

namespace rts // for ray tracing series
{
    class Camera
    {
    public:
        Camera(float aspectRatio)
            : origin(0.f, 0.f, 0.f)
            , lowerLeftCorner(-2.f, -2.f/aspectRatio, -1.f)
            , horizontal(4.f, 0.f, 0.f)
            , vertical(0.f, 4.f/aspectRatio, 0.f)
        {}

        // Return the ray starting at the camera position and oriented towards a specific point in space
        // this point is determined by applying the given offset to the arbitrary position lowerLeftCorner
        Ray getRay(float u, float v) const { return Ray(origin, lowerLeftCorner + u * horizontal + v * vertical - origin); }

    private:
        vec3 origin;
        vec3 lowerLeftCorner;
        vec3 horizontal;
        vec3 vertical;
    };
}
