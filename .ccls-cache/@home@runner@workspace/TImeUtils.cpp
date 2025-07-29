// TimeUtils.cpp
#include "TimeUtils.h"
#include <ctime>
#include <iomanip>
#include <sstream>

namespace TimeUtils {

std::string timePointToString(const std::chrono::system_clock::time_point &tp) {
  std::time_t time = std::chrono::system_clock::to_time_t(tp);
  std::tm tm{};
#if defined(_WIN32) || defined(_WIN64)
  localtime_s(&tm, &time); // Windows
#else
  localtime_r(&time, &tm); // POSIX
#endif

  std::ostringstream oss;
  oss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
  return oss.str();
}

} // namespace TimeUtils
