#pragma once

#include "../message.hpp"
#include <string>
#include <cstdint>

namespace nmealib {
namespace nmea0183 {

/**
 * @brief Represents an NMEA 0183 sentence.
 */
class Message0183 : public nmealib::Message {
public:
    Message0183() noexcept;
    explicit Message0183(std::string raw,
                         TimePoint ts = std::chrono::system_clock::now()) noexcept;
    ~Message0183() override = default;

    Message0183(const Message0183&) = default;
    Message0183& operator=(const Message0183&) = default;
    Message0183(Message0183&&) noexcept = default;
    Message0183& operator=(Message0183&&) noexcept = default;

    // Polymorphic copy
    std::unique_ptr<nmealib::Message> clone() const override;

    // Produce wire-format representation
    std::string serialize() const override;

    // Validate message contents (checksum, length, etc.)
    bool validate() const override;

    // Access the payload (without leading '$'/'!' and without trailing checksum or CRLF)
    std::string getPayload() const noexcept;

    // Return checksum part if present (two hex digits), empty otherwise
    std::string getChecksumStr() const noexcept;

    // Helper: compute XOR checksum over payload
    static std::uint8_t computeChecksum(const std::string& payload) noexcept;

private:
    static bool isHexByte(const std::string& s) noexcept;
};

} // namespace nmea0183
} // namespace nmealib