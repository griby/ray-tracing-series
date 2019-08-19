#include "dielectric.h"

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

            // TODO instead of normalizing here, we should get a unit vector out of rIn.direction() and pass it to getRefracted
            cosine = refIdx * dt / rIn.direction().length(); // TODO schlick's approximation cosine shouldn't involve the refraction index...
        }
        else
        {
            outwardNormal = rec.normal;
            niOverNt = 1.f / refIdx;

            // TODO instead of normalizing here, we should get a unit vector out of rIn.direction() and pass it to getRefracted
            cosine = -dt / rIn.direction().length();
        }

        // Determine the reflection probability
        vec3 refracted;
        float reflectProb;
        if (getRefracted(rIn.direction(), outwardNormal, niOverNt, refracted))
        {
            reflectProb = getSchlick(cosine, refIdx);
        }
        else
        {
            reflectProb = 1.f;
        }

        // Reflect or refract depending on the reflection probability
        if (random.get() < reflectProb)
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
