#pragma once

#include "nmea0183.hpp"
#include "rmc.hpp"

namespace nmealib {
namespace nmea0183 {

class Nmea0183Factory {
public:
    static std::unique_ptr<Message0183> create(const std::string& raw, Message::TimePoint ts = std::chrono::system_clock::now());
};

} // namespace nmea0183
} // namespace nmealib