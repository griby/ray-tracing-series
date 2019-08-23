/**
 * MIT License
 * Copyright (c) 2019 Guillaume Riby <guillaumeriby@gmail.com>
 *
 * GitHub repository - https://github.com/griby/ray-tracing-series
 *
 * A ray tracer implementation based on the Ray Tracing in One Weekend Book Series by Peter Shirley - https://raytracing.github.io/
 */

#pragma once

#include "vec3.h"

#include <limits>
#include <string>

namespace rts // for ray tracing series
{
    // Image
    const std::string IMAGE_FILE_PATH("output/image.ppm");
    const int IMAGE_WIDTH = 800;
    const int IMAGE_HEIGHT = 600;
    const float IMAGE_GAMMA_CORRECTION = 2.f;

    // Camera
    const float CAMERA_ASPECT_RATIO = static_cast<float>(IMAGE_WIDTH) / IMAGE_HEIGHT;
    const float CAMERA_FOV = 90.f;

    // Ray tracer
    const int RAY_COUNT_PER_PIXEL = 150;
    const int RAY_DEPTH_MAX = 20;
    const float RAY_LENGTH_MIN = 0.001f;
    const float RAY_LENGTH_MAX = std::numeric_limits<float>::max();

    // Multithreading
    const int MULTITHREADING_SUBTASK_COUNT = 16;

    // World
    const bool WORLD_GENERATION_RANDOM = true;
    const vec3 WORLD_BACKGROUND_COLOR_TOP(0.5f, 0.7f, 1.f);
    const vec3 WORLD_BACKGROUND_COLOR_BOTTOM(1.f, 1.f, 1.f);

    // Optimization
    // The following configuration parameter can be changed to speed up the image generation:
    // Project Properties > C/C++ > Code Generation > Floating Point Model
    // its value can be changed from Precise to Fast
}
