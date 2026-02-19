#include "rmc.hpp"
#include <cmath>

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
        double latitude =  fields[2].empty() ? 0.0 : std::stod(fields[2]);
        latitude = std::floor(latitude / 100) + std::fmod(latitude, 100) / 60; // Convert from ddmm.mmmm to decimal degrees
        char latDirection = fields[3].empty() ? '\0' : fields[3][0];
        double longitude = fields[4].empty() ? 0.0 : std::stod(fields[4]);
        longitude = std::floor(longitude / 100) + std::fmod(longitude, 100) / 60; // Convert from dddmm.mmmm to decimal degrees
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

std::unique_ptr<nmealib::Message> RMC::clone() const {
    return std::unique_ptr<RMC>(new RMC(*this));
}

std::string RMC::getStringContent(bool verbose) const noexcept {
    std::ostringstream ss;
    if (verbose) {
        ss << "RMC Message:\n"
           << "  UTC Fix: " << utcFix_ << "\n"
           << "  Status: " << status_ << "\n"
           << "  Latitude: " << latitude_ << "\n"
           << "  Longitude: " << longitude_ << "\n"
           << "  Speed Over Ground: " << speedOverGround_ << "\n"
           << "  Course Over Ground: " << courseOverGround_ << "\n"
           << "  Date: " << date_ << "\n"
           << "  Magnetic Variation: " << magneticVariation_;
    } else {
        ss << "RMC(UTC Fix=" << utcFix_
           << ", Status=" << status_
           << ", Lat=" << latitude_
           << ", Lon=" << longitude_
           << ", SOG=" << speedOverGround_
           << ", COG=" << courseOverGround_
           << ", Date=" << date_
           << ", MagVar=" << magneticVariation_ 
           << ")";
    }
    return ss.str();
}

} // namespace nmea0183
} // namespace nmealib