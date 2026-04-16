#pragma once

#include "nmealib/nmea2000.h"
#include "nmealib/nmea2000/dataTypes.h"

namespace nmealib {
namespace nmea2000 {

/**
 * @brief Strongly-typed class representing PGN 127245 - Rudder
 * 
 * PGN 127245 is used to report vessel rudder data.
 * The message contains the following fields:
 * 
 * - Rudder ID (1 byte):
 * 
 * - Direction (3 bits): 000 (0) = none, 001 (1) = starboard, 010 (2) = port
 * 
 * - Reserved (5 bits): Reserved for future use.
 * 
 * - Angle Order (2 bytes): Stored in a SignedAngle dataType, in radians [-pi, pi], resolution 1e-4 rad.
 * 
 * - Position (2 bytes): Stored in a SignedAngle dataType, in radians [-pi, pi], resolution 1e-4 rad.
 * 
 * - Reserved (2 bytes): Reserved for future use, set to 1.
 * 
 */
class PGN127245 : public Message2000 {
public:
    // Public constructor from parameters.
    PGN127245(uint8_t rudderId,
              HalfByte direction,
              SignedAngle angleOrder,
              SignedAngle position,
              Byte reserved = Byte::fromValue(0U),
              Byte reserved2 = Byte::fromRaw(255U),
              Byte reserved3 = Byte::fromRaw(255U)) noexcept;

    // Accessory constructors
    PGN127245(const PGN127245&) = default;
    PGN127245& operator=(const PGN127245&) = default;
    PGN127245(PGN127245&&) noexcept = default;
    PGN127245& operator=(PGN127245&&) noexcept = default;

    // Destructor
    ~PGN127245() override = default;

    // Polymorphic copy
    std::unique_ptr<nmealib::Message> clone() const override;

    // Getters for PGN 127245 specific fields
    /** @returns the rudder ID. */
    uint8_t getRudderId() const noexcept;
    /** @returns the direction (0=none, 1=starboard, 2=port). */
    HalfByte getDirection() const noexcept;
    /** @returns the angle order in radians. */
    SignedAngle getAngleOrder() const noexcept;
    /** @returns the position in radians. */
    SignedAngle getPosition() const noexcept;

    float getAngleOrderDegrees() const noexcept;
    float getPositionDegrees() const noexcept;
    std::string getDirectionString() const noexcept;

    // Overridden methods
    std::string getStringContent(bool verbose) const noexcept override;
    using Message2000::operator==;
    bool operator==(const PGN127245& other) const noexcept;

private:
    uint8_t rudderId_{0};
    HalfByte direction_;
    Byte reserved_; // Only 5 bits considered
    SignedAngle angleOrder_;
    SignedAngle position_;
    Byte reserved2;
    Byte reserved3;

    PGN127245() = delete;

    // Private constructor used by the factory method
    PGN127245(Message2000 baseMessage,
              uint8_t rudderId,
              HalfByte direction,
              Byte reserved,
              SignedAngle angleOrder,
              SignedAngle position,
              Byte reserved2,
              Byte reserved3) noexcept;

    // Private internal factory
    static std::unique_ptr<PGN127245> create(std::unique_ptr<Message2000> baseMessage);
    static std::string rawPayload(uint8_t rudderId,
                                    HalfByte direction,
                                    Byte reserved,
                                    SignedAngle angleOrder,
                                    SignedAngle position,
                                    Byte reserved2,
                                    Byte reserved3);

    friend class Nmea2000Factory;
    friend class MessageRegistry;
};

} // namespace nmea2000
} // namespace nmealib