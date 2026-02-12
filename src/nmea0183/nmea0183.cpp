#include "nmea0183.hpp"

#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <cstdlib>

namespace nmealib {
namespace nmea0183 {

Message0183::Message0183() noexcept {
    setType(Type::NMEA0183);
}

Message0183::Message0183(std::string raw, TimePoint ts) noexcept
    : nmealib::Message(std::move(raw), Type::NMEA0183, ts) {}

std::unique_ptr<nmealib::Message> Message0183::clone() const {
    return std::make_unique<Message0183>(*this);
}

std::string Message0183::serialize() const {
    const std::string payload = getPayload();
    if (payload.empty()) return std::string{};

    const std::uint8_t cs = computeChecksum(payload);
    std::ostringstream oss;
    oss << '$' << payload << '*'
        << std::uppercase << std::hex << std::setw(2) << std::setfill('0')
        << static_cast<int>(cs)
        << "\r\n";
    return oss.str();
}

bool Message0183::validate() const {
    if (rawData_.empty()) return false;

    const char first = rawData_.front();
    if (first != '$' && first != '!') return false;

    const auto star = rawData_.find('*');
    if (star == std::string::npos) return false;

    // need at least two hex digits after '*'
    if (star + 2 >= rawData_.size()) return false;
    const std::string csStr = rawData_.substr(star + 1, 2);
    if (!isHexByte(csStr)) return false;

    const std::string payload = rawData_.substr(1, star - 1);
    const std::uint8_t computed = computeChecksum(payload);

    // parse given checksum (safe using std::strtoul)
    const char* startPtr = csStr.c_str();
    char* endPtr = nullptr;
    const unsigned long given = std::strtoul(startPtr, &endPtr, 16);
    if (endPtr == startPtr) return false;

    return static_cast<std::uint8_t>(given & 0xFF) == computed;
}

std::string Message0183::getPayload() const noexcept {
    if (rawData_.empty()) return {};
    std::size_t start = 0;
    if (rawData_.front() == '$' || rawData_.front() == '!') start = 1;

    const auto star = rawData_.find('*');
    if (star != std::string::npos && star > start) {
        return rawData_.substr(start, star - start);
    }

    // no checksum present: strip trailing CR/LF
    auto end = rawData_.size();
    while (end > start && (rawData_[end - 1] == '\n' || rawData_[end - 1] == '\r')) --end;
    return rawData_.substr(start, end - start);
}

std::string Message0183::getChecksumStr() const noexcept {
    const auto star = rawData_.find('*');
    if (star == std::string::npos) return {};
    if (star + 2 >= rawData_.size()) return {};
    return rawData_.substr(star + 1, 2);
}

std::uint8_t Message0183::computeChecksum(const std::string& payload) noexcept {
    std::uint8_t cs = 0;
    for (unsigned char c : payload) cs ^= c;
    return cs;
}

bool Message0183::isHexByte(const std::string& s) noexcept {
    if (s.size() != 2) return false;
    return std::isxdigit(static_cast<unsigned char>(s[0])) &&
           std::isxdigit(static_cast<unsigned char>(s[1]));
}

} // namespace nmea0183
} // namespace nmealib