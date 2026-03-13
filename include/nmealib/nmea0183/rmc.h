#pragma once

#include <nmealib/nmea0183.h>

namespace nmealib {
namespace nmea0183 {

/**
 * @brief Exception thrown when a sentence is not a valid RMC sentence.
 *
 * This exception is raised during parsing when the sentence type is not "RMC"
 * or the payload does not conform to the expected RMC format.
 */
class NotRMCException : public NmeaException {
public:
    /**
     * @brief Construct a NotRMCException with context and optional details.
     *
     * @param context  A short string identifying where the exception was thrown.
     * @param details  Optional additional information about the failure.
     */
    explicit NotRMCException(const std::string& context, const std::string& details = "") : 
    NmeaException(context, "The sentence is not an RMC sentence", details) {}
};

/**
 * @brief Represents a parsed NMEA 0183 RMC (Recommended Minimum Navigation Information) sentence.
 *
 * RMC carries core navigation data including UTC time, status, position,
 * speed/course over ground, date, magnetic variation, and mode/status fields.
 *
 * Sentence format (common modern variant):
 * @code
 * $--RMC,hhmmss.ss,A,llll.ll,a,yyyyy.yy,a,x.x,x.x,ddmmyy,x.x,a,a,a*hh<CR><LF>
 * @endcode
 */
class RMC : public Message0183 {
public:
    /**
     * @brief Construct an RMC message from individual field values.
     * 
     * Latitude and longitude are expected in decimal degrees and are serialized
     * to NMEA coordinate format in the wire sentence.
     *
     * @param talkerId                    Two-character talker identifier.
     * @param utcFix                      UTC time in hhmmss[.ss] numeric form.
     * @param status                      Navigation receiver warning/status indicator.
     * @param latitude                    Latitude in decimal degrees.
     * @param latitudeDirection           Latitude hemisphere indicator ('N' or 'S').
     * @param longitude                   Longitude in decimal degrees.
     * @param longitudeDirection          Longitude hemisphere indicator ('E' or 'W').
     * @param speedOverGround             Speed over ground in knots.
     * @param courseOverGround            Course over ground in degrees.
     * @param date                        Date in ddmmyy numeric form.
     * @param magneticVariation           Magnetic variation in degrees.
     * @param magneticVariationDirection  Magnetic variation direction ('E' or 'W').
     * @param modeIndicator               Optional mode indicator.
     * @param navigationStatus            Navigation status indicator.
     */
    RMC(std::string talkerId,
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
        char navigationStatus
    );

    RMC(const RMC&) = default;
    RMC& operator=(const RMC&) = default;
    RMC(RMC&&) noexcept = default;
    RMC& operator=(RMC&&) noexcept = default;

    ~RMC() override = default;

    /**
     * @brief Create a polymorphic copy of this RMC message.
     *
     * @return std::unique_ptr<nmealib::Message> A heap-allocated copy of this object.
     */
    std::unique_ptr<nmealib::Message> clone() const override;

    /** @brief Get UTC fix time in hhmmss[.ss] numeric form. */
    unsigned int getUtcFix() const noexcept;
    /** @brief Get status indicator. */
    char getStatus() const noexcept;
    /** @brief Get latitude in decimal degrees. */
    double getLatitude() const noexcept;
    /** @brief Get latitude hemisphere indicator ('N' or 'S'). */
    char getLatitudeDirection() const noexcept;
    /** @brief Get longitude in decimal degrees. */
    double getLongitude() const noexcept;
    /** @brief Get longitude hemisphere indicator ('E' or 'W'). */
    char getLongitudeDirection() const noexcept;
    /** @brief Get speed over ground in knots. */
    double getSpeedOverGround() const noexcept;
    /** @brief Get course over ground in degrees. */
    double getCourseOverGround() const noexcept;
    /** @brief Get date in ddmmyy numeric form. */
    unsigned int getDate() const noexcept;
    /** @brief Get magnetic variation in degrees. */
    double getMagneticVariation() const noexcept;
    /** @brief Get magnetic variation direction ('E' or 'W'). */
    char getMagneticVariationDirection() const noexcept;
    /** @brief Get mode indicator character. */
    char getModeIndicator() const noexcept;
    /** @brief Get navigation status character. */
    char getNavigationStatus() const noexcept;

    /**
     * @brief Return a human-readable string representation of this message.
     *
     * @param verbose When true, returns a multi-line detailed representation.
     * @return std::string Formatted string content.
     */
    std::string getStringContent(bool verbose) const noexcept override;

    /**
     * @brief Compare two RMC messages for equality.
     *
     * Equality delegates to Message0183::operator==.
     *
     * @param other The RMC message to compare with.
     * @return bool true if equal, false otherwise.
     */
    bool operator==(const RMC& other) const noexcept;

    /**
     * @brief Compare the parsed content of two RMC messages.
     *
     * Content comparison delegates to Message0183::hasEqualContent.
     *
     * @param other The RMC message to compare with.
     * @return bool true if content is equal, false otherwise.
     */
    bool hasEqualContent(const RMC& other) const noexcept;

private:
    unsigned int utcFix_{};
    char status_{};
    double latitude_{};
    char latitudeDirection_{};
    double longitude_{};
    char longitudeDirection_{};
    double speedOverGround_{};
    double courseOverGround_{};
    unsigned int date_{};
    double magneticVariation_{};
    char magneticVariationDirection_{};
    char modeIndicator_{};
    char navigationStatus_{};

    RMC() = default;

    RMC(Message0183 baseMessage, 
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
        char navigationStatus
    ) noexcept;

    static std::unique_ptr<RMC> create(std::unique_ptr<Message0183> baseMessage);
    static std::string composeRaw(const std::string& talkerId, unsigned int utcFix, 
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
                            char navigationStatus);
    
    friend class Nmea0183Factory;
    friend class MessageRegistry;
};

} // namespace nmea0183
} // namespace nmealib