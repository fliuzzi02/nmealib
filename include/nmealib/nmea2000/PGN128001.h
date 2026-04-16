#pragma once

#include "nmealib/nmea2000.h"
#include "nmealib/nmea2000/dataTypes.h"

namespace nmealib {
namespace nmea2000 {

/**
 * @brief Strongly-typed class representing PGN 128001 - Vessel Acceleration
 * 
 * PGN 128001 is used to report the linear acceleraions of the vessel.
 * The message contains the following fields:
 * 
 * - Sequence ID (1 byte): A counter that increments with each new message instance.
 * 
 * - Longitudinal Acceleration (2 bytes): Acceleration of the vessel in the longitudinal direction, stored in an Acceleration dataType, in m/s² [-327.64, 327.64], resolution 0.01 m/s².
 * 
 * - Transverse Acceleration (2 bytes): Acceleration of the vessel in the transverse direction, stored in an Acceleration dataType, in m/s² [-327.64, 327.64], resolution 0.01 m/s².
 * 
 * - Vertical Acceleration (2 bytes): Acceleration of the vessel in the vertical direction, stored in an Acceleration dataType, in m/s² [-327.64, 327.64], resolution 0.01 m/s².
 * 
 * - Reserved (1 byte): Used for Alignment and future expansion, should be set to 1.
 * 
 */
class PGN128001 : public Message2000 {
public:
    // Public constructor from parameters
    PGN128001(uint8_t sequenceId,
              Acceleration longitudinalAcceleration,
              Acceleration transverseAcceleration,
              Acceleration verticalAcceleration,
              Byte reserved = Byte::fromRaw(255)) noexcept;

    // Accessory constructors
    PGN128001(const PGN128001&) = default;
    PGN128001& operator=(const PGN128001&) = default;
    PGN128001(PGN128001&&) noexcept = default;
    PGN128001& operator=(PGN128001&&) noexcept = default;

    // Destructor
    ~PGN128001() override = default;

    // Polymorphic copy
    std::unique_ptr<nmealib::Message> clone() const override;

    // Getters for PGN 128001 specific fields
    /** @returns the sequence ID. */
    uint8_t getSequenceId() const noexcept;
    /** @returns the longitudinal acceleration in m/s². */
    Acceleration getLongitudinalAcceleration() const noexcept;
    /** @returns the transverse acceleration in m/s². */
    Acceleration getTransverseAcceleration() const noexcept;
    /** @returns the vertical acceleration in m/s². */
    Acceleration getVerticalAcceleration() const noexcept;

    float getLongitudinalAccelerationG() const noexcept;
    float getTransverseAccelerationG() const noexcept;
    float getVerticalAccelerationG() const noexcept;

    // Overridden methods
    std::string getStringContent(bool verbose) const noexcept override;
    using Message2000::operator==;
    bool operator==(const PGN128001& other) const noexcept;

private:
    uint8_t sequenceId_{0};
    Acceleration longitudinalAcceleration_;
    Acceleration transverseAcceleration_;
    Acceleration verticalAcceleration_;
    Byte reserved_;

    PGN128001() = delete;

    // Private constructor used by the factory method
    PGN128001(Message2000 baseMessage,
              uint8_t sequenceId,
              Acceleration longitudinalAcceleration,
              Acceleration transverseAcceleration,
              Acceleration verticalAcceleration,
              Byte reserved) noexcept;

    // Private internal factory
    static std::unique_ptr<PGN128001> create(std::unique_ptr<Message2000> baseMessage);
    static std::string rawPayload(uint8_t sequenceId,
                                  Acceleration longitudinalAcceleration,
                                  Acceleration transverseAcceleration,
                                  Acceleration verticalAcceleration,
                                  Byte reserved);

    friend class Nmea2000Factory;
    friend class MessageRegistry;
};

} // namespace nmea2000
} // namespace nmealib