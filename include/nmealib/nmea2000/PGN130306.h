#pragma once

#include "nmealib/nmea2000.h"
#include "nmealib/nmea2000/dataTypes.h"

namespace nmealib {
namespace nmea2000 {

/**
 * @brief Strongly-typed class representing PGN 127250 - Wind Data
 * 
 * PGN 127250 is used to report the wind data, including wind speed, direction, and reference.
 * The message contains the following fields:
 * 
 * - Sequence ID (1 byte): A counter that increments with each new message instance.
 * 
 * - Wind Speed (2 bytes): The wind speed stored in an Speed dataType, in m/s [0, 655.35], resolution 1e-2 m/s.
 * 
 * - Wind Direction (2 bytes): The wind direction stored in an Angle dataType, in radians [0, 2π], resolution 1e-4 rad.
 * 
 * - Wind Reference (3 bits): An enumeration indicating the reference for the wind (0-True (north), 1-Magnetic(north), 2-Apparent, 3-True(boat), 4-True(water), 5/6/7-Error).
 * 
 * - Reserved (21 bits): Used for Alignment and future expansion, should be set to 0.
 * 
 */
class PGN130306 : public Message2000 {
public:
    // Public constructor from parameters
    PGN130306(uint8_t sequenceId,
              Speed windSpeed,
              Angle windDirection,
              HalfByte windReference
            ) noexcept;
    // TODO: Use only one constructor with default values for reserved fields, and remove the one with reserved parameters, since reserved fields should be set to 0 and not used by the user.
    // Accessory constructors that sets also reserved values
    PGN130306(uint8_t sequenceId,
              Speed windSpeed,
              Angle windDirection,
              HalfByte windReference,
              HalfByte reserved1,
              Byte reserved2,
              Byte reserved3
            ) noexcept;

    // Accessory constructors
    PGN130306(const PGN130306&) = default;
    PGN130306& operator=(const PGN130306&) = default;
    PGN130306(PGN130306&&) noexcept = default;
    PGN130306& operator=(PGN130306&&) noexcept = default;

    // Destructor
    ~PGN130306() override = default;

    // Polymorphic copy
    std::unique_ptr<nmealib::Message> clone() const override;

    // Getters for PGN 130306 specific fields
    /** @returns The sequence ID */
    uint8_t getSequenceId() const noexcept;
    /** @returns The wind speed in m/s */
    Speed getWindSpeed() const noexcept;
    /** @returns The wind direction in radians */
    Angle getWindDirection() const noexcept;
    /** @returns The wind reference */
    HalfByte getWindReference() const noexcept;
    HalfByte getReserved1() const noexcept;
    Byte getReserved2() const noexcept;
    Byte getReserved3() const noexcept;
    
    // Custom getters for convenience
    float getWindDirectionDegrees() const noexcept;
    float getWindSpeedKnots() const noexcept;

    // Overridden methods
    std::string getStringContent(bool verbose) const noexcept override;
    using Message2000::operator==;
    bool operator==(const PGN130306& other) const noexcept;

private:
    uint8_t sequenceId_{0};
    Speed windSpeed_;
    Angle windDirection_;
    HalfByte windReference_;
    HalfByte reserved1_ = HalfByte::fromValue(0U);
    Byte reserved2_ = Byte::fromValue(0U);
    Byte reserved3_ = Byte::fromValue(0U);

    PGN130306() = delete;

    // Private constructor used by the factory method
    PGN130306(Message2000 baseMessage,
              uint8_t sequenceId,
              Speed windSpeed,
              Angle windDirection,
              HalfByte windReference,
              HalfByte reserved1,
              Byte reserved2,
              Byte reserved3) noexcept;

    // Private internal factory
    static std::unique_ptr<PGN130306> create(std::unique_ptr<Message2000> baseMessage);
    static std::string rawPayload(uint8_t sequenceId,
                                    Speed windSpeed,
                                    Angle windDirection,
                                    HalfByte windReference,
                                    HalfByte reserved1,
                                    Byte reserved2,
                                    Byte reserved3);

    friend class Nmea2000Factory;
    friend class MessageRegistry;
};

} // namespace nmea2000
} // namespace nmealib