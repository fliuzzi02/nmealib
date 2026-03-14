#include "nmealib/nmea2000/PGN129025.h"

#include "nmealib/detail/errorSupport.h"

namespace nmealib {
namespace nmea2000 {

std::unique_ptr<PGN129025> PGN129025::create(std::unique_ptr<Message2000> baseMessage) {
    if (baseMessage->getCanFrameLength() != 8) {
        std::string context = "PGN129025::create()";
        NMEALIB_RETURN_ERROR(InvalidCanFrameException(context, "CAN frame must be 8 bytes for PGN129025"));
    }

    const int32_t latitudeRaw = static_cast<int32_t>(
        static_cast<uint32_t>(baseMessage->getCanFrame()[0]) |
        (static_cast<uint32_t>(baseMessage->getCanFrame()[1]) << 8) |
        (static_cast<uint32_t>(baseMessage->getCanFrame()[2]) << 16) |
        (static_cast<uint32_t>(baseMessage->getCanFrame()[3]) << 24));

    const int32_t longitudeRaw = static_cast<int32_t>(
        static_cast<uint32_t>(baseMessage->getCanFrame()[4]) |
        (static_cast<uint32_t>(baseMessage->getCanFrame()[5]) << 8) |
        (static_cast<uint32_t>(baseMessage->getCanFrame()[6]) << 16) |
        (static_cast<uint32_t>(baseMessage->getCanFrame()[7]) << 24));

    Latitude latitude = Latitude::fromRaw(latitudeRaw);
    Longitude longitude = Longitude::fromRaw(longitudeRaw);

    return std::unique_ptr<PGN129025>(new PGN129025(std::move(*baseMessage), latitude, longitude));
}

PGN129025::PGN129025(Message2000 baseMessage,
                     Latitude latitude,
                     Longitude longitude) noexcept :
                     Message2000(std::move(baseMessage)),
                     latitude_(latitude),
                     longitude_(longitude) {}

PGN129025::PGN129025(Latitude latitude,
                     Longitude longitude) noexcept :
                     Message2000(*Message2000::create(rawPayload(latitude, longitude))),
                     latitude_(latitude),
                     longitude_(longitude) {}

std::unique_ptr<Message> PGN129025::clone() const {
    return std::unique_ptr<PGN129025>(new PGN129025(*this));
}

std::string PGN129025::getStringContent(bool verbose) const noexcept {
    std::ostringstream oss;

    if (verbose) {
        oss << this->toString(true);
        oss << "\n";
        oss << "Fields:\n";
        oss << "\tLatitude: " << latitude_.toString() << "°\n";
        oss << "\tLongitude: " << longitude_.toString() << "°\n";
    } else {
        oss << this->toString(false);
        oss << "Latitude=" << latitude_.toString() << "°"
            << " Longitude=" << longitude_.toString() << "°";
    }

    return oss.str();
}

std::string PGN129025::rawPayload(Latitude latitude,
                                  Longitude longitude) {
    std::vector<uint8_t> canFrame(8, 0);

    const auto latitudeRaw = static_cast<uint32_t>(latitude.getRaw());
    canFrame[0] = static_cast<uint8_t>(latitudeRaw & 0xFF);
    canFrame[1] = static_cast<uint8_t>((latitudeRaw >> 8) & 0xFF);
    canFrame[2] = static_cast<uint8_t>((latitudeRaw >> 16) & 0xFF);
    canFrame[3] = static_cast<uint8_t>((latitudeRaw >> 24) & 0xFF);

    const auto longitudeRaw = static_cast<uint32_t>(longitude.getRaw());
    canFrame[4] = static_cast<uint8_t>(longitudeRaw & 0xFF);
    canFrame[5] = static_cast<uint8_t>((longitudeRaw >> 8) & 0xFF);
    canFrame[6] = static_cast<uint8_t>((longitudeRaw >> 16) & 0xFF);
    canFrame[7] = static_cast<uint8_t>((longitudeRaw >> 24) & 0xFF);

    const uint32_t canId = (129025U << 8U);
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::setfill('0');
    oss << std::setw(8) << canId << ":";
    for (uint8_t b : canFrame) {
        oss << std::setw(2) << static_cast<int>(b);
    }
    return oss.str();
}

Latitude PGN129025::getLatitude() const noexcept {
    return latitude_;
}

Longitude PGN129025::getLongitude() const noexcept {
    return longitude_;
}

bool PGN129025::operator==(const PGN129025& other) const noexcept {
    return Message2000::operator==(other) &&
           latitude_ == other.latitude_ &&
           longitude_ == other.longitude_;
}

} // namespace nmea2000
} // namespace nmealib