#pragma once

#include <nmealib/nmea0183.h>

namespace nmealib {
namespace nmea0183 {

/**
 * @brief Exception thrown when a sentence is not a valid APB sentence.
 *
 * This exception is raised during parsing when the sentence type is not "APB"
 * or the payload does not conform to the expected APB format.
 */
class NotAPBException : public NmeaException {
public:
    /**
     * @brief Construct a NotAPBException with context and optional details.
     *
     * @param context  A short string identifying where the exception was thrown.
     * @param details  Optional additional information about the failure.
     */
    explicit NotAPBException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "The sentence is not an APB sentence", details) {}
};

/**
 * @brief Represents a parsed NMEA 0183 APB (Autopilot Sentence "B") sentence.
 *
 * APB contains cross-track error, steering direction, waypoint arrival flags,
 * and bearings/headings used by autopilot systems.
 *
 * Sentence format:
 * @code
 * $--APB,A,A,x.x,a,N,A,A,x.x,a,c--c,x.x,a,x.x,a*hh<CR><LF>
 * @endcode
 */
class APB : public Message0183 {
public:
    /**
     * @brief Construct an APB message from individual field values.
     *
     * @param talkerId                               Two-character talker identifier.
     * @param status1                                Data validity status flag.
     * @param status2                                Cycle lock / secondary status flag.
     * @param crossTrackErrorMagnitude               Cross-track error magnitude in nautical miles.
     * @param directionToSteer                       Steering direction indicator ('L' or 'R').
     * @param crossTrackUnits                        Cross-track units indicator (typically 'N').
     * @param arrivalCircleStatus                    Arrival circle status flag.
     * @param perpendicularPassedAtWaypoint          Perpendicular-passed status flag.
     * @param bearingOriginToDestination             Bearing from origin to destination.
     * @param bearingOriginToDestinationType         Bearing type for origin-to-destination ('M' or 'T').
     * @param destinationWaypointId                  Destination waypoint identifier.
     * @param bearingPresentToDestination            Bearing from present position to destination.
     * @param bearingPresentToDestinationType        Bearing type for present-to-destination ('M' or 'T').
     * @param headingToSteerToDestinationWaypoint    Heading to steer toward destination waypoint.
     * @param headingToSteerToDestinationWaypointType Heading type for heading-to-steer ('M' or 'T').
     */
    APB(std::string talkerId,
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
        char headingToSteerToDestinationWaypointType
    );

    APB(const APB&) = default;
    APB& operator=(const APB&) = default;
    APB(APB&&) noexcept = default;
    APB& operator=(APB&&) noexcept = default;

    ~APB() override = default;

    /**
     * @brief Create a polymorphic copy of this APB message.
     *
     * @return std::unique_ptr<nmealib::Message> A heap-allocated copy of this object.
     */
    std::unique_ptr<nmealib::Message> clone() const override;

    /** @brief Get primary APB status flag. */
    char getStatus1() const noexcept;
    /** @brief Get secondary APB status flag. */
    char getStatus2() const noexcept;
    /** @brief Get cross-track error magnitude in nautical miles. */
    double getCrossTrackErrorMagnitude() const noexcept;
    /** @brief Get steering direction indicator ('L' or 'R'). */
    char getDirectionToSteer() const noexcept;
    /** @brief Get cross-track units indicator. */
    char getCrossTrackUnits() const noexcept;
    /** @brief Get arrival circle status flag. */
    char getArrivalCircleStatus() const noexcept;
    /** @brief Get perpendicular-passed status flag. */
    char getPerpendicularPassedAtWaypoint() const noexcept;
    /** @brief Get bearing from origin to destination. */
    double getBearingOriginToDestination() const noexcept;
    /** @brief Get bearing type for origin-to-destination ('M' or 'T'). */
    char getBearingOriginToDestinationType() const noexcept;
    /** @brief Get destination waypoint identifier. */
    std::string getDestinationWaypointId() const noexcept;
    /** @brief Get bearing from present position to destination. */
    double getBearingPresentToDestination() const noexcept;
    /** @brief Get bearing type for present-to-destination ('M' or 'T'). */
    char getBearingPresentToDestinationType() const noexcept;
    /** @brief Get heading to steer to destination waypoint. */
    double getHeadingToSteerToDestinationWaypoint() const noexcept;
    /** @brief Get heading type for heading-to-steer ('M' or 'T'). */
    char getHeadingToSteerToDestinationWaypointType() const noexcept;

    /**
     * @brief Return a human-readable string representation of this message.
     *
     * @param verbose When true, returns a multi-line detailed representation.
     * @return std::string Formatted string content.
     */
    std::string getStringContent(bool verbose) const noexcept override;

    /**
     * @brief Compare two APB messages for equality.
     *
     * Equality delegates to Message0183::operator==.
     *
     * @param other The APB message to compare with.
     * @return bool true if equal, false otherwise.
     */
    bool operator==(const APB& other) const noexcept;

private:
    char status1_{};
    char status2_{};
    double crossTrackErrorMagnitude_{};
    char directionToSteer_{};
    char crossTrackUnits_{};
    char arrivalCircleStatus_{};
    char perpendicularPassedAtWaypoint_{};
    double bearingOriginToDestination_{};
    char bearingOriginToDestinationType_{};
    std::string destinationWaypointId_{};
    double bearingPresentToDestination_{};
    char bearingPresentToDestinationType_{};
    double headingToSteerToDestinationWaypoint_{};
    char headingToSteerToDestinationWaypointType_{};

    APB() = delete;

    APB(Message0183 baseMessage,
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
        char headingToSteerToDestinationWaypointType
    ) noexcept;

    static std::unique_ptr<APB> create(std::unique_ptr<Message0183> baseMessage);
    static std::string composeRaw(const std::string& talkerId,
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
                                  char headingToSteerToDestinationWaypointType);

    friend class Nmea0183Factory;
    friend class MessageRegistry;
};

} // namespace nmea0183
} // namespace nmealib
