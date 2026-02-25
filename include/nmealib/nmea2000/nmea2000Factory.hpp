#pragma once

#include <nmealib/nmea2000.hpp>
#include <chrono>
#include <memory>
#include <string>

namespace nmealib {
namespace nmea2000 {

class Nmea2000Factory {
public:
    static std::unique_ptr<Message2000> create(const std::string& raw, Message::TimePoint ts = std::chrono::system_clock::now());
};

} // namespace nmea2000
} // namespace nmealib
