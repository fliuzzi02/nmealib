#pragma once

#include "nmealib/nmea2000.h"
#include "nmealib/nmea2000/dataTypes.h"

namespace nmealib {
namespace nmea2000 {

/**
 * @brief Strongly-typed class representing PGN 129026 - COG, SOG, Rapid Update
 * 
 * PGN 129026 is used to report COG and SOG updates
 * The message contains the following fields:
 * 
 * - sequenceId (1 byte): A counter that increments with each new message instance.
 * 
 * - COG Reference (2 bits): Indicates the reference for the COG value (0 = True, 1 = Magnetic, 2 = Error, 3 = Null).
 * 
 * - Reserved (6 bits): Reserved for future use, should be set to 0.
 * 
 * - Course Over Ground (2 bytes): Course over ground, stored in an Angle dataType, in radians [0, 2π], resolution 1e-4 rad.
 * 
 * - Speed Over Ground (2 bytes): Speed over ground, stored in a Speed dataType, in m/s [0, 655.32], resolution 1e-2 m/s.
 * 
 * - Reserved (2 Bytes): Reserved for future use, should be set to 0.
 * 
 */
class PGN129026 : public Message2000 {
public:
    // Public constructor from parameters
    PGN129026(uint8_t sequenceId,
              HalfByte cogReference,
              Angle cog,
              Speed sog
            ) noexcept;
    PGN129026(uint8_t sequenceId,
              HalfByte cogReference,
              Byte reserved,
              Angle cog,
              Speed sog,
              Byte reserved2,
              Byte reserved3
            ) noexcept;

    // Accessory constructors
    PGN129026(const PGN129026&) = default;
    PGN129026& operator=(const PGN129026&) = default;
    PGN129026(PGN129026&&) noexcept = default;
    PGN129026& operator=(PGN129026&&) noexcept = default;

    // Destructor
    ~PGN129026() override = default;

    // Polymorphic copy
    std::unique_ptr<nmealib::Message> clone() const override;

    // Getters for PGN 129026 specific fields
    /** @returns the sequence ID. */
    uint8_t getSequenceId() const noexcept;
    /** @returns the COG reference. */
    HalfByte getCogReference() const noexcept;
    /** @returns the course over ground in radians. */
    Angle getCog() const noexcept;
    /** @returns the speed over ground in m/s. */
    Speed getSog() const noexcept;
    /** @returns the reserved field. */
    Byte getReserved() const noexcept;
    /** @returns the second reserved field. */
    Byte getReserved2() const noexcept;
    /** @returns the third reserved field. */
    Byte getReserved3() const noexcept;

    // Custom getters for convenience
    /** @returns the course over ground in degrees. */
    float getCogDegrees() const noexcept;
    /** @returns the speed over ground in knots. */
    float getSogKnots() const noexcept;

    // Overridden methods
    std::string getStringContent(bool verbose) const noexcept override;
    using Message2000::operator==;
    bool operator==(const PGN129026& other) const noexcept;

private:
    uint8_t sequenceId_{0};
    HalfByte cogReference_;
    Angle cog_;
    Speed sog_;
    Byte reserved_ = Byte::fromValue(0U);
    Byte reserved2_ = Byte::fromValue(0U);
    Byte reserved3_ = Byte::fromValue(0U);

    PGN129026() = default;

    // Private constructor used by the factory method
    PGN129026(Message2000 baseMessage,
              uint8_t sequenceId,
              HalfByte cogReference,
              Byte reserved,
              Angle cog,
              Speed sog,
              Byte reserved2,
              Byte reserved3) noexcept;

    // Private internal factory
    static std::unique_ptr<PGN129026> create(std::unique_ptr<Message2000> baseMessage);
    static std::string rawPayload(uint8_t sequenceId,
                                  HalfByte cogReference,
                                  Byte reserved,
                                  Angle cog,
                                  Speed sog,
                                  Byte reserved2,
                                  Byte reserved3);

    friend class Nmea2000Factory;
    friend class MessageRegistry;
};

} // namespace nmea2000
} // namespace nmealib