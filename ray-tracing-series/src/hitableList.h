#pragma once

#include <array>

#include "hitable.h"

namespace rts // for ray tracing series
{
    template<int N>
    class hitableList : public std::array<std::unique_ptr<const hitable>, N>
    {
    public:
        virtual bool hit(const ray& r, float tMin, float tMax, hitRecord& rec) const;
    };

    template<int N>
    bool hitableList<N>::hit(const ray& r, float tMin, float tMax, hitRecord& rec) const
    {
        hitRecord tempRec;
        bool hitAnything = false;
        float closestSoFar = tMax;

        // Check every hitable objects and store the information for the closest one to the camera
        for (const auto& h : *this)
        {
            if (h->hit(r, tMin, closestSoFar, tempRec))
            {
                hitAnything = true;
                closestSoFar = tempRec.t;
                rec = tempRec;
            }
        }

        return hitAnything;
    }
}
