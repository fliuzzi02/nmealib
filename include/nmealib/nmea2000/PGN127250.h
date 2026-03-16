#pragma once

#include "nmealib/nmea2000.h"
#include "nmealib/nmea2000/dataTypes.h"

namespace nmealib {
namespace nmea2000 {

/**
 * @brief Strongly-typed class representing PGN 127250 - Heading
 * 
 * PGN 127250 is used to report the heading of the vessel.
 * The message contains the following fields:
 * 
 * - Sequence ID (1 byte): A counter that increments with each new message instance.
 * 
 * - Heading (2 bytes): The heading stored in an Angle dataType, in radians [0, 2π], resolution 1e-4 rad.
 * 
 * - Deviation (2 bytes): The deviation stored in a Signed Angle dataType, in radians [-π, π], resolution 1e-4 rad.
 * 
 * - Variation (2 byte): The variation stored in a Signed Angle dataType, in radians [-π, π], resolution 1e-4 rad.
 * 
 * - Heading Reference (4 bits): An enumeration indicating the reference for the heading sensor (0-True, 1-Magnetic, 2-Error, 3-Null).
 * 
 * - Reserved (6bits): Used for Alignment and future expansion, should be set to 0.
 * 
 */
class PGN127250 : public Message2000 {
public:
    // Public constructor from parameters, of the reserved field, only the 6 LSBits are considered
    PGN127250(uint8_t sequenceId,
              Angle heading,
              SignedAngle deviation,
              SignedAngle variation,
              HalfByte headingReference,
              Byte reserved) noexcept;

    // Accessory constructors
    PGN127250(const PGN127250&) = default;
    PGN127250& operator=(const PGN127250&) = default;
    PGN127250(PGN127250&&) noexcept = default;
    PGN127250& operator=(PGN127250&&) noexcept = default;

    // Destructor
    ~PGN127250() override = default;

    // Polymorphic copy
    std::unique_ptr<nmealib::Message> clone() const override;

    // Getters for PGN 127250 specific fields
    /** @returns the sequence ID. */
    uint8_t getSequenceId() const noexcept;
    /** @returns the heading in radians. */
    Angle getHeading() const noexcept;
    /** @returns the deviation in radians. */
    SignedAngle getDeviation() const noexcept;
    /** @returns the variation in radians. */
    SignedAngle getVariation() const noexcept;
    /** @returns the heading reference. */
    HalfByte getHeadingReference() const noexcept;

    float getHeadingDegrees() const noexcept;
    float getDeviationDegrees() const noexcept;
    float getVariationDegrees() const noexcept;

    // Overridden methods
    std::string getStringContent(bool verbose) const noexcept override;
    using Message2000::operator==;
    bool operator==(const PGN127250& other) const noexcept;

private:
    uint8_t sequenceId_{0};
    Angle heading_;
    SignedAngle deviation_;
    SignedAngle variation_;
    HalfByte headingReference_;
    Byte reserved_;

    PGN127250() = delete;

    // Private constructor used by the factory method
    PGN127250(Message2000 baseMessage,
              uint8_t sequenceId,
              Angle heading,
              SignedAngle deviation,
              SignedAngle variation,
              HalfByte headingReference,
              Byte reserved) noexcept;

    // Private internal factory
    static std::unique_ptr<PGN127250> create(std::unique_ptr<Message2000> baseMessage);
    static std::string rawPayload(uint8_t sequenceId,
                                  Angle heading,
                                  SignedAngle deviation,
                                  SignedAngle variation,
                                  HalfByte headingReference,
                                  Byte reserved);

    friend class Nmea2000Factory;
    friend class MessageRegistry;
};

} // namespace nmea2000
} // namespace nmealib