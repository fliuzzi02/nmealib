#include "nmealib/nmea2000/PGN128001.h"

#include "nmealib/detail/errorSupport.h"

namespace nmealib {
namespace nmea2000 {

std::unique_ptr<PGN128001> PGN128001::create(std::unique_ptr<Message2000> baseMessage) {
    if (baseMessage->getCanFrameLength() != 8) {
        std::string context = "PGN128001::create()";
        NMEALIB_RETURN_ERROR(InvalidCanFrameException(context, "CAN frame must be 8 bytes for PGN128001"));
    }

    const uint8_t sequenceId = baseMessage->getCanFrame()[0];
    const Acceleration longitudinalAcceleration =
        Acceleration::fromRaw(static_cast<int16_t>(static_cast<uint16_t>(baseMessage->getCanFrame()[1]) |
                                                   (static_cast<uint16_t>(baseMessage->getCanFrame()[2]) << 8)));
    const Acceleration transverseAcceleration =
        Acceleration::fromRaw(static_cast<int16_t>(static_cast<uint16_t>(baseMessage->getCanFrame()[3]) |
                                                   (static_cast<uint16_t>(baseMessage->getCanFrame()[4]) << 8)));
    const Acceleration verticalAcceleration =
        Acceleration::fromRaw(static_cast<int16_t>(static_cast<uint16_t>(baseMessage->getCanFrame()[5]) |
                                                   (static_cast<uint16_t>(baseMessage->getCanFrame()[6]) << 8)));
    const Byte reserved = Byte::fromRaw(baseMessage->getCanFrame()[7]);

    return std::unique_ptr<PGN128001>(new PGN128001(std::move(*baseMessage),
                                                     sequenceId,
                                                     longitudinalAcceleration,
                                                     transverseAcceleration,
                                                     verticalAcceleration,
                                                     reserved));
}

PGN128001::PGN128001(Message2000 baseMessage,
                     uint8_t sequenceId,
                     Acceleration longitudinalAcceleration,
                     Acceleration transverseAcceleration,
                     Acceleration verticalAcceleration,
                     Byte reserved) noexcept :
                     Message2000(std::move(baseMessage)),
                     sequenceId_(sequenceId),
                     longitudinalAcceleration_(longitudinalAcceleration),
                     transverseAcceleration_(transverseAcceleration),
                     verticalAcceleration_(verticalAcceleration),
                     reserved_(reserved) {}

PGN128001::PGN128001(uint8_t sequenceId,
                     Acceleration longitudinalAcceleration,
                     Acceleration transverseAcceleration,
                     Acceleration verticalAcceleration,
                     Byte reserved) noexcept :
                     Message2000(*Message2000::create(rawPayload(sequenceId,
                                                                longitudinalAcceleration,
                                                                transverseAcceleration,
                                                                verticalAcceleration,
                                                                reserved))),
                     sequenceId_(sequenceId),
                     longitudinalAcceleration_(longitudinalAcceleration),
                     transverseAcceleration_(transverseAcceleration),
                     verticalAcceleration_(verticalAcceleration),
                     reserved_(reserved) {}

std::unique_ptr<Message> PGN128001::clone() const {
    return std::unique_ptr<PGN128001>(new PGN128001(*this));
}

std::string PGN128001::getStringContent(bool verbose) const noexcept {
    std::ostringstream oss;

    if (verbose) {
        oss << this->toString(true);
        oss << "\n";
        oss << "Fields:\n";
        oss << "\tSequence ID: " << static_cast<int>(sequenceId_) << "\n";
        oss << "\tLongitudinal Acceleration: " << longitudinalAcceleration_.toString() << " m/s^2"
            << " (" << getLongitudinalAccelerationG() << " g)\n";
        oss << "\tTransverse Acceleration: " << transverseAcceleration_.toString() << " m/s^2"
            << " (" << getTransverseAccelerationG() << " g)\n";
        oss << "\tVertical Acceleration: " << verticalAcceleration_.toString() << " m/s^2"
            << " (" << getVerticalAccelerationG() << " g)\n";
        oss << "\tReserved: " << static_cast<int>(reserved_.getValue()) << "\n";
    } else {
        oss << this->toString(false);
        oss << "SeqID=" << static_cast<int>(sequenceId_)
            << " Long=" << longitudinalAcceleration_.toString() << "m/s^2"
            << " Trans=" << transverseAcceleration_.toString() << "m/s^2"
            << " Vert=" << verticalAcceleration_.toString() << "m/s^2";
    }

    return oss.str();
}

std::string PGN128001::rawPayload(uint8_t sequenceId,
                                  Acceleration longitudinalAcceleration,
                                  Acceleration transverseAcceleration,
                                  Acceleration verticalAcceleration,
                                  Byte reserved) {
    std::vector<uint8_t> canFrame(8, 0);
    canFrame[0] = sequenceId;

    const uint16_t rawLongitudinal = static_cast<uint16_t>(longitudinalAcceleration.getRaw());
    const uint16_t rawTransverse = static_cast<uint16_t>(transverseAcceleration.getRaw());
    const uint16_t rawVertical = static_cast<uint16_t>(verticalAcceleration.getRaw());

    canFrame[1] = static_cast<uint8_t>(rawLongitudinal & 0xFFU);
    canFrame[2] = static_cast<uint8_t>((rawLongitudinal >> 8) & 0xFFU);
    canFrame[3] = static_cast<uint8_t>(rawTransverse & 0xFFU);
    canFrame[4] = static_cast<uint8_t>((rawTransverse >> 8) & 0xFFU);
    canFrame[5] = static_cast<uint8_t>(rawVertical & 0xFFU);
    canFrame[6] = static_cast<uint8_t>((rawVertical >> 8) & 0xFFU);
    canFrame[7] = reserved.getRaw();

    const uint32_t canId = (128001U << 8U);
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::setfill('0');
    oss << std::setw(8) << canId << ":";
    for (uint8_t b : canFrame) {
        oss << std::setw(2) << static_cast<int>(b);
    }
    return oss.str();
}

uint8_t PGN128001::getSequenceId() const noexcept {
    return sequenceId_;
}

Acceleration PGN128001::getLongitudinalAcceleration() const noexcept {
    return longitudinalAcceleration_;
}

Acceleration PGN128001::getTransverseAcceleration() const noexcept {
    return transverseAcceleration_;
}

Acceleration PGN128001::getVerticalAcceleration() const noexcept {
    return verticalAcceleration_;
}

float PGN128001::getLongitudinalAccelerationG() const noexcept {
    return longitudinalAcceleration_.getValue() / 9.80665f;
}

float PGN128001::getTransverseAccelerationG() const noexcept {
    return transverseAcceleration_.getValue() / 9.80665f;
}

float PGN128001::getVerticalAccelerationG() const noexcept {
    return verticalAcceleration_.getValue() / 9.80665f;
}

bool PGN128001::operator==(const PGN128001& other) const noexcept {
    return Message2000::operator==(other) &&
           sequenceId_ == other.sequenceId_ &&
           longitudinalAcceleration_ == other.longitudinalAcceleration_ &&
           transverseAcceleration_ == other.transverseAcceleration_ &&
           verticalAcceleration_ == other.verticalAcceleration_ &&
           reserved_ == other.reserved_;
}

} // namespace nmea2000
} // namespace nmealib
