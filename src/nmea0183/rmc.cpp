#include "nmealib/nmea0183/rmc.hpp"
#include <cmath>

#include "nmealib/nmea0183/nmea0183Factory.hpp"

namespace nmealib {
namespace nmea0183 {

std::unique_ptr<RMC> RMC::create(std::unique_ptr<Message0183> baseMessage) {
    std::string context = "RMC::create";
    if (baseMessage->getSentenceType() != "RMC") {
        throw NotRMCException(context, "Expected sentence type 'RMC', got " + baseMessage->getSentenceType());
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

    if (fields.size() != 13) {
        throw NotRMCException(context, "Insufficient fields in RMC payload: expected 13, got " + std::to_string(fields.size()) + ". Payload: " + payload);
    }

    try {
        unsigned int utcFix = fields[0].empty() ? 0 : std::stoul(fields[0]);
        char status = fields[1].empty() ? '\0' : fields[1][0];
        double latitude = Message0183::convertNmeaCoordinateToDecimalDegrees(fields[2]);
        char latDirection = fields[3].empty() ? '\0' : fields[3][0];
        double longitude = Message0183::convertNmeaCoordinateToDecimalDegrees(fields[4]);
        char lonDirection = fields[5].empty() ? '\0' : fields[5][0];
        double speedOverGround = fields[6].empty() ? 0.0 : std::stod(fields[6]);
        double courseOverGround = fields[7].empty() ? 0.0 : std::stod(fields[7]);
        unsigned int date = fields[8].empty() ? 0 : std::stoul(fields[8]);
        double magneticVariation = fields[9].empty() ? 0.0 : std::stod(fields[9]);
        char magneticVariationDirection = fields[10].empty() ? '\0' : fields[10][0];
        char modeIndicator = fields[11].empty() ? '\0' : fields[11][0];
        char navigationStatus = fields[12].empty() ? '\0' : fields[12][0];

        return std::unique_ptr<RMC>(new RMC(std::move(*baseMessage), utcFix, status, latitude, latDirection, longitude, lonDirection, speedOverGround, courseOverGround, date, magneticVariation, magneticVariationDirection, modeIndicator, navigationStatus));
    } catch (const std::exception& e) {
        throw NmeaException(context, "Error parsing RMC fields: " + std::string(e.what()) + ", Fields: " + fields[0] + ", " + fields[1] + ", " + fields[2] + ", " + fields[3] + ", " + fields[4] + ", " + fields[5] + ", " + fields[6] + ", " + fields[7]);
    }
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
        ss << "\tUTC Fix: " << utcFix_ << "\n";
        ss << "\tStatus: " << status_ << "\n";
        ss << "\tLatitude: " << latitudeStr << "\n";
        ss << "\tLatitude Direction: " << latitudeDirection_ << "\n";
        ss << "\tLongitude: " << longitudeStr << "\n";
        ss << "\tLongitude Direction: " << longitudeDirection_ << "\n";
        ss << "\tSpeed Over Ground: " << speedOverGround_ << "\n";
        ss << "\tCourse Over Ground: " << courseOverGround_ << "\n";
        ss << "\tDate: " << date_ << "\n";
        ss << "\tMagnetic Variation: " << magneticVariation_ << "\n";
        ss << "\tMagnetic Variation Direction: " << magneticVariationDirection_ << "\n";
        ss << "\tMode Indicator: " << modeIndicator_ << "\n";
        ss << "\tNavigation Status: " << navigationStatus_;

    } else {
        ss << "[" << validity << "] " << typeToString(type_) << " " << getTalker() << " " << getSentenceType() << ": "
           << "UTC Fix=" << utcFix_
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

std::string RMC::composeRaw(std::string talkerId, unsigned int utcFix, 
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

} // namespace nmea0183
} // namespace nmealib