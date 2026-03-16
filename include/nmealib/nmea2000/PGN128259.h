#pragma once

#include "nmealib/nmea2000.h"
#include "nmealib/nmea2000/dataTypes.h"

namespace nmealib {
namespace nmea2000 {

/**
 * @brief Strongly-typed class representing PGN 128259 - Speed, Water Referenced.
 * 
 * PGN 128259 is used to report the water-referenced speed of a vessel, along with
 * the ground-referenced speed, the type of water speed sensor, and the direction of movement.
 * The message contains the following fields:
 * 
 * - Sequence ID (1 byte): A counter that increments with each new message instance.
 * 
 * - Speed Water Referenced (2 bytes): The speed of the vessel through the water,
 *  encoded as a 16-bit unsigned integer with a resolution of 0.01 m/s (range: 0-655.32 m/s).
 * 
 * - Speed Ground Referenced (2 bytes): The speed of the vessel over the ground,
 * encoded similarly to the water-referenced speed.
 * 
 * - Speed Water Referenced Type (1 byte): An enumeration indicating the type of sensor used for water speed measurement
 * (e.g., paddle wheel, pitot tube, Doppler).
 * 
 * - Speed Direction (4 bits): Indicates the direction of movement relative to the water (e.g., forward, reverse).
 * 
 * - Reserved (8 bits + 4 bits): Reserved for future use.
 * 
 */
class PGN128259 : public Message2000 {
public:
    // Public constructor from parameters
    PGN128259(uint8_t sequenceId,
              Speed speedWaterReferenced,
              Speed speedGroundReferenced,
              Byte speedWaterReferencedType,
              HalfByte speedDirection,
              Byte reserved1,
              HalfByte reserved2);

    // Accessory constructors
    PGN128259(const PGN128259&) = default;
    PGN128259& operator=(const PGN128259&) = default;
    PGN128259(PGN128259&&) noexcept = default;
    PGN128259& operator=(PGN128259&&) noexcept = default;

    // Destructor
    ~PGN128259() override = default;

    // Polymorphic copy
    std::unique_ptr<nmealib::Message> clone() const override;

    // Getters for PGN 128259 specific fields
    /** @returns the sequence ID. */
    uint8_t getSequenceId() const noexcept;
    /** @returns the water-referenced speed. */
    Speed getSpeedWaterReferenced() const noexcept;
    /** @returns the ground-referenced speed. */
    Speed getSpeedGroundReferenced() const noexcept;
    /** @returns the water-referenced speed type. */
    Byte getSpeedWaterReferencedType() const noexcept;
    /** @returns the speed direction. */
    HalfByte getSpeedDirection() const noexcept;
    Byte getReserved1() const noexcept;
    HalfByte getReserved2() const noexcept;

    // Overridden methods
    std::string getStringContent(bool verbose) const noexcept override;
    using Message2000::operator==;
    bool operator==(const PGN128259& other) const noexcept;

private:
    uint8_t sequenceId_{0};
    Speed speedWaterReferenced_;
    Speed speedGroundReferenced_;
    Byte speedWaterReferencedType_;
    HalfByte speedDirection_;
    Byte reserved1_;
    HalfByte reserved2_;

    PGN128259() = delete;

    // Private constructor used by the factory method
    PGN128259(Message2000 baseMessage,
              uint8_t sequenceId,
              Speed speedWaterReferenced,
              Speed speedGroundReferenced,
              Byte speedWaterReferencedType,
              HalfByte speedDirection,
              Byte reserved1,
              HalfByte reserved2) noexcept;

    // Private internal factory
    static std::unique_ptr<PGN128259> create(std::unique_ptr<Message2000> baseMessage);
    static std::string rawPayload(uint8_t sequenceId,
                                  Speed speedWaterReferenced,
                                  Speed speedGroundReferenced,
                                  Byte speedWaterReferencedType,
                                  HalfByte speedDirection,
                                  Byte reserved1,
                                  HalfByte reserved2);

    friend class Nmea2000Factory;
    friend class MessageRegistry;
};

} // namespace nmea2000
} // namespace nmealib