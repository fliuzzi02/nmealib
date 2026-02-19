#include "gll.hpp"

#include <cmath>

namespace nmealib {
namespace nmea0183 {

std::unique_ptr<GLL> GLL::create(std::unique_ptr<Message0183> baseMessage) {
    std::string context = "GLL::create";
    if (baseMessage->getSentenceType() != "GLL") {
        throw NotGLLException(context, "Expected sentence type 'GLL', got " + baseMessage->getSentenceType());
    }

    // Parse the payload to extract GLL-specific fields
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

    // Drop first element which is the sentence type (e.g. "GPGLL")
    if (!fields.empty()) {
        fields.erase(fields.begin());
    }

    if (fields.size() != 7) {
        throw NotGLLException(context, "Insufficient fields in GLL payload: expected 7, got " + std::to_string(fields.size()) + ". Payload: " + payload);
    }

    try {
        double latitude = fields[0].empty() ? 0.0 : std::stod(fields[0]);
        latitude = std::floor(latitude / 100) + std::fmod(latitude, 100) / 60; // Convert from ddmm.mmmm to decimal degrees
        char latitudeDirection = fields[1].empty() ? '\0' : fields[1][0];
        double longitude = fields[2].empty() ? 0.0 : std::stod(fields[2]);
        longitude = std::floor(longitude / 100) + std::fmod(longitude, 100) / 60; // Convert from dddmm.mmmm to decimal degrees
        char longitudeDirection = fields[3].empty() ? '\0' : fields[3][0];
        double timestamp = fields[4].empty() ? 0.0 : std::stod(fields[4]);
        char status = fields[5].empty() ? '\0' : fields[5][0];
        char modeIndicator = fields[6].empty() ? '\0' : fields[6][0];

        return std::unique_ptr<GLL>(new GLL(std::move(*baseMessage),
                                            latitude,
                                            latitudeDirection,
                                            longitude,
                                            longitudeDirection,
                                            timestamp,
                                            status,
                                            modeIndicator));
    } catch (const std::exception& e) {
        throw NmeaException(context, "Error parsing GLL fields: " + std::string(e.what()));
    }
}

GLL::GLL(Message0183 baseMessage,
         double latitude,
         char latitudeDirection,
         double longitude,
         char longitudeDirection,
         double timestamp,
         char status,
         char modeIndicator) noexcept
    : Message0183(std::move(baseMessage)),
      latitude_(latitude),
      latitudeDirection_(latitudeDirection),
      longitude_(longitude),
      longitudeDirection_(longitudeDirection),
      timestamp_(timestamp),
      status_(status),
      modeIndicator_(modeIndicator) {}

GLL::GLL(std::string talkerId,
         double latitude,
         char latitudeDirection,
         double longitude,
         char longitudeDirection,
         double timestamp,
         char status,
         char modeIndicator)
    : Message0183(*Message0183::create(composeRaw(talkerId,
                                                  latitude,
                                                  latitudeDirection,
                                                  longitude,
                                                  longitudeDirection,
                                                  timestamp,
                                                  status,
                                                  modeIndicator))),
      latitude_(latitude),
      latitudeDirection_(latitudeDirection),
      longitude_(longitude),
      longitudeDirection_(longitudeDirection),
      timestamp_(timestamp),
      status_(status),
      modeIndicator_(modeIndicator) {}

std::unique_ptr<nmealib::Message> GLL::clone() const {
    return std::unique_ptr<GLL>(new GLL(*this));
}

std::string GLL::getStringContent(bool verbose) const noexcept {
    std::ostringstream ss;
    if (verbose) {
        ss << "GLL Message:\n"
           << "  Latitude: " << latitude_ << "\n"
           << "  Latitude Direction: " << latitudeDirection_ << "\n"
           << "  Longitude: " << longitude_ << "\n"
           << "  Longitude Direction: " << longitudeDirection_ << "\n"
           << "  Timestamp: " << timestamp_ << "\n"
           << "  Status: " << status_ << "\n"
           << "  Mode Indicator: " << modeIndicator_;
    } else {
        ss << "GLL(Lat=" << latitude_ << latitudeDirection_
           << ", Lon=" << longitude_ << longitudeDirection_
           << ", Time=" << timestamp_
           << ", Status=" << status_
           << ", Mode=" << modeIndicator_
           << ")";
    }
    return ss.str();
}

std::string GLL::composeRaw(std::string talkerId,
                            double latitude,
                            char latitudeDirection,
                            double longitude,
                            char longitudeDirection,
                            double timestamp,
                            char status,
                            char modeIndicator) {
    std::ostringstream payloadStream;
    payloadStream << talkerId << "GLL,";

    double latitudeDegrees = std::floor(latitude);
    double latitudeMinutes = (latitude - latitudeDegrees) * 60.0;
    double latitudeNmea = latitudeDegrees * 100.0 + latitudeMinutes; // ddmm.mmmm

    double longitudeDegrees = std::floor(longitude);
    double longitudeMinutes = (longitude - longitudeDegrees) * 60.0;
    double longitudeNmea = longitudeDegrees * 100.0 + longitudeMinutes; // dddmm.mmmm

    payloadStream << std::fixed << std::setprecision(6) << std::setw(10) << std::setfill('0') << latitudeNmea << ",";
    payloadStream << latitudeDirection << ",";
    payloadStream << std::fixed << std::setprecision(6) << std::setw(11) << std::setfill('0') << longitudeNmea << ",";
    payloadStream << longitudeDirection << ",";
    payloadStream << std::fixed << std::setprecision(2) << std::setw(9) << std::setfill('0') << timestamp << ",";
    payloadStream << status << ",";
    payloadStream << modeIndicator;

    std::string payload = payloadStream.str();
    return "$" + payload + "\r\n";
}

} // namespace nmea0183
} // namespace nmealib
