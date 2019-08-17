#pragma once

#include <chrono>

namespace rts // for ray tracing series
{
    class Timer
    {
    public:
        Timer() : startTime() {}

        void setStartTime()
        {
            startTime = std::chrono::system_clock::now();
        }

        double getElapsedTime()
        {
            auto endTime = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsedSeconds = endTime - startTime;
            return elapsedSeconds.count();
        }

    private:
        std::chrono::system_clock::time_point startTime;
    };
}
