#include "nmealib/nmea2000/PGN129026.h"

#include "nmealib/detail/errorSupport.h"

namespace nmealib {
namespace nmea2000 {

std::unique_ptr<PGN129026> PGN129026::create(std::unique_ptr<Message2000> baseMessage) {
    if (baseMessage->getCanFrameLength() != 8) {
        std::string context = "PGN129026::create()";
        NMEALIB_RETURN_ERROR(InvalidCanFrameException(context, "CAN frame must be 8 bytes for PGN129026"));
    }

    const uint8_t sequenceId = baseMessage->getCanFrame()[0];
    const HalfByte cogReference = HalfByte::fromRaw(baseMessage->getCanFrame()[1] & 0x03U);
    const Byte reserved = Byte::fromRaw((baseMessage->getCanFrame()[1] >> 2) & 0x3FU);
    const Angle cog = Angle::fromRaw(static_cast<uint16_t>(baseMessage->getCanFrame()[2]) |
                                     (static_cast<uint16_t>(baseMessage->getCanFrame()[3]) << 8));
    const Speed sog = Speed::fromRaw(static_cast<uint16_t>(baseMessage->getCanFrame()[4]) |
                                     (static_cast<uint16_t>(baseMessage->getCanFrame()[5]) << 8));
    const Byte reserved2 = Byte::fromRaw(baseMessage->getCanFrame()[6]);
    const Byte reserved3 = Byte::fromRaw(baseMessage->getCanFrame()[7]);

    return std::unique_ptr<PGN129026>(new PGN129026(std::move(*baseMessage),
                                                    sequenceId,
                                                    cogReference,
                                                    reserved,
                                                    cog,
                                                    sog,
                                                    reserved2,
                                                    reserved3));
}

PGN129026::PGN129026(Message2000 baseMessage,
                     uint8_t sequenceId,
                     HalfByte cogReference,
                     Byte reserved,
                     Angle cog,
                     Speed sog,
                     Byte reserved2,
                     Byte reserved3) noexcept :
                     Message2000(std::move(baseMessage)),
                     sequenceId_(sequenceId),
                     cogReference_(cogReference),
                     cog_(cog),
                     sog_(sog),
                     reserved_(reserved),
                     reserved2_(reserved2),
                     reserved3_(reserved3) {}

PGN129026::PGN129026(uint8_t sequenceId,
                     HalfByte cogReference,
                     Angle cog,
                     Speed sog) noexcept :
                     PGN129026(sequenceId,
                               cogReference,
                               Byte::fromValue(0U),
                               cog,
                               sog,
                               Byte::fromValue(0U),
                               Byte::fromValue(0U)) {}

PGN129026::PGN129026(uint8_t sequenceId,
                     HalfByte cogReference,
                     Byte reserved,
                     Angle cog,
                     Speed sog,
                     Byte reserved2,
                     Byte reserved3) noexcept :
                     Message2000(*Message2000::create(rawPayload(sequenceId,
                                                                cogReference,
                                                                reserved,
                                                                cog,
                                                                sog,
                                                                reserved2,
                                                                reserved3))),
                     sequenceId_(sequenceId),
                     cogReference_(cogReference),
                     cog_(cog),
                     sog_(sog),
                     reserved_(reserved),
                     reserved2_(reserved2),
                     reserved3_(reserved3) {}

std::unique_ptr<Message> PGN129026::clone() const {
    return std::unique_ptr<PGN129026>(new PGN129026(*this));
}

std::string PGN129026::getStringContent(bool verbose) const noexcept {
    std::ostringstream oss;

    if (verbose) {
        oss << this->toString(true);
        oss << "\n";
        oss << "Fields:\n";
        oss << "\tSequence ID: " << static_cast<int>(sequenceId_) << "\n";
        oss << "\tCourse Over Ground: " << cog_.toString() << "rad, " << getCogDegrees() << "°\n";
        oss << "\tSpeed Over Ground: " << getSogKnots() << " knots\n";
        oss << "\tCOG Reference: ";
        switch (cogReference_.getValue()) {
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
            << " COG=" << getCogDegrees() << "°"
            << " SOG=" << getSogKnots() << " knots"
            << " COGRef=" << static_cast<int>(cogReference_.getValue());
    }

    return oss.str();
}

std::string PGN129026::rawPayload(uint8_t sequenceId,
                                  HalfByte cogReference,
                                  Byte reserved,
                                  Angle cog,
                                  Speed sog,
                                  Byte reserved2,
                                  Byte reserved3) {
    std::vector<uint8_t> canFrame(8, 0);
    canFrame[0] = sequenceId;
    canFrame[1] = static_cast<uint8_t>(((reserved.getRaw() & 0x3FU) << 2) | (cogReference.getRaw() & 0x03U));
    canFrame[2] = static_cast<uint8_t>(cog.getRaw() & 0xFFU);
    canFrame[3] = static_cast<uint8_t>((cog.getRaw() >> 8) & 0xFFU);
    canFrame[4] = static_cast<uint8_t>(sog.getRaw() & 0xFFU);
    canFrame[5] = static_cast<uint8_t>((sog.getRaw() >> 8) & 0xFFU);
    canFrame[6] = reserved2.getRaw();
    canFrame[7] = reserved3.getRaw();

    const uint32_t canId = (129026U << 8U);
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::setfill('0');
    oss << std::setw(8) << canId << ":";
    for (uint8_t b : canFrame) {
        oss << std::setw(2) << static_cast<int>(b);
    }
    return oss.str();
}

uint8_t PGN129026::getSequenceId() const noexcept {
    return sequenceId_;
}

HalfByte PGN129026::getCogReference() const noexcept {
    return cogReference_;
}

Angle PGN129026::getCog() const noexcept {
    return cog_;
}

Speed PGN129026::getSog() const noexcept {
    return sog_;
}

Byte PGN129026::getReserved1() const noexcept {
    return reserved_;
}

Byte PGN129026::getReserved2() const noexcept {
    return reserved2_;
}

Byte PGN129026::getReserved3() const noexcept {
    return reserved3_;
}

float PGN129026::getCogDegrees() const noexcept {
    return cog_.getValue() * 180.0f / static_cast<float>(M_PI);
}

float PGN129026::getSogKnots() const noexcept {
    return sog_.getValue() * 1.94384f;
}

bool PGN129026::operator==(const PGN129026& other) const noexcept {
    return Message2000::operator==(other) &&
           sequenceId_ == other.sequenceId_ &&
           cogReference_ == other.cogReference_ &&
           cog_ == other.cog_ &&
           sog_ == other.sog_ &&
           reserved_ == other.reserved_ &&
           reserved2_ == other.reserved2_ &&
           reserved3_ == other.reserved3_;
}

} // namespace nmea2000
} // namespace nmealib
