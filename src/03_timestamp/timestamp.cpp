#include "timestamp.hpp"
#include <iomanip>
#include <chrono>
#include <ctime>
#include <iostream>

std::string get_timestamp() {

    const std::chrono::time_point<std::chrono::system_clock> now =
          std::chrono::system_clock::now();

    const std::time_t t_c = std::chrono::system_clock::to_time_t(now);

    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>
                        (now.time_since_epoch()).count();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>
                        (now.time_since_epoch()).count();

    std::stringstream ss;
    ss << std::put_time(std::localtime(&t_c), "%F %T.") << milliseconds - seconds * 1000;

    std::string result = ss.str();    

    return result;
}

