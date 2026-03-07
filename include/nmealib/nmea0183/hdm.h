#pragma once

#include <nmealib/nmea0183.h>

namespace nmealib {
namespace nmea0183 {

/**
 * @brief Exception thrown when a sentence is not a valid HDM sentence.
 *
 * This exception is raised during parsing when the sentence type is not "HDM"
 * or the payload does not conform to the expected HDM format.
 */
class NotHDMException : public NmeaException {
public:
    /**
     * @brief Construct a NotHDMException with context and optional details.
     *
     * @param context  A short string identifying where the exception was thrown
     *                 (e.g., the name of the function or class).
     * @param details  Optional additional information about the failure.
     */
    explicit NotHDMException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "The sentence is not an HDM sentence", details) {}
};

/**
 * @brief Represents a parsed NMEA 0183 HDM (Heading – Magnetic) sentence.
 *
 * The HDM sentence reports the vessel heading in degrees with respect to
 * magnetic north, as produced by any device or system providing magnetic
 * heading. The sentence format is:
 *
 * @code
 * $--HDM,x.x,M*hh<CR><LF>
 * @endcode
 *
 * Fields:
 *  - Field 1 (@c x.x): Heading in degrees magnetic (floating-point, 0–359.9).
 *  - Field 2 (@c M): Magnetic heading indicator. Always 'M' per the NMEA 0183 standard.
 *
 * Example sentence:
 * @code
 * $IIHDM,45.0,M*13\r\n
 * @endcode
 */
class HDM : public Message0183 {
public:
    /**
     * @brief Construct an HDM message from individual field values.
     *
     * Composes the raw NMEA sentence from the supplied fields and initialises
     * the object. A checksum is automatically appended to the raw sentence.
     *
     * @param talkerId  Two-character talker identifier (e.g., "II", "HE").
     * @param heading   Vessel heading in degrees magnetic (0.0–359.9).
     * @param indicator Magnetic heading indicator. Should be 'M' as required
     *                  by the NMEA 0183 standard.
     */
    HDM(std::string talkerId,
        double heading,
        char indicator);

    HDM(const HDM&) = default;
    HDM& operator=(const HDM&) = default;
    HDM(HDM&&) noexcept = default;
    HDM& operator=(HDM&&) noexcept = default;

    ~HDM() override = default;

    /**
     * @brief Create a polymorphic copy of this HDM message.
     *
     * @return std::unique_ptr<nmealib::Message> A heap-allocated copy of this object.
     */
    std::unique_ptr<nmealib::Message> clone() const override;

    /**
     * @brief Get the vessel heading in degrees magnetic.
     *
     * @return double The heading value (in degrees, 0.0–359.9).
     */
    double getHeading() const noexcept;

    /**
     * @brief Get the magnetic heading indicator character.
     *
     * Per the NMEA 0183 standard this is always 'M' (magnetic).
     *
     * @return char The indicator character (typically 'M').
     */
    char getIndicator() const noexcept;

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
     * @brief Compare two HDM messages for equality.
     *
     * Two HDM messages are equal when their underlying Message0183 representations
     * are equal (raw sentence, talker, checksum, and timestamp all match).
     *
     * @param other The HDM message to compare with.
     * @return bool @c true if the messages are equal, @c false otherwise.
     */
    bool operator==(const HDM& other) const noexcept;

    /**
     * @brief Compare the parsed content of two HDM messages.
     *
     * Unlike @c operator==, this method compares the decoded field values
     * rather than the raw sentence bytes.
     *
     * @param other The HDM message to compare with.
     * @return bool @c true if the content is equal, @c false otherwise.
     */
    bool hasEqualContent(const HDM& other) const noexcept;

private:
    double heading_{};
    char indicator_{};

    HDM() = default;

    HDM(Message0183 baseMessage,
        double heading,
        char indicator) noexcept;

    static std::unique_ptr<HDM> create(std::unique_ptr<Message0183> baseMessage);
    static std::string composeRaw(const std::string& talkerId,
                                  double heading,
                                  char indicator);

    friend class Nmea0183Factory;
    friend class MessageRegistry;
};

} // namespace nmea0183
} // namespace nmealib
