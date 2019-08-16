
// Ray Tracing in One Weekend Book Series by Peter Shirley

#include <iostream>
#include <fstream>

#include "vec3.h"
#include "ray.h"

using namespace rts;

const std::string IMAGE_FILE_PATH = "output/image.ppm";

// to determine if a ray hits a sphere consider this,
// any point P that's on a sphere of radius R and center C must satisfy the following equation:
//      (Px-Cx)*(Px-Cx) + (Py-Cy)*(Py-Cy) + (Pz-Cz)*(Pz-Cz) = R*R
// which happens to be:
//      dot(P-C, P-C) = R*R
// we want to determine if the ray (P(t) = A + t*B) ever hits the sphere,
// in other words, if there's a point on the ray which satisfies this equation:
//      dot(P(t)-C, P(t)-C) = R*R
//      dot(A+t*B-C, A+t*B-C) = R*R
// which can be rearranged into the following quadratic equation:
//      t*t*dot(B, B) + 2*t*dot(B, A-C) + dot(A-C, A-C) - R*R = 0
bool hitSphere(const vec3& center, float radius, const ray& r)
{
    // solve the quadratic equation (a*x^2 + b*x + c = 0)
    //      t*t*dot(B, B) + 2*t*dot(B, A-C) + dot(A-C, A-C) - R*R = 0
    // where A is the ray's origin and B its direction, C is the center of the sphere and R its radius
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = 2.f * dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;

    // if the discriminant if greater than zero the ray hits the sphere (twice)
    // at zero the ray is only tangent to the sphere (it hits it once)
    return discriminant > 0;
}

vec3 color(const ray& r)
{
    // check if the ray hits a sphere at the center of the screen
    if (hitSphere(vec3(0.f, 0.f, -1.f), 0.5f, r))
    {
        return vec3(1.f, 0.f, 0.f);
    }

    // unitDirection Y is between -1 and +1
    vec3 unitDirection = unitVector(r.direction());

    // scale unitDirection Y between 0 and +1
    float t = 0.5f * (unitDirection.y() + 1.f);

    // blend between white and blue depending on the direction Y
    return (1.f - t) * vec3(1.f, 1.f, 1.f) + t * vec3(0.5f, 0.7f, 1.f);
}

int main()
{
    std::ofstream imageFile(IMAGE_FILE_PATH);
    if (imageFile.is_open())
    {
        int nx = 200;
        int ny = 100;

        imageFile << "P3" << std::endl;
        imageFile << nx << " " << ny << std::endl;
        imageFile << "255" << std::endl;

        vec3 lowerLeftCorner(-2.f, -1.f, -1.f);
        vec3 horizontal(4.f, 0.f, 0.f);
        vec3 vertical(0.f, 2.f, 0.f);
        vec3 origin(0.f, 0.f, 0.f);

        // From top to bottom
        for (int j = ny - 1; j >= 0; --j)
        {
            // From left to right
            for (int i = 0; i < nx; ++i)
            {
                float u = float(i) / float(nx);
                float v = float(j) / float(ny);

                // Create a ray between the camera (origin) and the current pixel (offset from the lower-left corner)
                ray r(origin, lowerLeftCorner + u * horizontal + v * vertical);
                vec3 c = color(r);

                int ir = int(255.99f * c[0]);
                int ig = int(255.99f * c[1]);
                int ib = int(255.99f * c[2]);

                imageFile << ir << " " << ig << " " << ib << std::endl;
            }
        }

        imageFile.close();
    }

    return 0;
}
