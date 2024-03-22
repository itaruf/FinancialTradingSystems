#pragma once

#include <chrono>

// Helper class for time-centric helper functions
class TimeHelper
{
public:
    static long long get_timestamps_in_microseconds()
    {
        // in microseconds
        auto now = std::chrono::high_resolution_clock::now();
        auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();

        return microseconds;
    }
};