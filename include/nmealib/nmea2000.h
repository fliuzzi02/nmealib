#pragma once

#include "message.h"
#include "nmeaException.h"

#include <chrono>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace nmealib {
namespace nmea2000 {

/**
 * @brief Exception thrown when an NMEA 2000 PGN is outside the valid range.
 */
class InvalidPgnException : public NmeaException {
public:
    explicit InvalidPgnException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "NMEA 2000 PGN is invalid", details) {}
};

/**
 * @brief Exception thrown when the CAN frame is invalid.
 */
class InvalidCanFrameException : public NmeaException {
public:
    explicit InvalidCanFrameException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "NMEA 2000 CAN frame is invalid", details) {}
};

/**
 * @brief Exception thrown when a CAN frame exceeds the maximum length of 223 bytes.
 */
class FrameTooLongException : public NmeaException {
public:
    explicit FrameTooLongException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "NMEA 2000 CAN frame exceeds maximum length of 223 bytes", details) {}
};

/**
 * @brief Represents a generic NMEA 2000 message encapsulating a CAN frame.
 * 
 * An NMEA 2000 message is built on the CAN protocol with:
 * - A CAN identifier (29-bit extended ID) that encodes the PGN and other info
 * - Up to 223 bytes of data payload (single-frame: 0-8 bytes, multi-frame/fast-packet: 0-223 bytes)
 * - The PGN (Parameter Group Number) extracted from the CAN ID
 * 
 * This implementation supports any 18-bit PGN and both single-frame and fast-packet frames.
 */
class Message2000 : public nmealib::Message {
public:
    /**
     * @brief Copy constructor.
     */
    Message2000(const Message2000&) = default;

    /**
     * @brief Copy assignment operator.
     */
    Message2000& operator=(const Message2000&) = default;

    /**
     * @brief Move constructor.
     */
    Message2000(Message2000&&) noexcept = default;

    /**
     * @brief Move assignment operator.
     */
    Message2000& operator=(Message2000&&) noexcept = default;

    /**
     * @brief Destructor.
     */
    ~Message2000() override = default;

    /**
     * @brief Creates a polymorphic deep copy of this Message2000.
     *
     * @return std::unique_ptr<nmealib::Message> A unique pointer to the cloned message.
     */
    std::unique_ptr<nmealib::Message> clone() const override;

    /**
     * @brief Returns the PGN (Parameter Group Number) of this message.
     *
     * @return uint32_t The PGN value (0-262143).
     */
    uint32_t getPgn() const noexcept;

    /**
     * @brief Returns the raw CAN frame data (up to 8 bytes).
     *
     * @return const std::vector<uint8_t>& A const reference to the CAN frame data.
     */
    const std::vector<uint8_t>& getCanFrame() const noexcept;

    /**
     * @brief Returns the length of the CAN frame in bytes.
     *
     * @return uint8_t The CAN frame data length (0-8).
     */
    uint8_t getCanFrameLength() const noexcept;

    /**
     * @brief Returns a human-readable string representation of the message content.
     *
     * @param verbose Selects whether to print a one-liner or a more detailed multi-line string.
     * @return std::string The string representation of the message content.
     */
    virtual std::string getStringContent(bool verbose) const noexcept;

    /**
     * @brief Returns the wire-format representation of the NMEA 2000 message.
     *
     * @return std::string The serialized CAN frame as a hex string.
     */
    std::string serialize() const override;

    /**
     * @brief Compares two Message2000 objects for equality.
     *
     * @param other The other Message2000 object to compare with.
     * @return true  If all fields and the base Message data are equal.
     * @return false Otherwise.
     */
    virtual bool operator==(const Message2000& other) const noexcept;

    /**
     * @brief Returns whether the message is valid.
     *
     * @return true If the message is valid (PGN in valid range, frame length 0-8).
     * @return false Otherwise.
     */
    bool validate() const override;

protected:
    uint32_t pgn_{0};                           ///< Parameter Group Number (0-262143)
    std::vector<uint8_t> canFrame_;             ///< CAN frame data (0-8 bytes)

    /**
     * @brief Protected factory method to create a Message2000 from a raw CAN frame string.
     *
     * Parses the raw CAN frame string in the format "CANID:data" where CANID is the 
     * 29-bit CAN identifier in hex format and data is the frame payload in hex format.
     * Validates the frame format and extracts the PGN. Intended to be called from
     * Nmea2000Factory.
     *
     * @param raw The raw CAN frame string in format "CANID:data" (e.g., "18FF1234:AABBCCDD").
     * @param ts Optional timestamp; defaults to the current system time.
     * @return std::unique_ptr<Message2000> A unique pointer to the created Message2000 instance.
     * @throws InvalidCanFrameException If the raw format is invalid.
     * @throws InvalidPgnException If the PGN is outside the valid range.
     * @throws FrameTooLongException If the frame exceeds 223 bytes.
     */
    static std::unique_ptr<Message2000> create(std::string raw, 
                                               TimePoint ts = std::chrono::system_clock::now());

private:
    explicit Message2000(std::string raw,
                        TimePoint ts,
                        uint32_t pgn,
                        std::vector<uint8_t> canFrame) noexcept;

    /**
     * @brief Extracts the PGN from the CAN identifier.
     *
     * @param canId The 29-bit CAN identifier.
     * @return uint32_t The extracted PGN.
     */
    static uint32_t extractPgnFromCanId(uint32_t canId) noexcept;

    /**
     * @brief Validates the PGN fits within 18 bits (valid NMEA 2000 PGN range).
     *
     * @param pgn The PGN to validate.
     * @return true If the PGN is valid (0x000000-0x3FFFF).
     * @return false Otherwise.
     */
    static bool isValidPgn(uint32_t pgn) noexcept;

    friend class Nmea2000Factory;
};

} // namespace nmea2000
} // namespace nmealib
