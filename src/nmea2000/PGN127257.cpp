#include "nmealib/nmea2000/PGN127257.h"

#include "nmealib/detail/errorSupport.h"

namespace nmealib {
namespace nmea2000 {

std::unique_ptr<PGN127257> PGN127257::create(std::unique_ptr<Message2000> baseMessage) {
    if (baseMessage->getCanFrameLength() != 8) {
        std::string context = "PGN127257::create()";
        NMEALIB_RETURN_ERROR(InvalidCanFrameException(context, "CAN frame must be 8 bytes for PGN127257"));
    }

    const uint8_t sequenceId = baseMessage->getCanFrame()[0];
    const SignedAngle yaw = SignedAngle::fromRaw(static_cast<uint16_t>(baseMessage->getCanFrame()[1]) |
                                                  (static_cast<uint16_t>(baseMessage->getCanFrame()[2]) << 8));
    const SignedAngle pitch = SignedAngle::fromRaw(static_cast<uint16_t>(baseMessage->getCanFrame()[3]) |
                                                    (static_cast<uint16_t>(baseMessage->getCanFrame()[4]) << 8));
    const SignedAngle roll = SignedAngle::fromRaw(static_cast<uint16_t>(baseMessage->getCanFrame()[5]) |
                                                   (static_cast<uint16_t>(baseMessage->getCanFrame()[6]) << 8));
    const Byte reserved = Byte::fromRaw(baseMessage->getCanFrame()[7]);

    return std::unique_ptr<PGN127257>(new PGN127257(std::move(*baseMessage),
                                                     sequenceId,
                                                     yaw,
                                                     pitch,
                                                     roll,
                                                     reserved));
}

PGN127257::PGN127257(Message2000 baseMessage,
                     uint8_t sequenceId,
                     SignedAngle yaw,
                     SignedAngle pitch,
                     SignedAngle roll,
                     Byte reserved) noexcept :
                     Message2000(std::move(baseMessage)),
                     sequenceId_(sequenceId),
                     yaw_(yaw),
                     pitch_(pitch),
                     roll_(roll),
                     reserved_(reserved) {}

PGN127257::PGN127257(uint8_t sequenceId,
                     SignedAngle yaw,
                     SignedAngle pitch,
                     SignedAngle roll,
                     Byte reserved) noexcept :
                     Message2000(*Message2000::create(rawPayload(sequenceId,
                                                                yaw,
                                                                pitch,
                                                                roll,
                                                                reserved))),
                     sequenceId_(sequenceId),
                     yaw_(yaw),
                     pitch_(pitch),
                     roll_(roll),
                     reserved_(reserved) {}

std::unique_ptr<Message> PGN127257::clone() const {
    return std::unique_ptr<PGN127257>(new PGN127257(*this));
}

std::string PGN127257::getStringContent(bool verbose) const noexcept {
    std::ostringstream oss;

    if (verbose) {
        oss << this->toString(true);
        oss << "\n";
        oss << "Fields:\n";
        oss << "\tSequence ID: " << static_cast<int>(sequenceId_) << "\n";
        oss << "\tYaw: " << yaw_.toString() << "rad, " << getYawDegrees() << "°\n";
        oss << "\tPitch: " << pitch_.toString() << "rad, " << getPitchDegrees() << "°\n";
        oss << "\tRoll: " << roll_.toString() << "rad, " << getRollDegrees() << "°\n";
    } else {
        oss << this->toString(false);
        oss << "SeqID=" << static_cast<int>(sequenceId_)
            << " Yaw=" << getYawDegrees() << "°"
            << " Pitch=" << getPitchDegrees() << "°"
            << " Roll=" << getRollDegrees() << "°";
    }

    return oss.str();
}

std::string PGN127257::rawPayload(uint8_t sequenceId,
                                  SignedAngle yaw,
                                  SignedAngle pitch,
                                  SignedAngle roll,
                                  Byte reserved) {
    std::vector<uint8_t> canFrame(8, 0);
    canFrame[0] = sequenceId;
    canFrame[1] = static_cast<uint8_t>(yaw.getRaw() & 0xFFU);
    canFrame[2] = static_cast<uint8_t>((yaw.getRaw() >> 8) & 0xFFU);
    canFrame[3] = static_cast<uint8_t>(pitch.getRaw() & 0xFFU);
    canFrame[4] = static_cast<uint8_t>((pitch.getRaw() >> 8) & 0xFFU);
    canFrame[5] = static_cast<uint8_t>(roll.getRaw() & 0xFFU);
    canFrame[6] = static_cast<uint8_t>((roll.getRaw() >> 8) & 0xFFU);
    canFrame[7] = reserved.getRaw();

    const uint32_t canId = (127257U << 8U);
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::setfill('0');
    oss << std::setw(8) << canId << ":";
    for (uint8_t b : canFrame) {
        oss << std::setw(2) << static_cast<int>(b);
    }
    return oss.str();
}

uint8_t PGN127257::getSequenceId() const noexcept {
    return sequenceId_;
}

SignedAngle PGN127257::getYaw() const noexcept {
    return yaw_;
}

SignedAngle PGN127257::getPitch() const noexcept {
    return pitch_;
}

SignedAngle PGN127257::getRoll() const noexcept {
    return roll_;
}

Byte PGN127257::getReserved() const noexcept {
    return reserved_;
}

float PGN127257::getYawDegrees() const noexcept {
    return yaw_.getValue() * 180.0f / static_cast<float>(M_PI);
}

float PGN127257::getPitchDegrees() const noexcept {
    return pitch_.getValue() * 180.0f / static_cast<float>(M_PI);
}

float PGN127257::getRollDegrees() const noexcept {
    return roll_.getValue() * 180.0f / static_cast<float>(M_PI);
}

bool PGN127257::operator==(const PGN127257& other) const noexcept {
    return Message2000::operator==(other) &&
           sequenceId_ == other.sequenceId_ &&
           yaw_ == other.yaw_ &&
           pitch_ == other.pitch_ &&
           roll_ == other.roll_ &&
           reserved_ == other.reserved_;
}

} // namespace nmea2000
} // namespace nmealib