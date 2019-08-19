#include "sphere.h"

#include "ray.h"

namespace rts
{
    bool Sphere::hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const
    {
        // Compute the discriminant as described in the comments at the end of this file
        // Note that a bunch of redundant "times 2" factors have been removed
        vec3 oc = r.origin() - center;
        float a = dot(r.direction(), r.direction());
        float b = dot(oc, r.direction());
        float c = dot(oc, oc) - radius * radius;
        float discriminant = b * b - a * c;

        // There's 2 real solutions to the quadratic equation
        if (discriminant > 0.f)
        {
            float discriminantSqrt = sqrt(discriminant);

            // First solution with the smallest t
            // the closest one to the camera if it's not behind it
            float t = (-b - discriminantSqrt) / a;
            if (tMin < t && t < tMax)
            {
                setHitRecord(rec, t, r, material.get());
                return true;
            }

            // Second solution
            t = (-b + discriminantSqrt) / a;
            if (tMin < t && t < tMax)
            {
                setHitRecord(rec, t, r, material.get());
                return true;
            }
        }

        return false;
    }

    void Sphere::setHitRecord(HitRecord& rec, float t, const Ray& r, const Material* material) const
    {
        rec.t = t;
        rec.p = r.pointAtParameter(rec.t);
        rec.normal = (rec.p - center) / radius;
        rec.matPtr = material;
    }

    // Previous hitSphere function which has been replaced by the Sphere:hit method (see above)
    // To determine if a ray hits a sphere consider this,
    // any point P that's on a sphere of radius R and center C must satisfy the following equation:
    //      (Px-Cx)*(Px-Cx) + (Py-Cy)*(Py-Cy) + (Pz-Cz)*(Pz-Cz) = R*R
    // which happens to be:
    //      dot(P-C, P-C) = R*R
    // we want to know if the ray (P(t) = A + t*B) ever hits the sphere,
    // in other words, if there's a point on the ray which satisfies this equation:
    //      dot(P(t)-C, P(t)-C) = R*R
    //      dot(A+t*B-C, A+t*B-C) = R*R
    // which can be rearranged into the following quadratic equation (a*x^2 + b*x + c = 0):
    //      t*t*dot(B, B) + 2*t*dot(B, A-C) + dot(A-C, A-C) - R*R = 0
    //      a = dot(B, B)
    //      b = 2*dot(B, A-C)
    //      c = dot(A-C, A-C) - R*R
    // where A is the ray's origin and B its direction, C is the center of the sphere and R its radius
    // solve the quadratic equation by computing the discriminant (b*b-4*a*c):
    //      if discriminant > 0, there's 2 real solutions which means the ray hits the sphere twice
    //      if discriminant = 0, there's only one real solution which means the ray hits the sphere only once (it is tangent to the sphere)
    //      if discriminant < 0, there's no real solution which means that the ray doesn't hit the sphere
    /*float hitSphere(const vec3& center, float radius, const Ray& r)
    {
        vec3 oc = r.origin() - center;
        float a = dot(r.direction(), r.direction());
        float b = 2.f * dot(oc, r.direction());
        float c = dot(oc, oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;

        if (discriminant < 0)
        {
            return -1.f;
        }
        else
        {
            // Let's assume the smallest solution value is the closest to the camera
            // if it's negative, it is behind the camera and will be ignored
            return (-b - sqrt(discriminant)) / (2.f * a);
        }
    }*/
}
