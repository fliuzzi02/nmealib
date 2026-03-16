#pragma once

#include <nmealib/nmea0183.h>

namespace nmealib {
namespace nmea0183 {

/**
 * @brief Exception thrown when a sentence is not a valid VHW sentence.
 *
 * This exception is raised during parsing when the sentence type is not "VHW"
 * or the payload does not conform to the expected VHW format.
 */
class NotVHWException : public NmeaException {
public:
    /**
     * @brief Construct a NotVHWException with context and optional details.
     *
     * @param context  A short string identifying where the exception was thrown.
     * @param details  Optional additional information about the failure.
     */
    explicit NotVHWException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "The sentence is not a VHW sentence", details) {}
};

/**
 * @brief Represents a parsed NMEA 0183 VHW (Water Speed and Heading) sentence.
 *
 * VHW carries true/magnetic heading and speed through water in knots and km/h.
 *
 * Sentence format:
 * @code
 * $--VHW,x.x,T,x.x,M,x.x,N,x.x,K*hh<CR><LF>
 * @endcode
 */
class VHW : public Message0183 {
public:
    /**
     * @brief Construct a VHW message from individual field values.
     *
     * The constructor sets heading/speed unit type indicators to the standard
     * values used by composeRaw: 'T', 'M', 'N', and 'K'.
     *
     * @param talkerId         Two-character talker identifier.
     * @param headingTrue      True heading in degrees.
     * @param headingMagnetic  Magnetic heading in degrees.
     * @param speedKnots       Speed through water in knots.
     * @param speedKph         Speed through water in kilometers per hour.
     */
    VHW(std::string talkerId,
        double headingTrue,
        double headingMagnetic,
        double speedKnots,
        double speedKph
    );

    VHW(const VHW&) = default;
    VHW& operator=(const VHW&) = default;
    VHW(VHW&&) noexcept = default;
    VHW& operator=(VHW&&) noexcept = default;

    ~VHW() override = default;

    /**
     * @brief Create a polymorphic copy of this VHW message.
     *
     * @return std::unique_ptr<nmealib::Message> A heap-allocated copy of this object.
     */
    std::unique_ptr<nmealib::Message> clone() const override;

    /** @brief Get true heading in degrees. */
    double getHeadingTrue() const noexcept;
    /** @brief Get true heading type indicator (typically 'T'). */
    char getHeadingTrueType() const noexcept;
    /** @brief Get magnetic heading in degrees. */
    double getHeadingMagnetic() const noexcept;
    /** @brief Get magnetic heading type indicator (typically 'M'). */
    char getHeadingMagneticType() const noexcept;
    /** @brief Get speed through water in knots. */
    double getSpeedKnots() const noexcept;
    /** @brief Get knots unit indicator (typically 'N'). */
    char getSpeedKnotsType() const noexcept;
    /** @brief Get speed through water in kilometers per hour. */
    double getSpeedKph() const noexcept;
    /** @brief Get km/h unit indicator (typically 'K'). */
    char getSpeedKphType() const noexcept;

    /**
     * @brief Return a human-readable string representation of this message.
     *
     * @param verbose When true, returns a multi-line detailed representation.
     * @return std::string Formatted string content.
     */
    std::string getStringContent(bool verbose) const noexcept override;

    /**
     * @brief Compare two VHW messages for equality.
     *
     * Equality delegates to Message0183::operator==.
     *
     * @param other The VHW message to compare with.
     * @return bool true if equal, false otherwise.
     */
    bool operator==(const VHW& other) const noexcept;

private:
    double headingTrue_{};
    char headingTrueType_{};
    double headingMagnetic_{};
    char headingMagneticType_{};
    double speedKnots_{};
    char speedKnotsType_{};
    double speedKph_{};
    char speedKphType_{};

    VHW() = delete;

    VHW(Message0183 baseMessage,
        double headingTrue,
        char headingTrueType,
        double headingMagnetic,
        char headingMagneticType,
        double speedKnots,
        char speedKnotsType,
        double speedKph,
        char speedKphType
    ) noexcept;

    static std::unique_ptr<VHW> create(std::unique_ptr<Message0183> baseMessage);
    static std::string composeRaw(const std::string& talkerId,
                                  double headingTrue,
                                  double headingMagnetic,
                                  double speedKnots,
                                  double speedKph);

    friend class Nmea0183Factory;
    friend class MessageRegistry;
};

} // namespace nmea0183
} // namespace nmealib
