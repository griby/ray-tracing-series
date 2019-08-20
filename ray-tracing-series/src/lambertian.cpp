#include "lambertian.h"

#include "hitable.h"
#include "ray.h"
#include "rayTracer.h"

namespace rts
{
    bool Lambertian::scatter(const Ray& rIn, const HitRecord& rec, vec3& attenuation, Ray& scattered, Random& random) const
    {
        // Diffuse scattering: determine a new random target to bounce off the surface
        vec3 target = rec.p + rec.normal + getRandomPointInUnitSphere(random);
        scattered = Ray(rec.p, target - rec.p);

        attenuation = m_albedo;

        return true;
    }
}
