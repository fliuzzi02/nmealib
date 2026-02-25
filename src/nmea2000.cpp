#include "nmealib/nmea2000.hpp"

#include <cctype>
#include <iomanip>
#include <sstream>
#include <vector>

namespace nmealib {
namespace nmea2000 {

Message2000::Message2000(std::string raw,
                        TimePoint ts,
                        std::uint32_t pgn,
                        bool hasCanId,
                        std::uint32_t canId,
                        std::uint8_t priority,
                        std::uint8_t dataPage,
                        std::uint8_t pduFormat,
                        std::uint8_t pduSpecific,
                        std::uint8_t sourceAddress,
                        std::string payload,
                        std::vector<std::uint8_t> payloadBytes) noexcept
    : Message(std::move(raw), Type::NMEA2000, ts),
    pgn_(pgn),
    hasCanId_(hasCanId),
    canId_(canId),
    priority_(priority),
    dataPage_(dataPage),
    pduFormat_(pduFormat),
    pduSpecific_(pduSpecific),
    sourceAddress_(sourceAddress),
    payload_(std::move(payload)),
    payloadBytes_(std::move(payloadBytes)) {}

std::unique_ptr<Message2000> Message2000::create(std::string raw, TimePoint ts) {
    std::string context = "Message2000::create";
    validateFormat(context, raw);

    bool hasCanId = false;
    std::uint32_t canId = 0;
    std::uint8_t priority = 0;
    std::uint8_t dataPage = 0;
    std::uint8_t pduFormat = 0;
    std::uint8_t pduSpecific = 0;
    std::uint8_t sourceAddress = 0;

    std::uint32_t pgn = 0;
    std::string payloadToken;

    if (raw.find('#') != std::string::npos) {
        std::size_t payloadSeparator = raw.find('#');
        std::string leftToken = raw.substr(0, payloadSeparator);
        payloadToken = raw.substr(payloadSeparator + 1);

        std::uint64_t tokenValue = 0;
        bool parsedNumeric = false;
        try {
            std::size_t parsedChars = 0;
            tokenValue = std::stoull(leftToken, &parsedChars, 0);
            parsedNumeric = parsedChars == leftToken.size();
        } catch (...) {
            parsedNumeric = false;
        }

        if (parsedNumeric && tokenValue > 0x3FFFFULL && tokenValue <= 0x1FFFFFFFULL) {
            hasCanId = true;
            canId = parseCanId(context, leftToken);
            decodeCanIdFields(canId, priority, dataPage, pduFormat, pduSpecific, sourceAddress);
            pgn = extractPgnFromCanId(canId);
        } else {
            pgn = parsePgn(context, leftToken);
        }
    } else {
        std::istringstream ss(raw);
        std::vector<std::string> tokens;
        std::string token;
        while (ss >> token) {
            tokens.push_back(token);
        }

        if (tokens.empty()) {
            throw InvalidPgnException(context, "Input message is empty");
        }

        if (tokens.size() == 1) {
            const std::string& compactToken = tokens[0];

            bool allHex = !compactToken.empty();
            for (char c : compactToken) {
                if (!std::isxdigit(static_cast<unsigned char>(c))) {
                    allHex = false;
                    break;
                }
            }

            const bool looksLikeCompactCanFrame =
                allHex &&
                compactToken.size() > 8 &&
                ((compactToken.size() - 8) % 2 == 0);

            if (looksLikeCompactCanFrame) {
                std::string canIdToken = compactToken.substr(0, 8);
                payloadToken = compactToken.substr(8);

                canId = parseCanId(context, canIdToken);
                hasCanId = true;
                decodeCanIdFields(canId, priority, dataPage, pduFormat, pduSpecific, sourceAddress);
                pgn = extractPgnFromCanId(canId);
            } else {
                pgn = parsePgn(context, compactToken);
            }
        } else {
            canId = parseCanId(context, tokens[0]);
            hasCanId = true;
            decodeCanIdFields(canId, priority, dataPage, pduFormat, pduSpecific, sourceAddress);
            pgn = extractPgnFromCanId(canId);

            std::ostringstream payloadStream;
            for (std::size_t i = 1; i < tokens.size(); ++i) {
                if (i > 1) {
                    payloadStream << ' ';
                }
                payloadStream << tokens[i];
            }
            payloadToken = payloadStream.str();
        }
    }

    std::vector<std::uint8_t> payloadBytes = parsePayloadBytes(context, payloadToken);
    return std::unique_ptr<Message2000>(new Message2000(std::move(raw),
                                                        ts,
                                                        pgn,
                                                        hasCanId,
                                                        canId,
                                                        priority,
                                                        dataPage,
                                                        pduFormat,
                                                        pduSpecific,
                                                        sourceAddress,
                                                        bytesToHex(payloadBytes),
                                                        std::move(payloadBytes)));
}

void Message2000::validateFormat(const std::string& context, const std::string& raw) {
    if (raw.empty()) {
        throw InvalidPgnException(context, "Input message is empty");
    }
}

std::uint32_t Message2000::parsePgn(const std::string& context, const std::string& token) {
    if (token.empty()) {
        throw InvalidPgnException(context, "PGN token is empty");
    }

    std::string normalized = token;
    if (normalized.rfind("PGN", 0) == 0) {
        normalized = normalized.substr(3);
    }

    if (normalized.empty()) {
        throw InvalidPgnException(context, "PGN token is empty after PGN prefix removal");
    }

    std::uint64_t parsed = 0;
    try {
        std::size_t parsedChars = 0;
        parsed = static_cast<std::uint64_t>(std::stoull(normalized, &parsedChars, 0));
        if (parsedChars != normalized.size()) {
            throw InvalidPgnException(context, "Invalid PGN token: " + token);
        }
    } catch (const std::exception& e) {
        throw InvalidPgnException(context, "Failed to parse PGN token '" + token + "': " + e.what());
    }

    if (parsed > 262143ULL) {
        throw InvalidPgnException(context, "PGN out of range (0..262143): " + std::to_string(parsed));
    }

    return static_cast<std::uint32_t>(parsed);
}

std::uint32_t Message2000::parseCanId(const std::string& context, const std::string& token) {
    if (token.empty()) {
        throw InvalidPgnException(context, "CAN ID token is empty");
    }

    std::uint64_t parsed = 0;
    try {
        std::size_t parsedChars = 0;
        parsed = static_cast<std::uint64_t>(std::stoull(token, &parsedChars, 16));
        if (parsedChars != token.size()) {
            throw InvalidPgnException(context, "Invalid CAN ID token: " + token);
        }
    } catch (const std::exception& e) {
        throw InvalidPgnException(context, "Failed to parse CAN ID token '" + token + "': " + e.what());
    }

    if (parsed > 0x1FFFFFFFULL) {
        throw InvalidPgnException(context, "CAN ID out of 29-bit range: " + token);
    }

    return static_cast<std::uint32_t>(parsed);
}

std::uint32_t Message2000::extractPgnFromCanId(std::uint32_t canId) {
    const std::uint8_t pduFormat = static_cast<std::uint8_t>((canId >> 16) & 0xFFU);
    const std::uint8_t pduSpecific = static_cast<std::uint8_t>((canId >> 8) & 0xFFU);
    const std::uint8_t dataPage = static_cast<std::uint8_t>((canId >> 24) & 0x01U);

    if (pduFormat < 240U) {
        return (static_cast<std::uint32_t>(dataPage) << 16) |
               (static_cast<std::uint32_t>(pduFormat) << 8);
    }

    std::uint32_t pgn = (static_cast<std::uint32_t>(dataPage) << 16) |
           (static_cast<std::uint32_t>(pduFormat) << 8) |
           static_cast<std::uint32_t>(pduSpecific);

    if ((pgn & 0x10000U) == 0U) {
        pgn |= 0x10000U;
    }
    return pgn;
}

void Message2000::decodeCanIdFields(std::uint32_t canId,
                                    std::uint8_t& priority,
                                    std::uint8_t& dataPage,
                                    std::uint8_t& pduFormat,
                                    std::uint8_t& pduSpecific,
                                    std::uint8_t& sourceAddress) {
    priority = static_cast<std::uint8_t>((canId >> 26) & 0x07U);
    dataPage = static_cast<std::uint8_t>((canId >> 24) & 0x01U);
    pduFormat = static_cast<std::uint8_t>((canId >> 16) & 0xFFU);
    pduSpecific = static_cast<std::uint8_t>((canId >> 8) & 0xFFU);
    sourceAddress = static_cast<std::uint8_t>(canId & 0xFFU);
}

std::vector<std::uint8_t> Message2000::parsePayloadBytes(const std::string& context, const std::string& payloadToken) {
    std::vector<std::uint8_t> bytes;
    if (payloadToken.empty()) {
        return bytes;
    }

    std::string compact;
    compact.reserve(payloadToken.size());
    for (char c : payloadToken) {
        if (!std::isspace(static_cast<unsigned char>(c)) && c != ':') {
            compact.push_back(c);
        }
    }

    if (compact.size() % 2 != 0) {
        throw InvalidPgnException(context, "Payload hex string must have even length");
    }

    for (std::size_t i = 0; i < compact.size(); i += 2) {
        const char hi = compact[i];
        const char lo = compact[i + 1];
        if (!std::isxdigit(static_cast<unsigned char>(hi)) || !std::isxdigit(static_cast<unsigned char>(lo))) {
            throw InvalidPgnException(context, "Payload contains non-hex character");
        }
        std::uint8_t value = static_cast<std::uint8_t>(std::stoul(compact.substr(i, 2), nullptr, 16));
        bytes.push_back(value);
    }

    return bytes;
}

std::string Message2000::bytesToHex(const std::vector<std::uint8_t>& bytes) {
    std::ostringstream ss;
    ss << std::uppercase << std::hex;
    for (std::size_t i = 0; i < bytes.size(); ++i) {
        ss << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(bytes[i]);
    }
    return ss.str();
}

std::string Message2000::bytesToSpacedHex(const std::vector<std::uint8_t>& bytes) {
    std::ostringstream ss;
    ss << std::uppercase << std::hex;
    for (std::size_t i = 0; i < bytes.size(); ++i) {
        if (i > 0) {
            ss << ' ';
        }
        ss << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(bytes[i]);
    }
    return ss.str();
}

std::unique_ptr<nmealib::Message> Message2000::clone() const {
    return std::unique_ptr<Message2000>(new Message2000(*this));
}

std::uint32_t Message2000::getPgn() const noexcept {
    return pgn_;
}

bool Message2000::hasCanId() const noexcept {
    return hasCanId_;
}

std::uint32_t Message2000::getCanId() const noexcept {
    return canId_;
}

std::uint8_t Message2000::getPriority() const noexcept {
    return priority_;
}

std::uint8_t Message2000::getDataPage() const noexcept {
    return dataPage_;
}

std::uint8_t Message2000::getPduFormat() const noexcept {
    return pduFormat_;
}

std::uint8_t Message2000::getPduSpecific() const noexcept {
    return pduSpecific_;
}

std::uint8_t Message2000::getSourceAddress() const noexcept {
    return sourceAddress_;
}

std::string Message2000::getPayload() const noexcept {
    return payload_;
}

const std::vector<std::uint8_t>& Message2000::getPayloadBytes() const noexcept {
    return payloadBytes_;
}

std::string Message2000::getStringContent(bool verbose) const noexcept {
    std::stringstream ss;

    if (!verbose) {
        ss << "[OK] " << typeToString(type_) << " PGN " << pgn_;
        if (hasCanId_) {
            ss << " CANID=0x" << std::uppercase << std::hex << canId_ << std::dec;
        }
        ss << ": " << "Unimplemented PGN";
    } else {
        ss << "Protocol: " << typeToString(type_) << "\n";
        ss << "PGN: " << pgn_ << "\n";
        if (hasCanId_) {
            ss << "CAN ID: 0x" << std::uppercase << std::hex << canId_ << std::dec << "\n";
            ss << "Priority: " << static_cast<unsigned int>(priority_) << "\n";
            ss << "Data Page: " << static_cast<unsigned int>(dataPage_) << "\n";
            ss << "PF: " << static_cast<unsigned int>(pduFormat_) << "\n";
            ss << "PS: " << static_cast<unsigned int>(pduSpecific_) << "\n";
            ss << "Source: " << static_cast<unsigned int>(sourceAddress_) << "\n";
        }
        ss << "Fields: \n";
        ss << "\tUnimplemented PGN";
    }

    return ss.str();
}

std::string Message2000::serialize() const {
    if (hasCanId_) {
        std::ostringstream ss;
        ss << std::uppercase << std::hex << std::setw(8) << std::setfill('0') << canId_;
        if (!payloadBytes_.empty()) {
            ss << ' ' << bytesToSpacedHex(payloadBytes_);
        }
        return ss.str();
    }
    return std::to_string(pgn_) + "#" + bytesToHex(payloadBytes_);
}

bool Message2000::validate() const {
    return true;
}

bool Message2000::operator==(const Message2000& other) const noexcept {
    return pgn_ == other.pgn_ &&
           hasCanId_ == other.hasCanId_ &&
           canId_ == other.canId_ &&
           priority_ == other.priority_ &&
           dataPage_ == other.dataPage_ &&
           pduFormat_ == other.pduFormat_ &&
           pduSpecific_ == other.pduSpecific_ &&
           sourceAddress_ == other.sourceAddress_ &&
           payloadBytes_ == other.payloadBytes_ &&
           Message::operator==(other);
}

bool Message2000::hasEqualContent(const Message2000& other) const noexcept {
    return pgn_ == other.pgn_ &&
           hasCanId_ == other.hasCanId_ &&
           canId_ == other.canId_ &&
           priority_ == other.priority_ &&
           dataPage_ == other.dataPage_ &&
           pduFormat_ == other.pduFormat_ &&
           pduSpecific_ == other.pduSpecific_ &&
           sourceAddress_ == other.sourceAddress_ &&
           payloadBytes_ == other.payloadBytes_;
}

} // namespace nmea2000
} // namespace nmealib
