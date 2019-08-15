#pragma once

#include <math.h>
#include <iosfwd>

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
        inline vec3 operator-() const { return vec3(e[0], e[1], e[2]); }
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

        // Additional non-member functions
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

    // Additional non-member function, this one doesn't need to access the vec3 class members
    inline vec3 unitVector(const vec3& v);
}
