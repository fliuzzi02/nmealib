#include "nmealib/nmea0183/nmea0183.hpp"

#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <numeric>
#include <cmath>

namespace nmealib {
namespace nmea0183 {

Message0183::Message0183(std::string raw,
                        TimePoint ts,
                        char startChar,
                        std::string talker,
                        std::string sentenceType,
                        std::string payload) noexcept
    : Message(std::move(raw), Type::NMEA0183, ts),
    startChar_(startChar), 
    talker_(std::move(talker)), 
    sentenceType_(std::move(sentenceType)), 
    payload_(std::move(payload)) {
    calculatedChecksumStr_ = computeChecksum(payload_);
}

Message0183::Message0183(std::string raw,
                        TimePoint ts,
                        char startChar,
                        std::string talker,
                        std::string sentenceType,
                        std::string payload,
                        std::string checksumStr) noexcept
    : Message(std::move(raw), Type::NMEA0183, ts),
    startChar_(startChar), 
    talker_(std::move(talker)), 
    sentenceType_(std::move(sentenceType)), 
    payload_(std::move(payload)),
    checksumStr_(std::move(checksumStr)) {
    calculatedChecksumStr_ = computeChecksum(payload_);
}

std::unique_ptr<Message0183> Message0183::create(std::string raw, TimePoint ts) {
    std::string context = "Message0183::create";
    validateFormat(context, raw);

    char startChar = raw[0];

    bool hasCRLF = raw.size() >= 2 && raw.substr(raw.size() - 2) == "\r\n";

    // Extract talker and sentence type from the raw sentence.
    std::string talker = raw.substr(1, 2);
    std::string sentenceType = raw.substr(3, 3);
    bool hasChecksum = raw.find('*') != std::string::npos;
    if(hasChecksum) {
        size_t asteriskPos = raw.find('*');
        std::string payload = raw.substr(1, asteriskPos - 1); // Exclude start char and checksum part
        std::string checksumStr = raw.substr(asteriskPos + 1, 2);
        return std::unique_ptr<Message0183>(new Message0183(raw, ts, startChar, talker, sentenceType, payload, checksumStr));
    } else {
        if (hasCRLF) {
            std::string payload = raw.substr(1, raw.size() - 3); // Exclude start char and CRLF
            return std::unique_ptr<Message0183>(new Message0183(raw, ts, startChar, talker, sentenceType, payload));
        } else {
            std::string payload = raw.substr(1); // Exclude start char only
            return std::unique_ptr<Message0183>(new Message0183(raw, ts, startChar, talker, sentenceType, payload));
        }
    }
}

void Message0183::validateFormat(const std::string& context, const std::string& raw) {
    // TODO: I have to check that it correspons to the minimum sentence: $XXXXX*ZZ<CR><LF>
    // and also without checksum: $XXXXX<CR><LF>
    // and also without CRLF: $XXXXX*ZZ and $XXXXX
    if(raw.size() > 82) {
        throw TooLongSentenceException(context, "Input string length: " + std::to_string(raw.size()));
    }
    if(raw.empty() || (raw[0] != '$' && raw[0] != '!')) {
        throw InvalidStartCharacterException("NMEA 0183 sentence must start with '$' or '!'");
    }

    bool hasChecksum = raw.find('*') != std::string::npos;
    if (hasChecksum) {
        // If it has a checksum, find the position of '*' and extract payload and checksum accordingly.
        size_t asteriskPos = raw.find('*');
        std::string checksumStr = raw.substr(asteriskPos + 1, 2);
        if(!isHexByte(checksumStr)) {
            throw NoChecksumException(context, "Invalid checksum format: " + checksumStr);
        }
    }
}

std::unique_ptr<nmealib::Message> Message0183::clone() const {
    return std::unique_ptr<Message0183>(new Message0183(*this));
}

std::string Message0183::getPayload() const noexcept {
    return payload_;
}

char Message0183::getStartChar() const noexcept {
    return startChar_;
}

std::string Message0183::getTalker() const noexcept {
    return talker_;
}

std::string Message0183::getSentenceType() const noexcept {
    return sentenceType_;
}

std::string Message0183::getChecksumStr() const {
    if (checksumStr_.empty()) {
        throw NoChecksumException("Message0183::getChecksumStr", "This sentence does not contain a checksum");
    }
    return checksumStr_;
}

std::string Message0183::getCalculatedChecksumStr() const noexcept {
    return calculatedChecksumStr_;
}

std::string Message0183::getStringContent(bool verbose) const noexcept {
    std::stringstream ss;
    std::string validity = "KO";
    if(validate()) {
        validity = "OK";
    }

    if (!verbose) {
        ss << "[" << validity << "] " << typeToString(type_) << " " << getTalker() << " " << getSentenceType() << ": " << "Unimplemented sentence type";
    } else {
        ss << "Protocol: " << typeToString(type_) << "\n";
        ss << "Talker: " << getTalker() << "\n";
        ss << "Sentence Type: " << getSentenceType() << "\n";
        ss << "Checksum: " << (checksumStr_.empty() ? "None" : validity) << "\n";
        ss << "Fields: \n";
        ss << "\tUnimplemented sentence type";
    }

    return ss.str();
}

std::string Message0183::serialize() const {
    return rawData_;
}

bool Message0183::validate() const {
    if (checksumStr_.empty()) {
        return true; // No checksum means we consider it valid by default
    }
    return getChecksumStr() == getCalculatedChecksumStr();
}

std::string Message0183::computeChecksum(const std::string& payload) noexcept {
    uint8_t checksum = std::accumulate(payload.begin(), payload.end(), static_cast<uint8_t>(0),
        [](uint8_t acc, char c) { return acc ^ static_cast<uint8_t>(c); });
    std::stringstream ss;
    ss << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(checksum);
    return ss.str();
}

bool Message0183::isHexByte(const std::string& s) noexcept {
    if (s.size() != 2) return false;
    return std::isxdigit(static_cast<unsigned char>(s[0])) &&
           std::isxdigit(static_cast<unsigned char>(s[1]));
}

double Message0183::convertNmeaCoordinateToDecimalDegrees(const std::string& nmeaCoordinate) {
    if (nmeaCoordinate.empty()) {
        return 0.0;
    }

    const long double raw = std::stold(nmeaCoordinate);
    const long double degrees = std::floor(raw / 100.0L);
    const long double minutes = raw - (degrees * 100.0L);
    return static_cast<double>(degrees + (minutes / 60.0L));
}

} // namespace nmea0183
} // namespace nmealib