#pragma once

#include "nmealib/nmea2000.h"
#include "nmealib/nmea2000/dataTypes.h"

namespace nmealib {
namespace nmea2000 {

/**
 * @brief Strongly-typed class representing PGN 127257 - Attitude
 * 
 * PGN 127257 is used to report vessel attitude.
 * The message contains the following fields:
 * 
 * - Sequence ID (1 byte): A counter that increments with each new message instance.
 * 
 * - Yaw (2 bytes): Stored in a SignedAngle dataType, in radians [-pi, pi], resolution 1e-4 rad.
 * 
 * - Pitch (2 bytes): Stored in a SignedAngle dataType, in radians [-pi, pi], resolution 1e-4 rad.
 * 
 * - Roll (2 bytes): Stored in a SignedAngle dataType, in radians [-pi, pi], resolution 1e-4 rad.
 * 
 * - Reserved (1 byte): Reserved for future use.
 * 
 */
class PGN127257 : public Message2000 {
public:
    // Public constructor from parameters.
    PGN127257(uint8_t sequenceId,
              SignedAngle yaw,
              SignedAngle pitch,
              SignedAngle roll,
              Byte reserved) noexcept;

    // Accessory constructors
    PGN127257(const PGN127257&) = default;
    PGN127257& operator=(const PGN127257&) = default;
    PGN127257(PGN127257&&) noexcept = default;
    PGN127257& operator=(PGN127257&&) noexcept = default;

    // Destructor
    ~PGN127257() override = default;

    // Polymorphic copy
    std::unique_ptr<nmealib::Message> clone() const override;

    // Getters for PGN 127257 specific fields
    /** @returns the sequence ID. */
    uint8_t getSequenceId() const noexcept;
    /** @returns the yaw in radians. */
    SignedAngle getYaw() const noexcept;
    /** @returns the pitch in radians. */
    SignedAngle getPitch() const noexcept;
    /** @returns the roll in radians. */
    SignedAngle getRoll() const noexcept;
    /** @returns reserved byte. */
    Byte getReserved() const noexcept;

    float getYawDegrees() const noexcept;
    float getPitchDegrees() const noexcept;
    float getRollDegrees() const noexcept;

    // Overridden methods
    std::string getStringContent(bool verbose) const noexcept override;
    using Message2000::operator==;
    bool operator==(const PGN127257& other) const noexcept;

private:
    uint8_t sequenceId_{0};
    SignedAngle yaw_;
    SignedAngle pitch_;
    SignedAngle roll_;
    Byte reserved_;

    PGN127257() = delete;

    // Private constructor used by the factory method
    PGN127257(Message2000 baseMessage,
              uint8_t sequenceId,
              SignedAngle yaw,
              SignedAngle pitch,
              SignedAngle roll,
              Byte reserved) noexcept;

    // Private internal factory
    static std::unique_ptr<PGN127257> create(std::unique_ptr<Message2000> baseMessage);
    static std::string rawPayload(uint8_t sequenceId,
                                  SignedAngle yaw,
                                  SignedAngle pitch,
                                  SignedAngle roll,
                                  Byte reserved);

    friend class Nmea2000Factory;
    friend class MessageRegistry;
};

} // namespace nmea2000
} // namespace nmealib