#pragma once

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
