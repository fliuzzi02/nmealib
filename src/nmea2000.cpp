#include "nmealib/nmea2000.h"

#include <cctype>
#include <iomanip>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstring>

namespace nmealib {
namespace nmea2000 {

/**
 * @brief Normalizes raw CAN frame string to the canonical "CANID:data" format.
 * 
 * Supports multiple input formats:
 * - "CANID:data" (already canonical, returned as-is)
 * - "CANID#data" (alternative separator)
 * - "0xCANID, 0xBB 0xCC 0xDD ..." (comma-separated with 0x prefix)
 * - "0xCANID 0xBB 0xCC 0xDD ..." (space-separated with 0x prefix)
 * - "CANID BB CC DD ..." (space-separated without prefix)
 * 
 * @param raw The raw CAN frame string in any supported format.
 * @return std::string The normalized string in "CANID:data" format.
 */
static std::string normalizeRawFormat(const std::string& raw) {
    // If already in CANID:data format, use as-is
    if (raw.find(':') != std::string::npos) {
        return raw;
    }

    // Check for alternative separator (e.g., "CANID#data")
    if (raw.find('#') != std::string::npos) {
        std::string normalized = raw;
        std::replace(normalized.begin(), normalized.end(), '#', ':');
        return normalized;
    }
    
    // Check for comma-separated format (e.g., "0x1CFF63CC, 0x3B 0x9F ...")
    size_t commaPos = raw.find(',');
    if (commaPos != std::string::npos) {
        std::string canIdPart = raw.substr(0, commaPos);
        std::string dataPart = raw.substr(commaPos + 1);
        
        // Clean up CAN ID: remove whitespace and 0x prefix
        canIdPart.erase(std::remove_if(canIdPart.begin(), canIdPart.end(), 
                                       [](unsigned char c) { return std::isspace(c); }), 
                       canIdPart.end());
        if (canIdPart.size() >= 2 && (canIdPart.substr(0, 2) == "0x" || canIdPart.substr(0, 2) == "0X")) {
            canIdPart = canIdPart.substr(2);
        }
        
        // Process data bytes: extract hex values, remove 0x prefix
        std::string dataHex;
        std::istringstream iss(dataPart);
        std::string token;
        while (iss >> token) {
            if (token.size() >= 2 && (token.substr(0, 2) == "0x" || token.substr(0, 2) == "0X")) {
                token = token.substr(2);
            }
            dataHex += token;
        }
        
        return canIdPart + ":" + dataHex;
    }
    
    // Check for space-separated format (e.g., "0x1CFF63CC 0x3B 0x9F ..." or "1CFF63CC 3B 9F ...")
    if (raw.find(' ') != std::string::npos) {
        std::istringstream iss(raw);
        std::string token;
        std::string canId;
        std::string dataHex;
        bool firstToken = true;
        
        while (iss >> token) {
            // Remove 0x prefix if present
            if (token.size() >= 2 && (token.substr(0, 2) == "0x" || token.substr(0, 2) == "0X")) {
                token = token.substr(2);
            }
            
            if (firstToken) {
                canId = token;
                firstToken = false;
            } else {
                dataHex += token;
            }
        }
        
        return canId + ":" + dataHex;
    }
    
    // If no special formatting detected, return as-is and let create() handle validation
    return raw;
}

Message2000::Message2000(std::string raw,
                         TimePoint ts,
                         uint32_t pgn,
                         std::vector<uint8_t> canFrame) noexcept
    : Message(std::move(raw), Type::NMEA2000, ts),
      pgn_(pgn),
      canFrame_(std::move(canFrame)) {
}

std::unique_ptr<Message2000> Message2000::create(std::string raw, TimePoint ts) {
    std::string context = "Message2000::create";
    
    // Normalize raw format to canonical "CANID:data" format
    std::string normalizedRaw = normalizeRawFormat(raw);
    
    // Parse raw string in format "CANID:data"
    size_t colonPos = normalizedRaw.find(':');
    if (colonPos == std::string::npos) {
        throw InvalidCanFrameException(context, "This formatting is not supported");
    }

    // Extract and parse CAN ID
    std::string canIdStr = normalizedRaw.substr(0, colonPos);
    uint32_t canId;
    try {
        canId = std::stoul(canIdStr, nullptr, 16);
    } catch (const std::exception& e) {
        throw InvalidCanFrameException(context, "Invalid CAN ID format: " + canIdStr);
    }

    // Extract and parse frame data
    std::string dataStr = normalizedRaw.substr(colonPos + 1);
    std::vector<uint8_t> frameData;
    
    if (!dataStr.empty()) {
        if (dataStr.length() % 2 != 0) {
            throw InvalidCanFrameException(context, 
                                           "Frame data must have even number of hex characters");
        }

        try {
            for (size_t i = 0; i < dataStr.length(); i += 2) {
                std::string byteStr = dataStr.substr(i, 2);
                uint8_t byte = static_cast<uint8_t>(std::stoul(byteStr, nullptr, 16));
                frameData.push_back(byte);
            }
        } catch (const std::exception& e) {
            throw InvalidCanFrameException(context, "Invalid frame data hex format");
        }
    }

    // Validate frame length (supports single-frame and fast-packet up to 223 bytes)
    if (frameData.size() > 223) {
        throw FrameTooLongException(context, "Frame length: " + std::to_string(frameData.size()));
    }

    // Extract and validate PGN from CAN ID
    uint32_t pgn = extractPgnFromCanId(canId);
    if (!isValidPgn(pgn)) {
        throw InvalidPgnException(context, "PGN out of valid range: 0x" + 
                                 std::to_string(pgn));
    }

    return std::unique_ptr<Message2000>(
        new Message2000(std::move(raw), ts, pgn, frameData)
    );
}

uint32_t Message2000::extractPgnFromCanId(uint32_t canId) noexcept {
    // In NMEA2000, the CAN ID structure is 29-bit extended format:
    // Priority (3 bits) | Reserved (1 bit) | PGN (18 bits) | Source Address (8 bits)
    // The PGN is in bits 8-25 (counting from bit 0)
    return (canId >> 8) & 0x3FFFF;  // Mask 18 bits for PGN
}

bool Message2000::isValidPgn(uint32_t pgn) noexcept {
    // Accept any PGN that fits within 18 bits (0x000000-0x3FFFF)
    // Both single-frame and fast-packet messages use this range
    return pgn <= 0x3FFFF;
}

std::unique_ptr<nmealib::Message> Message2000::clone() const {
    return std::unique_ptr<Message2000>(new Message2000(*this));
}

uint32_t Message2000::getPgn() const noexcept {
    return pgn_;
}

const std::vector<uint8_t>& Message2000::getCanFrame() const noexcept {
    return canFrame_;
}

uint8_t Message2000::getCanFrameLength() const noexcept {
    return static_cast<uint8_t>(canFrame_.size());
}

std::string Message2000::getStringContent(bool verbose) const noexcept {
    std::ostringstream oss;

    if (verbose) {
        oss << "PGN: 0x" << std::hex << std::setfill('0') << std::setw(5) << pgn_ << std::dec << "\n";
        oss << "Frame Length: " << static_cast<int>(getCanFrameLength()) << " bytes\n";
        oss << "Frame Data: ";
        for (size_t i = 0; i < canFrame_.size(); ++i) {
            if (i > 0) oss << " ";
            oss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(canFrame_[i]);
        }
        oss << std::dec;
    } else {
        oss << "PGN=0x" << std::hex << std::setfill('0') << std::setw(5) << pgn_ 
            << " Len=" << static_cast<int>(getCanFrameLength()) << " Data=";
        for (size_t i = 0; i < canFrame_.size(); ++i) {
            if (i > 0) oss << " ";
            oss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(canFrame_[i]);
        }
        oss << std::dec;
    }

    return oss.str();
}

std::string Message2000::serialize() const {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');

    // Output PGN
    oss << std::setw(5) << pgn_ << ":";

    // Output frame data as hex bytes
    for (size_t i = 0; i < canFrame_.size(); ++i) {
        oss << std::setw(2) << static_cast<int>(canFrame_[i]);
    }

    return oss.str();
}

bool Message2000::operator==(const Message2000& other) const noexcept {
    return Message::operator==(other) && pgn_ == other.pgn_ && canFrame_ == other.canFrame_;
}

bool Message2000::validate() const {
    // Validate PGN is valid (fits in 18 bits)
    if (!isValidPgn(pgn_)) {
        return false;
    }

    // Validate frame length (CAN single/fast-packet is 0-223 bytes)
    if (canFrame_.size() > 223) {
        return false;
    }

    return true;
}

} // namespace nmea2000
} // namespace nmealib
