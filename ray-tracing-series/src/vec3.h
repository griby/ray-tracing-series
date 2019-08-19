#pragma once

#include <cmath>
#include <iostream>
#include <math.h>

namespace rts // for ray tracing series
{
    class vec3
    {
    public:
        vec3() { e[0] = e[1] = e[2] = 0.f; }
        vec3(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }

        // X, Y, Z accessors
        inline float x() const { return e[0]; }
        inline float y() const { return e[1]; }
        inline float z() const { return e[2]; }

        // R, G, B accessors
        inline float r() const { return e[0]; }
        inline float g() const { return e[1]; }
        inline float b() const { return e[2]; }

        inline const vec3& operator+() const { return *this; }
        inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
        inline float operator[](int i) const { return e[i]; }
        inline float& operator[](int i) { return e[i]; }

        inline vec3& operator+=(const vec3& v);
        inline vec3& operator-=(const vec3& v);
        inline vec3& operator*=(const vec3& v);
        inline vec3& operator/=(const vec3& v);
        inline vec3& operator*=(const float t);
        inline vec3& operator/=(const float t);

        inline float squaredLength() const { return (e[0] * e[0]) + (e[1] * e[1]) + (e[2] * e[2]); }
        inline float length() const { return static_cast<float>(sqrt(squaredLength())); }
        inline void makeUnitVector();

    private:
        float e[3];

        // Additional non-member functions declaration
        friend inline vec3 operator+(const vec3& v1, const vec3& v2);
        friend inline vec3 operator-(const vec3& v1, const vec3& v2);
        friend inline vec3 operator*(const vec3& v1, const vec3& v2);
        friend inline vec3 operator/(const vec3& v1, const vec3& v2);
        friend inline vec3 operator*(const vec3& v, float t);
        friend inline vec3 operator/(const vec3& v, float t);
        friend inline vec3 operator*(float t, const vec3& v);

        friend inline float dot(const vec3& v1, const vec3& v2);
        friend inline vec3 cross(const vec3& v1, const vec3& v2);

        friend inline std::istream& operator>>(std::istream& is, vec3& t);
        friend inline std::ostream& operator<<(std::ostream& os, const vec3& t);
    };

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

    // Additional non-member functions definition
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

    // Additional non-member function definition, this one doesn't need to access the vec3 class members
    inline vec3 unitVector(const vec3& v)
    {
        return v / v.length();
    }
}
