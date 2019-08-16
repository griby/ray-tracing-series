#pragma once

#include <array>

#include "hitable.h"

namespace rts // for ray tracing series
{
    template<int N>
    class HitableList : public std::array<std::unique_ptr<const Hitable>, N>
    {
    public:
        virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const;
    };

    template<int N>
    bool HitableList<N>::hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const
    {
        HitRecord tempRec;
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
