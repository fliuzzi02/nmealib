#include "nmealib/nmea2000/PGN127251.h"

#include "nmealib/detail/errorSupport.h"

namespace nmealib {
namespace nmea2000 {

std::unique_ptr<PGN127251> PGN127251::create(std::unique_ptr<Message2000> baseMessage) {
    if (baseMessage->getCanFrameLength() != 8) {
        std::string context = "PGN127251::create()";
        NMEALIB_RETURN_ERROR(InvalidCanFrameException(context, "CAN frame must be 8 bytes for PGN127251"));
    }

    const uint8_t sequenceId = baseMessage->getCanFrame()[0];
    const uint32_t rawRateBits = static_cast<uint32_t>(baseMessage->getCanFrame()[1]) |
                                 (static_cast<uint32_t>(baseMessage->getCanFrame()[2]) << 8) |
                                 (static_cast<uint32_t>(baseMessage->getCanFrame()[3]) << 16) |
                                 (static_cast<uint32_t>(baseMessage->getCanFrame()[4]) << 24);
    const AngularRate rate = AngularRate::fromRaw(static_cast<int32_t>(rawRateBits));
    const Byte reserved1 = Byte::fromRaw(baseMessage->getCanFrame()[5]);
    const Byte reserved2 = Byte::fromRaw(baseMessage->getCanFrame()[6]);
    const Byte reserved3 = Byte::fromRaw(baseMessage->getCanFrame()[7]);

    return std::unique_ptr<PGN127251>(new PGN127251(std::move(*baseMessage),
                                                     sequenceId,
                                                     rate,
                                                     reserved1,
                                                     reserved2,
                                                     reserved3));
}

PGN127251::PGN127251(Message2000 baseMessage,
                     uint8_t sequenceId,
                     AngularRate rate,
                     Byte reserved1,
                     Byte reserved2,
                     Byte reserved3) noexcept :
                     Message2000(std::move(baseMessage)),
                     sequenceId_(sequenceId),
                     rate_(rate),
                     reserved1_(reserved1),
                     reserved2_(reserved2),
                     reserved3_(reserved3) {}

PGN127251::PGN127251(uint8_t sequenceId,
                     AngularRate rate,
                     Byte reserved1,
                     Byte reserved2,
                     Byte reserved3) noexcept :
                     Message2000(*Message2000::create(rawPayload(sequenceId,
                                                                rate,
                                                                reserved1,
                                                                reserved2,
                                                                reserved3))),
                     sequenceId_(sequenceId),
                     rate_(rate),
                     reserved1_(reserved1),
                     reserved2_(reserved2),
                     reserved3_(reserved3) {}

std::unique_ptr<Message> PGN127251::clone() const {
    return std::unique_ptr<PGN127251>(new PGN127251(*this));
}

std::string PGN127251::getStringContent(bool verbose) const noexcept {
    std::ostringstream oss;

    if (verbose) {
        oss << this->toString(true);
        oss << "\n";
        oss << "Fields:\n";
        oss << "\tSequence ID: " << static_cast<int>(sequenceId_) << "\n";
        oss << "\tRate: " << rate_.toString() << " rad/s\n";
        oss << "\tReserved1: " << static_cast<int>(reserved1_.getValue()) << "\n";
        oss << "\tReserved2: " << static_cast<int>(reserved2_.getValue()) << "\n";
        oss << "\tReserved3: " << static_cast<int>(reserved3_.getValue()) << "\n";
    } else {
        oss << this->toString(false);
        oss << "SeqID=" << static_cast<int>(sequenceId_)
            << " Rate=" << rate_.toString() << "rad/s"
            << " R1=" << static_cast<int>(reserved1_.getValue())
            << " R2=" << static_cast<int>(reserved2_.getValue())
            << " R3=" << static_cast<int>(reserved3_.getValue());
    }

    return oss.str();
}

std::string PGN127251::rawPayload(uint8_t sequenceId,
                                  AngularRate rate,
                                  Byte reserved1,
                                  Byte reserved2,
                                  Byte reserved3) noexcept {
    std::vector<uint8_t> canFrame(8, 0);
    canFrame[0] = sequenceId;

    const uint32_t rawRateBits = static_cast<uint32_t>(rate.getRaw());
    canFrame[1] = static_cast<uint8_t>(rawRateBits & 0xFFU);
    canFrame[2] = static_cast<uint8_t>((rawRateBits >> 8) & 0xFFU);
    canFrame[3] = static_cast<uint8_t>((rawRateBits >> 16) & 0xFFU);
    canFrame[4] = static_cast<uint8_t>((rawRateBits >> 24) & 0xFFU);
    canFrame[5] = reserved1.getRaw();
    canFrame[6] = reserved2.getRaw();
    canFrame[7] = reserved3.getRaw();

    const uint32_t canId = (127251U << 8U);
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::setfill('0');
    oss << std::setw(8) << canId << ":";
    for (uint8_t b : canFrame) {
        oss << std::setw(2) << static_cast<int>(b);
    }
    return oss.str();
}

uint8_t PGN127251::getSequenceId() const noexcept {
    return sequenceId_;
}

AngularRate PGN127251::getRate() const noexcept {
    return rate_;
}

Byte PGN127251::getReserved1() const noexcept {
    return reserved1_;
}

Byte PGN127251::getReserved2() const noexcept {
    return reserved2_;
}

Byte PGN127251::getReserved3() const noexcept {
    return reserved3_;
}

bool PGN127251::operator==(const PGN127251& other) const noexcept {
    return Message2000::operator==(other) &&
           sequenceId_ == other.sequenceId_ &&
           rate_ == other.rate_ &&
           reserved1_ == other.reserved1_ &&
           reserved2_ == other.reserved2_ &&
           reserved3_ == other.reserved3_;
}

} // namespace nmea2000
} // namespace nmealib