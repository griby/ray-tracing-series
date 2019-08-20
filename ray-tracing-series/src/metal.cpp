/**
 * MIT License
 * Copyright (c) 2019 Guillaume Riby <guillaumeriby@gmail.com>
 *
 * GitHub repository - https://github.com/griby/ray-tracing-series
 *
 * A ray tracer implementation based on the Ray Tracing in One Weekend Book Series by Peter Shirley - https://raytracing.github.io/
 */

#include "metal.h"

#include "hitable.h"
#include "ray.h"
#include "rayTracer.h"

namespace rts
{
    bool Metal::scatter(const Ray& rIn, const HitRecord& rec, vec3& attenuation, Ray& scattered, Random& random) const
    {
        // Metallic scattering: determine a new target to bounce off the surface
        // the fuzziness adds some noise to the reflected vector
        vec3 reflected = getReflected(unitVector(rIn.direction()), rec.normal);
        scattered = Ray(rec.p, reflected + m_fuzz * getRandomPointInUnitSphere(random));

        attenuation = m_albedo;

        // Due to the fuzz factor or grazing rays we may scatter below the surface
        // in that case absorb the scattered ray by returning false
        return dot(scattered.direction(), rec.normal) > 0.f;
    }
}
