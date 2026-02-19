#include "gsa.hpp"

#include <iomanip>
#include <sstream>
#include <vector>

namespace nmealib {
namespace nmea0183 {

std::unique_ptr<GSA> GSA::create(std::unique_ptr<Message0183> baseMessage) {
    std::string context = "GSA::create";
    if (baseMessage->getSentenceType() != "GSA") {
        throw NotGSAException(context, "Expected sentence type 'GSA', got " + baseMessage->getSentenceType());
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
        throw NotGSAException(context, "Invalid fields in GSA payload: expected 17 or 18, got " + std::to_string(fields.size()) + ". Payload: " + payload);
    }

    try {
        char selectionMode = fields[0].empty() ? '\0' : fields[0][0];
        unsigned int mode = fields[1].empty() ? 0u : std::stoul(fields[1]);

        std::array<unsigned int, 12> satelliteIds{};
        for (size_t index = 0; index < satelliteIds.size(); ++index) {
            const std::string& field = fields[2 + index];
            satelliteIds[index] = field.empty() ? 0u : std::stoul(field);
        }

        double pdop = fields[14].empty() ? 0.0 : std::stod(fields[14]);
        double hdop = fields[15].empty() ? 0.0 : std::stod(fields[15]);
        double vdop = fields[16].empty() ? 0.0 : std::stod(fields[16]);
        std::optional<unsigned int> systemId = std::nullopt;

        if (fields.size() == 18 && !fields[17].empty()) {
            systemId = std::stoul(fields[17]);
        }

        return std::unique_ptr<GSA>(new GSA(std::move(*baseMessage),
                                            selectionMode,
                                            mode,
                                            satelliteIds,
                                            pdop,
                                            hdop,
                                            vdop,
                                            systemId));
    } catch (const std::exception& e) {
        throw NmeaException(context, "Error parsing GSA fields: " + std::string(e.what()));
    }
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
    if (verbose) {
        ss << "GSA Message:\n"
           << "  Selection Mode: " << selectionMode_ << "\n"
           << "  Mode: " << mode_ << "\n"
           << "  Satellites: ";

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
           << "  PDOP: " << pdop_ << "\n"
           << "  HDOP: " << hdop_ << "\n"
           << "  VDOP: " << vdop_;

        if (systemId_.has_value()) {
            ss << "\n"
               << "  System ID: " << systemId_.value();
        }
    } else {
        ss << "GSA(SelectionMode=" << selectionMode_
           << ", Mode=" << mode_
           << ", PDOP=" << pdop_
           << ", HDOP=" << hdop_
           << ", VDOP=" << vdop_;
        if (systemId_.has_value()) {
            ss << ", SystemID=" << systemId_.value();
        }
        ss << ")";
    }

    return ss.str();
}

std::string GSA::composeRaw(std::string talkerId,
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

} // namespace nmea0183
} // namespace nmealib
