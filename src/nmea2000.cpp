#include "nmealib/nmea2000.h"

#include "nmealib/detail/errorSupport.h"
#include "nmealib/detail/parse.h"

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
    if (raw.find(':') != std::string::npos) {
        return raw;
    }

    if (raw.find('#') != std::string::npos) {
        std::string normalized = raw;
        std::replace(normalized.begin(), normalized.end(), '#', ':');
        return normalized;
    }

    size_t commaPos = raw.find(',');
    if (commaPos != std::string::npos) {
        std::string canIdPart = raw.substr(0, commaPos);
        std::string dataPart  = raw.substr(commaPos + 1);

        canIdPart.erase(std::remove_if(canIdPart.begin(), canIdPart.end(),
                                       [](unsigned char c) { return std::isspace(c); }),
                        canIdPart.end());
        if (canIdPart.size() >= 2 &&
            (canIdPart.substr(0, 2) == "0x" || canIdPart.substr(0, 2) == "0X")) {
            canIdPart = canIdPart.substr(2);
        }

        std::string dataHex;
        std::istringstream iss(dataPart);
        std::string token;
        while (iss >> token) {
            if (token.size() >= 2 &&
                (token.substr(0, 2) == "0x" || token.substr(0, 2) == "0X")) {
                token = token.substr(2);
            }
            dataHex += token;
        }

        return canIdPart + ":" + dataHex;
    }

    if (raw.find(' ') != std::string::npos) {
        std::istringstream iss(raw);
        std::string token;
        std::string canId;
        std::string dataHex;
        bool firstToken = true;

        while (iss >> token) {
            if (token.size() >= 2 &&
                (token.substr(0, 2) == "0x" || token.substr(0, 2) == "0X")) {
                token = token.substr(2);
            }
            if (firstToken) {
                canId      = token;
                firstToken = false;
            } else {
                dataHex += token;
            }
        }

        return canId + ":" + dataHex;
    }

    return raw;
}

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------

Message2000::Message2000(std::string raw,
                         TimePoint ts,
                         std::vector<uint8_t> canId,
                         std::vector<uint8_t> canFrame) noexcept
    : Message(std::move(raw), Type::NMEA2000, ts),
      canId_(std::move(canId)),
      canFrame_(std::move(canFrame)) {}

// ---------------------------------------------------------------------------
// Factory
// ---------------------------------------------------------------------------

std::unique_ptr<Message2000> Message2000::create(std::string raw, TimePoint ts) {
    const std::string context = "Message2000::create";

    std::string normalizedRaw = normalizeRawFormat(raw);

    // ---- Split at ':' -------------------------------------------------------
    size_t colonPos = normalizedRaw.find(':');
    if (colonPos == std::string::npos) {
        NMEALIB_RETURN_ERROR(InvalidCanFrameException(context, "This formatting is not supported"));
    }

    // ---- Parse CAN ID (29-bit value, stored in 4 bytes big-endian) ----------
    std::string canIdStr = normalizedRaw.substr(0, colonPos);
    std::transform(canIdStr.begin(), canIdStr.end(), canIdStr.begin(), ::toupper);
    std::vector<uint8_t> canId;

    if (canIdStr.empty()) {
        NMEALIB_RETURN_ERROR(InvalidCanFrameException(context, "CAN ID is missing"));
    }

    unsigned long canIdValue = 0;
    // A 29-bit value fits in at most 8 hex digits; the top 3 bits of a uint32
    // must be zero (max valid value is 0x1FFFFFFF).
    if (!detail::parseUnsigned(canIdStr, canIdValue, 16) || canIdValue > 0x1FFFFFFFU) {
        NMEALIB_RETURN_ERROR(InvalidCanFrameException(context, "Invalid CAN ID format"));
    }

    // Store as 4 bytes, big-endian.
    //
    // With a 29-bit value right-aligned in a uint32_t the byte layout is:
    //
    //   canId[0] = bits 28-21: [0  0  0  P3 P2 P1 R1 DP ]
    //   canId[1] = bits 20-13: [PF8 PF7 PF6 PF5 PF4 PF3 PF2 PF1]
    //   canId[2] = bits 12- 5: [PS8 PS7 PS6 PS5 PS4 PS3 PS2 PS1]
    //   canId[3] = bits  4- 0: [SA8 SA7 SA6 SA5 SA4 SA3 SA2 SA1]
    //                           (the three MSBs of byte 0 are always 0)
    //
    // Note: RTR and DLC are part of the CAN bus framing layer; they are handled
    // by the hardware/driver and are NOT present in the 29-bit CAN Id.
    canId.push_back(static_cast<uint8_t>((canIdValue >> 24) & 0xFF)); // [0]
    canId.push_back(static_cast<uint8_t>((canIdValue >> 16) & 0xFF)); // [1]
    canId.push_back(static_cast<uint8_t>((canIdValue >>  8) & 0xFF)); // [2]
    canId.push_back(static_cast<uint8_t>( canIdValue        & 0xFF)); // [3]

    // ---- Parse frame data ---------------------------------------------------
    std::string dataStr = normalizedRaw.substr(colonPos + 1);
    std::vector<uint8_t> frameData;

    if (!dataStr.empty()) {
        if (dataStr.length() % 2 != 0) {
            NMEALIB_RETURN_ERROR(InvalidCanFrameException(
                context, "Frame data must have even number of hex characters"));
        }

        for (size_t i = 0; i < dataStr.length(); i += 2) {
            std::string byteStr = dataStr.substr(i, 2);
            unsigned int byte   = 0;
            if (!detail::parseUnsigned(byteStr, byte, 16) || byte > 0xFFU) {
                NMEALIB_RETURN_ERROR(
                    InvalidCanFrameException(context, "Invalid frame data hex format"));
            }
            frameData.push_back(static_cast<uint8_t>(byte));
        }
    }

    // ---- Validate frame length (0-223 bytes) --------------------------------
    if (frameData.size() > 223) {
        NMEALIB_RETURN_ERROR(FrameTooLongException(
            context, "Frame length: " + std::to_string(frameData.size())));
    }

    // ---- Validate PGN -------------------------------------------------------
    uint32_t pgn = extractPgnFromCanId(canId);
    if (!isValidPgn(pgn)) {
        NMEALIB_RETURN_ERROR(InvalidPgnException(
            context, "PGN out of valid range: 0x" + std::to_string(pgn)));
    }

    return std::unique_ptr<Message2000>(
        new Message2000(std::move(raw), ts, std::move(canId), std::move(frameData)));
}

