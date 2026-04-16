#pragma once

#include "nmealib/nmea2000.h"
#include "nmealib/nmea2000/dataTypes.h"

namespace nmealib {
namespace nmea2000 {

/**
 * @brief Strongly-typed class representing PGN 127251 - Rate of Turn
 * 
 * PGN 127251 is used to report the rate of turn of the vessel.
 * The message contains the following fields:
 * 
 * - Sequence ID (1 byte): A counter that increments with each new message instance.
 * 
 * - Rate (4 Bytes): Rate of turn, stored as a AngularRate data type
 * 
 * - Reserved (3 bytes): Reserved for future use; should be set to 1 when sending and ignored when receiving.
 * 
 */
class PGN127251 : public Message2000 {
public:
    // Public constructor from parameters, of the reserved field, only the 6 LSBits are considered
    PGN127251(uint8_t sequenceId,
              AngularRate rate,
              Byte reserved1 = Byte::fromValue(0U),
              Byte reserved2 = Byte::fromValue(0U),
              Byte reserved3 = Byte::fromValue(0U)) noexcept;

    // Accessory constructors
    PGN127251(const PGN127251&) = default;
    PGN127251& operator=(const PGN127251&) = default;
    PGN127251(PGN127251&&) noexcept = default;
    PGN127251& operator=(PGN127251&&) noexcept = default;

    // Destructor
    ~PGN127251() override = default;

    // Polymorphic copy
    std::unique_ptr<nmealib::Message> clone() const override;

    // Getters for PGN 127251 specific fields
    /** @returns the sequence ID. */
    uint8_t getSequenceId() const noexcept;
    /** @returns the rate of turn. */
    AngularRate getRate() const noexcept;
    /** @returns the reserved byte 1. */
    Byte getReserved1() const noexcept;
    /** @returns the reserved byte 2. */
    Byte getReserved2() const noexcept;
    /** @returns the reserved byte 3. */
    Byte getReserved3() const noexcept;

    // Overridden methods
    std::string getStringContent(bool verbose) const noexcept override;
    using Message2000::operator==;
    bool operator==(const PGN127251& other) const noexcept;

private:
    uint8_t sequenceId_{0};
    AngularRate rate_;
    Byte reserved1_;
    Byte reserved2_;
    Byte reserved3_;

    PGN127251() = delete;

    // Private constructor used by the factory method
    PGN127251(Message2000 baseMessage,
              uint8_t sequenceId,
              AngularRate rate,
              Byte reserved1,
              Byte reserved2,
              Byte reserved3) noexcept;

    // Private internal factory
    static std::unique_ptr<PGN127251> create(std::unique_ptr<Message2000> baseMessage);
    static std::string rawPayload(uint8_t sequenceId,
                                  AngularRate rate,
                                  Byte reserved1,
                                  Byte reserved2,
                                  Byte reserved3) noexcept;

    friend class Nmea2000Factory;
    friend class MessageRegistry;
};

} // namespace nmea2000
} // namespace nmealib