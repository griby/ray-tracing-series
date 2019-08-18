#pragma once

#include <array>
#include <tuple>

#include "config.h"
#include "vec3.h"

namespace rts // for ray tracing series
{
    class Camera;
    class HitableList;
    class Random;
    class Ray;

    // Generate a random point in a unit sphere
    vec3 getRandomPointInUnitSphere(Random& random);

    // Find the color for the given ray
    vec3 getColor(const Ray& r, const HitableList& world, Random& random);

    // Declare aliases for the image data
    using Color = std::tuple<int, int, int>;
    using ImageData = std::array<Color, IMAGE_WIDTH* IMAGE_HEIGHT>;

    // The ray tracing sub task which takes care of updating the image lines in the range [startLine, endLine)
    void rayTracingSubTask(const Camera& camera, const HitableList& world, ImageData* imageData, int startLine, int endLine, int taskId);

    // The ray tracing main task which spawns multiple ray tracing sub tasks
    void rayTracingMainTask(const Camera& camera, const HitableList& world, ImageData* imageData);
}
