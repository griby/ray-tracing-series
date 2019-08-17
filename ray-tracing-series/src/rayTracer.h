#pragma once

#include <array>
#include <tuple>

#include "config.h"
#include "vec3.h"

namespace rts // for ray tracing series
{
    class Camera;
    class HitableList;
    class Ray;

    vec3 getRandomPointInUnitSphere();

    vec3 getColor(const Ray& r, const HitableList& world);

    // Declare aliases for the image data
    using Color = std::tuple<int, int, int>;
    using ImageData = std::array<Color, IMAGE_WIDTH* IMAGE_HEIGHT>;

    void performRayTracing(const Camera& camera, const HitableList& world, ImageData* imageData);
}
