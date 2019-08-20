/**
 * MIT License
 * Copyright (c) 2019 Guillaume Riby <guillaumeriby@gmail.com>
 *
 * GitHub repository - https://github.com/griby/ray-tracing-series
 *
 * A ray tracer implementation based on the Ray Tracing in One Weekend Book Series by Peter Shirley - https://raytracing.github.io/
 */

#pragma once

#include <limits>
#include <string>

namespace rts // for ray tracing series
{
    // Image output path
    const std::string IMAGE_FILE_PATH = "output/image.ppm";

    // Image resolution
    const int IMAGE_WIDTH = 800;
    const int IMAGE_HEIGHT = 600;
    const float IMAGE_ASPECT_RATIO = static_cast<float>(IMAGE_WIDTH) / IMAGE_HEIGHT;

    // Ray tracing parameters
    const int RAY_COUNT_PER_PIXEL = 100;
    const int RAY_MAX_DEPTH = 50;

    // The shortest and longest ray length
    const float T_MIN = 0.001f;
    const float T_MAX = std::numeric_limits<float>::max();

    // Multithreading parameters
    const int SUBTASK_COUNT = 8;

    // Optimization
    // The following configuration parameter can be changed to speed up the image generation:
    // Project Properties > C/C++ > Code Generation > Floating Point Model
    // its value can be changed from Precise to Fast
}
