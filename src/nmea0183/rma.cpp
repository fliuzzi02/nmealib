#include "nmealib/nmea0183/rma.h"

#include "nmealib/detail/errorSupport.h"
#include "nmealib/detail/parse.h"

#include <cmath>
#include <iomanip>
#include <sstream>
#include <vector>

namespace nmealib {
namespace nmea0183 {

std::unique_ptr<RMA> RMA::create(std::unique_ptr<Message0183> baseMessage) {
    std::string context = "RMA::create";
    if (baseMessage->getSentenceType() != "RMA") {
        NMEALIB_RETURN_ERROR(NotRMAException(context, "Expected sentence type 'RMA', got " + baseMessage->getSentenceType()));
    }

    std::string payload = baseMessage->getPayload();
    std::istringstream ss(payload);
    std::string token;
    std::vector<std::string> fields;

    while (std::getline(ss, token, ',')) {
        fields.push_back(token);
    }

    if (!fields.empty()) {
        fields.erase(fields.begin());
    }

    if (fields.size() != 11) {
        NMEALIB_RETURN_ERROR(NotRMAException(context, "Invalid fields in RMA payload: expected 11, got " + std::to_string(fields.size()) + ". Payload: " + payload));
    }

    double latitude = 0.0;
    double longitude = 0.0;
    double timeDifferenceA = 0.0;
    double timeDifferenceB = 0.0;
    double speedOverGround = 0.0;
    double trackMadeGood = 0.0;
    double magneticVariation = 0.0;
    if (!detail::parseNmeaCoordinate(fields[1], latitude) ||
        !detail::parseNmeaCoordinate(fields[3], longitude) ||
        !detail::parseOptionalDouble(fields[5], timeDifferenceA) ||
        !detail::parseOptionalDouble(fields[6], timeDifferenceB) ||
        !detail::parseOptionalDouble(fields[7], speedOverGround) ||
        !detail::parseOptionalDouble(fields[8], trackMadeGood) ||
        !detail::parseOptionalDouble(fields[9], magneticVariation)) {
        NMEALIB_RETURN_ERROR(NmeaException(context, "Error parsing RMA fields"));
    }

    const char status = fields[0].empty() ? '\0' : fields[0][0];
    const char latitudeDirection = fields[2].empty() ? '\0' : fields[2][0];
    const char longitudeDirection = fields[4].empty() ? '\0' : fields[4][0];
    const char variationDirection = fields[10].empty() ? '\0' : fields[10][0];

    return std::unique_ptr<RMA>(new RMA(std::move(*baseMessage),
                                        status,
                                        latitude,
                                        latitudeDirection,
                                        longitude,
                                        longitudeDirection,
                                        timeDifferenceA,
                                        timeDifferenceB,
                                        speedOverGround,
                                        trackMadeGood,
                                        magneticVariation,
                                        variationDirection));
}

RMA::RMA(Message0183 baseMessage,
         char status,
         double latitude,
         char latitudeDirection,
         double longitude,
         char longitudeDirection,
         double timeDifferenceA,
         double timeDifferenceB,
         double speedOverGround,
         double trackMadeGood,
         double magneticVariation,
         char variationDirection) noexcept
    : Message0183(std::move(baseMessage)),
      status_(status),
      latitude_(latitude),
      latitudeDirection_(latitudeDirection),
      longitude_(longitude),
      longitudeDirection_(longitudeDirection),
      timeDifferenceA_(timeDifferenceA),
      timeDifferenceB_(timeDifferenceB),
      speedOverGround_(speedOverGround),
      trackMadeGood_(trackMadeGood),
      magneticVariation_(magneticVariation),
      variationDirection_(variationDirection) {}

RMA::RMA(std::string talkerId,
         char status,
         double latitude,
         char latitudeDirection,
         double longitude,
         char longitudeDirection,
         double timeDifferenceA,
         double timeDifferenceB,
         double speedOverGround,
         double trackMadeGood,
         double magneticVariation,
         char variationDirection)
    : Message0183(*Message0183::create(composeRaw(talkerId,
                                                  status,
                                                  latitude,
                                                  latitudeDirection,
                                                  longitude,
                                                  longitudeDirection,
                                                  timeDifferenceA,
                                                  timeDifferenceB,
                                                  speedOverGround,
                                                  trackMadeGood,
                                                  magneticVariation,
                                                  variationDirection))),
      status_(status),
      latitude_(latitude),
      latitudeDirection_(latitudeDirection),
      longitude_(longitude),
      longitudeDirection_(longitudeDirection),
      timeDifferenceA_(timeDifferenceA),
      timeDifferenceB_(timeDifferenceB),
      speedOverGround_(speedOverGround),
      trackMadeGood_(trackMadeGood),
      magneticVariation_(magneticVariation),
      variationDirection_(variationDirection) {}

std::unique_ptr<nmealib::Message> RMA::clone() const {
    return std::unique_ptr<RMA>(new RMA(*this));
}

std::string RMA::getStringContent(bool verbose) const noexcept {
    std::ostringstream ss;
    ss << this->toString(verbose);
    if (verbose) {
        ss << "\tStatus: " << status_ << "\n";
        ss << "\tLatitude: " << latitude_ << " " << latitudeDirection_ << "\n";
        ss << "\tLongitude: " << longitude_ << " " << longitudeDirection_ << "\n";
        ss << "\tTime Difference A: " << timeDifferenceA_ << "\n";
        ss << "\tTime Difference B: " << timeDifferenceB_ << "\n";
        ss << "\tSpeed Over Ground: " << speedOverGround_ << "\n";
        ss << "\tTrack Made Good: " << trackMadeGood_ << "\n";
        ss << "\tMagnetic Variation: " << magneticVariation_ << " " << variationDirection_ << "\n";
        ss << "\n";
    } else {
        ss << "Status=" << status_
           << ", Lat=" << latitude_ << latitudeDirection_
           << ", Lon=" << longitude_ << longitudeDirection_
           << ", TDA=" << timeDifferenceA_
           << ", TDB=" << timeDifferenceB_
           << ", SOG=" << speedOverGround_
           << ", Track=" << trackMadeGood_
           << ", MagVar=" << magneticVariation_ << variationDirection_;
    }
    return ss.str();
}

std::string RMA::composeRaw(const std::string& talkerId,
                            char status,
                            double latitude,
                            char latitudeDirection,
                            double longitude,
                            char longitudeDirection,
                            double timeDifferenceA,
                            double timeDifferenceB,
                            double speedOverGround,
                            double trackMadeGood,
                            double magneticVariation,
                            char variationDirection) {
    std::ostringstream payloadStream;
    payloadStream << talkerId << "RMA,";
    payloadStream << status << ",";

    const double latitudeDegrees = std::floor(latitude);
    const double latitudeMinutes = (latitude - latitudeDegrees) * 60.0;
    const double latitudeNmea = latitudeDegrees * 100.0 + latitudeMinutes;

    const double longitudeDegrees = std::floor(longitude);
    const double longitudeMinutes = (longitude - longitudeDegrees) * 60.0;
    const double longitudeNmea = longitudeDegrees * 100.0 + longitudeMinutes;

    payloadStream << std::fixed << std::setprecision(4) << latitudeNmea << ",";
    payloadStream << latitudeDirection << ",";
    payloadStream << std::fixed << std::setprecision(4) << longitudeNmea << ",";
    payloadStream << longitudeDirection << ",";
    payloadStream << std::fixed << std::setprecision(1) << timeDifferenceA << ",";
    payloadStream << std::fixed << std::setprecision(1) << timeDifferenceB << ",";
    payloadStream << std::fixed << std::setprecision(1) << speedOverGround << ",";
    payloadStream << std::fixed << std::setprecision(1) << trackMadeGood << ",";
    payloadStream << std::fixed << std::setprecision(1) << magneticVariation << ",";
    payloadStream << variationDirection;

    return "$" + payloadStream.str() + "\r\n";
}

char RMA::getStatus() const noexcept { return status_; }
double RMA::getLatitude() const noexcept { return latitude_; }
char RMA::getLatitudeDirection() const noexcept { return latitudeDirection_; }
double RMA::getLongitude() const noexcept { return longitude_; }
char RMA::getLongitudeDirection() const noexcept { return longitudeDirection_; }
double RMA::getTimeDifferenceA() const noexcept { return timeDifferenceA_; }
double RMA::getTimeDifferenceB() const noexcept { return timeDifferenceB_; }
double RMA::getSpeedOverGround() const noexcept { return speedOverGround_; }
double RMA::getTrackMadeGood() const noexcept { return trackMadeGood_; }
double RMA::getMagneticVariation() const noexcept { return magneticVariation_; }
char RMA::getVariationDirection() const noexcept { return variationDirection_; }

bool RMA::operator==(const RMA& other) const noexcept {
    return Message0183::operator==(other) &&
           status_ == other.status_ &&
           latitude_ == other.latitude_ &&
           latitudeDirection_ == other.latitudeDirection_ &&
           longitude_ == other.longitude_ &&
           longitudeDirection_ == other.longitudeDirection_ &&
           timeDifferenceA_ == other.timeDifferenceA_ &&
           timeDifferenceB_ == other.timeDifferenceB_ &&
           speedOverGround_ == other.speedOverGround_ &&
           trackMadeGood_ == other.trackMadeGood_ &&
           magneticVariation_ == other.magneticVariation_ &&
           variationDirection_ == other.variationDirection_;
}

} // namespace nmea0183
} // namespace nmealib
