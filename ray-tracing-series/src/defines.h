/**
 * MIT License
 * Copyright (c) 2019 Guillaume Riby <guillaumeriby@gmail.com>
 *
 * GitHub repository - https://github.com/griby/ray-tracing-series
 *
 * A ray tracer implementation based on the Ray Tracing in One Weekend Book Series by Peter Shirley - https://raytracing.github.io/
 */

#pragma once

 // To get a global access to M_PI
#define _USE_MATH_DEFINES
#include <math.h>

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

#define RTS_UNUSED(var) (void)(sizeof(var))
}
