#pragma once

#include <nmealib/nmea0183.h>

namespace nmealib {
namespace nmea0183 {

/**
 * @brief Exception thrown when a sentence is not a valid MTW sentence.
 *
 * This exception is raised during parsing when the sentence type is not "MTW"
 * or the payload does not conform to the expected MTW format.
 */
class NotMTWException : public NmeaException {
public:
    /**
     * @brief Construct a NotMTWException with context and optional details.
     *
     * @param context  A short string identifying where the exception was thrown
     *                 (e.g., the name of the function or class).
     * @param details  Optional additional information about the failure.
     */
    explicit NotMTWException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "The sentence is not an MTW sentence", details) {}
};

/**
 * @brief Represents a parsed NMEA 0183 MTW (Mean Temperature of Water) sentence.
 *
 * The MTW sentence reports the mean temperature of the water as measured by
 * the instrument. The sentence format is:
 *
 * @code
 * $--MTW,x.x,C*hh<CR><LF>
 * @endcode
 *
 * Fields:
 *  - Field 1 (@c x.x): Water temperature value (floating-point).
 *  - Field 2 (@c C): Unit of temperature. Always 'C' (degrees Celsius) per the NMEA 0183 standard.
 *
 * Example sentence:
 * @code
 * $IIMTW,23.5,C*1E\r\n
 * @endcode
 */
class MTW : public Message0183 {
public:
    /**
     * @brief Construct an MTW message from individual field values.
     *
     * Composes the raw NMEA sentence from the supplied fields and initialises
     * the object. A checksum is automatically appended to the raw sentence.
     *
     * @param talkerId      Two-character talker identifier (e.g., "II", "GP").
     * @param temperature   Water temperature in degrees Celsius.
     * @param unit          Unit of temperature. Should be 'C' (Celsius) as
     *                      required by the NMEA 0183 standard.
     */
    MTW(std::string talkerId,
        double temperature,
        char unit);

    MTW(const MTW&) = default;
    MTW& operator=(const MTW&) = default;
    MTW(MTW&&) noexcept = default;
    MTW& operator=(MTW&&) noexcept = default;

    ~MTW() override = default;

    /**
     * @brief Create a polymorphic copy of this MTW message.
     *
     * @return std::unique_ptr<nmealib::Message> A heap-allocated copy of this object.
     */
    std::unique_ptr<nmealib::Message> clone() const override;

    /**
     * @brief Get the water temperature value.
     *
     * @return double The water temperature (in the units indicated by @c getUnit()).
     */
    double getTemperature() const noexcept;

    /**
     * @brief Get the temperature unit character.
     *
     * Per the NMEA 0183 standard this is always 'C' (degrees Celsius).
     *
     * @return char The unit character (typically 'C').
     */
    char getUnit() const noexcept;

    /**
     * @brief Return a human-readable string representation of this message.
     *
     * @param verbose When @c true, every field is printed on its own labelled
     *                line. When @c false, a compact single-line summary is
     *                returned instead.
     * @return std::string The formatted string content.
     */
    std::string getStringContent(bool verbose) const noexcept override;

    /**
     * @brief Compare two MTW messages for equality.
     *
     * Two MTW messages are equal when their underlying Message0183 representations
     * are equal (raw sentence, talker, checksum, and timestamp all match).
     *
     * @param other The MTW message to compare with.
     * @return bool @c true if the messages are equal, @c false otherwise.
     */
    bool operator==(const MTW& other) const noexcept;

    /**
     * @brief Compare the parsed content of two MTW messages.
     *
     * Unlike @c operator==, this method compares the decoded field values
     * rather than the raw sentence bytes.
     *
     * @param other The MTW message to compare with.
     * @return bool @c true if the content is equal, @c false otherwise.
     */
    bool hasEqualContent(const MTW& other) const noexcept;

private:
    double temperature_{};
    char unit_{};

    MTW() = default;

    MTW(Message0183 baseMessage,
        double temperature,
        char unit) noexcept;

    static std::unique_ptr<MTW> create(std::unique_ptr<Message0183> baseMessage);
    static std::string composeRaw(const std::string& talkerId,
                                  double temperature,
                                  char unit);

    friend class Nmea0183Factory;
    friend class MessageRegistry;
};

} // namespace nmea0183
} // namespace nmealib
