#pragma once

#include <nmealib/nmea0183.hpp>
#include <chrono>
#include <memory>
#include <string>

namespace nmealib {
namespace nmea0183 {

/**
 * @brief Factory for creating typed NMEA 0183 message objects from raw sentence strings.
 *
 * The factory parses the sentence type field of the raw input and returns the most
 * specific concrete type available (e.g., GGA, RMC, GLL ...). If no specific type
 * matches, a base Message0183 is returned.
 */
class Nmea0183Factory {
public:
    /**
     * @brief Creates a typed NMEA 0183 message from a raw sentence string.
     *
     * @param raw The raw NMEA 0183 sentence string to parse.
     * @param ts  Optional timestamp to associate with the message; defaults to the current system time.
     * @return std::unique_ptr<Message0183> A unique pointer to the most specific message type for the sentence.
     * @throws TooLongSentenceException If the input string exceeds the maximum allowed length of 82 characters.
     * @throws InvalidStartCharacterException If the input string does not start with either '$' or '!'.
     */
    static std::unique_ptr<Message0183> create(const std::string& raw, Message::TimePoint ts = std::chrono::system_clock::now());
};

} // namespace nmea0183
} // namespace nmealib
