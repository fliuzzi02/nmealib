#include "nmealib/nmea0183/gll.hpp"

#include <cmath>
#include <iomanip>
#include <sstream>
#include <vector>

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
        double latitude = Message0183::convertNmeaCoordinateToDecimalDegrees(fields[0]);
        char latitudeDirection = fields[1].empty() ? '\0' : fields[1][0];
        double longitude = Message0183::convertNmeaCoordinateToDecimalDegrees(fields[2]);
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
            utcTime_(timestamp),
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
    utcTime_(timestamp),
      status_(status),
      modeIndicator_(modeIndicator) {}

std::unique_ptr<nmealib::Message> GLL::clone() const {
    return std::unique_ptr<GLL>(new GLL(*this));
}

std::string GLL::getStringContent(bool verbose) const noexcept {
    std::ostringstream ss;
    std::ostringstream latStream;
    latStream << std::setprecision(10) << latitude_;
    const std::string latitudeStr = latStream.str();

    std::ostringstream lonStream;
    lonStream << std::setprecision(10) << longitude_;
    const std::string longitudeStr = lonStream.str();

    std::string validity = "KO";
    if (validate()) {
        validity = "OK";
    }

    if (verbose) {
        ss << "Protocol: " << typeToString(type_) << "\n";
        ss << "Talker: " << getTalker() << "\n";
        ss << "Sentence Type: " << getSentenceType() << "\n";
        ss << "Checksum: " << (checksumStr_.empty() ? "None" : validity) << "\n";
        ss << "Fields:\n";
        ss << "\tLatitude: " << latitudeStr << "\n";
        ss << "\tLatitude Direction: " << latitudeDirection_ << "\n";
        ss << "\tLongitude: " << longitudeStr << "\n";
        ss << "\tLongitude Direction: " << longitudeDirection_ << "\n";
        ss << "\tTimestamp: " << utcTime_ << "\n";
        ss << "\tStatus: " << status_ << "\n";
        ss << "\tMode Indicator: " << modeIndicator_;
    } else {
        ss << "[" << validity << "] " << typeToString(type_) << " " << getTalker() << " " << getSentenceType() << ": "
              << "Lat=" << latitudeStr << latitudeDirection_
              << ", Lon=" << longitudeStr << longitudeDirection_
           << ", Time=" << utcTime_
           << ", Status=" << status_
           << ", Mode=" << modeIndicator_;
    }
    return ss.str();
}

std::string GLL::composeRaw(const std::string& talkerId,
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

double GLL::getLatitude() const noexcept {
    return latitude_;
}

char GLL::getLatitudeDirection() const noexcept {
    return latitudeDirection_;
}

double GLL::getLongitude() const noexcept {
    return longitude_;
}

char GLL::getLongitudeDirection() const noexcept {
    return longitudeDirection_;
}

double GLL::getUtcTime() const noexcept {
    return utcTime_;
}

char GLL::getStatus() const noexcept {
    return status_;
}

char GLL::getModeIndicator() const noexcept {
    return modeIndicator_;
}

bool GLL::operator==(const GLL& other) const noexcept {
    return Message0183::operator==(other);
}

bool GLL::hasEqualContent(const GLL& other) const noexcept {
    return Message0183::hasEqualContent(other);
}

} // namespace nmea0183
} // namespace nmealib
