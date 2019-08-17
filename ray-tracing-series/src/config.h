#pragma once

#include <limits>
#include <string>

namespace rts // for ray tracing series
{
    // The configuration defines as set in the preprocessor definitions:
    // Project Properties > C/C++ > Preprocessor > Preprocessor Definitions
    //  * DETERMINISTIC_EXECUTION=1
    //  * NORMAL_MAP_COLOR=0

    // Image output path
    const std::string IMAGE_FILE_PATH = "output/image.ppm";

    // Image resolution
    const int IMAGE_WIDTH = 200;
    const int IMAGE_HEIGHT = 100;
    const float IMAGE_ASPECT_RATIO = static_cast<float>(IMAGE_WIDTH) / IMAGE_HEIGHT;

    // Sampling per pixel
    const int SAMPLES_PER_PIXEL = 100;

    // The shortest and longest ray length
    const float T_MIN = 0.001f;
    const float T_MAX = std::numeric_limits<float>::max();
}
