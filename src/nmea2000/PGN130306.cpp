#include "nmealib/nmea2000/PGN130306.h"

#include "nmealib/detail/errorSupport.h"

namespace nmealib {
namespace nmea2000 {

std::unique_ptr<PGN130306> PGN130306::create(std::unique_ptr<Message2000> baseMessage) {
    std::string context = "PGN130306::create()";

    if (baseMessage->getCanFrameLength() != 8) {
        NMEALIB_RETURN_ERROR(InvalidCanFrameException(context, "CAN frame must be 8 bytes for PGN130306"));
    }

    uint8_t sequenceId = baseMessage->getCanFrame()[0];
    Speed windSpeed = Speed::fromRaw(baseMessage->getCanFrame()[1] | (baseMessage->getCanFrame()[2] << 8));
    Angle windDirection = Angle::fromRaw(baseMessage->getCanFrame()[3] | (baseMessage->getCanFrame()[4] << 8));
    HalfByte windReference = HalfByte::fromRaw(baseMessage->getCanFrame()[5] & 0x07);
    HalfByte reserved1 = HalfByte::fromRaw((baseMessage->getCanFrame()[5] >> 3) & 0x1F);
    Byte reserved2 = Byte::fromRaw(baseMessage->getCanFrame()[6]);
    Byte reserved3 = Byte::fromRaw(baseMessage->getCanFrame()[7]);

    return std::unique_ptr<PGN130306>(new PGN130306(std::move(*baseMessage),
                                                    sequenceId,
                                                    windSpeed,
                                                    windDirection,
                                                    windReference,
                                                    reserved1,
                                                    reserved2,
                                                    reserved3));
}

PGN130306::PGN130306(Message2000 baseMessage,
                    uint8_t sequenceId,
                    Speed windSpeed,
                    Angle windDirection,
                    HalfByte windReference,
                    HalfByte reserved1,
                    Byte reserved2,
                    Byte reserved3) noexcept : 
                    Message2000(std::move(baseMessage)),
                    sequenceId_(sequenceId),
                    windSpeed_(windSpeed),
                    windDirection_(windDirection),
                    windReference_(windReference),
                    reserved1_(reserved1),
                    reserved2_(reserved2),
                    reserved3_(reserved3) {}

PGN130306::PGN130306(uint8_t sequenceId,
                    Speed windSpeed,
                    Angle windDirection,
                    HalfByte windReference,
                    HalfByte reserved1,
                    Byte reserved2,
                    Byte reserved3) noexcept :
                    Message2000(*Message2000::create(rawPayload(sequenceId, 
                        windSpeed, 
                        windDirection, 
                        windReference, 
                        reserved1, 
                        reserved2, 
                        reserved3))),
                    sequenceId_(sequenceId),
                    windSpeed_(windSpeed),
                    windDirection_(windDirection),
                    windReference_(windReference),
                    reserved1_(reserved1),
                    reserved2_(reserved2),
                    reserved3_(reserved3) {}

PGN130306::PGN130306(uint8_t sequenceId,
                    Speed windSpeed,
                    Angle windDirection,
                    HalfByte windReference) noexcept :
                    PGN130306(sequenceId, windSpeed, windDirection, windReference,
                              HalfByte::fromValue(0U), Byte::fromValue(0U), Byte::fromValue(0U)) {}

std::unique_ptr<Message> PGN130306::clone() const {
    return std::unique_ptr<PGN130306>(new PGN130306(*this));
}

std::string PGN130306::getStringContent(bool verbose) const noexcept {
    std::ostringstream oss;

    if (verbose) {
        oss << this->toString(true);
        oss << "\n";
        oss << "Fields:\n";
        oss << "\tSequence ID: " << static_cast<int>(sequenceId_) << "\n";
        oss << "\tWind Speed: " << windSpeed_.toString() << " knots" << "\n";
        oss << "\tWind Direction: " << windDirection_.toString() << "rad, " << getWindDirectionDegrees() << "°" << "\n";
        oss << "\tWind Reference: ";
            switch (windReference_.getValue()) {
                case 0: oss << "True(North)"; break;
                case 1: oss << "Magnetic(North)"; break;
                case 2: oss << "Apparent"; break;
                case 3: oss << "True(Boat)"; break;
                case 4: oss << "True(Water)"; break;
                default: oss << "Unavailable/Reserved"; break;
            }
        oss << "\n";
    } else {
        oss << this->toString(false);
        oss << "SeqID=" << static_cast<int>(sequenceId_)
            << " WindSpeed=" << windSpeed_.getValue() << " knots"
            << " WindDirection=" << getWindDirectionDegrees() << "°"
            << " WindReference=" << static_cast<int>(windReference_.getValue());
    }
    return oss.str();
}

std::string PGN130306::rawPayload(uint8_t sequenceId,
                            Speed windSpeed,
                            Angle windDirection,
                            HalfByte windReference,
                            HalfByte reserved1,
                            Byte reserved2,
                            Byte reserved3) {
    std::vector<uint8_t> canFrame(8, 0);
    canFrame[0] = sequenceId;
    canFrame[1] = windSpeed.getRaw() & 0xFF;
    canFrame[2] = (windSpeed.getRaw() >> 8) & 0xFF;
    canFrame[3] = windDirection.getRaw() & 0xFF;
    canFrame[4] = (windDirection.getRaw() >> 8) & 0xFF;
    // Byte 5: lower 3 bits = windReference, upper 5 bits = reserved1
    canFrame[5] = (reserved1.getRaw() << 3) | (windReference.getRaw() & 0x07);
    canFrame[6] = reserved2.getRaw();
    canFrame[7] = reserved3.getRaw();

    // Produce the "CANID:data" hex string expected by Message2000::create()
    // CAN ID encodes PGN 130306 (0x1F50A) in bits [24:8], source address 0
    const uint32_t canId = (130306U << 8U);  // 0x01F50A00
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::setfill('0');
    oss << std::setw(8) << canId << ":";
    for (uint8_t b : canFrame) {
        oss << std::setw(2) << static_cast<int>(b);
    }
    return oss.str();
}

// Getters
uint8_t PGN130306::getSequenceId() const noexcept {
    return sequenceId_;
}
Speed PGN130306::getWindSpeed() const noexcept {
    return windSpeed_;
}
Angle PGN130306::getWindDirection() const noexcept {
    return windDirection_;
}
HalfByte PGN130306::getWindReference() const noexcept {
    return windReference_;
}
HalfByte PGN130306::getReserved1() const noexcept {
    return reserved1_;
}
Byte PGN130306::getReserved2() const noexcept {
    return reserved2_;
}
Byte PGN130306::getReserved3() const noexcept {
    return reserved3_;
}
float PGN130306::getWindDirectionDegrees() const noexcept {
    return windDirection_.getValue() * 180.0f / static_cast<float>(M_PI);
}
float PGN130306::getWindSpeedKnots() const noexcept {
    return windSpeed_.getValue() * 1.94384f;
}

bool PGN130306::operator==(const PGN130306& other) const noexcept {
    return Message2000::operator==(other) &&
           sequenceId_ == other.sequenceId_ &&
           windSpeed_ == other.windSpeed_ &&
           windDirection_ == other.windDirection_ &&
           windReference_ == other.windReference_ &&
           reserved1_ == other.reserved1_ &&
           reserved2_ == other.reserved2_ &&
           reserved3_ == other.reserved3_;
}
    
} // namespace nmea2000
} // namespace nmealib