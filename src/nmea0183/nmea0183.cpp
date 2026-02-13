#include "nmea0183.hpp"

#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <cstdlib>

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

std::unique_ptr<Message0183> Message0183::create(const std::string& raw, TimePoint ts) {
    std::string context = "Message0183::create";
    if(raw.size() > 82) {
        throw TooLongSentenceException(context, "Input string length: " + std::to_string(raw.size()));
    }
    if(raw.empty() || (raw[0] != '$' && raw[0] != '!')) {
        throw InvalidStartCharacterException("NMEA 0183 sentence must start with '$' or '!'");
    }

    char startChar = raw[0];

    // TODO: Decide whether to allow sentences that end with just LF, or just CR, or neither. For now we require CRLF.
    bool hasCRLF = raw.size() >= 2 && raw.substr(raw.size() - 2) == "\r\n";
    if(!hasCRLF) {
        throw NoEndlineException(context, "Input string: " + raw);
    }

    // Extract talker and sentence type from the raw sentence.
    std::string talker = raw.substr(1, 2);
    std::string sentenceType = raw.substr(3, 3);
    std::string payload;
    bool hasChecksum = raw.find('*') != std::string::npos;

    if (hasChecksum) {
        // If it has a checksum, find the position of '*' and extract payload and checksum accordingly.
        size_t asteriskPos = raw.find('*');
        payload = raw.substr(1, asteriskPos - 1); // Exclude start char and checksum part
        std::string checksumStr = raw.substr(asteriskPos + 1, 2);
        if(!isHexByte(checksumStr)) {
            throw NoChecksumException(context, "Invalid checksum format: " + checksumStr);
        }
        return std::unique_ptr<Message0183>(new Message0183(raw, ts, startChar, talker, sentenceType, payload, checksumStr));
    } else {

        payload = raw.substr(1, raw.size() - 3); // Exclude start char and CRLF
        return std::unique_ptr<Message0183>(new Message0183(raw, ts, startChar, talker, sentenceType, payload));
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

std::string Message0183::serialize() const {
    std::string out;
    out.push_back(startChar_);
    out += payload_;
    if (!checksumStr_.empty()) {
        out.push_back('*');
        out += checksumStr_;
    }
    out += "\r\n";
    return out;
}

std::string Message0183::computeChecksum(const std::string& payload) noexcept {
    uint8_t checksum = 0;
    for (char c : payload) {
        checksum ^= static_cast<uint8_t>(c);
    }
    std::stringstream ss;
    ss << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(checksum);
    return ss.str();
}

bool Message0183::isHexByte(const std::string& s) noexcept {
    if (s.size() != 2) return false;
    return std::isxdigit(static_cast<unsigned char>(s[0])) &&
           std::isxdigit(static_cast<unsigned char>(s[1]));
}

} // namespace nmea0183
} // namespace nmealib