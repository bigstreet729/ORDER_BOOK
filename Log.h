#ifndef LOG_H
#define LOG_H

#include <iostream>

// #def ENABLE_LOGGING
#ifdef ENABLE_LOGGING

inline void LOG() {
    std::cout << std::endl;
}

template<typename T, typename... Args>
inline void LOG(const T& first, const Args&... rest) {
    std::cout << first;
    if constexpr (sizeof...(rest) > 0) {
        std::cout << " ";
        LOG(rest...);
    } else {
        std::cout << std::endl;
    }
}
#else

template<typename... Args>
inline void LOG(const Args&...) {}
#endif
#endif 
