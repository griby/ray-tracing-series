#pragma once

#include <random>

namespace rts // for ray tracing series
{
    class Random
    {
    public:
        // Initialize the random number generator
        Random(unsigned int customSeed = DEFAULT_SEED)
            : m_dist(0.f, 1.f) // distribute the results in [0, 1)
#ifdef DETERMINISTIC_RNG
            // initialize the mersenne twister engine with a constant
            , m_gen((customSeed != DEFAULT_SEED) ? customSeed + DEFAULT_SEED : DEFAULT_SEED)
#endif // DETERMINISTIC_RNG
        {
#ifndef DETERMINISTIC_RNG
            std::random_device rd;                      // create a random device to seed the pseudo-random generator
            m_gen = std::mt19937(customSeed + rd());    // initialize the mersenne twister engine with a random seed (we could also use the clock)
#endif // !DETERMINISTIC_RNG
        }

        // Return a random float in [0, 1)
        float get() { return m_dist(m_gen); }

    private:
        std::uniform_real_distribution<float> m_dist;
        std::mt19937 m_gen;

        static const unsigned int DEFAULT_SEED = std::mt19937::default_seed;
    };
}
