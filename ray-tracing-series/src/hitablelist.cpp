/**
 * MIT License
 * Copyright (c) 2019 Guillaume Riby <guillaumeriby@gmail.com>
 *
 * GitHub repository - https://github.com/griby/ray-tracing-series
 *
 * A ray tracer implementation based on the Ray Tracing in One Weekend Book Series by Peter Shirley - https://raytracing.github.io/
 */

#include "hitableList.h"

namespace rts
{
    bool HitableList::hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const
    {
        HitRecord tempRec;
        bool hitAnything = false;
        float closestSoFar = tMax;

        // Check every hitable objects and store the information for the closest one to the camera
        for (const auto& h : m_list)
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
