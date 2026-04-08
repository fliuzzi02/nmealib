#include "nmealib/nmea2000/PGN127245.h"

#include "nmealib/detail/errorSupport.h"

namespace nmealib {
namespace nmea2000 {

std::unique_ptr<PGN127245> PGN127245::create(std::unique_ptr<Message2000> baseMessage) {
    if (baseMessage->getCanFrameLength() != 8) {
        std::string context = "PGN127245::create()";
        NMEALIB_RETURN_ERROR(InvalidCanFrameException(context, "CAN frame must be 8 bytes for PGN127245"));
    }

    const uint8_t rudderId = baseMessage->getCanFrame()[0];
    const HalfByte direction = HalfByte::fromRaw(baseMessage->getCanFrame()[1] & 0x07U);
    const Byte reserved = Byte::fromRaw((baseMessage->getCanFrame()[1] >> 3) & 0x1FU);
    const SignedAngle angleOrder = SignedAngle::fromRaw(static_cast<uint16_t>(baseMessage->getCanFrame()[2]) |
                                                         (static_cast<uint16_t>(baseMessage->getCanFrame()[3]) << 8));
    const SignedAngle position = SignedAngle::fromRaw(static_cast<uint16_t>(baseMessage->getCanFrame()[4]) |
                                                       (static_cast<uint16_t>(baseMessage->getCanFrame()[5]) << 8));
    const Byte reserved2 = Byte::fromRaw(baseMessage->getCanFrame()[6]);
    const Byte reserved3 = Byte::fromRaw(baseMessage->getCanFrame()[7]);

    return std::unique_ptr<PGN127245>(new PGN127245(std::move(*baseMessage),
                                                     rudderId,
                                                     direction,
                                                     reserved,
                                                     angleOrder,
                                                     position,
                                                     reserved2,
                                                     reserved3));
}

PGN127245::PGN127245(Message2000 baseMessage,
                     uint8_t rudderId,
                     HalfByte direction,
                     Byte reserved,
                     SignedAngle angleOrder,
                     SignedAngle position,
                     Byte reserved2,
                     Byte reserved3) noexcept :
                     Message2000(std::move(baseMessage)),
                     rudderId_(rudderId),
                     direction_(direction),
                     reserved_(reserved),
                     angleOrder_(angleOrder),
                     position_(position),
                     reserved2(reserved2),
                     reserved3(reserved3) {}

PGN127245::PGN127245(uint8_t rudderId,
                     HalfByte direction,
                     SignedAngle angleOrder,
                     SignedAngle position) noexcept :
                     Message2000(*Message2000::create(rawPayload(rudderId,
                                                                direction,
                                                                Byte::fromValue(0U),
                                                                angleOrder,
                                                                position,
                                                                Byte::fromValue(255U),
                                                                Byte::fromValue(255U)))),
                     rudderId_(rudderId),
                     direction_(direction),
                     reserved_(Byte::fromValue(0U)),
                     angleOrder_(angleOrder),
                     position_(position),
                     reserved2(Byte::fromValue(255U)),
                     reserved3(Byte::fromValue(255U)) {}

std::unique_ptr<Message> PGN127245::clone() const {
    return std::unique_ptr<PGN127245>(new PGN127245(*this));
}

std::string PGN127245::getStringContent(bool verbose) const noexcept {
    std::ostringstream oss;

    if (verbose) {
        oss << this->toString(true);
        oss << "\n";
        oss << "Fields:\n";
        oss << "\tRudder ID: " << static_cast<int>(rudderId_) << "\n";
        oss << "\tDirection: " << getDirectionString() << " (" << static_cast<int>(direction_.getValue()) << ")\n";
        oss << "\tAngle Order: " << angleOrder_.toString() << "rad, " << getAngleOrderDegrees() << "°\n";
        oss << "\tPosition: " << position_.toString() << "rad, " << getPositionDegrees() << "°\n";
    } else {
        oss << this->toString(false);
        oss << "RudderID=" << static_cast<int>(rudderId_)
            << " Dir=" << getDirectionString()
            << " AngleOrder=" << getAngleOrderDegrees() << "°"
            << " Position=" << getPositionDegrees() << "°";
    }

    return oss.str();
}

std::string PGN127245::rawPayload(uint8_t rudderId,
                                  HalfByte direction,
                                  Byte reserved,
                                  SignedAngle angleOrder,
                                  SignedAngle position,
                                  Byte reserved2,
                                  Byte reserved3) {
    std::vector<uint8_t> canFrame(8, 0);
    canFrame[0] = rudderId;
    canFrame[1] = static_cast<uint8_t>(((reserved.getRaw() & 0x1FU) << 3) | (direction.getRaw() & 0x07U));
    canFrame[2] = static_cast<uint8_t>(angleOrder.getRaw() & 0xFFU);
    canFrame[3] = static_cast<uint8_t>((angleOrder.getRaw() >> 8) & 0xFFU);
    canFrame[4] = static_cast<uint8_t>(position.getRaw() & 0xFFU);
    canFrame[5] = static_cast<uint8_t>((position.getRaw() >> 8) & 0xFFU);
    canFrame[6] = reserved2.getRaw();
    canFrame[7] = reserved3.getRaw();

    const uint32_t canId = (127245U << 8U);
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::setfill('0');
    oss << std::setw(8) << canId << ":";
    for (uint8_t b : canFrame) {
        oss << std::setw(2) << static_cast<int>(b);
    }
    return oss.str();
}

uint8_t PGN127245::getRudderId() const noexcept {
    return rudderId_;
}

HalfByte PGN127245::getDirection() const noexcept {
    return direction_;
}

SignedAngle PGN127245::getAngleOrder() const noexcept {
    return angleOrder_;
}

SignedAngle PGN127245::getPosition() const noexcept {
    return position_;
}

float PGN127245::getAngleOrderDegrees() const noexcept {
    return angleOrder_.getValue() * 180.0f / static_cast<float>(M_PI);
}

float PGN127245::getPositionDegrees() const noexcept {
    return position_.getValue() * 180.0f / static_cast<float>(M_PI);
}

std::string PGN127245::getDirectionString() const noexcept {
    switch (direction_.getValue()) {
        case 0: return "none";
        case 1: return "starboard";
        case 2: return "port";
        default: return "unavailable/reserved";
    }
}

bool PGN127245::operator==(const PGN127245& other) const noexcept {
    return Message2000::operator==(other) &&
           rudderId_ == other.rudderId_ &&
           direction_ == other.direction_ &&
           reserved_ == other.reserved_ &&
           angleOrder_ == other.angleOrder_ &&
           position_ == other.position_ &&
           reserved2 == other.reserved2 &&
           reserved3 == other.reserved3;
}

} // namespace nmea2000
} // namespace nmealib