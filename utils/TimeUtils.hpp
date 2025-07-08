#ifndef TIME_UTILS_HPP
#define TIME_UTILS_HPP
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

inline std::string getCurrentIsoTimestampUTC() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    std::tm utc_tm;
    gmtime_r(&now_time, &utc_tm);  // thread-safe version of gmtime

    std::ostringstream oss;
    oss << std::put_time(&utc_tm, "%Y-%m-%dT%H:%M:%SZ");
    return oss.str();
}
#endif  // TIME_UTILS_HPP
