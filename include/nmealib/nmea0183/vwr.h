#pragma once

#include <nmealib/nmea0183.h>

namespace nmealib {
namespace nmea0183 {

/**
 * @brief Exception thrown when a sentence that is not a VWR sentence is processed as one.
 */
class NotVWRException : public NmeaException {
public:
    explicit NotVWRException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "The sentence is not a VWR sentence", details) {}
};

/**
 * @brief Represents a parsed NMEA 0183 VWR sentence (Relative Wind Speed and Angle).
 *
 * The VWR sentence reports wind speed and direction relative to the vessel.
 *
 * Sentence format:
 * @code
 * $--VWR,x.x,a,x.x,N,x.x,M,x.x,K*hh
 * @endcode
 *
 * Fields:
 *  - Wind direction magnitude in degrees (0–180)
 *  - Wind direction side: L = Left (port), R = Right (starboard)
 *  - Wind speed in knots
 *  - N (knots unit indicator)
 *  - Wind speed in meters per second
 *  - M (m/s unit indicator)
 *  - Wind speed in kilometers per hour
 *  - K (km/h unit indicator)
 */
class VWR : public Message0183 {
public:
    /**
     * @brief Construct a VWR sentence from individual field values.
     *
     * @param talkerId     Two-character talker identifier (e.g., "II", "WI").
     * @param windAngle    Wind direction magnitude in degrees from 0 to 180.
     * @param windSide     Side of the wind relative to the vessel: 'L' (port) or 'R' (starboard).
     * @param speedKnots   Wind speed in knots.
     * @param speedMps     Wind speed in meters per second.
     * @param speedKph     Wind speed in kilometers per hour.
     */
    VWR(std::string talkerId,
        double windAngle,
        char windSide,
        double speedKnots,
        double speedMps,
        double speedKph
    );

    VWR(const VWR&) = default;
    VWR& operator=(const VWR&) = default;
    VWR(VWR&&) noexcept = default;
    VWR& operator=(VWR&&) noexcept = default;

    ~VWR() override = default;

    std::unique_ptr<nmealib::Message> clone() const override;

    /**
     * @brief Returns the wind direction magnitude in degrees (0–180).
     *
     * @return double Wind direction magnitude.
     */
    double getWindAngle() const noexcept;

    /**
     * @brief Returns the wind direction side relative to the vessel.
     *
     * @return char 'L' for port (left), 'R' for starboard (right).
     */
    char getWindSide() const noexcept;

    /**
     * @brief Returns the wind speed in knots.
     *
     * @return double Wind speed in knots.
     */
    double getSpeedKnots() const noexcept;

    /**
     * @brief Returns the knots unit indicator character.
     *
     * @return char Always 'N'.
     */
    char getSpeedKnotsUnit() const noexcept;

    /**
     * @brief Returns the wind speed in meters per second.
     *
     * @return double Wind speed in m/s.
     */
    double getSpeedMps() const noexcept;

    /**
     * @brief Returns the meters-per-second unit indicator character.
     *
     * @return char Always 'M'.
     */
    char getSpeedMpsUnit() const noexcept;

    /**
     * @brief Returns the wind speed in kilometers per hour.
     *
     * @return double Wind speed in km/h.
     */
    double getSpeedKph() const noexcept;

    /**
     * @brief Returns the kilometers-per-hour unit indicator character.
     *
     * @return char Always 'K'.
     */
    char getSpeedKphUnit() const noexcept;

    std::string getStringContent(bool verbose) const noexcept override;
    bool operator==(const VWR& other) const noexcept;

private:
    double windAngle_{};
    char windSide_{};
    double speedKnots_{};
    char speedKnotsUnit_{};
    double speedMps_{};
    char speedMpsUnit_{};
    double speedKph_{};
    char speedKphUnit_{};

    VWR() = default;

    VWR(Message0183 baseMessage,
        double windAngle,
        char windSide,
        double speedKnots,
        char speedKnotsUnit,
        double speedMps,
        char speedMpsUnit,
        double speedKph,
        char speedKphUnit
    ) noexcept;

    static std::unique_ptr<VWR> create(std::unique_ptr<Message0183> baseMessage);
    static std::string composeRaw(const std::string& talkerId,
                                  double windAngle,
                                  char windSide,
                                  double speedKnots,
                                  double speedMps,
                                  double speedKph);

    friend class Nmea0183Factory;
    friend class MessageRegistry;
};

} // namespace nmea0183
} // namespace nmealib
