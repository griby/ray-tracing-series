/**
 * MIT License
 * Copyright (c) 2019 Guillaume Riby <guillaumeriby@gmail.com>
 *
 * GitHub repository - https://github.com/griby/ray-tracing-series
 *
 * A ray tracer implementation based on the Ray Tracing in One Weekend Book Series by Peter Shirley - https://raytracing.github.io/
 */

#include "lambertian.h"

#include "defines.h"
#include "hitable.h"
#include "ray.h"
#include "utils.h"

namespace rts
{
    bool Lambertian::scatter(const Ray& rIn, const HitRecord& rec, vec3& attenuation, Ray& scattered, Random& random) const
    {
        RTS_UNUSED(rIn);

        // Diffuse scattering: determine a new random target to bounce off the surface
        vec3 target = rec.p + rec.normal + getRandomPointInUnitSphere(random);
        scattered = Ray(rec.p, target - rec.p);

        attenuation = m_albedo;

        return true;
    }
}
