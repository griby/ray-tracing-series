
#include "vec3.h"

#include <iostream>

namespace rts
{
    // Member functions definition
    inline vec3& vec3::operator+=(const vec3& v)
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    inline vec3& vec3::operator-=(const vec3& v)
    {
        e[0] -= v.e[0];
        e[1] -= v.e[1];
        e[2] -= v.e[2];
        return *this;
    }

    inline vec3& vec3::operator*=(const vec3& v)
    {
        e[0] *= v.e[0];
        e[1] *= v.e[1];
        e[2] *= v.e[2];
        return *this;
    }

    inline vec3& vec3::operator/=(const vec3& v)
    {
        e[0] /= v.e[0];
        e[1] /= v.e[1];
        e[2] /= v.e[2];
        return *this;
    }

    inline vec3& vec3::operator*=(const float t)
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    inline vec3& vec3::operator/=(const float t)
    {
        e[0] /= t;
        e[1] /= t;
        e[2] /= t;
        return *this;
    }

    inline void vec3::makeUnitVector()
    {
        float k = 1.f / length();
        e[0] *= k;
        e[1] *= k;
        e[2] *= k;
    }

    // Non-member function definition
    inline vec3 operator+(const vec3& v1, const vec3& v2)
    {
        return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
    }

    inline vec3 operator-(const vec3& v1, const vec3& v2)
    {
        return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
    }

    inline vec3 operator*(const vec3& v1, const vec3& v2)
    {
        return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
    }

    inline vec3 operator/(const vec3& v1, const vec3& v2)
    {
        return vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
    }

    inline vec3 operator*(const vec3& v, float t)
    {
        return vec3(v.e[0] * t, v.e[1] * t, v.e[2] * t);
    }

    inline vec3 operator/(const vec3& v, float t)
    {
        return vec3(v.e[0] / t, v.e[1] / t, v.e[2] / t);
    }

    inline vec3 operator*(float t, const vec3& v)
    {
        return vec3(v.e[0] * t, v.e[1] * t, v.e[2] * t);
    }

    inline float dot(const vec3& v1, const vec3& v2)
    {
        return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
    }

    inline vec3 cross(const vec3& v1, const vec3& v2)
    {
        return vec3(
            (v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
            (-(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0])),
            (v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0])
        );
    }

    inline vec3 unitVector(const vec3& v)
    {
        return v / v.length();
    }

    inline std::istream& operator>>(std::istream& is, vec3& t)
    {
        is >> t.e[0] >> t.e[1] >> t.e[2];
        return is;
    }

    inline std::ostream& operator<<(std::ostream& os, const vec3& t)
    {
        os << t.e[0] << t.e[1] << t.e[2];
        return os;
    }
}
