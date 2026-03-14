#include "nmealib/nmea0183.h"

#include "nmealib/detail/errorSupport.h"
#include "nmealib/detail/parse.h"

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

std::unique_ptr<Message0183> Message0183::create(const std::string& raw, TimePoint ts) {
    std::string context = "Message0183::create";
#if defined(NMEALIB_NO_EXCEPTIONS)
    if (!validateFormat(context, raw)) {
        return nullptr;
    }
#else
    validateFormat(context, raw);
#endif

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

bool Message0183::validateFormat(const std::string& context, const std::string& raw) {
    // TODO: I have to check that it correspons to the minimum sentence: $XXXXX*ZZ<CR><LF>
    // and also without checksum: $XXXXX<CR><LF>
    // and also without CRLF: $XXXXX*ZZ and $XXXXX
    if(raw.size() > 82) {
        NMEALIB_RETURN_ERROR_VALUE(TooLongSentenceException(context, "Input string length: " + std::to_string(raw.size())), false);
    }
    if(raw.empty() || (raw[0] != '$' && raw[0] != '!')) {
        NMEALIB_RETURN_ERROR_VALUE(InvalidStartCharacterException(context), false);
    }

    bool hasChecksum = raw.find('*') != std::string::npos;
    if (hasChecksum) {
        // If it has a checksum, find the position of '*' and extract payload and checksum accordingly.
        size_t asteriskPos = raw.find('*');
        std::string checksumStr = raw.substr(asteriskPos + 1, 2);
        if(!isHexByte(checksumStr)) {
            NMEALIB_RETURN_ERROR_VALUE(NoChecksumException(context, "Invalid checksum format: " + checksumStr), false);
        }
    }

    return true;
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
        NMEALIB_RETURN_ERROR_VALUE(NoChecksumException("Message0183::getChecksumStr", "This sentence does not contain a checksum"), std::string());
    }
    return checksumStr_;
}

std::string Message0183::getCalculatedChecksumStr() const noexcept {
    return calculatedChecksumStr_;
}

std::string Message0183::getStringContent(bool verbose) const noexcept {
    std::stringstream ss;

    ss << this->toString(verbose);

    if (verbose) ss << "\tUnimplemented sentence type";
    else ss << "Unimplemented sentence type";

    return ss.str();
}

std::string Message0183::toString(bool verbose) const noexcept {
    std::stringstream ss;
    std::string validity = "KO";
    if(validate()) {
        validity = "OK";
    }

    if (verbose) {
        ss << "--------------------------------\n";
        ss << "Protocol: " << typeToString(type_) << "\n";
        ss << "Talker: " << getTalker() << "\n";
        ss << "Sentence Type: " << getSentenceType() << "\n";
        ss << "Checksum: " << (checksumStr_.empty() ? "None" : validity) << "\n";
        ss << "Fields:\n";
    } else {
        ss << "[" << validity << "] " << typeToString(type_) << " " << getTalker() << " " << getSentenceType() << ": ";
    }

    return ss.str();
}

std::string Message0183::serialize() const {
    return rawData_;
}

bool Message0183::validate() const noexcept{
    if (checksumStr_.empty()) {
        return true;
    }

    return checksumStr_ == getCalculatedChecksumStr();
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
    double converted = 0.0;
    if (!detail::parseNmeaCoordinate(nmeaCoordinate, converted)) {
        NMEALIB_RETURN_ERROR_VALUE(NmeaException("Message0183::convertNmeaCoordinateToDecimalDegrees", "Invalid coordinate format", nmeaCoordinate), 0.0);
    }

    return converted;
}

// TODO: Re-expose this method to all child classes and restrict the inherited ones like i did in 
bool Message0183::operator==(const Message0183& other) const noexcept {
    return startChar_ == other.startChar_ &&
           talker_ == other.talker_ &&
           sentenceType_ == other.sentenceType_ &&
           payload_ == other.payload_ &&
           checksumStr_ == other.checksumStr_ &&
           calculatedChecksumStr_ == other.calculatedChecksumStr_ &&
           Message::operator==(other);
}

} // namespace nmea0183
} // namespace nmealib
