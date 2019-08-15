#pragma once

#include "vec3.h"

namespace rts // for ray tracing series
{
    class ray
    {
    public:
        ray() : a(), b() {}
        ray(const vec3& _a, const vec3& _b) : a(_a), b(_b) {}

        vec3 origin() const { return a; }
        vec3 direction() const { return b; }

        vec3 pointAtParameter(float t) const { return a + t * b; }

    private:
        vec3 a;
        vec3 b;
    };
}
