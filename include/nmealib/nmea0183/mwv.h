#pragma once

#include <nmealib/nmea0183.h>

namespace nmealib {
namespace nmea0183 {

/**
 * @brief Exception thrown when a sentence is not a valid MWV sentence.
 *
 * This exception is raised during parsing when the sentence type is not "MWV"
 * or the payload does not conform to the expected MWV format.
 */
class NotMWVException : public NmeaException {
public:
    /**
     * @brief Construct a NotMWVException with context and optional details.
     *
     * @param context  A short string identifying where the exception was thrown.
     * @param details  Optional additional information about the failure.
     */
    explicit NotMWVException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "The sentence is not an MWV sentence", details) {}
};

/**
 * @brief Represents a parsed NMEA 0183 MWV (Wind Speed and Angle) sentence.
 *
 * The MWV sentence reports wind angle and speed relative to either true or
 * apparent reference.
 *
 * Sentence format:
 * @code
 * $--MWV,x.x,a,x.x,u,s*hh<CR><LF>
 * @endcode
 */
class MWV : public Message0183 {
public:
    /**
     * @brief Construct an MWV message from individual field values.
     *
     * @param talkerId        Two-character talker identifier.
     * @param windAngle       Wind angle in degrees.
     * @param reference       Wind reference indicator (for example 'R' or 'T').
     * @param windSpeed       Wind speed value.
     * @param windSpeedUnits  Wind speed unit indicator.
     * @param status          Data status indicator.
     */
    MWV(std::string talkerId,
        double windAngle,
        char reference,
        double windSpeed,
        char windSpeedUnits,
        char status
    );

    MWV(const MWV&) = default;
    MWV& operator=(const MWV&) = default;
    MWV(MWV&&) noexcept = default;
    MWV& operator=(MWV&&) noexcept = default;

    ~MWV() override = default;

    /**
     * @brief Create a polymorphic copy of this MWV message.
     *
     * @return std::unique_ptr<nmealib::Message> A heap-allocated copy of this object.
     */
    std::unique_ptr<nmealib::Message> clone() const override;

    /** @brief Get wind angle in degrees. */
    double getWindAngle() const noexcept;
    /** @brief Get reference indicator. */
    char getReference() const noexcept;
    /** @brief Get wind speed value. */
    double getWindSpeed() const noexcept;
    /** @brief Get wind speed unit indicator. */
    char getWindSpeedUnits() const noexcept;
    /** @brief Get data status indicator. */
    char getStatus() const noexcept;

    /**
     * @brief Return a human-readable string representation of this message.
     *
     * @param verbose When true, returns a multi-line detailed representation.
     * @return std::string Formatted string content.
     */
    std::string getStringContent(bool verbose) const noexcept override;

    /**
     * @brief Compare two MWV messages for equality.
     *
     * Equality delegates to Message0183::operator==.
     *
     * @param other The MWV message to compare with.
     * @return bool true if equal, false otherwise.
     */
    bool operator==(const MWV& other) const noexcept;

private:
    double windAngle_{};
    char reference_{};
    double windSpeed_{};
    char windSpeedUnits_{};
    char status_{};

    MWV() = delete;

    MWV(Message0183 baseMessage,
        double windAngle,
        char reference,
        double windSpeed,
        char windSpeedUnits,
        char status
    ) noexcept;

    static std::unique_ptr<MWV> create(std::unique_ptr<Message0183> baseMessage);
    static std::string composeRaw(const std::string& talkerId,
                                  double windAngle,
                                  char reference,
                                  double windSpeed,
                                  char windSpeedUnits,
                                  char status);

    friend class Nmea0183Factory;
    friend class MessageRegistry;
};

} // namespace nmea0183
} // namespace nmealib
