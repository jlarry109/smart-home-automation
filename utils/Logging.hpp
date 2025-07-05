#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <iostream>
#include <mutex>

inline std::mutex coutMutex;

#define THREAD_SAFE_COUT(x) \
    { std::lock_guard<std::mutex> lock(coutMutex); std::cout << x << std::endl; }

#endif // LOGGING_HPP
