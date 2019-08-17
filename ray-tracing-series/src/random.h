#pragma once

#include <random>

namespace rts // for ray tracing series
{
    class Random
    {
    public:
        // Initialize the random number generator
        Random()
            : dist(0.f, 1.f)    // distribute the results in [0, 1)
#if DETERMINISTIC_EXECUTION
            , gen()             // initialize the mersenne twister engine without a seed so that it falls back on a default constant
#endif
        {
#if !DETERMINISTIC_EXECUTION
            std::random_device rd;      // create a random device to seed the pseudo-random generator
            gen = std::mt19937(rd());   // initialize the mersenne twister engine with a random seed (we could also use the clock)
#endif
        }

        // Return a random float in [0, 1)
        float get() { return dist(gen); }

    private:
        std::uniform_real_distribution<float> dist;
        std::mt19937 gen;
    };
}
