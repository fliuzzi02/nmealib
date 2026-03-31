#include "nmealib/nmea0183/gll.h"

#include "nmealib/detail/errorSupport.h"
#include "nmealib/detail/parse.h"

#include <cmath>
#include <iomanip>
#include <sstream>
#include <vector>

namespace nmealib {
namespace nmea0183 {

std::unique_ptr<GLL> GLL::create(std::unique_ptr<Message0183> baseMessage) {
    std::string context = "GLL::create";
    if (baseMessage->getSentenceType() != "GLL") {
        NMEALIB_RETURN_ERROR(NotGLLException(context, "Expected sentence type 'GLL', got " + baseMessage->getSentenceType()));
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

    if (fields.size() != 6 && fields.size() != 7) {
        NMEALIB_RETURN_ERROR(NotGLLException(context, "Insufficient fields in GLL payload: expected 6 or 7, got " + std::to_string(fields.size()) + ". Payload: " + payload));
    }

    double latitude = 0.0;
    double longitude = 0.0;
    double timestamp = 0.0;
    if (!detail::parseNmeaCoordinate(fields[0], latitude) ||
        !detail::parseNmeaCoordinate(fields[2], longitude) ||
        !detail::parseOptionalDouble(fields[4], timestamp)) {
        NMEALIB_RETURN_ERROR(NmeaException(context, "Error parsing GLL fields"));
    }

    char latitudeDirection = fields[1].empty() ? '\0' : fields[1][0];
    char longitudeDirection = fields[3].empty() ? '\0' : fields[3][0];
    char status = fields[5].empty() ? '\0' : fields[5][0];
    char modeIndicator = (fields.size() > 6 && !fields[6].empty()) ? fields[6][0] : '\0';

    return std::unique_ptr<GLL>(new GLL(std::move(*baseMessage),
                                        latitude,
                                        latitudeDirection,
                                        longitude,
                                        longitudeDirection,
                                        timestamp,
                                        status,
                                        modeIndicator));
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
    ss << this->toString(verbose);
    std::ostringstream latStream;
    latStream << std::setprecision(10) << latitude_;
    const std::string latitudeStr = latStream.str();

    std::ostringstream lonStream;
    lonStream << std::setprecision(10) << longitude_;
    const std::string longitudeStr = lonStream.str();

    if (verbose) {
        ss << "\tLatitude: " << latitudeStr << " " << latitudeDirection_ << "\n";
        ss << "\tLongitude: " << longitudeStr << " " << longitudeDirection_ << "\n";
        ss << "\tTimestamp: " << utcTime_ << "\n";
        ss << "\tStatus: " << status_ << "\n";
        ss << "\tMode Indicator: " << modeIndicator_;
        ss << "\n";
    } else {
        ss << "Lat=" << latitudeStr << latitudeDirection_
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

} // namespace nmea0183
} // namespace nmealib
