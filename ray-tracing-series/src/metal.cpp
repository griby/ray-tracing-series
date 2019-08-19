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
        scattered = Ray(rec.p, reflected + fuzz * getRandomPointInUnitSphere(random));

        attenuation = albedo;

        // Due to the fuzz factor or grazing rays we may scatter below the surface
        // in that case absorb the scattered ray by returning false
        return dot(scattered.direction(), rec.normal) > 0.f;
    }
}
