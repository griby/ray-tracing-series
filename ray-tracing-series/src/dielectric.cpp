#include "dielectric.h"

#include <cmath>

#include "hitable.h"
#include "random.h"
#include "ray.h"
#include "rayTracer.h"

namespace rts
{
    bool Dielectric::scatter(const Ray& rIn, const HitRecord& rec, vec3& attenuation, Ray& scattered, Random& random) const
    {
        // Dielectric scattering...
        attenuation = vec3(1.f, 1.f, 1.f); // TODO could be a member of the Dielectric class to affect its tint

        // Determine the outward normal and the refraction ratio
        vec3 outwardNormal;
        float niOverNt;
        float cosine;
        float dt = dot(rIn.direction(), rec.normal);
        if (dt > 0.f)
        {
            outwardNormal = -rec.normal;
            niOverNt = refIdx;
        }
        else
        {
            outwardNormal = rec.normal;
            niOverNt = 1.f / refIdx;
        }

        // Determine the reflection probability
        vec3 refracted;
        float reflectProb = 1.f;
        if (getRefracted(rIn.direction(), outwardNormal, niOverNt, refracted))
        {
            // Compute cosine here, where it's needed
            if (dt > 0.f)
            {
                // Previous computation of cosine as described in the book (it's bugged!)
                //cosine = refIdx * dt / rIn.direction().length();

                // Compute the cosine to pass to Schlick's approximation function as fixed by the following post
                // http://psgraphics.blogspot.com/2016/03/my-buggy-implimentation-of-schlick.html

                // it's the cosine of the greatest of the 2 angles involved in the refraction, regardless of which direction the ray is travelling in
                // in this case the ray is travelling from inside the sphere and towards the outside,
                // the angle of exit will be greater since air's refraction index is lower than the material's

                // TODO instead of normalizing here, we should get a unit vector out of rIn.direction() and pass it to getRefracted
                cosine = dt / rIn.direction().length(); // this is the cosine of the incoming angle (the smallest of the 2 angles)
                float discriminant = 1.f - refIdx * refIdx * (1.f - cosine * cosine);

                // Proceed with the Schlick approximation only if the discriminant is positive
                // when it's negative it means that there's total internal reflection
                if (discriminant > 0.f)
                {
                    cosine = sqrt(discriminant);
                    reflectProb = getSchlick(cosine, refIdx);
                }
            }
            else
            {
                // TODO instead of normalizing here, we should get a unit vector out of rIn.direction() and pass it to getRefracted
                cosine = -dt / rIn.direction().length();
                reflectProb = getSchlick(cosine, refIdx);
            }
        }

        // Reflect or refract depending on the reflection probability
        if (reflectProb == 1.f || random.get() < reflectProb)
        {
            vec3 reflected = getReflected(rIn.direction(), rec.normal);
            scattered = Ray(rec.p, reflected);
        }
        else
        {
            scattered = Ray(rec.p, refracted);
        }
        
        return true;
    }
}
