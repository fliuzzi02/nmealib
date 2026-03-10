#include "nmealib/nmea2000/PGN128259.h"

namespace nmealib {
namespace nmea2000 {

std::unique_ptr<PGN128259> PGN128259::create(std::unique_ptr<Message2000> baseMessage) {
    std::string context = "PGN128259::create()";

    if (baseMessage->getCanFrameLength() != 8) {
        throw InvalidCanFrameException(context, "CAN frame must be 8 bytes for PGN128259");
    }

    try {
        uint8_t sequenceId = baseMessage->getCanFrame()[0];
        Speed speedWaterReferenced = Speed::fromRaw(baseMessage->getCanFrame()[1] | (baseMessage->getCanFrame()[2] << 8));
        Speed speedGroundReferenced = Speed::fromRaw(baseMessage->getCanFrame()[3] | (baseMessage->getCanFrame()[4] << 8));
        Byte speedWaterReferencedType = Byte::fromRaw(baseMessage->getCanFrame()[5]);
        // Byte 6: lower nibble = speedDirection, upper nibble = reserved2
        HalfByte speedDirection = HalfByte::fromRaw(baseMessage->getCanFrame()[6] & 0x0F);
        HalfByte reserved2 = HalfByte::fromRaw((baseMessage->getCanFrame()[6] >> 4) & 0x0F);
        // Byte 7: full byte = reserved1
        Byte reserved1 = Byte::fromRaw(baseMessage->getCanFrame()[7]);

        return std::unique_ptr<PGN128259>(new PGN128259(std::move(*baseMessage),
                                                        sequenceId,
                                                        speedWaterReferenced,
                                                        speedGroundReferenced,
                                                        speedWaterReferencedType,
                                                        speedDirection,
                                                        reserved1,
                                                        reserved2));
    } catch (const std::exception& e) {
        throw NmeaException(context, "Error creating PGN128259 message: " + std::string(e.what()));
    }
}

PGN128259::PGN128259(Message2000 baseMessage,
                    uint8_t sequenceId,
                    Speed speedWaterReferenced,
                    Speed speedGroundReferenced,
                    Byte speedWaterReferencedType,
                    HalfByte speedDirection,
                    Byte reserved1,
                    HalfByte reserved2) noexcept : 
                    Message2000(std::move(baseMessage)),
                    sequenceId_(sequenceId),
                    speedWaterReferenced_(speedWaterReferenced),
                    speedGroundReferenced_(speedGroundReferenced),
                    speedWaterReferencedType_(speedWaterReferencedType),
                    speedDirection_(speedDirection),
                    reserved1_(reserved1),
                    reserved2_(reserved2) {}

PGN128259::PGN128259(uint8_t sequenceId,
                    Speed speedWaterReferenced,
                    Speed speedGroundReferenced,
                    Byte speedWaterReferencedType,
                    HalfByte speedDirection,
                    Byte reserved1,
                    HalfByte reserved2) : 
                    Message2000(*Message2000::create(rawPayload(sequenceId, 
                        speedWaterReferenced, 
                        speedGroundReferenced, 
                        speedWaterReferencedType, 
                        speedDirection, 
                        reserved1, 
                        reserved2))),
                    sequenceId_(sequenceId),
                    speedWaterReferenced_(speedWaterReferenced),
                    speedGroundReferenced_(speedGroundReferenced),
                    speedWaterReferencedType_(speedWaterReferencedType),
                    speedDirection_(speedDirection),
                    reserved1_(reserved1),
                    reserved2_(reserved2) {}


std::unique_ptr<Message> PGN128259::clone() const{
    return std::unique_ptr<PGN128259>(new PGN128259(*this));
}

std::string PGN128259::getStringContent(bool verbose) const noexcept {
    std::ostringstream oss;

    if (verbose) {
        oss << this->toString(true);
        oss << "\n";
        oss << "Fields:\n";
        oss << "\tSequence ID: " << static_cast<int>(sequenceId_) << "\n";
        oss << "\tSpeed Water Referenced: " << speedWaterReferenced_.toString() << " m/s\n";
        oss << "\tSpeed Ground Referenced: " << speedGroundReferenced_.toString() << " m/s\n";
        oss << "\tSpeed Water Referenced Type: ";
            switch (speedWaterReferencedType_.getValue()) {
                case 0: oss << "Paddle"; break;
                case 1: oss << "Pitot"; break;
                case 2: oss << "Doppler"; break;
                case 3: oss << "Correlation"; break;
                case 4: oss << "EM"; break;
                default: oss << "Unavailable/Reserved"; break;
            }
        oss << "\n";
        oss << "\tSpeed Direction: ";
            if (speedDirection_.getValue() == 0b00) oss << "Forward";
            else if (speedDirection_.getValue() == 0b01) oss << "Reverse";
            else oss << "Unavailable/Reserved";
        oss << "\n";
    } else {
        oss << this->toString(false);
        oss << "SeqID=" << static_cast<int>(sequenceId_)
            << " SpeedWater=" << speedWaterReferenced_.toString() << "m/s"
            << " SpeedGround=" << speedGroundReferenced_.toString() << "m/s"
            << " WaterRefType=" << static_cast<int>(speedWaterReferencedType_.getValue())
            << " SpeedDir=";
                if (speedDirection_.getValue() == 0b00) oss << "F";
                else if (speedDirection_.getValue() == 0b01) oss << "R";
                else oss << "X";
    }
    return oss.str();
}

std::string PGN128259::rawPayload(uint8_t sequenceId,
                                  Speed speedWaterReferenced,
                                  Speed speedGroundReferenced,
                                  Byte speedWaterReferencedType,
                                  HalfByte speedDirection,
                                  Byte reserved1,
                                  HalfByte reserved2) {
    std::vector<uint8_t> canFrame(8, 0);
    canFrame[0] = sequenceId;
    canFrame[1] = speedWaterReferenced.getRaw() & 0xFF;
    canFrame[2] = (speedWaterReferenced.getRaw() >> 8) & 0xFF;
    canFrame[3] = speedGroundReferenced.getRaw() & 0xFF;
    canFrame[4] = (speedGroundReferenced.getRaw() >> 8) & 0xFF;
    canFrame[5] = speedWaterReferencedType.getRaw();
    // Byte 6: upper nibble = reserved2, lower nibble = speedDirection
    canFrame[6] = (reserved2.getRaw() << 4) | (speedDirection.getRaw() & 0x0F);
    // Byte 7: full 8-bit reserved field
    canFrame[7] = reserved1.getRaw();

    // Produce the "CANID:data" hex string expected by Message2000::create()
    // CAN ID encodes PGN 128259 (0x1F503) in bits [24:8], source address 0
    const uint32_t canId = (128259U << 8U);  // 0x01F50300
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::setfill('0');
    oss << std::setw(8) << canId << ":";
    for (uint8_t b : canFrame) {
        oss << std::setw(2) << static_cast<int>(b);
    }
    return oss.str();
}

uint8_t PGN128259::getSequenceId() const noexcept {
    return sequenceId_;
}

Speed PGN128259::getSpeedWaterReferenced() const noexcept {
    return speedWaterReferenced_;
}

Speed PGN128259::getSpeedGroundReferenced() const noexcept {
    return speedGroundReferenced_;
}

Byte PGN128259::getSpeedWaterReferencedType() const noexcept {
    return speedWaterReferencedType_;
}

HalfByte PGN128259::getSpeedDirection() const noexcept {
    return speedDirection_;
}

Byte PGN128259::getReserved1() const noexcept {
    return reserved1_;
}

HalfByte PGN128259::getReserved2() const noexcept {
    return reserved2_;
}

bool PGN128259::operator==(const PGN128259& other) const noexcept {
    return Message2000::operator==(other) &&
           sequenceId_ == other.sequenceId_ &&
           speedWaterReferenced_ == other.speedWaterReferenced_ &&
           speedGroundReferenced_ == other.speedGroundReferenced_ &&
           speedWaterReferencedType_ == other.speedWaterReferencedType_ &&
           speedDirection_ == other.speedDirection_ &&
           reserved1_ == other.reserved1_ &&
           reserved2_ == other.reserved2_;
}

} // namespace nmea2000
} // namespace nmealib