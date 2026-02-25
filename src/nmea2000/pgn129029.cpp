#include "nmealib/nmea2000/pgn129029.hpp"

#include <cmath>
#include <cstdint>
#include <iomanip>
#include <sstream>

namespace nmealib {
namespace nmea2000 {

namespace {

std::uint32_t readU32BE(const std::vector<std::uint8_t>& payload, std::size_t offset) {
    return (static_cast<std::uint32_t>(payload[offset]) << 24) |
           (static_cast<std::uint32_t>(payload[offset + 1]) << 16) |
           (static_cast<std::uint32_t>(payload[offset + 2]) << 8) |
           static_cast<std::uint32_t>(payload[offset + 3]);
}

std::int32_t readI32BE(const std::vector<std::uint8_t>& payload, std::size_t offset) {
    return static_cast<std::int32_t>(readU32BE(payload, offset));
}

void writeU32BE(std::vector<std::uint8_t>& payload, std::size_t offset, std::uint32_t value) {
    payload[offset] = static_cast<std::uint8_t>((value >> 24) & 0xFFU);
    payload[offset + 1] = static_cast<std::uint8_t>((value >> 16) & 0xFFU);
    payload[offset + 2] = static_cast<std::uint8_t>((value >> 8) & 0xFFU);
    payload[offset + 3] = static_cast<std::uint8_t>(value & 0xFFU);
}

void writeI32BE(std::vector<std::uint8_t>& payload, std::size_t offset, std::int32_t value) {
    writeU32BE(payload, offset, static_cast<std::uint32_t>(value));
}

} // namespace

std::unique_ptr<PGN129029> PGN129029::create(std::unique_ptr<Message2000> baseMessage) {
    std::string context = "PGN129029::create";
    if (baseMessage->getPgn() != 129029) {
        throw NotPGN129029Exception(context, "Expected PGN 129029, got " + std::to_string(baseMessage->getPgn()));
    }

    const auto& payload = baseMessage->getPayloadBytes();
    constexpr std::size_t minPayloadSize = 18;
    if (payload.size() < minPayloadSize) {
        throw NotPGN129029Exception(context,
                                    "Insufficient payload length for PGN129029: expected at least 18 bytes, got " +
                                    std::to_string(payload.size()));
    }

    try {
        std::uint8_t sid = payload[0];
        double latitude = static_cast<double>(readI32BE(payload, 1)) * 1e-7;
        double longitude = static_cast<double>(readI32BE(payload, 5)) * 1e-7;
        double altitude = static_cast<double>(readI32BE(payload, 9)) * 1e-3;
        std::uint8_t fixStatus = payload[13];
        double utcTime = static_cast<double>(readU32BE(payload, 14)) * 1e-4;

        return std::unique_ptr<PGN129029>(new PGN129029(std::move(*baseMessage),
                                                         sid,
                                                         latitude,
                                                         longitude,
                                                         altitude,
                                                         fixStatus,
                                                         utcTime));
    } catch (const std::exception& e) {
        throw NmeaException(context, "Error parsing PGN129029 fields: " + std::string(e.what()));
    }
}

PGN129029::PGN129029(Message2000 baseMessage,
                     std::uint8_t sid,
                     double latitude,
                     double longitude,
                     double altitude,
                     std::uint8_t fixStatus,
                     double utcTime) noexcept
    : Message2000(std::move(baseMessage)),
      sid_(sid),
      latitude_(latitude),
      longitude_(longitude),
      altitude_(altitude),
      fixStatus_(fixStatus),
      utcTime_(utcTime) {}

PGN129029::PGN129029(std::uint8_t sid,
                     double latitude,
                     double longitude,
                     double altitude,
                     std::uint8_t fixStatus,
                     double utcTime,
                     std::uint8_t sourceAddress,
                     std::uint8_t priority)
    : Message2000(*Message2000::create(composeRaw(sid,
                                                  latitude,
                                                  longitude,
                                                  altitude,
                                                  fixStatus,
                                                  utcTime,
                                                  sourceAddress,
                                                  priority))),
      sid_(sid),
      latitude_(latitude),
      longitude_(longitude),
      altitude_(altitude),
      fixStatus_(fixStatus),
      utcTime_(utcTime) {}

std::unique_ptr<nmealib::Message> PGN129029::clone() const {
    return std::unique_ptr<PGN129029>(new PGN129029(*this));
}

std::string PGN129029::getStringContent(bool verbose) const noexcept {
    std::ostringstream ss;

    if (verbose) {
        ss << "Protocol: " << typeToString(type_) << "\n";
        if (hasCanId()) {
            ss << "CAN ID: 0x" << std::uppercase << std::hex << getCanId() << std::dec << "\n";
        }
        ss << "PGN: " << getPgn() << "\n";
        ss << "Fields:\n";
        ss << "\tSID: " << static_cast<unsigned int>(sid_) << "\n";
        ss << "\tLatitude: " << latitude_ << "\n";
        ss << "\tLongitude: " << longitude_ << "\n";
        ss << "\tAltitude: " << altitude_ << "\n";
        ss << "\tFix Status: " << static_cast<unsigned int>(fixStatus_) << "\n";
        ss << "\tFix Type: " << static_cast<unsigned int>(getFixType()) << "\n";
        ss << "\tStatus Flags: " << static_cast<unsigned int>(getStatusFlags()) << "\n";
        ss << "\tUTC Time: " << utcTime_;
    } else {
        ss << "[OK] " << typeToString(type_) << " PGN " << getPgn() << ": "
           << "SID=" << static_cast<unsigned int>(sid_)
           << ", Lat=" << latitude_
           << ", Lon=" << longitude_
           << ", Alt=" << altitude_
           << ", FixStatus=" << static_cast<unsigned int>(fixStatus_)
           << ", UTCTime=" << utcTime_;
    }

    return ss.str();
}

std::string PGN129029::composeRaw(std::uint8_t sid,
                                  double latitude,
                                  double longitude,
                                  double altitude,
                                  std::uint8_t fixStatus,
                                  double utcTime,
                                  std::uint8_t sourceAddress,
                                  std::uint8_t priority) {
    std::vector<std::uint8_t> payload(18, 0x00U);

    payload[0] = sid;
    writeI32BE(payload, 1, static_cast<std::int32_t>(std::llround(latitude / 1e-7)));
    writeI32BE(payload, 5, static_cast<std::int32_t>(std::llround(longitude / 1e-7)));
    writeI32BE(payload, 9, static_cast<std::int32_t>(std::llround(altitude / 1e-3)));
    payload[13] = fixStatus;
    writeU32BE(payload, 14, static_cast<std::uint32_t>(std::llround(utcTime / 1e-4)));

    // Keep wire format aligned with the sample CAN ID convention used in this project.
    const std::uint32_t canId = ((static_cast<std::uint32_t>(priority & 0x07U)) << 26) |
                                (static_cast<std::uint32_t>(0x0F805U) << 8) |
                                static_cast<std::uint32_t>(sourceAddress);

    std::ostringstream ss;
    ss << std::uppercase << std::hex << std::setw(8) << std::setfill('0') << canId;
    for (std::uint8_t b : payload) {
        ss << ' ' << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(b);
    }
    return ss.str();
}

std::uint8_t PGN129029::getSid() const noexcept {
    return sid_;
}

double PGN129029::getLatitude() const noexcept {
    return latitude_;
}

double PGN129029::getLongitude() const noexcept {
    return longitude_;
}

double PGN129029::getAltitude() const noexcept {
    return altitude_;
}

std::uint8_t PGN129029::getFixStatus() const noexcept {
    return fixStatus_;
}

std::uint8_t PGN129029::getFixType() const noexcept {
    return static_cast<std::uint8_t>(fixStatus_ & 0x07U);
}

std::uint8_t PGN129029::getStatusFlags() const noexcept {
    return static_cast<std::uint8_t>((fixStatus_ >> 3) & 0x1FU);
}

double PGN129029::getUtcTime() const noexcept {
    return utcTime_;
}

bool PGN129029::operator==(const PGN129029& other) const noexcept {
    return Message2000::operator==(other);
}

bool PGN129029::hasEqualContent(const PGN129029& other) const noexcept {
    return Message2000::hasEqualContent(other) &&
           sid_ == other.sid_ &&
           latitude_ == other.latitude_ &&
           longitude_ == other.longitude_ &&
           altitude_ == other.altitude_ &&
           fixStatus_ == other.fixStatus_ &&
           utcTime_ == other.utcTime_;
}

} // namespace nmea2000
} // namespace nmealib
