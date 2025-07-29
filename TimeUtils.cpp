// TimeUtils.cpp
#include "TimeUtils.h"
#include <ctime>
#include <iomanip>
#include <sstream>

namespace TimeUtils {

std::string timePointToString(const std::chrono::system_clock::time_point &tp) {
  // Extract time_t seconds since epoch
  std::time_t time = std::chrono::system_clock::to_time_t(tp);
  std::tm tm{};
#if defined(_WIN32) || defined(_WIN64)
  localtime_s(&tm, &time); // Windows
#else
  localtime_r(&time, &tm); // POSIX
#endif

  // Calculate the nanoseconds part
  auto duration = tp.time_since_epoch();
  auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
  auto nanoseconds =
      std::chrono::duration_cast<std::chrono::nanoseconds>(duration - seconds)
          .count();

  std::ostringstream oss;
  oss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S") << '.' << std::setw(9)
      << std::setfill('0') << nanoseconds; // pad with zeros to 9 digits

  return oss.str();
}

} // namespace TimeUtils
