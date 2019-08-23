/**
 * MIT License
 * Copyright (c) 2019 Guillaume Riby <guillaumeriby@gmail.com>
 *
 * GitHub repository - https://github.com/griby/ray-tracing-series
 *
 * A ray tracer implementation based on the Ray Tracing in One Weekend Book Series by Peter Shirley - https://raytracing.github.io/
 */

#include "dielectric.h"

#include <cmath>

#include "hitable.h"
#include "random.h"
#include "ray.h"
#include "utils.h"

namespace rts
{
    bool Dielectric::scatter(const Ray& rIn, const HitRecord& rec, vec3& attenuation, Ray& scattered, Random& random) const
    {
        attenuation = m_albedo;

        // Dielectric scattering: Determine the outward normal and the refraction indexes ratio
        vec3 outwardNormal;
        float refIdxRatio;
        vec3 unitDirection = unitVector(rIn.direction());
        float dt = dot(unitDirection, rec.normal);
        if (dt > 0.f)
        {
            outwardNormal = -rec.normal;
            refIdxRatio = m_refIdx;
        }
        else
        {
            outwardNormal = rec.normal;
            refIdxRatio = 1.f / m_refIdx;
        }

        // Determine the reflection probability
        vec3 refracted;
        float reflectProb = 1.f;
        if (getRefractedVector(unitDirection, outwardNormal, refIdxRatio, refracted))
        {
            // Compute the reflection probability
            if (dt > 0.f)
            {
                // Previous computation of cosine as described in the book (it's bugged!)
                //cosine = m_refIdx * dt / rIn.direction().length();

                // Compute the cosine to pass to Schlick's approximation function as fixed by the following post
                // http://psgraphics.blogspot.com/2016/03/my-buggy-implimentation-of-schlick.html

                // it's the cosine of the greatest of the 2 angles involved in the refraction, regardless of which direction the ray is travelling in
                // in this case the ray is travelling from inside the sphere and towards the outside,
                // the angle of exit will be greater since air's refraction index is lower than the material's

                // dt is the cosine of the incoming angle (the smallest of the 2 angles)
                // compute the cosine of the exiting angle
                float discriminant = 1.f - m_refIdx * m_refIdx * (1.f - dt * dt);

                // Proceed with the Schlick approximation only if the discriminant is positive
                // when it's negative it means that there's total internal reflection
                if (discriminant > 0.f)
                {
                    reflectProb = getSchlickApproximation(sqrt(discriminant), m_refIdx);
                }
            }
            else
            {
                reflectProb = getSchlickApproximation(-dt, m_refIdx);
            }
        }

        // Reflect or refract depending on the reflection probability
        if (reflectProb == 1.f || random.get() < reflectProb)
        {
            vec3 reflected = getReflectedVector(rIn.direction(), rec.normal);
            scattered = Ray(rec.p, reflected);
        }
        else
        {
            scattered = Ray(rec.p, refracted);
        }
        
        return true;
    }
}
