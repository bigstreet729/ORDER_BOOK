#pragma once
#include<chrono>
#include<string>

namespace TimeUtils {
    std::string timePointToString(const std::chrono::system_clock::time_point& tp);
}
