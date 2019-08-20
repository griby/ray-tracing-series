#pragma once

#include <limits>
#include <string>

namespace rts // for ray tracing series
{
    // The following configuration defines can be added to the preprocessor definitions:
    // Project Properties > C/C++ > Preprocessor > Preprocessor Definitions
    //  * MULTITHREADING_ON         // To activate the multithreading support
    //  * MULTITHREADING_LOGS       // To display logs related to multithreading
    //  * DETERMINISTIC_RNG         // To render identical images given the same input (fixed random seeds even in multithread)
    //  * RENDER_NORMAL_MAP         // To render the normal map of the scene
    //  * RENDER_NO_MATERIAL        // To render the image ignoring the objects material
    //  * RENDER_GRAYSCALE          // To render the grayscale image of the scene

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
