#include "mwv.hpp"

#include <iomanip>
#include <sstream>
#include <vector>

namespace nmealib {
namespace nmea0183 {

std::unique_ptr<MWV> MWV::create(std::unique_ptr<Message0183> baseMessage) {
    std::string context = "MWV::create";
    if (baseMessage->getSentenceType() != "MWV") {
        throw NotMWVException(context, "Expected sentence type 'MWV', got " + baseMessage->getSentenceType());
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

    if (fields.size() != 5) {
        throw NotMWVException(context, "Invalid fields in MWV payload: expected 5, got " + std::to_string(fields.size()) + ". Payload: " + payload);
    }

    try {
        double windAngle = fields[0].empty() ? 0.0 : std::stod(fields[0]);
        char reference = fields[1].empty() ? '\0' : fields[1][0];
        double windSpeed = fields[2].empty() ? 0.0 : std::stod(fields[2]);
        char windSpeedUnits = fields[3].empty() ? '\0' : fields[3][0];
        char status = fields[4].empty() ? '\0' : fields[4][0];

        return std::unique_ptr<MWV>(new MWV(std::move(*baseMessage),
                                            windAngle,
                                            reference,
                                            windSpeed,
                                            windSpeedUnits,
                                            status));
    } catch (const std::exception& e) {
        throw NmeaException(context, "Error parsing MWV fields: " + std::string(e.what()));
    }
}

MWV::MWV(Message0183 baseMessage,
         double windAngle,
         char reference,
         double windSpeed,
         char windSpeedUnits,
         char status) noexcept
    : Message0183(std::move(baseMessage)),
      windAngle_(windAngle),
      reference_(reference),
      windSpeed_(windSpeed),
      windSpeedUnits_(windSpeedUnits),
      status_(status) {}

MWV::MWV(std::string talkerId,
         double windAngle,
         char reference,
         double windSpeed,
         char windSpeedUnits,
         char status)
    : Message0183(*Message0183::create(composeRaw(talkerId,
                                                  windAngle,
                                                  reference,
                                                  windSpeed,
                                                  windSpeedUnits,
                                                  status))),
      windAngle_(windAngle),
      reference_(reference),
      windSpeed_(windSpeed),
      windSpeedUnits_(windSpeedUnits),
      status_(status) {}

std::unique_ptr<nmealib::Message> MWV::clone() const {
    return std::unique_ptr<MWV>(new MWV(*this));
}

std::string MWV::getStringContent(bool verbose) const noexcept {
    std::ostringstream ss;
    if (verbose) {
        ss << "MWV Message:\n"
           << "  Wind Angle: " << windAngle_ << "\n"
           << "  Reference: " << reference_ << "\n"
           << "  Wind Speed: " << windSpeed_ << "\n"
           << "  Wind Speed Units: " << windSpeedUnits_ << "\n"
           << "  Status: " << status_;
    } else {
        ss << "MWV(Angle=" << windAngle_
           << ", Ref=" << reference_
           << ", Speed=" << windSpeed_ << windSpeedUnits_
           << ", Status=" << status_
           << ")";
    }

    return ss.str();
}

std::string MWV::composeRaw(std::string talkerId,
                            double windAngle,
                            char reference,
                            double windSpeed,
                            char windSpeedUnits,
                            char status) {
    std::ostringstream payloadStream;
    payloadStream << talkerId << "MWV,";
    payloadStream << std::fixed << std::setprecision(1) << windAngle << ",";
    payloadStream << reference << ",";
    payloadStream << std::fixed << std::setprecision(1) << windSpeed << ",";
    payloadStream << windSpeedUnits << ",";
    payloadStream << status;

    std::string payload = payloadStream.str();
    return "$" + payload + "\r\n";
}

} // namespace nmea0183
} // namespace nmealib
