#include "nmealib/nmea0183/apb.h"

#include "nmealib/detail/errorSupport.h"
#include "nmealib/detail/parse.h"

#include <iomanip>
#include <sstream>
#include <vector>

namespace nmealib {
namespace nmea0183 {

std::unique_ptr<APB> APB::create(std::unique_ptr<Message0183> baseMessage) {
    std::string context = "APB::create";
    if (baseMessage->getSentenceType() != "APB") {
        NMEALIB_RETURN_ERROR(NotAPBException(context, "Expected sentence type 'APB', got " + baseMessage->getSentenceType()));
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

    if (fields.size() != 14) {
        NMEALIB_RETURN_ERROR(NotAPBException(context, "Invalid fields in APB payload: expected 14, got " + std::to_string(fields.size()) + ". Payload: " + payload));
    }

    double crossTrackErrorMagnitude = 0.0;
    double bearingOriginToDestination = 0.0;
    double bearingPresentToDestination = 0.0;
    double headingToSteerToDestinationWaypoint = 0.0;
    if (!detail::parseOptionalDouble(fields[2], crossTrackErrorMagnitude) ||
        !detail::parseOptionalDouble(fields[7], bearingOriginToDestination) ||
        !detail::parseOptionalDouble(fields[10], bearingPresentToDestination) ||
        !detail::parseOptionalDouble(fields[12], headingToSteerToDestinationWaypoint)) {
        NMEALIB_RETURN_ERROR(NmeaException(context, "Error parsing APB fields"));
    }

    char status1 = fields[0].empty() ? '\0' : fields[0][0];
    char status2 = fields[1].empty() ? '\0' : fields[1][0];
    char directionToSteer = fields[3].empty() ? '\0' : fields[3][0];
    char crossTrackUnits = fields[4].empty() ? '\0' : fields[4][0];
    char arrivalCircleStatus = fields[5].empty() ? '\0' : fields[5][0];
    char perpendicularPassedAtWaypoint = fields[6].empty() ? '\0' : fields[6][0];
    char bearingOriginToDestinationType = fields[8].empty() ? '\0' : fields[8][0];
    std::string destinationWaypointId = fields[9];
    char bearingPresentToDestinationType = fields[11].empty() ? '\0' : fields[11][0];
    char headingToSteerToDestinationWaypointType = fields[13].empty() ? '\0' : fields[13][0];

    return std::unique_ptr<APB>(new APB(std::move(*baseMessage),
                                        status1,
                                        status2,
                                        crossTrackErrorMagnitude,
                                        directionToSteer,
                                        crossTrackUnits,
                                        arrivalCircleStatus,
                                        perpendicularPassedAtWaypoint,
                                        bearingOriginToDestination,
                                        bearingOriginToDestinationType,
                                        destinationWaypointId,
                                        bearingPresentToDestination,
                                        bearingPresentToDestinationType,
                                        headingToSteerToDestinationWaypoint,
                                        headingToSteerToDestinationWaypointType));
}

APB::APB(Message0183 baseMessage,
         char status1,
         char status2,
         double crossTrackErrorMagnitude,
         char directionToSteer,
         char crossTrackUnits,
         char arrivalCircleStatus,
         char perpendicularPassedAtWaypoint,
         double bearingOriginToDestination,
         char bearingOriginToDestinationType,
         std::string destinationWaypointId,
         double bearingPresentToDestination,
         char bearingPresentToDestinationType,
         double headingToSteerToDestinationWaypoint,
         char headingToSteerToDestinationWaypointType) noexcept
    : Message0183(std::move(baseMessage)),
      status1_(status1),
      status2_(status2),
      crossTrackErrorMagnitude_(crossTrackErrorMagnitude),
      directionToSteer_(directionToSteer),
      crossTrackUnits_(crossTrackUnits),
      arrivalCircleStatus_(arrivalCircleStatus),
      perpendicularPassedAtWaypoint_(perpendicularPassedAtWaypoint),
      bearingOriginToDestination_(bearingOriginToDestination),
      bearingOriginToDestinationType_(bearingOriginToDestinationType),
      destinationWaypointId_(std::move(destinationWaypointId)),
      bearingPresentToDestination_(bearingPresentToDestination),
      bearingPresentToDestinationType_(bearingPresentToDestinationType),
      headingToSteerToDestinationWaypoint_(headingToSteerToDestinationWaypoint),
      headingToSteerToDestinationWaypointType_(headingToSteerToDestinationWaypointType) {}

APB::APB(std::string talkerId,
         char status1,
         char status2,
         double crossTrackErrorMagnitude,
         char directionToSteer,
         char crossTrackUnits,
         char arrivalCircleStatus,
         char perpendicularPassedAtWaypoint,
         double bearingOriginToDestination,
         char bearingOriginToDestinationType,
         std::string destinationWaypointId,
         double bearingPresentToDestination,
         char bearingPresentToDestinationType,
         double headingToSteerToDestinationWaypoint,
         char headingToSteerToDestinationWaypointType)
    : Message0183(*Message0183::create(composeRaw(talkerId,
                                                  status1,
                                                  status2,
                                                  crossTrackErrorMagnitude,
                                                  directionToSteer,
                                                  crossTrackUnits,
                                                  arrivalCircleStatus,
                                                  perpendicularPassedAtWaypoint,
                                                  bearingOriginToDestination,
                                                  bearingOriginToDestinationType,
                                                  destinationWaypointId,
                                                  bearingPresentToDestination,
                                                  bearingPresentToDestinationType,
                                                  headingToSteerToDestinationWaypoint,
                                                  headingToSteerToDestinationWaypointType))),
      status1_(status1),
      status2_(status2),
      crossTrackErrorMagnitude_(crossTrackErrorMagnitude),
      directionToSteer_(directionToSteer),
      crossTrackUnits_(crossTrackUnits),
      arrivalCircleStatus_(arrivalCircleStatus),
      perpendicularPassedAtWaypoint_(perpendicularPassedAtWaypoint),
      bearingOriginToDestination_(bearingOriginToDestination),
      bearingOriginToDestinationType_(bearingOriginToDestinationType),
      destinationWaypointId_(std::move(destinationWaypointId)),
      bearingPresentToDestination_(bearingPresentToDestination),
      bearingPresentToDestinationType_(bearingPresentToDestinationType),
      headingToSteerToDestinationWaypoint_(headingToSteerToDestinationWaypoint),
      headingToSteerToDestinationWaypointType_(headingToSteerToDestinationWaypointType) {}

std::unique_ptr<nmealib::Message> APB::clone() const {
    return std::unique_ptr<APB>(new APB(*this));
}

std::string APB::getStringContent(bool verbose) const noexcept {
    std::ostringstream ss;
    ss << this->toString(verbose);
    ss << std::fixed << std::setprecision(2);

    if (verbose) {
        ss << "\tStatus 1: " << status1_ << "\n";
        ss << "\tStatus 2: " << status2_ << "\n";
        ss << "\tCross Track Error Magnitude: " << crossTrackErrorMagnitude_ << "\n";
        ss << "\tDirection to Steer: " << directionToSteer_ << "\n";
        ss << "\tCross Track Units: " << crossTrackUnits_ << "\n";
        ss << "\tArrival Circle Status: " << arrivalCircleStatus_ << "\n";
        ss << "\tPerpendicular Passed at Waypoint: " << perpendicularPassedAtWaypoint_ << "\n";
        ss << "\tBearing Origin->Destination: " << bearingOriginToDestination_ << " " << bearingOriginToDestinationType_ << "\n";
        ss << "\tDestination Waypoint ID: " << destinationWaypointId_ << "\n";
        ss << "\tBearing Present->Destination: " << bearingPresentToDestination_ << " " << bearingPresentToDestinationType_ << "\n";
        ss << "\tHeading to Steer: " << headingToSteerToDestinationWaypoint_ << " " << headingToSteerToDestinationWaypointType_ << "\n";
    } else {
        ss << "Status1=" << status1_
           << ", Status2=" << status2_
           << ", XTE=" << crossTrackErrorMagnitude_ << crossTrackUnits_
           << ", Steer=" << directionToSteer_
           << ", Arrival=" << arrivalCircleStatus_
           << ", Perpendicular=" << perpendicularPassedAtWaypoint_
           << ", BOD=" << bearingOriginToDestination_ << bearingOriginToDestinationType_
           << ", Dest=" << destinationWaypointId_
           << ", BPD=" << bearingPresentToDestination_ << bearingPresentToDestinationType_
           << ", Heading=" << headingToSteerToDestinationWaypoint_ << headingToSteerToDestinationWaypointType_;
    }

    return ss.str();
}

std::string APB::composeRaw(const std::string& talkerId,
                            char status1,
                            char status2,
                            double crossTrackErrorMagnitude,
                            char directionToSteer,
                            char crossTrackUnits,
                            char arrivalCircleStatus,
                            char perpendicularPassedAtWaypoint,
                            double bearingOriginToDestination,
                            char bearingOriginToDestinationType,
                            const std::string& destinationWaypointId,
                            double bearingPresentToDestination,
                            char bearingPresentToDestinationType,
                            double headingToSteerToDestinationWaypoint,
                            char headingToSteerToDestinationWaypointType) {
    std::ostringstream payloadStream;
    payloadStream << talkerId << "APB,";
    payloadStream << status1 << ",";
    payloadStream << status2 << ",";
    payloadStream << std::fixed << std::setprecision(2) << crossTrackErrorMagnitude << ",";
    payloadStream << directionToSteer << ",";
    payloadStream << crossTrackUnits << ",";
    payloadStream << arrivalCircleStatus << ",";
    payloadStream << perpendicularPassedAtWaypoint << ",";
    payloadStream << std::fixed << std::setprecision(3) << bearingOriginToDestination << ",";
    payloadStream << bearingOriginToDestinationType << ",";
    payloadStream << destinationWaypointId << ",";
    payloadStream << std::fixed << std::setprecision(3) << bearingPresentToDestination << ",";
    payloadStream << bearingPresentToDestinationType << ",";
    payloadStream << std::fixed << std::setprecision(3) << headingToSteerToDestinationWaypoint << ",";
    payloadStream << headingToSteerToDestinationWaypointType;

    std::string payload = payloadStream.str();
    return "$" + payload + "\r\n";
}

char APB::getStatus1() const noexcept {
    return status1_;
}

char APB::getStatus2() const noexcept {
    return status2_;
}

double APB::getCrossTrackErrorMagnitude() const noexcept {
    return crossTrackErrorMagnitude_;
}

char APB::getDirectionToSteer() const noexcept {
    return directionToSteer_;
}

char APB::getCrossTrackUnits() const noexcept {
    return crossTrackUnits_;
}

char APB::getArrivalCircleStatus() const noexcept {
    return arrivalCircleStatus_;
}

char APB::getPerpendicularPassedAtWaypoint() const noexcept {
    return perpendicularPassedAtWaypoint_;
}

double APB::getBearingOriginToDestination() const noexcept {
    return bearingOriginToDestination_;
}

char APB::getBearingOriginToDestinationType() const noexcept {
    return bearingOriginToDestinationType_;
}

std::string APB::getDestinationWaypointId() const noexcept {
    return destinationWaypointId_;
}

double APB::getBearingPresentToDestination() const noexcept {
    return bearingPresentToDestination_;
}

char APB::getBearingPresentToDestinationType() const noexcept {
    return bearingPresentToDestinationType_;
}

double APB::getHeadingToSteerToDestinationWaypoint() const noexcept {
    return headingToSteerToDestinationWaypoint_;
}

char APB::getHeadingToSteerToDestinationWaypointType() const noexcept {
    return headingToSteerToDestinationWaypointType_;
}

bool APB::operator==(const APB& other) const noexcept {
    return Message0183::operator==(other);
}

} // namespace nmea0183
} // namespace nmealib
