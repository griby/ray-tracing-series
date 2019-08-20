/**
 * MIT License
 * Copyright (c) 2019 Guillaume Riby <guillaumeriby@gmail.com>
 *
 * GitHub repository - https://github.com/griby/ray-tracing-series
 *
 * A ray tracer implementation based on the Ray Tracing in One Weekend Book Series by Peter Shirley - https://raytracing.github.io/
 */

#pragma once

#include <chrono>

namespace rts // for ray tracing series
{
    class Timer final
    {
    public:
        Timer() : m_startTime() {}

        void setStartTime()
        {
            m_startTime = std::chrono::system_clock::now();
        }

        double getElapsedTime()
        {
            auto endTime = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsedSeconds = endTime - m_startTime;
            return elapsedSeconds.count();
        }

    private:
        std::chrono::system_clock::time_point m_startTime;
    };
}
