#pragma once

#include <nmealib/nmea0183.h>

namespace nmealib {
namespace nmea0183 {

/**
 * @brief Exception thrown when a sentence is not a valid GLL sentence.
 *
 * This exception is raised during parsing when the sentence type is not "GLL"
 * or the payload does not conform to the expected GLL format.
 */
class NotGLLException : public NmeaException {
public:
    /**
     * @brief Construct a NotGLLException with context and optional details.
     *
     * @param context  A short string identifying where the exception was thrown.
     * @param details  Optional additional information about the failure.
     */
    explicit NotGLLException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "The sentence is not a GLL sentence", details) {}
};

/**
 * @brief Represents a parsed NMEA 0183 GLL (Geographic Position - Latitude/Longitude) sentence.
 *
 * GLL carries geographic position, UTC time, status, and mode information.
 *
 * Sentence format:
 * @code
 * $--GLL,llll.ll,a,yyyyy.yy,a,hhmmss.ss,A,a*hh<CR><LF>
 * @endcode
 */
class GLL : public Message0183 {
public:
    /**
     * @brief Construct a GLL message from individual field values.
     *
     * Latitude and longitude are expected in decimal degrees and are serialized
     * to NMEA coordinate format in the wire sentence.
     *
     * @param talkerId              Two-character talker identifier.
     * @param latitude              Latitude in decimal degrees.
     * @param latitudeDirection     Latitude hemisphere indicator ('N' or 'S').
     * @param longitude             Longitude in decimal degrees.
     * @param longitudeDirection    Longitude hemisphere indicator ('E' or 'W').
     * @param timestamp             UTC time in hhmmss.ss format.
     * @param status                Data status indicator (typically 'A' or 'V').
     * @param modeIndicator         Mode indicator.
     */
    GLL(std::string talkerId,
        double latitude,
        char latitudeDirection,
        double longitude,
        char longitudeDirection,
        double timestamp,
        char status,
        char modeIndicator
    );

    GLL(const GLL&) = default;
    GLL& operator=(const GLL&) = default;
    GLL(GLL&&) noexcept = default;
    GLL& operator=(GLL&&) noexcept = default;

    ~GLL() override = default;

    /**
     * @brief Create a polymorphic copy of this GLL message.
     *
     * @return std::unique_ptr<nmealib::Message> A heap-allocated copy of this object.
     */
    std::unique_ptr<nmealib::Message> clone() const override;

    /** @brief Get latitude in decimal degrees. */
    double getLatitude() const noexcept;
    /** @brief Get latitude hemisphere indicator ('N' or 'S'). */
    char getLatitudeDirection() const noexcept;
    /** @brief Get longitude in decimal degrees. */
    double getLongitude() const noexcept;
    /** @brief Get longitude hemisphere indicator ('E' or 'W'). */
    char getLongitudeDirection() const noexcept;
    /** @brief Get UTC time in hhmmss.ss numeric form. */
    double getUtcTime() const noexcept;
    /** @brief Get status indicator (typically 'A' or 'V'). */
    char getStatus() const noexcept;
    /** @brief Get mode indicator character. */
    char getModeIndicator() const noexcept;

    /**
     * @brief Return a human-readable string representation of this message.
     *
     * @param verbose When true, returns a multi-line detailed representation.
     * @return std::string Formatted string content.
     */
    std::string getStringContent(bool verbose) const noexcept override;

    /**
     * @brief Compare two GLL messages for equality.
     *
     * Equality delegates to Message0183::operator==.
     *
     * @param other The GLL message to compare with.
     * @return bool true if equal, false otherwise.
     */
    bool operator==(const GLL& other) const noexcept;

private:
    double latitude_{};
    char latitudeDirection_{};
    double longitude_{};
    char longitudeDirection_{};
    double utcTime_{};
    char status_{};
    char modeIndicator_{};

    GLL() = default;

    GLL(Message0183 baseMessage,
        double latitude,
        char latitudeDirection,
        double longitude,
        char longitudeDirection,
        double timestamp,
        char status,
        char modeIndicator
    ) noexcept;

    static std::unique_ptr<GLL> create(std::unique_ptr<Message0183> baseMessage);
    static std::string composeRaw(const std::string& talkerId,
                                  double latitude,
                                  char latitudeDirection,
                                  double longitude,
                                  char longitudeDirection,
                                  double timestamp,
                                  char status,
                                  char modeIndicator);

    friend class Nmea0183Factory;
    friend class MessageRegistry;
};

} // namespace nmea0183
} // namespace nmealib
