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
 * An NMEA 2000 message is built on the CAN 2.0B protocol (29-bit extended identifier).
 * The 29-bit CAN Id is stored as 4 bytes (big-endian, value right-aligned):
 *
 *   canId_[0]: [ 0   0   0  P3  P2  P1  R1  DP ]
 *   canId_[1]: [PF8 PF7 PF6 PF5 PF4 PF3 PF2 PF1]   ← PDU Format
 *   canId_[2]: [PS8 PS7 PS6 PS5 PS4 PS3 PS2 PS1]    ← PDU Specific
 *   canId_[3]: [SA8 SA7 SA6 SA5 SA4 SA3 SA2 SA1]    ← Source Address
 *
 * PGN extraction follows the ISO 11783 / NMEA 2000 rules:
 *   - PDU1 (PF < 0xF0): PS is the destination address; PGN = [RDP][PF][00]
 *   - PDU2 (PF >= 0xF0): destination is global (255);  PGN = [RDP][PF][PS]
 *
 * RTR and DLC are CAN bus framing fields managed by hardware/drivers and are
 * NOT part of the 29-bit CAN Id; they are not stored here.
 *
 * Payload: up to 223 bytes (single-frame: 0-8 bytes, fast-packet: 0-223 bytes).
 */
class Message2000 : public nmealib::Message {
public:
    Message2000(const Message2000&)            = default;
    Message2000& operator=(const Message2000&) = default;
    Message2000(Message2000&&) noexcept        = default;
    Message2000& operator=(Message2000&&)      = default;
    ~Message2000() override                    = default;

    /**
     * @brief Creates a polymorphic deep copy of this Message2000.
     */
    std::unique_ptr<nmealib::Message> clone() const override;

    // -------------------------------------------------------------------------
    // Raw data accessors
    // -------------------------------------------------------------------------

    /**
     * @brief Returns the 29-bit CAN identifier as 4 bytes (big-endian).
     */
    const std::vector<uint8_t>& getCanId() const noexcept;

    /**
     * @brief Returns the raw CAN frame payload (0-223 bytes).
     */
    const std::vector<uint8_t>& getCanFrame() const noexcept;

    /**
     * @brief Returns the number of bytes in the CAN frame payload.
     */
    uint8_t getCanFrameLength() const noexcept;

    // -------------------------------------------------------------------------
    // CAN Id field accessors
    //
    // canId_[0]: [ 0   0   0  P3  P2  P1  R1  DP ]
    // canId_[1]: PDU Format  (PF8..PF1)
    // canId_[2]: PDU Specific (PS8..PS1)
    // canId_[3]: Source Address (SA8..SA1)
    // -------------------------------------------------------------------------

    /**
     * @brief Returns the 3-bit message priority (0 = highest, 7 = lowest).
     */
    uint8_t getPriority()  const noexcept;

    /**
     * @brief Returns individual priority bits (P3 is the MSB).
     */
    bool getPriority3() const noexcept;  ///< Bit 4 of canId_[0]
    bool getPriority2() const noexcept;  ///< Bit 3 of canId_[0]
    bool getPriority1() const noexcept;  ///< Bit 2 of canId_[0]

    /**
     * @brief Returns the Reserved bit (R1). Always 0 in NMEA 2000; non-zero in J1939.
     */
    bool getReserved()  const noexcept;  ///< Bit 1 of canId_[0]

    /**
     * @brief Returns the Data Page bit (DP).
     */
    bool getDataPage()  const noexcept;  ///< Bit 0 of canId_[0]

    /**
     * @brief Returns the PDU Format byte (PF).
     *
     * Determines addressing mode:
     *   PF < 0xF0  → PDU1 (addressed): canId_[2] is the destination address.
     *   PF >= 0xF0 → PDU2 (broadcast): canId_[2] is the PGN group extension.
     */
    uint8_t getPDUFormat()   const noexcept;  ///< canId_[1]

    /**
     * @brief Returns the PDU Specific byte (PS).
     *
     * Interpretation depends on PDU Format (see getPDUFormat()).
     */
    uint8_t getPDUSpecific() const noexcept;  ///< canId_[2]

    /**
     * @brief Returns the source address of the transmitting device.
     *
     * 0-253 = valid node address, 254 = anonymous, 255 = broadcast/global.
     */
    uint8_t getSourceAddress()      const noexcept;  ///< canId_[3]

    /**
     * @brief Returns the destination address.
     *
     * For PDU1 messages (PF < 0xF0) this is getPDUSpecific().
     * For PDU2 messages the destination is always 255 (global).
     */
    uint8_t getDestinationAddress() const noexcept;

    // NOTE: getRemoteTransmissionRequest() and getDataLengthCode() have been
    // removed. RTR and DLC are CAN bus framing fields handled by hardware/drivers
    // and are not encoded in the 29-bit CAN Id.

    // -------------------------------------------------------------------------
    // PGN accessor
    // -------------------------------------------------------------------------

    /**
     * @brief Returns the PGN (Parameter Group Number) extracted from the CAN Id.
     *
     * The PGN fits in 18 bits (0x00000 - 0x3FFFF).
     */
    uint32_t getPgn() const noexcept;

    // -------------------------------------------------------------------------
    // Output
    // -------------------------------------------------------------------------

    /**
     * @brief Returns a human-readable string representation of the message.
     *
     * Verbose: multi-line dump of all fields.
     * Non-verbose: single-line summary.
     */
    virtual std::string getStringContent(bool verbose) const noexcept;

    /**
     * @brief Serializes the message to "CANID:data" hex format.
     */
    std::string serialize() const override;

    // -------------------------------------------------------------------------
    // Comparison and validation
    // -------------------------------------------------------------------------

    virtual bool operator==(const Message2000& other) const noexcept;

    /**
     * @brief Returns true if the PGN fits in 18 bits and the frame is 0-223 bytes.
     */
    bool validate() const override;

protected:
    std::vector<uint8_t> canId_;     ///< 29-bit CAN Id stored as 4 bytes (big-endian)
    std::vector<uint8_t> canFrame_;  ///< CAN frame payload (0-223 bytes)

    /**
     * @brief Protected factory — parses "CANID:data" (and variant formats) into a Message2000.
     *
     * @throws InvalidCanFrameException If the format is unrecognized.
     * @throws InvalidPgnException      If the extracted PGN exceeds 18 bits.
     * @throws FrameTooLongException    If the payload exceeds 223 bytes.
     */
    static std::unique_ptr<Message2000> create(std::string raw,
                                               TimePoint ts = std::chrono::system_clock::now());

    /**
     * @brief Returns the base string common to all PGNs (used by getStringContent).
     */
    std::string toString(bool verbose) const noexcept;

private:
    explicit Message2000(std::string raw,
                         TimePoint ts,
                         std::vector<uint8_t> canId,
                         std::vector<uint8_t> canFrame) noexcept;

    /**
     * @brief Extracts the PGN from the 4-byte canId vector.
     *
     * Applies the PDU1 / PDU2 distinction per ISO 11783 / NMEA 2000.
     */
    static uint32_t extractPgnFromCanId(const std::vector<uint8_t>& canId) noexcept;

    /**
     * @brief Returns true if the PGN fits within 18 bits (0x00000 - 0x3FFFF).
     */
    static bool isValidPgn(uint32_t pgn) noexcept;

    friend class Nmea2000Factory;
};

} // namespace nmea2000
} // namespace nmealib