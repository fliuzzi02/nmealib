#include "rmc.hpp"

namespace nmealib {
namespace nmea0183 {

std::unique_ptr<RMC> RMC::create(std::unique_ptr<Message0183> baseMessage) {
    std::string context = "RMC::create";
    if (baseMessage->getSentenceType() != "RMC") {
        throw NotRMCException(context, "Expected sentence type 'RMC', got '" + baseMessage->getSentenceType() + "'");
    }

    // Parse the payload to extract RMC-specific fields
    std::string payload = baseMessage->getPayload();
    std::istringstream ss(payload);
    std::string token;
    std::vector<std::string> fields;

    while (std::getline(ss, token, ',')) {
        fields.push_back(token);
    }

    if (fields.size() < 9) {
        throw NmeaException(context, "Insufficient fields in RMC payload: expected at least 9, got " + std::to_string(fields.size()));
    }

    // TODO: Convert fields appropriately, this is AI garbage. Make sure to adhere to test and +/- signs for long, lat and var
    try {
        unsigned int utcFix = std::stoul(fields[0]);
        char status = fields[1].empty() ? '\0' : fields[1][0];
        double latitude = std::stod(fields[2]);
        double longitude = std::stod(fields[3]);
        double speedOverGround = std::stod(fields[4]);
        double courseOverGround = std::stod(fields[5]);
        unsigned int date = std::stoul(fields[6]);
        double magneticVariation = std::stod(fields[7]);

        return std::unique_ptr<RMC>(new RMC(std::move(*baseMessage), utcFix, status, latitude, longitude, speedOverGround, courseOverGround, date, magneticVariation));
    } catch (const std::exception& e) {
        throw NmeaException(context, "Error parsing RMC fields: " + std::string(e.what()));
    }
    
}

RMC::RMC(Message0183 baseMessage, 
         unsigned int utcFix, 
         char status, 
         double latitude, 
         double longitude, 
         double speedOverGround, 
         double courseOverGround, 
         unsigned int date, 
         double magneticVariation) noexcept
    : Message0183(std::move(baseMessage)), 
      utcFix_(utcFix), 
      status_(status), 
      latitude_(latitude), 
      longitude_(longitude), 
      speedOverGround_(speedOverGround), 
      courseOverGround_(courseOverGround), 
      date_(date), 
      magneticVariation_(magneticVariation) {}

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