// ---------------------------------------------------------------------------
// PGN extraction
// ---------------------------------------------------------------------------

uint32_t Message2000::extractPgnFromCanId(const std::vector<uint8_t>& canId) noexcept {
    // canId layout (4 bytes, big-endian, 29-bit value right-aligned):
    //
    //   canId[0]: [0  0  0  P3 P2 P1 R1 DP ]
    //   canId[1]: [PF8 PF7 PF6 PF5 PF4 PF3 PF2 PF1]  ← full PDU Format byte
    //   canId[2]: [PS8 PS7 PS6 PS5 PS4 PS3 PS2 PS1]  ← full PDU Specific byte
    //   canId[3]: [SA8 SA7 SA6 SA5 SA4 SA3 SA2 SA1]  ← full Source Address byte
    //
    // RDP = { R1, DP } = the two LSBs of canId[0].
    // PF  = canId[1]
    // PS  = canId[2]
    //
    // PGN formation (per ISO 11783 / CANboat spec):
    //   PDU1 (PF < 0xF0): PS is the destination address; PGN lower byte is 0.
    //       PGN = [RDP][PF][00]
    //   PDU2 (PF >= 0xF0): destination is always global (255); PS is part of PGN.
    //       PGN = [RDP][PF][PS]

    const uint8_t rdp = canId[0] & 0x03; // bits: R1=1, DP=0
    const uint8_t pf  = canId[1];        // PDU Format
    const uint8_t ps  = canId[2];        // PDU Specific

    if (pf < 0xF0) {
        // PDU1 — addressed; lower 8 bits of PGN are always zero
        return (static_cast<uint32_t>(rdp) << 16) |
               (static_cast<uint32_t>(pf)  <<  8);
    } else {
        // PDU2 — broadcast; PS contributes to the PGN
        return (static_cast<uint32_t>(rdp) << 16) |
               (static_cast<uint32_t>(pf)  <<  8) |
                static_cast<uint32_t>(ps);
    }
}

// ---------------------------------------------------------------------------
// PGN validation
// ---------------------------------------------------------------------------

bool Message2000::isValidPgn(uint32_t pgn) noexcept {
    // PGN is 18 bits wide (RDP=2 + PF=8 + PS=8), so max value is 0x3FFFF.
    return pgn <= 0x3FFFFU;
}

// ---------------------------------------------------------------------------
// Clone
// ---------------------------------------------------------------------------

std::unique_ptr<nmealib::Message> Message2000::clone() const {
    return std::unique_ptr<Message2000>(new Message2000(*this));
}

// ---------------------------------------------------------------------------
// CAN Id field accessors
//
// canId[0]: [0  0  0  P3 P2 P1 R1 DP]
// canId[1]: [PF8 PF7 PF6 PF5 PF4 PF3 PF2 PF1]
// canId[2]: [PS8 PS7 PS6 PS5 PS4 PS3 PS2 PS1]
// canId[3]: [SA8 SA7 SA6 SA5 SA4 SA3 SA2 SA1]
// ---------------------------------------------------------------------------

// Priority is a 3-bit value in bits [4:2] of canId[0].
uint8_t Message2000::getPriority() const noexcept {
    return (canId_[0] >> 2) & 0x07;
}

// Individual priority bits (P3 is the MSB of the 3-bit priority field).
bool Message2000::getPriority3() const noexcept { return (canId_[0] & 0x10) != 0; } // bit 4
bool Message2000::getPriority2() const noexcept { return (canId_[0] & 0x08) != 0; } // bit 3
bool Message2000::getPriority1() const noexcept { return (canId_[0] & 0x04) != 0; } // bit 2

