#pragma once

#include <nmealib/nmea0183.h>

namespace nmealib {
namespace nmea0183 {

/**
 * @brief Exception thrown when a sentence is not a valid GGA sentence.
 *
 * This exception is raised during parsing when the sentence type is not "GGA"
 * or the payload does not conform to the expected GGA format.
 */
class NotGGAException : public NmeaException {
public:
    /**
     * @brief Construct a NotGGAException with context and optional details.
     *
     * @param context  A short string identifying where the exception was thrown.
     * @param details  Optional additional information about the failure.
     */
    explicit NotGGAException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "The sentence is not a GGA sentence", details) {}
};

/**
 * @brief Represents a parsed NMEA 0183 GGA (Global Positioning System Fix Data) sentence.
 *
 * GGA contains UTC fix time, position, fix quality, and related metadata.
 *
 * Sentence format:
 * @code
 * $--GGA,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh<CR><LF>
 * @endcode
 */
class GGA : public Message0183 {
public:
    /**
     * @brief Construct a GGA message from individual field values.
     *
     * Latitude and longitude are expected in decimal degrees and are serialized
     * to NMEA coordinate format in the wire sentence.
     *
     * @param talkerId                    Two-character talker identifier.
     * @param timestamp                   UTC time in hhmmss.ss format.
     * @param latitude                    Latitude in decimal degrees.
     * @param latitudeDirection           Latitude hemisphere indicator ('N' or 'S').
     * @param longitude                   Longitude in decimal degrees.
     * @param longitudeDirection          Longitude hemisphere indicator ('E' or 'W').
     * @param gpsQuality                  GPS fix quality indicator.
     * @param satellites                  Number of satellites used.
     * @param hdop                        Horizontal dilution of precision.
     * @param altitude                    Antenna altitude above mean sea level.
     * @param altitudeUnits               Altitude unit indicator (typically 'M').
     * @param geoidalSeparation           Geoidal separation from WGS-84 ellipsoid.
     * @param geoidalSeparationUnits      Geoidal separation unit indicator (typically 'M').
     * @param dgpsAge                     Age of DGPS data in seconds.
     * @param dgpsReferenceStationId      DGPS reference station identifier.
     */
    GGA(std::string talkerId,
        double timestamp,
        double latitude,
        char latitudeDirection,
        double longitude,
        char longitudeDirection,
        unsigned int gpsQuality,
        unsigned int satellites,
        double hdop,
        double altitude,
        char altitudeUnits,
        double geoidalSeparation,
        char geoidalSeparationUnits,
        double dgpsAge,
        std::string dgpsReferenceStationId
    );

    GGA(const GGA&) = default;
    GGA& operator=(const GGA&) = default;
    GGA(GGA&&) noexcept = default;
    GGA& operator=(GGA&&) noexcept = default;

    ~GGA() override = default;

    /**
     * @brief Create a polymorphic copy of this GGA message.
     *
     * @return std::unique_ptr<nmealib::Message> A heap-allocated copy of this object.
     */
    std::unique_ptr<nmealib::Message> clone() const override;

    /** @brief Get UTC fix time in hhmmss.ss numeric form. */
    double getUtcTime() const noexcept;
    /** @brief Get latitude in decimal degrees. */
    double getLatitude() const noexcept;
    /** @brief Get latitude hemisphere indicator ('N' or 'S'). */
    char getLatitudeDirection() const noexcept;
    /** @brief Get longitude in decimal degrees. */
    double getLongitude() const noexcept;
    /** @brief Get longitude hemisphere indicator ('E' or 'W'). */
    char getLongitudeDirection() const noexcept;
    /** @brief Get GPS quality indicator. */
    unsigned int getGpsQuality() const noexcept;
    /** @brief Get number of satellites used. */
    unsigned int getSatellites() const noexcept;
    /** @brief Get horizontal dilution of precision. */
    double getHdop() const noexcept;
    /** @brief Get altitude value. */
    double getAltitude() const noexcept;
    /** @brief Get altitude units (typically 'M'). */
    char getAltitudeUnits() const noexcept;
    /** @brief Get geoidal separation value. */
    double getGeoidalSeparation() const noexcept;
    /** @brief Get geoidal separation units (typically 'M'). */
    char getGeoidalSeparationUnits() const noexcept;
    /** @brief Get age of DGPS data in seconds. */
    double getDgpsAge() const noexcept;
    /** @brief Get DGPS reference station identifier. */
    std::string getDgpsReferenceStationId() const noexcept;

    /**
     * @brief Return a human-readable string representation of this message.
     *
     * @param verbose When true, returns a multi-line detailed representation.
     * @return std::string Formatted string content.
     */
    std::string getStringContent(bool verbose) const noexcept override;

    /**
     * @brief Compare two GGA messages for equality.
     *
     * Equality delegates to Message0183::operator==.
     *
     * @param other The GGA message to compare with.
     * @return bool true if equal, false otherwise.
     */
    bool operator==(const GGA& other) const noexcept;

    /**
     * @brief Compare the parsed content of two GGA messages.
     *
     * Content comparison delegates to Message0183::hasEqualContent.
     *
     * @param other The GGA message to compare with.
     * @return bool true if content is equal, false otherwise.
     */
    bool hasEqualContent(const GGA& other) const noexcept;

private:
    double utcTime_{};
    double latitude_{};
    char latitudeDirection_{};
    double longitude_{};
    char longitudeDirection_{};
    unsigned int gpsQuality_{};
    unsigned int satellites_{};
    double hdop_{};
    double altitude_{};
    char altitudeUnits_{};
    double geoidalSeparation_{};
    char geoidalSeparationUnits_{};
    double dgpsAge_{};
    std::string dgpsReferenceStationId_{};

    GGA() = default;

    GGA(Message0183 baseMessage,
        double timestamp,
        double latitude,
        char latitudeDirection,
        double longitude,
        char longitudeDirection,
        unsigned int gpsQuality,
        unsigned int satellites,
        double hdop,
        double altitude,
        char altitudeUnits,
        double geoidalSeparation,
        char geoidalSeparationUnits,
        double dgpsAge,
        std::string dgpsReferenceStationId
    ) noexcept;

    static std::unique_ptr<GGA> create(std::unique_ptr<Message0183> baseMessage);
    static std::string composeRaw(const std::string& talkerId,
                                  double timestamp,
                                  double latitude,
                                  char latitudeDirection,
                                  double longitude,
                                  char longitudeDirection,
                                  unsigned int gpsQuality,
                                  unsigned int satellites,
                                  double hdop,
                                  double altitude,
                                  char altitudeUnits,
                                  double geoidalSeparation,
                                  char geoidalSeparationUnits,
                                  double dgpsAge,
                                  const std::string& dgpsReferenceStationId);

    friend class Nmea0183Factory;
    friend class MessageRegistry;
};

} // namespace nmea0183
} // namespace nmealib
