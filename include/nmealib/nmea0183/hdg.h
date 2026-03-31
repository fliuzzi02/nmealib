#pragma once

#include <nmealib/nmea0183.h>

namespace nmealib {
namespace nmea0183 {

/**
 * @brief Exception thrown when a sentence is not a valid HDG sentence.
 *
 * This exception is raised during parsing when the sentence type is not "HDG"
 * or the payload does not conform to the expected HDG format.
 */
class NotHDGException : public NmeaException {
public:
    /**
     * @brief Construct a NotHDGException with context and optional details.
     *
     * @param context  A short string identifying where the exception was thrown.
     * @param details  Optional additional information about the failure.
     */
    explicit NotHDGException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "The sentence is not an HDG sentence", details) {}
};

/**
 * @brief Represents a parsed NMEA 0183 HDG (Heading, Deviation & Variation) sentence.
 *
 * HDG carries magnetic heading from the sensor and optional magnetic deviation
 * and variation with their East/West direction indicators.
 *
 * Sentence format:
 * @code
 * $--HDG,x.x,x.x,a,x.x,a*hh<CR><LF>
 * @endcode
 */
class HDG : public Message0183 {
public:
    /**
     * @brief Construct an HDG message from individual field values.
     *
     * @param talkerId            Two-character talker identifier.
     * @param magneticHeading     Magnetic sensor heading in degrees.
     * @param magneticDeviation   Magnetic deviation in degrees.
     * @param deviationDirection  Deviation direction ('E' or 'W').
     * @param magneticVariation   Magnetic variation in degrees.
     * @param variationDirection  Variation direction ('E' or 'W').
     */
    HDG(std::string talkerId,
        double magneticHeading,
        double magneticDeviation,
        char deviationDirection,
        double magneticVariation,
        char variationDirection);

    HDG(const HDG&) = default;
    HDG& operator=(const HDG&) = default;
    HDG(HDG&&) noexcept = default;
    HDG& operator=(HDG&&) noexcept = default;

    ~HDG() override = default;

    std::unique_ptr<nmealib::Message> clone() const override;

    double getMagneticHeading() const noexcept;
    double getMagneticDeviation() const noexcept;
    char getDeviationDirection() const noexcept;
    double getMagneticVariation() const noexcept;
    char getVariationDirection() const noexcept;

    std::string getStringContent(bool verbose) const noexcept override;

    bool operator==(const HDG& other) const noexcept;

private:
    double magneticHeading_{};
    double magneticDeviation_{};
    char deviationDirection_{};
    double magneticVariation_{};
    char variationDirection_{};

    HDG() = delete;

    HDG(Message0183 baseMessage,
        double magneticHeading,
        double magneticDeviation,
        char deviationDirection,
        double magneticVariation,
        char variationDirection) noexcept;

    static std::unique_ptr<HDG> create(std::unique_ptr<Message0183> baseMessage);
    static std::string composeRaw(const std::string& talkerId,
                                  double magneticHeading,
                                  double magneticDeviation,
                                  char deviationDirection,
                                  double magneticVariation,
                                  char variationDirection);

    friend class Nmea0183Factory;
    friend class MessageRegistry;
};

} // namespace nmea0183
} // namespace nmealib
