#include "nmealib/nmea2000/PGN127250.h"

namespace nmealib {
namespace nmea2000 {

std::unique_ptr<PGN127250> PGN127250::create(std::unique_ptr<Message2000> baseMessage) {
    std::string context = "PGN127250::create()";

    if (baseMessage->getCanFrameLength() != 8) {
        throw InvalidCanFrameException(context, "CAN frame must be 8 bytes for PGN127250");
    }

    try {
        uint8_t sequenceId = baseMessage->getCanFrame()[0];
        Angle heading = Angle::fromRaw(baseMessage->getCanFrame()[1] | (baseMessage->getCanFrame()[2] << 8));
        SignedAngle deviation = SignedAngle::fromRaw(baseMessage->getCanFrame()[3] | (baseMessage->getCanFrame()[4] << 8));
        SignedAngle variation = SignedAngle::fromRaw(baseMessage->getCanFrame()[5] | (baseMessage->getCanFrame()[6] << 8));
        // Byte 7: lower 2 bits = headingReference, upper 6 bits = reserved
        HalfByte headingReference = HalfByte::fromRaw(baseMessage->getCanFrame()[7] & 0x03);
        Byte reserved = Byte::fromRaw((baseMessage->getCanFrame()[7] >> 6) & 0x3F);

        return std::unique_ptr<PGN127250>(new PGN127250(std::move(*baseMessage),
                                                        sequenceId,
                                                        heading,
                                                        deviation,
                                                        variation,
                                                        headingReference,
                                                        reserved));
    } catch (const std::exception& e) {
        throw NmeaException(context, "Error creating PGN127250 message: " + std::string(e.what()));
    }
}

PGN127250::PGN127250(Message2000 baseMessage,
                    uint8_t sequenceId,
                    Angle heading,
                    SignedAngle deviation,
                    SignedAngle variation,
                    HalfByte headingReference,
                    Byte reserved) noexcept : 
                    Message2000(std::move(baseMessage)),
                    sequenceId_(sequenceId),
                    heading_(heading),
                    deviation_(deviation),
                    variation_(variation),
                    headingReference_(headingReference),
                    reserved_(reserved) {}

PGN127250::PGN127250(uint8_t sequenceId,
                    Angle heading,
                    SignedAngle deviation,
                    SignedAngle variation,
                    HalfByte headingReference,
                    Byte reserved) noexcept : 
                    Message2000(*Message2000::create(rawPayload(sequenceId, 
                        heading, 
                        deviation, 
                        variation, 
                        headingReference, 
                        reserved))),
                    sequenceId_(sequenceId),
                    heading_(heading),
                    deviation_(deviation),
                    variation_(variation),
                    headingReference_(headingReference),
                    reserved_(reserved) {}


std::unique_ptr<Message> PGN127250::clone() const{
    return std::unique_ptr<PGN127250>(new PGN127250(*this));
}

std::string PGN127250::getStringContent(bool verbose) const noexcept {
    std::ostringstream oss;

    if (verbose) {
        oss << this->toString(true);
        oss << "\n";
        oss << "Fields:\n";
        oss << "\tSequence ID: " << static_cast<int>(sequenceId_) << "\n";
        oss << "\tHeading: " << heading_.toString() << "rad, " << getHeadingDegrees() << "°" << "\n";
        oss << "\tDeviation: " << deviation_.toString() << "rad, " << getDeviationDegrees() << "°" << "\n";
        oss << "\tVariation: " << variation_.toString() << "rad, " << getVariationDegrees() << "°" << "\n";
        oss << "\tHeading Reference: ";
            switch (headingReference_.getValue()) {
                case 0: oss << "True"; break;
                case 1: oss << "Magnetic"; break;
                case 2: oss << "Error"; break;
                case 3: oss << "Null"; break;
                default: oss << "Unavailable/Reserved"; break;
            }
        oss << "\n";
    } else {
        oss << this->toString(false);
        oss << "SeqID=" << static_cast<int>(sequenceId_)
            << " Heading=" << heading_.toString() << "rad"
            << " Deviation=" << deviation_.toString() << "rad"
            << " Variation=" << variation_.toString() << "rad"
            << " HeadingRef=" << static_cast<int>(headingReference_.getValue());
    }
    return oss.str();
}

std::string PGN127250::rawPayload(uint8_t sequenceId,
                                  Angle heading,
                                  SignedAngle deviation,
                                  SignedAngle variation,
                                  HalfByte headingReference,
                                  Byte reserved) {
    std::vector<uint8_t> canFrame(8, 0);
    canFrame[0] = sequenceId;
    canFrame[1] = heading.getRaw() & 0xFF;
    canFrame[2] = (heading.getRaw() >> 8) & 0xFF;
    canFrame[3] = deviation.getRaw() & 0xFF;
    canFrame[4] = (deviation.getRaw() >> 8) & 0xFF;
    canFrame[5] = variation.getRaw() & 0xFF;
    canFrame[6] = (variation.getRaw() >> 8) & 0xFF;
    // Byte 7: lower 2 bits = headingReference, upper 6 bits = reserved
    canFrame[7] = (reserved.getRaw() << 6) | (headingReference.getRaw() & 0x03);

    // Produce the "CANID:data" hex string expected by Message2000::create()
    // CAN ID encodes PGN 127250 (0x1F503) in bits [24:8], source address 0
    const uint32_t canId = (127250U << 8U);  // 0x01F50300
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::setfill('0');
    oss << std::setw(8) << canId << ":";
    for (uint8_t b : canFrame) {
        oss << std::setw(2) << static_cast<int>(b);
    }
    return oss.str();
}

uint8_t PGN127250::getSequenceId() const noexcept {
    return sequenceId_;
}
Angle PGN127250::getHeading() const noexcept {
    return heading_;
}
SignedAngle PGN127250::getDeviation() const noexcept {
    return deviation_;
}
SignedAngle PGN127250::getVariation() const noexcept {
    return variation_;
}
HalfByte PGN127250::getHeadingReference() const noexcept {
    return headingReference_;
}
float PGN127250::getHeadingDegrees() const noexcept {
    return heading_.getValue() * 180.0f / static_cast<float>(M_PI);
}
float PGN127250::getDeviationDegrees() const noexcept {
    return deviation_.getValue() * 180.0f / static_cast<float>(M_PI);
}
float PGN127250::getVariationDegrees() const noexcept {
    return variation_.getValue() * 180.0f / static_cast<float>(M_PI);
}

bool PGN127250::operator==(const PGN127250& other) const noexcept {
    return Message2000::operator==(other) &&
           sequenceId_ == other.sequenceId_ &&
           heading_ == other.heading_ &&
           deviation_ == other.deviation_ &&
           variation_ == other.variation_ &&
           headingReference_ == other.headingReference_ &&
           reserved_ == other.reserved_;
}

} // namespace nmea2000
} // namespace nmealib