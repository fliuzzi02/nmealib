#include "nmealib/nmea0183/gsa.h"

#include "nmealib/detail/errorSupport.h"
#include "nmealib/detail/parse.h"

#include <iomanip>
#include <sstream>
#include <vector>

namespace nmealib {
namespace nmea0183 {

std::unique_ptr<GSA> GSA::create(std::unique_ptr<Message0183> baseMessage) {
    std::string context = "GSA::create";
    if (baseMessage->getSentenceType() != "GSA") {
        NMEALIB_RETURN_ERROR(NotGSAException(context, "Expected sentence type 'GSA', got " + baseMessage->getSentenceType()));
    }

    std::string payload = baseMessage->getPayload();
    std::istringstream ss(payload);
    std::string token;
    std::vector<std::string> fields;

    while (std::getline(ss, token, ',')) {
        fields.push_back(token);
    }

    if (!payload.empty() && payload.back() == ',') {
        fields.push_back("");
    }

    if (!fields.empty()) {
        fields.erase(fields.begin());
    }

    if (fields.size() != 17 && fields.size() != 18) {
        NMEALIB_RETURN_ERROR(NotGSAException(context, "Invalid fields in GSA payload: expected 17 or 18, got " + std::to_string(fields.size()) + ". Payload: " + payload));
    }

    char selectionMode = fields[0].empty() ? '\0' : fields[0][0];
    unsigned int mode = 0U;
    if (!detail::parseOptionalUnsigned(fields[1], mode)) {
        NMEALIB_RETURN_ERROR(NmeaException(context, "Error parsing GSA fields"));
    }

    std::array<unsigned int, 12> satelliteIds{};
    for (size_t index = 0; index < satelliteIds.size(); ++index) {
        if (!detail::parseOptionalUnsigned(fields[2 + index], satelliteIds[index])) {
            NMEALIB_RETURN_ERROR(NmeaException(context, "Error parsing GSA fields"));
        }
    }

    double pdop = 0.0;
    double hdop = 0.0;
    double vdop = 0.0;
    if (!detail::parseOptionalDouble(fields[14], pdop) ||
        !detail::parseOptionalDouble(fields[15], hdop) ||
        !detail::parseOptionalDouble(fields[16], vdop)) {
        NMEALIB_RETURN_ERROR(NmeaException(context, "Error parsing GSA fields"));
    }

    std::optional<unsigned int> systemId = std::nullopt;
    if (fields.size() == 18 && !fields[17].empty()) {
        unsigned int parsedSystemId = 0U;
        if (!detail::parseUnsigned(fields[17], parsedSystemId)) {
            NMEALIB_RETURN_ERROR(NmeaException(context, "Error parsing GSA fields"));
        }
        systemId = parsedSystemId;
    }

    return std::unique_ptr<GSA>(new GSA(std::move(*baseMessage),
                                        selectionMode,
                                        mode,
                                        satelliteIds,
                                        pdop,
                                        hdop,
                                        vdop,
                                        systemId));
}

GSA::GSA(Message0183 baseMessage,
         char selectionMode,
         unsigned int mode,
         std::array<unsigned int, 12> satelliteIds,
         double pdop,
         double hdop,
         double vdop,
         std::optional<unsigned int> systemId) noexcept
    : Message0183(std::move(baseMessage)),
      selectionMode_(selectionMode),
      mode_(mode),
      satelliteIds_(satelliteIds),
      pdop_(pdop),
      hdop_(hdop),
      vdop_(vdop),
      systemId_(systemId) {}

GSA::GSA(std::string talkerId,
         char selectionMode,
         unsigned int mode,
         std::array<unsigned int, 12> satelliteIds,
         double pdop,
         double hdop,
         double vdop,
         std::optional<unsigned int> systemId)
    : Message0183(*Message0183::create(composeRaw(talkerId,
                                                  selectionMode,
                                                  mode,
                                                  satelliteIds,
                                                  pdop,
                                                  hdop,
                                                  vdop,
                                                  systemId))),
      selectionMode_(selectionMode),
      mode_(mode),
      satelliteIds_(satelliteIds),
      pdop_(pdop),
      hdop_(hdop),
      vdop_(vdop),
      systemId_(systemId) {}

std::unique_ptr<nmealib::Message> GSA::clone() const {
    return std::unique_ptr<GSA>(new GSA(*this));
}

std::string GSA::getStringContent(bool verbose) const noexcept {
    std::ostringstream ss;
    ss << this->toString(verbose);

    if (verbose) {
        ss << "\tSelection Mode: " << selectionMode_ << "\n";
        ss << "\tMode: " << mode_ << "\n";
        ss << "\tSatellites: ";

        bool first = true;
        for (unsigned int satelliteId : satelliteIds_) {
            if (satelliteId == 0u) {
                continue;
            }
            if (!first) {
                ss << ",";
            }
            ss << satelliteId;
            first = false;
        }
        if (first) {
            ss << "None";
        }

          ss << "\n"
              << "\tPDOP: " << pdop_ << "\n"
              << "\tHDOP: " << hdop_ << "\n"
              << "\tVDOP: " << vdop_;

        if (systemId_.has_value()) {
            ss << "\n"
                    << "\tSystem ID: " << systemId_.value();
        }
        ss << "\n";
    } else {
        ss << "SelectionMode=" << selectionMode_
           << ", Mode=" << mode_
           << ", PDOP=" << pdop_
           << ", HDOP=" << hdop_
           << ", VDOP=" << vdop_;
        if (systemId_.has_value()) {
            ss << ", SystemID=" << systemId_.value();
        }
    }

    return ss.str();
}

std::string GSA::composeRaw(const std::string& talkerId,
                            char selectionMode,
                            unsigned int mode,
                            std::array<unsigned int, 12> satelliteIds,
                            double pdop,
                            double hdop,
                            double vdop,
                            std::optional<unsigned int> systemId) {
    std::ostringstream payloadStream;
    payloadStream << talkerId << "GSA,";
    payloadStream << selectionMode << ",";
    payloadStream << mode;

    for (unsigned int satelliteId : satelliteIds) {
        payloadStream << ",";
        if (satelliteId != 0u) {
            payloadStream << std::setw(2) << std::setfill('0') << satelliteId;
        }
    }

    payloadStream << "," << std::fixed << std::setprecision(2) << pdop;
    payloadStream << "," << std::fixed << std::setprecision(2) << hdop;
    payloadStream << "," << std::fixed << std::setprecision(2) << vdop;

    if (systemId.has_value()) {
        payloadStream << "," << systemId.value();
    }

    std::string payload = payloadStream.str();
    return "$" + payload + "\r\n";
}

char GSA::getSelectionMode() const noexcept {
    return selectionMode_;
}

unsigned int GSA::getMode() const noexcept {
    return mode_;
}

std::array<unsigned int, 12> GSA::getSatelliteIds() const noexcept {
    return satelliteIds_;
}

unsigned int GSA::getSatelliteId(size_t index) const noexcept {
    return index < satelliteIds_.size() ? satelliteIds_[index] : 0u;
}

double GSA::getPdop() const noexcept {
    return pdop_;
}

double GSA::getHdop() const noexcept {
    return hdop_;
}

double GSA::getVdop() const noexcept {
    return vdop_;
}

bool GSA::hasSystemId() const noexcept {
    return systemId_.has_value();
}

std::optional<unsigned int> GSA::getSystemId() const noexcept {
    return systemId_;
}

bool GSA::operator==(const GSA& other) const noexcept {
    return Message0183::operator==(other);
}

} // namespace nmea0183
} // namespace nmealib