// Reserved bit (R1) is bit 1 of canId[0].
bool Message2000::getReserved() const noexcept  { return (canId_[0] & 0x02) != 0; }

// Data Page (DP) is bit 0 of canId[0].
bool Message2000::getDataPage() const noexcept  { return (canId_[0] & 0x01) != 0; }

// PDU Format is the full canId[1] byte.
uint8_t Message2000::getPDUFormat() const noexcept   { return canId_[1]; }

// PDU Specific is the full canId[2] byte.
// When PF < 0xF0 this is the destination address; otherwise it is the PGN group extension.
uint8_t Message2000::getPDUSpecific() const noexcept { return canId_[2]; }

// Source Address is the full canId[3] byte.
uint8_t Message2000::getSourceAddress() const noexcept { return canId_[3]; }

// Destination address: only meaningful for PDU1 messages (PF < 0xF0).
// For PDU2 messages the destination is always global (255).
uint8_t Message2000::getDestinationAddress() const noexcept {
    return (getPDUFormat() < 0xF0) ? getPDUSpecific() : 0xFF;
}

// NOTE: RTR and DLC are CAN bus framing fields handled by the hardware/driver.
// They are NOT encoded in the 29-bit CAN Id and therefore cannot be extracted
// from canId_. The original getRemoteTransmissionRequest() and getDataLengthCode()
// methods have been removed. If needed they must be stored as separate fields.

// ---------------------------------------------------------------------------
// PGN accessor
// ---------------------------------------------------------------------------

uint32_t Message2000::getPgn() const noexcept {
    return extractPgnFromCanId(canId_);
}

// ---------------------------------------------------------------------------
// Frame accessors
// ---------------------------------------------------------------------------

const std::vector<uint8_t>& Message2000::getCanId() const noexcept {
    return canId_;
}

const std::vector<uint8_t>& Message2000::getCanFrame() const noexcept {
    return canFrame_;
}

uint8_t Message2000::getCanFrameLength() const noexcept {
    return static_cast<uint8_t>(canFrame_.size());
}

// ---------------------------------------------------------------------------
// String representation
// ---------------------------------------------------------------------------

std::string Message2000::getStringContent(bool verbose) const noexcept {
    std::ostringstream oss;

    if (verbose) {
        oss << toString(true) << "\n";
    } else {
        oss << toString(false) << "Data=";
        for (size_t i = 0; i < canFrame_.size(); ++i) {
            if (i > 0) oss << " ";
            oss << std::hex << std::setfill('0') << std::setw(2)
                << static_cast<int>(canFrame_[i]);
        }
        oss << std::dec;
    }
    return oss.str();
}

std::string Message2000::toString(bool verbose) const noexcept {
    std::ostringstream oss;

    if (verbose) {
        const uint32_t pgn = getPgn();
        const uint8_t  pf  = getPDUFormat();

        oss << "--------------------------------\n";
        oss << "Protocol:    " << typeToString(type_) << "\n";
        oss << "Priority:    " << static_cast<int>(getPriority()) << "\n";
        oss << "Data Page:   " << (getDataPage() ? "1" : "0") << "\n";
        oss << "PDU Format:  0x" << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<int>(pf) << std::dec
            << (pf < 0xF0 ? " (PDU1 - addressed)" : " (PDU2 - broadcast)") << "\n";
        oss << "Destination: ";
        if (pf < 0xF0) {
            oss << static_cast<int>(getDestinationAddress());
        } else {
            oss << "255 (global)";
        }
        oss << "\n";
        oss << "Source Addr: " << static_cast<int>(getSourceAddress()) << "\n";
        oss << "PGN:         " << pgn
            << " (0x" << std::hex << pgn << std::dec << ")\n";
        oss << "Frame Len:   " << static_cast<int>(getCanFrameLength()) << " bytes\n";
        oss << "Frame Data:  ";
        for (size_t i = 0; i < canFrame_.size(); ++i) {
            if (i > 0) oss << " ";
            oss << std::hex << std::setfill('0') << std::setw(2)
                << static_cast<int>(canFrame_[i]);
        }
        oss << std::dec;
    } else {
        oss << "[OK] " << typeToString(type_) << " PGN" << getPgn() << ": ";
    }

    return oss.str();
}

// ---------------------------------------------------------------------------
// Serialization
// ---------------------------------------------------------------------------

std::string Message2000::serialize() const {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');

    for (const auto byte : canId_) {
        oss << std::setw(2) << static_cast<int>(byte);
    }

    oss << ":";

    for (const auto byte : canFrame_) {
        oss << std::setw(2) << static_cast<int>(byte);
    }

    return oss.str();
}

// ---------------------------------------------------------------------------
// Equality and validation
// ---------------------------------------------------------------------------

bool Message2000::operator==(const Message2000& other) const noexcept {
    return type_ == other.type_ &&
           canId_ == other.canId_ &&
           canFrame_ == other.canFrame_;
}

bool Message2000::validate() const {
    if (!isValidPgn(getPgn())) {
        return false;
    }
    if (canFrame_.size() > 223) {
        return false;
    }
    return true;
}

} // namespace nmea2000
} // namespace nmealib