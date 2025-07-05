#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <iostream>
#include <sstream>
#include <mutex>

inline std::mutex coutMutex;

inline void threadSafeLog(const std::string& message) {
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << message << std::endl;
}

#endif // LOGGING_HPP
