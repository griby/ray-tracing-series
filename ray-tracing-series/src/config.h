#pragma once

#include <limits>
#include <string>

namespace rts // for ray tracing series
{
    // The following configuration defines can be set in the preprocessor definitions:
    // Project Properties > C/C++ > Preprocessor > Preprocessor Definitions
    //  * THREADING_ON = [0|1]
    //  * DETERMINISTIC_EXECUTION = [0|1]
    //  * NORMAL_MAP_COLOR = [0|1]
    //  * RAY_TRACING_LOG = [0|1]

    // Image output path
    const std::string IMAGE_FILE_PATH = "output/image.ppm";

    // Image resolution
    const int IMAGE_WIDTH = 800;
    const int IMAGE_HEIGHT = 600;
    const float IMAGE_ASPECT_RATIO = static_cast<float>(IMAGE_WIDTH) / IMAGE_HEIGHT;

    // Sampling per pixel
    const int SAMPLES_PER_PIXEL = 100;

    // The shortest and longest ray length
    const float T_MIN = 0.001f;
    const float T_MAX = std::numeric_limits<float>::max();

    // Number of concurrent tasks
    const int NUMBER_OF_TASKS = 8;
}
