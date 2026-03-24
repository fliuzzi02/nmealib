#include "nmealib/nmea0183/rmb.h"

#include "nmealib/detail/errorSupport.h"
#include "nmealib/detail/parse.h"

#include <cmath>
#include <iomanip>
#include <sstream>
#include <vector>

namespace nmealib {
namespace nmea0183 {

std::unique_ptr<RMB> RMB::create(std::unique_ptr<Message0183> baseMessage) {
    std::string context = "RMB::create";
    if (baseMessage->getSentenceType() != "RMB") {
        NMEALIB_RETURN_ERROR(NotRMBException(context, "Expected sentence type 'RMB', got " + baseMessage->getSentenceType()));
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

    if (fields.size() != 13 && fields.size() != 14) {
        NMEALIB_RETURN_ERROR(NotRMBException(context, "Invalid fields in RMB payload: expected 13 or 14, got " + std::to_string(fields.size()) + ". Payload: " + payload));
    }

    double crossTrackErrorNm = 0.0;
    double destinationLatitude = 0.0;
    double destinationLongitude = 0.0;
    double rangeToDestinationNm = 0.0;
    double bearingToDestinationTrue = 0.0;
    double destinationClosingVelocityKnots = 0.0;
    if (!detail::parseOptionalDouble(fields[1], crossTrackErrorNm) ||
        !detail::parseNmeaCoordinate(fields[5], destinationLatitude) ||
        !detail::parseNmeaCoordinate(fields[7], destinationLongitude) ||
        !detail::parseOptionalDouble(fields[9], rangeToDestinationNm) ||
        !detail::parseOptionalDouble(fields[10], bearingToDestinationTrue) ||
        !detail::parseOptionalDouble(fields[11], destinationClosingVelocityKnots)) {
        NMEALIB_RETURN_ERROR(NmeaException(context, "Error parsing RMB fields"));
    }

    char status = fields[0].empty() ? '\0' : fields[0][0];
    char directionToSteer = fields[2].empty() ? '\0' : fields[2][0];
    const std::string originWaypointId = fields[3];
    const std::string destinationWaypointId = fields[4];
    char destinationLatitudeHemisphere = fields[6].empty() ? '\0' : fields[6][0];
    char destinationLongitudeHemisphere = fields[8].empty() ? '\0' : fields[8][0];
    char arrivalStatus = fields[12].empty() ? '\0' : fields[12][0];
    std::optional<char> faaModeIndicator = std::nullopt;
    if (fields.size() == 14 && !fields[13].empty()) {
        faaModeIndicator = fields[13][0];
    }

    return std::unique_ptr<RMB>(new RMB(std::move(*baseMessage),
                                        status,
                                        crossTrackErrorNm,
                                        directionToSteer,
                                        originWaypointId,
                                        destinationWaypointId,
                                        destinationLatitude,
                                        destinationLatitudeHemisphere,
                                        destinationLongitude,
                                        destinationLongitudeHemisphere,
                                        rangeToDestinationNm,
                                        bearingToDestinationTrue,
                                        destinationClosingVelocityKnots,
                                        arrivalStatus,
                                        faaModeIndicator));
}

RMB::RMB(Message0183 baseMessage,
         char status,
         double crossTrackErrorNm,
         char directionToSteer,
         std::string originWaypointId,
         std::string destinationWaypointId,
         double destinationLatitude,
         char destinationLatitudeHemisphere,
         double destinationLongitude,
         char destinationLongitudeHemisphere,
         double rangeToDestinationNm,
         double bearingToDestinationTrue,
         double destinationClosingVelocityKnots,
         char arrivalStatus,
         std::optional<char> faaModeIndicator) noexcept
    : Message0183(std::move(baseMessage)),
      status_(status),
      crossTrackErrorNm_(crossTrackErrorNm),
      directionToSteer_(directionToSteer),
      originWaypointId_(std::move(originWaypointId)),
      destinationWaypointId_(std::move(destinationWaypointId)),
      destinationLatitude_(destinationLatitude),
      destinationLatitudeHemisphere_(destinationLatitudeHemisphere),
      destinationLongitude_(destinationLongitude),
      destinationLongitudeHemisphere_(destinationLongitudeHemisphere),
      rangeToDestinationNm_(rangeToDestinationNm),
      bearingToDestinationTrue_(bearingToDestinationTrue),
      destinationClosingVelocityKnots_(destinationClosingVelocityKnots),
      arrivalStatus_(arrivalStatus),
      faaModeIndicator_(faaModeIndicator) {}

RMB::RMB(std::string talkerId,
         char status,
         double crossTrackErrorNm,
         char directionToSteer,
         std::string originWaypointId,
         std::string destinationWaypointId,
         double destinationLatitude,
         char destinationLatitudeHemisphere,
         double destinationLongitude,
         char destinationLongitudeHemisphere,
         double rangeToDestinationNm,
         double bearingToDestinationTrue,
         double destinationClosingVelocityKnots,
         char arrivalStatus,
         std::optional<char> faaModeIndicator)
    : Message0183(*Message0183::create(composeRaw(talkerId,
                                                  status,
                                                  crossTrackErrorNm,
                                                  directionToSteer,
                                                  originWaypointId,
                                                  destinationWaypointId,
                                                  destinationLatitude,
                                                  destinationLatitudeHemisphere,
                                                  destinationLongitude,
                                                  destinationLongitudeHemisphere,
                                                  rangeToDestinationNm,
                                                  bearingToDestinationTrue,
                                                  destinationClosingVelocityKnots,
                                                  arrivalStatus,
                                                  faaModeIndicator))),
      status_(status),
      crossTrackErrorNm_(crossTrackErrorNm),
      directionToSteer_(directionToSteer),
      originWaypointId_(std::move(originWaypointId)),
      destinationWaypointId_(std::move(destinationWaypointId)),
      destinationLatitude_(destinationLatitude),
      destinationLatitudeHemisphere_(destinationLatitudeHemisphere),
      destinationLongitude_(destinationLongitude),
      destinationLongitudeHemisphere_(destinationLongitudeHemisphere),
      rangeToDestinationNm_(rangeToDestinationNm),
      bearingToDestinationTrue_(bearingToDestinationTrue),
      destinationClosingVelocityKnots_(destinationClosingVelocityKnots),
      arrivalStatus_(arrivalStatus),
      faaModeIndicator_(faaModeIndicator) {}

std::unique_ptr<nmealib::Message> RMB::clone() const {
    return std::unique_ptr<RMB>(new RMB(*this));
}

std::string RMB::getStringContent(bool verbose) const noexcept {
    std::ostringstream ss;
    ss << this->toString(verbose);
    if (verbose) {
        ss << "\tStatus: " << status_ << "\n";
        ss << "\tCross Track Error (nm): " << crossTrackErrorNm_ << "\n";
        ss << "\tDirection To Steer: " << directionToSteer_ << "\n";
        ss << "\tOrigin Waypoint ID: " << originWaypointId_ << "\n";
        ss << "\tDestination Waypoint ID: " << destinationWaypointId_ << "\n";
        ss << "\tDestination Latitude: " << destinationLatitude_ << " " << destinationLatitudeHemisphere_ << "\n";
        ss << "\tDestination Longitude: " << destinationLongitude_ << " " << destinationLongitudeHemisphere_ << "\n";
        ss << "\tRange To Destination (nm): " << rangeToDestinationNm_ << "\n";
        ss << "\tBearing To Destination (true): " << bearingToDestinationTrue_ << "\n";
        ss << "\tDestination Closing Velocity (knots): " << destinationClosingVelocityKnots_ << "\n";
        ss << "\tArrival Status: " << arrivalStatus_;
        if (faaModeIndicator_.has_value()) {
            ss << "\n"
               << "\tFAA Mode Indicator: " << faaModeIndicator_.value();
        }
        ss << "\n";
    } else {
        ss << "Status=" << status_
           << ", XTE=" << crossTrackErrorNm_
           << directionToSteer_
           << ", Origin=" << originWaypointId_
           << ", Dest=" << destinationWaypointId_
           << ", DestLat=" << destinationLatitude_ << destinationLatitudeHemisphere_
           << ", DestLon=" << destinationLongitude_ << destinationLongitudeHemisphere_
           << ", Range=" << rangeToDestinationNm_
           << ", BearingT=" << bearingToDestinationTrue_
           << ", ClosingVel=" << destinationClosingVelocityKnots_
           << ", Arrival=" << arrivalStatus_;
        if (faaModeIndicator_.has_value()) {
            ss << ", FAA=" << faaModeIndicator_.value();
        }
    }
    return ss.str();
}

std::string RMB::composeRaw(const std::string& talkerId,
                            char status,
                            double crossTrackErrorNm,
                            char directionToSteer,
                            const std::string& originWaypointId,
                            const std::string& destinationWaypointId,
                            double destinationLatitude,
                            char destinationLatitudeHemisphere,
                            double destinationLongitude,
                            char destinationLongitudeHemisphere,
                            double rangeToDestinationNm,
                            double bearingToDestinationTrue,
                            double destinationClosingVelocityKnots,
                            char arrivalStatus,
                            std::optional<char> faaModeIndicator) {
    std::ostringstream payloadStream;
    payloadStream << talkerId << "RMB,";
    payloadStream << status << ",";
    payloadStream << std::fixed << std::setprecision(2) << crossTrackErrorNm << ",";
    payloadStream << directionToSteer << ",";
    payloadStream << originWaypointId << ",";
    payloadStream << destinationWaypointId << ",";

    const double latitudeDegrees = std::floor(destinationLatitude);
    const double latitudeMinutes = (destinationLatitude - latitudeDegrees) * 60.0;
    const double latitudeNmea = latitudeDegrees * 100.0 + latitudeMinutes;

    const double longitudeDegrees = std::floor(destinationLongitude);
    const double longitudeMinutes = (destinationLongitude - longitudeDegrees) * 60.0;
    const double longitudeNmea = longitudeDegrees * 100.0 + longitudeMinutes;

    payloadStream << std::fixed << std::setprecision(4) << latitudeNmea << ",";
    payloadStream << destinationLatitudeHemisphere << ",";
    payloadStream << std::fixed << std::setprecision(4) << longitudeNmea << ",";
    payloadStream << destinationLongitudeHemisphere << ",";
    payloadStream << std::fixed << std::setprecision(1) << rangeToDestinationNm << ",";
    payloadStream << std::fixed << std::setprecision(1) << bearingToDestinationTrue << ",";
    payloadStream << std::fixed << std::setprecision(1) << destinationClosingVelocityKnots << ",";
    payloadStream << arrivalStatus;
    if (faaModeIndicator.has_value()) {
        payloadStream << "," << faaModeIndicator.value();
    }

    std::string payload = payloadStream.str();
    return "$" + payload + "\r\n";
}

char RMB::getStatus() const noexcept {
    return status_;
}

double RMB::getCrossTrackErrorNm() const noexcept {
    return crossTrackErrorNm_;
}

char RMB::getDirectionToSteer() const noexcept {
    return directionToSteer_;
}

std::string RMB::getOriginWaypointId() const noexcept {
    return originWaypointId_;
}

std::string RMB::getDestinationWaypointId() const noexcept {
    return destinationWaypointId_;
}

double RMB::getDestinationLatitude() const noexcept {
    return destinationLatitude_;
}

char RMB::getDestinationLatitudeHemisphere() const noexcept {
    return destinationLatitudeHemisphere_;
}

double RMB::getDestinationLongitude() const noexcept {
    return destinationLongitude_;
}

char RMB::getDestinationLongitudeHemisphere() const noexcept {
    return destinationLongitudeHemisphere_;
}

double RMB::getRangeToDestinationNm() const noexcept {
    return rangeToDestinationNm_;
}

double RMB::getBearingToDestinationTrue() const noexcept {
    return bearingToDestinationTrue_;
}

double RMB::getDestinationClosingVelocityKnots() const noexcept {
    return destinationClosingVelocityKnots_;
}

char RMB::getArrivalStatus() const noexcept {
    return arrivalStatus_;
}

bool RMB::hasFaaModeIndicator() const noexcept {
    return faaModeIndicator_.has_value();
}

std::optional<char> RMB::getFaaModeIndicator() const noexcept {
    return faaModeIndicator_;
}

bool RMB::operator==(const RMB& other) const noexcept {
    return Message0183::operator==(other);
}

} // namespace nmea0183
} // namespace nmealib
