#pragma once

#include <vector>

#include "hitable.h"

namespace rts // for ray tracing series
{
    class HitableList : public std::vector<std::unique_ptr<const Hitable>>
    {
    public:
        virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const;
    };
}
