#include "nmealib/nmea0183/rmc.h"

#include "nmealib/detail/errorSupport.h"
#include "nmealib/detail/parse.h"
#include <cmath>
#include <iomanip>
#include <sstream>
#include <vector>

#include "nmealib/nmea0183/nmea0183Factory.h"

namespace nmealib {
namespace nmea0183 {

std::unique_ptr<RMC> RMC::create(std::unique_ptr<Message0183> baseMessage) {
    std::string context = "RMC::create";
    if (baseMessage->getSentenceType() != "RMC") {
        NMEALIB_RETURN_ERROR(NotRMCException(context, "Expected sentence type 'RMC', got " + baseMessage->getSentenceType()));
    }

    // Parse the payload to extract RMC-specific fields
    std::string payload = baseMessage->getPayload();
    std::istringstream ss(payload);
    std::string token;
    std::vector<std::string> fields;

    while (std::getline(ss, token, ',')) {
        fields.push_back(token);
    }

    // Drop first element which is the sentence type (e.g. "GPRMC")
    if (!fields.empty()) {
        fields.erase(fields.begin());
    }

    size_t messageSize = fields.size();
    if (messageSize != 12 && messageSize != 13) {
        NMEALIB_RETURN_ERROR(NotRMCException(context, "Insufficient fields in RMC payload: expected 12 or 13, got " + std::to_string(fields.size()) + ". Payload: " + payload));
    }

    unsigned int utcFix = 0U;
    double latitude = 0.0;
    double longitude = 0.0;
    double speedOverGround = 0.0;
    double courseOverGround = 0.0;
    unsigned int date = 0U;
    double magneticVariation = 0.0;
    if (!detail::parseOptionalUnsigned(fields[0], utcFix) ||
        !detail::parseNmeaCoordinate(fields[2], latitude) ||
        !detail::parseNmeaCoordinate(fields[4], longitude) ||
        !detail::parseOptionalDouble(fields[6], speedOverGround) ||
        !detail::parseOptionalDouble(fields[7], courseOverGround) ||
        !detail::parseOptionalUnsigned(fields[8], date) ||
        !detail::parseOptionalDouble(fields[9], magneticVariation)) {
        NMEALIB_RETURN_ERROR(NmeaException(context, "Error parsing RMC fields"));
    }

    char status = fields[1].empty() ? '\0' : fields[1][0];
    char latDirection = fields[3].empty() ? '\0' : fields[3][0];
    char lonDirection = fields[5].empty() ? '\0' : fields[5][0];
    char magneticVariationDirection = fields[10].empty() ? '\0' : fields[10][0];
    char modeIndicator = '\0';
    char navigationStatus = '\0';

    if (messageSize == 12) {
        navigationStatus = fields[11].empty() ? '\0' : fields[11][0];
    } else {
        modeIndicator = fields[11].empty() ? '\0' : fields[11][0];
        navigationStatus = fields[12].empty() ? '\0' : fields[12][0];
    }

    return std::unique_ptr<RMC>(new RMC(std::move(*baseMessage), utcFix, status, latitude, latDirection, longitude, lonDirection, speedOverGround, courseOverGround, date, magneticVariation, magneticVariationDirection, modeIndicator, navigationStatus));
}

RMC::RMC(Message0183 baseMessage, 
         unsigned int utcFix, 
         char status, 
         double latitude,
         char latitudeDirection, 
         double longitude, 
         char longitudeDirection,
         double speedOverGround, 
         double courseOverGround, 
         unsigned int date, 
         double magneticVariation,
         char magneticVariationDirection,
         char modeIndicator,
         char navigationStatus) noexcept
    : Message0183(std::move(baseMessage)), 
      utcFix_(utcFix), 
      status_(status), 
      latitude_(latitude), 
      latitudeDirection_(latitudeDirection),
      longitude_(longitude), 
      longitudeDirection_(longitudeDirection),
      speedOverGround_(speedOverGround), 
      courseOverGround_(courseOverGround), 
      date_(date), 
      magneticVariation_(magneticVariation),
      magneticVariationDirection_(magneticVariationDirection),
      modeIndicator_(modeIndicator),
      navigationStatus_(navigationStatus) {}

RMC::RMC(std::string talkerId, unsigned int utcFix, 
        char status, 
        double latitude,
        char latitudeDirection, 
        double longitude,
        char longitudeDirection,
        double speedOverGround, 
        double courseOverGround, 
        unsigned int date, 
        double magneticVariation,
        char magneticVariationDirection,
        char modeIndicator,
        char navigationStatus) : 
        Message0183(*Message0183::create(composeRaw(talkerId, utcFix, status, latitude, 
                                latitudeDirection, longitude, longitudeDirection, 
                                speedOverGround, courseOverGround, date, magneticVariation, 
                                magneticVariationDirection, modeIndicator, navigationStatus))),
        utcFix_(utcFix),
        status_(status),
        latitude_(latitude),
        latitudeDirection_(latitudeDirection),
        longitude_(longitude),
        longitudeDirection_(longitudeDirection),
        speedOverGround_(speedOverGround),
        courseOverGround_(courseOverGround),
        date_(date),
        magneticVariation_(magneticVariation),
        magneticVariationDirection_(magneticVariationDirection),
        modeIndicator_(modeIndicator),
        navigationStatus_(navigationStatus) {}

std::unique_ptr<nmealib::Message> RMC::clone() const {
    return std::unique_ptr<RMC>(new RMC(*this));
}

std::string RMC::getStringContent(bool verbose) const noexcept {
    std::ostringstream ss;
    ss << this->toString(verbose);
    std::ostringstream latStream;
    latStream << std::setprecision(10) << latitude_;
    const std::string latitudeStr = latStream.str();

    std::ostringstream lonStream;
    lonStream << std::setprecision(10) << longitude_;
    const std::string longitudeStr = lonStream.str();

    if (verbose) {
        ss << "\tUTC Fix: " << utcFix_ << "\n";
        ss << "\tStatus: " << status_ << "\n";
        ss << "\tLatitude: " << latitudeStr << " " << latitudeDirection_ << "\n";
        ss << "\tLongitude: " << longitudeStr << " " << longitudeDirection_ << "\n";
        ss << "\tSpeed Over Ground: " << speedOverGround_ << "\n";
        ss << "\tCourse Over Ground: " << courseOverGround_ << "\n";
        ss << "\tDate: " << date_ << "\n";
        ss << "\tMagnetic Variation: " << magneticVariation_ << " " << magneticVariationDirection_ << "\n";
        ss << "\tMode Indicator: " << modeIndicator_ << "\n";
        ss << "\tNavigation Status: " << navigationStatus_;
        ss << "\n";
    } else {
        ss << "UTC Fix=" << utcFix_
           << ", Status=" << status_
           << ", Lat=" << latitudeStr << latitudeDirection_
           << ", Lon=" << longitudeStr << longitudeDirection_
           << ", SOG=" << speedOverGround_
           << ", COG=" << courseOverGround_
           << ", Date=" << date_
           << ", MagVar=" << magneticVariation_ << magneticVariationDirection_
           << ", Mode=" << modeIndicator_
           << ", NavStatus=" << navigationStatus_;
    }
    return ss.str();
}

std::string RMC::composeRaw(const std::string& talkerId, unsigned int utcFix, 
                            char status, 
                            double latitude,
                            char latitudeDirection, 
                            double longitude,
                            char longitudeDirection,
                            double speedOverGround, 
                            double courseOverGround, 
                            unsigned int date, 
                            double magneticVariation,
                            char magneticVariationDirection,
                            char modeIndicator,
                            char navigationStatus) {
    std::ostringstream payloadStream;
    payloadStream << talkerId << "RMC,";
    payloadStream << std::setw(6) << std::setfill('0') << utcFix << ",";
    payloadStream << status << ",";
    double latitudeDegrees = std::floor(latitude);
    double latitudeMinutes = (latitude - latitudeDegrees) * 60.0;
    latitude = latitudeDegrees * 100.0 + latitudeMinutes; // Convert from decimal degrees to ddmm.mmmm
    double longitudeDegrees = std::floor(longitude);
    double longitudeMinutes = (longitude - longitudeDegrees) * 60.0;
    longitude = longitudeDegrees * 100.0 + longitudeMinutes; // Convert from decimal degrees to dddmm.mmmm
    payloadStream << std::fixed << std::setprecision(4) << latitude << ",";
    payloadStream << latitudeDirection << ",";
    payloadStream << std::fixed << std::setprecision(4) << longitude << ",";
    payloadStream << longitudeDirection << ",";
    payloadStream << std::fixed << std::setprecision(1) << speedOverGround << ",";
    payloadStream << std::fixed << std::setprecision(1) << courseOverGround << ",";
    payloadStream << std::setw(6) << std::setfill('0') << date << ",";
    payloadStream << std::fixed << std::setprecision(1) << magneticVariation << ",";
    payloadStream << magneticVariationDirection << ",";
    payloadStream << modeIndicator << ",";
    payloadStream << navigationStatus;

    std::string payload = payloadStream.str();
    return "$" + payload + "\r\n";
}

unsigned int RMC::getUtcFix() const noexcept {
    return utcFix_;
}

char RMC::getStatus() const noexcept {
    return status_;
}

double RMC::getLatitude() const noexcept {
    return latitude_;
}

char RMC::getLatitudeDirection() const noexcept {
    return latitudeDirection_;
}

double RMC::getLongitude() const noexcept {
    return longitude_;
}

char RMC::getLongitudeDirection() const noexcept {
    return longitudeDirection_;
}

double RMC::getSpeedOverGround() const noexcept {
    return speedOverGround_;
}

double RMC::getCourseOverGround() const noexcept {
    return courseOverGround_;
}

unsigned int RMC::getDate() const noexcept {
    return date_;
}

double RMC::getMagneticVariation() const noexcept {
    return magneticVariation_;
}

char RMC::getMagneticVariationDirection() const noexcept {
    return magneticVariationDirection_;
}

char RMC::getModeIndicator() const noexcept {
    return modeIndicator_;
}

char RMC::getNavigationStatus() const noexcept {
    return navigationStatus_;
}

bool RMC::operator==(const RMC& other) const noexcept {
    return Message0183::operator==(other);
}

} // namespace nmea0183
} // namespace nmealib