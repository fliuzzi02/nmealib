#pragma once

#include "nmealib/nmea2000.h"
#include "nmealib/nmea2000/dataTypes.h"

namespace nmealib {
namespace nmea2000 {

/**
 * @brief Strongly-typed class representing PGN 129025 - Position, Rapid Update
 * 
 * PGN 129025 is used to report rapid position updates
 * The message contains the following fields:
 * 
 * - Latitude(4 bytes): The latitude stored in a Latitude dataType, in degrees [-90, 90], resolution 1e-7 deg.
 * 
 * - Longitude(4 bytes): The longitude stored in a Longitude dataType, in degrees [-180, 180], resolution 1e-7 deg.
 * 
 */
class PGN129025 : public Message2000 {
public:
    // Public constructor from parameters, of the reserved field, only the 6 LSBits are considered
    PGN129025(Latitude latitude,
              Longitude longitude) noexcept;

    // Accessory constructors
    PGN129025(const PGN129025&) = default;
    PGN129025& operator=(const PGN129025&) = default;
    PGN129025(PGN129025&&) noexcept = default;
    PGN129025& operator=(PGN129025&&) noexcept = default;

    // Destructor
    ~PGN129025() override = default;

    // Polymorphic copy
    std::unique_ptr<nmealib::Message> clone() const override;

    // Getters for PGN 129025 specific fields
    /** @returns the latitude. */
    Latitude getLatitude() const noexcept;
    /** @returns the longitude. */
    Longitude getLongitude() const noexcept;

    // Overridden methods
    std::string getStringContent(bool verbose) const noexcept override;
    using Message2000::operator==;
    bool operator==(const PGN129025& other) const noexcept;

private:
    Latitude latitude_;
    Longitude longitude_;

    PGN129025() = default;

    // Private constructor used by the factory method
    PGN129025(Message2000 baseMessage,
              Latitude latitude,
              Longitude longitude) noexcept;

    // Private internal factory
    static std::unique_ptr<PGN129025> create(std::unique_ptr<Message2000> baseMessage);
    static std::string rawPayload(Latitude latitude,
                                  Longitude longitude);

    friend class Nmea2000Factory;
    friend class MessageRegistry;
};

} // namespace nmea2000
} // namespace nmealib