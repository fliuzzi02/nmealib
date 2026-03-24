#pragma once

#include <nmealib/nmea0183.h>

#include <optional>

namespace nmealib {
namespace nmea0183 {

/**
 * @brief Exception thrown when a sentence is not a valid RMB sentence.
 *
 * This exception is raised during parsing when the sentence type is not "RMB"
 * or the payload does not conform to one of the supported RMB layouts.
 */
class NotRMBException : public NmeaException {
public:
    /**
     * @brief Construct a NotRMBException with context and optional details.
     *
     * @param context  A short string identifying where the exception was thrown.
     * @param details  Optional additional information about the failure.
     */
    explicit NotRMBException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "The sentence is not an RMB sentence", details) {}
};

/**
 * @brief Represents a parsed NMEA 0183 RMB (Recommended Minimum Navigation Information) sentence.
 *
 * RMB is emitted when a destination waypoint is active and carries cross-track
 * error, waypoint IDs, destination coordinates, range/bearing, closing
 * velocity, arrival status, and (for NMEA 2.3+) an optional FAA mode indicator.
 *
 * Sentence formats:
 * @code
 * $--RMB,A,x.x,a,c--c,c--c,llll.ll,a,yyyyy.yy,a,x.x,x.x,x.x,A*hh<CR><LF>
 * $--RMB,A,x.x,a,c--c,c--c,llll.ll,a,yyyyy.yy,a,x.x,x.x,x.x,A,m*hh<CR><LF>
 * @endcode
 */
class RMB : public Message0183 {
public:
    /**
     * @brief Construct an RMB message from individual field values.
     *
     * Latitude and longitude are expected in decimal degrees and are serialized
     * to NMEA coordinate format in the wire sentence.
     *
     * @param talkerId                   Two-character talker identifier.
     * @param status                     Data status indicator ('A' active, 'V' invalid).
     * @param crossTrackErrorNm          Cross track error in nautical miles.
     * @param directionToSteer           Direction to steer ('L' or 'R').
     * @param originWaypointId           Origin waypoint identifier.
     * @param destinationWaypointId      Destination waypoint identifier.
     * @param destinationLatitude        Destination waypoint latitude in decimal degrees.
     * @param destinationLatitudeHemisphere  Latitude hemisphere indicator ('N' or 'S').
     * @param destinationLongitude       Destination waypoint longitude in decimal degrees.
     * @param destinationLongitudeHemisphere Longitude hemisphere indicator ('E' or 'W').
     * @param rangeToDestinationNm       Range to destination in nautical miles.
     * @param bearingToDestinationTrue   Bearing to destination in true degrees.
     * @param destinationClosingVelocityKnots Destination closing velocity in knots.
     * @param arrivalStatus              Arrival status ('A' arrived, 'V' not arrived).
     * @param faaModeIndicator           Optional FAA mode indicator (NMEA 2.3+).
     */
    RMB(std::string talkerId,
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
        std::optional<char> faaModeIndicator = std::nullopt
    );

    RMB(const RMB&) = default;
    RMB& operator=(const RMB&) = default;
    RMB(RMB&&) noexcept = default;
    RMB& operator=(RMB&&) noexcept = default;

    ~RMB() override = default;

    /**
     * @brief Create a polymorphic copy of this RMB message.
     *
     * @return std::unique_ptr<nmealib::Message> A heap-allocated copy of this object.
     */
    std::unique_ptr<nmealib::Message> clone() const override;

    /** @brief Get status indicator ('A' active, 'V' invalid). */
    char getStatus() const noexcept;
    /** @brief Get cross track error in nautical miles. */
    double getCrossTrackErrorNm() const noexcept;
    /** @brief Get direction to steer ('L' or 'R'). */
    char getDirectionToSteer() const noexcept;
    /** @brief Get origin waypoint identifier. */
    std::string getOriginWaypointId() const noexcept;
    /** @brief Get destination waypoint identifier. */
    std::string getDestinationWaypointId() const noexcept;
    /** @brief Get destination waypoint latitude in decimal degrees. */
    double getDestinationLatitude() const noexcept;
    /** @brief Get destination latitude hemisphere indicator ('N' or 'S'). */
    char getDestinationLatitudeHemisphere() const noexcept;
    /** @brief Get destination waypoint longitude in decimal degrees. */
    double getDestinationLongitude() const noexcept;
    /** @brief Get destination longitude hemisphere indicator ('E' or 'W'). */
    char getDestinationLongitudeHemisphere() const noexcept;
    /** @brief Get range to destination in nautical miles. */
    double getRangeToDestinationNm() const noexcept;
    /** @brief Get bearing to destination in true degrees. */
    double getBearingToDestinationTrue() const noexcept;
    /** @brief Get destination closing velocity in knots. */
    double getDestinationClosingVelocityKnots() const noexcept;
    /** @brief Get arrival status ('A' arrived, 'V' not arrived). */
    char getArrivalStatus() const noexcept;
    /** @brief Return whether FAA mode indicator is present. */
    bool hasFaaModeIndicator() const noexcept;
    /** @brief Get optional FAA mode indicator (NMEA 2.3+). */
    std::optional<char> getFaaModeIndicator() const noexcept;

    /**
     * @brief Return a human-readable string representation of this message.
     *
     * @param verbose When true, returns a multi-line detailed representation.
     * @return std::string Formatted string content.
     */
    std::string getStringContent(bool verbose) const noexcept override;

    /**
     * @brief Compare two RMB messages for equality.
     *
     * Equality delegates to Message0183::operator==.
     *
     * @param other The RMB message to compare with.
     * @return bool true if equal, false otherwise.
     */
    bool operator==(const RMB& other) const noexcept;

private:
    char status_{};
    double crossTrackErrorNm_{};
    char directionToSteer_{};
    std::string originWaypointId_{};
    std::string destinationWaypointId_{};
    double destinationLatitude_{};
    char destinationLatitudeHemisphere_{};
    double destinationLongitude_{};
    char destinationLongitudeHemisphere_{};
    double rangeToDestinationNm_{};
    double bearingToDestinationTrue_{};
    double destinationClosingVelocityKnots_{};
    char arrivalStatus_{};
    std::optional<char> faaModeIndicator_{};

    RMB() = delete;

    RMB(Message0183 baseMessage,
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
        std::optional<char> faaModeIndicator
    ) noexcept;

    static std::unique_ptr<RMB> create(std::unique_ptr<Message0183> baseMessage);
    static std::string composeRaw(const std::string& talkerId,
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
                                  std::optional<char> faaModeIndicator);

    friend class Nmea0183Factory;
    friend class MessageRegistry;
};

} // namespace nmea0183
} // namespace nmealib
