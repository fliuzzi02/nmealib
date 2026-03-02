#pragma once

#include <nmealib/nmea2000.h>
#include <chrono>
#include <memory>
#include <string>

namespace nmealib {
namespace nmea2000 {

class Nmea2000Factory {
public:
    /**
     * @brief Create a Message2000 from a raw CAN frame string.
     * 
     * Supports multiple input formats:
     * - "CANID:data" (e.g., "18FD1234:0102030405060708")
     * - "0xCANID, 0xBB 0xCC ..." (e.g., "0x18FD1234, 0x01 0x02 0x03 ...")
     * - "0xCANID 0xBB 0xCC ..." (e.g., "0x18FD1234 0x01 0x02 0x03 ...")
     * - "CANID BB CC ..." (e.g., "18FD1234 01 02 03 ...")
     * 
     * @param raw The raw CAN frame string in any supported format.
     * @param ts Optional timestamp; defaults to the current system time.
     * @return std::unique_ptr<Message2000> A unique pointer to the created Message2000 instance.
     */
    static std::unique_ptr<Message2000> create(const std::string& raw, Message::TimePoint ts = std::chrono::system_clock::now());
};

} // namespace nmea2000
} // namespace nmealib
