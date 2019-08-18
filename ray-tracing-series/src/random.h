#pragma once

#include <random>

namespace rts // for ray tracing series
{
    class Random
    {
    public:
        // Initialize the random number generator
        Random(unsigned int customSeed = DEFAULT_SEED)
            : dist(0.f, 1.f) // distribute the results in [0, 1)
#if DETERMINISTIC_RNG
            // initialize the mersenne twister engine with a constant
            , gen((customSeed != DEFAULT_SEED) ? customSeed + DEFAULT_SEED : DEFAULT_SEED)
#endif
        {
#if !DETERMINISTIC_RNG
            std::random_device rd;                  // create a random device to seed the pseudo-random generator
            gen = std::mt19937(customSeed + rd());  // initialize the mersenne twister engine with a random seed (we could also use the clock)
#endif
        }

        // Return a random float in [0, 1)
        float get() { return dist(gen); }

    private:
        std::uniform_real_distribution<float> dist;
        std::mt19937 gen;

        static const unsigned int DEFAULT_SEED = std::mt19937::default_seed;
    };
}
