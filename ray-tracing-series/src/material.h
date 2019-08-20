#pragma once

namespace rts // for ray tracing series
{
    class Random;
    class Ray;
    class vec3;
    struct HitRecord;

    class Material
    {
    public:
        virtual ~Material() {}

        virtual bool scatter(const Ray& rIn, const HitRecord& rec, vec3& attenuation, Ray& scattered, Random& random) const = 0;
    };
}
