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
