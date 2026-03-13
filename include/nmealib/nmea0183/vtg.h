#pragma once

#include <nmealib/nmea0183.h>

#include <optional>

namespace nmealib {
namespace nmea0183 {

/**
 * @brief Exception thrown when a sentence is not a valid VTG sentence.
 *
 * This exception is raised during parsing when the sentence type is not "VTG"
 * or the payload does not conform to one of the supported VTG layouts.
 */
class NotVTGException : public NmeaException {
public:
    /**
     * @brief Construct a NotVTGException with context and optional details.
     *
     * @param context  A short string identifying where the exception was thrown.
     * @param details  Optional additional information about the failure.
     */
    explicit NotVTGException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "The sentence is not a VTG sentence", details) {}
};

/**
 * @brief Represents a parsed NMEA 0183 VTG (Course Over Ground and Ground Speed) sentence.
 *
 * VTG supports both modern tagged format and a legacy compact format.
 * In legacy format, type indicators are omitted and only numeric values are present.
 *
 * Modern sentence format:
 * @code
 * $--VTG,x.x,T,x.x,M,x.x,N,x.x,K[,a]*hh<CR><LF>
 * @endcode
 *
 * Legacy sentence format:
 * @code
 * $--VTG,x.x,x.x,x.x,x.x*hh<CR><LF>
 * @endcode
 */
class VTG : public Message0183 {
public:
    /**
     * @brief Construct a VTG message from individual field values.
     *
     * @param talkerId                 Two-character talker identifier.
     * @param courseOverGroundTrue     True course over ground in degrees.
     * @param courseOverGroundMagnetic Magnetic course over ground in degrees.
     * @param speedOverGroundKnots     Speed over ground in knots.
     * @param speedOverGroundKph       Speed over ground in kilometers per hour.
     * @param faaModeIndicator         Optional FAA mode indicator for modern format.
     * @param legacyFormat             When true, serializes/parses legacy compact format.
     */
    VTG(std::string talkerId,
        double courseOverGroundTrue,
        double courseOverGroundMagnetic,
        double speedOverGroundKnots,
        double speedOverGroundKph,
        std::optional<char> faaModeIndicator = std::nullopt,
        bool legacyFormat = false
    );

    VTG(const VTG&) = default;
    VTG& operator=(const VTG&) = default;
    VTG(VTG&&) noexcept = default;
    VTG& operator=(VTG&&) noexcept = default;

    ~VTG() override = default;

    /**
     * @brief Create a polymorphic copy of this VTG message.
     *
     * @return std::unique_ptr<nmealib::Message> A heap-allocated copy of this object.
     */
    std::unique_ptr<nmealib::Message> clone() const override;

    /** @brief Get true course over ground in degrees. */
    double getCourseOverGroundTrue() const noexcept;
    /** @brief Get true course type indicator (typically 'T', or '\0' in legacy format). */
    char getCourseOverGroundTrueType() const noexcept;
    /** @brief Get magnetic course over ground in degrees. */
    double getCourseOverGroundMagnetic() const noexcept;
    /** @brief Get magnetic course type indicator (typically 'M', or '\0' in legacy format). */
    char getCourseOverGroundMagneticType() const noexcept;
    /** @brief Get speed over ground in knots. */
    double getSpeedOverGroundKnots() const noexcept;
    /** @brief Get knots unit indicator (typically 'N', or '\0' in legacy format). */
    char getSpeedOverGroundKnotsType() const noexcept;
    /** @brief Get speed over ground in kilometers per hour. */
    double getSpeedOverGroundKph() const noexcept;
    /** @brief Get km/h unit indicator (typically 'K', or '\0' in legacy format). */
    char getSpeedOverGroundKphType() const noexcept;
    /** @brief Return whether FAA mode indicator is present. */
    bool hasFaaModeIndicator() const noexcept;
    /** @brief Get optional FAA mode indicator. */
    std::optional<char> getFaaModeIndicator() const noexcept;
    /** @brief Return whether this sentence uses legacy compact VTG format. */
    bool isLegacyFormat() const noexcept;

    /**
     * @brief Return a human-readable string representation of this message.
     *
     * @param verbose When true, returns a multi-line detailed representation.
     * @return std::string Formatted string content.
     */
    std::string getStringContent(bool verbose) const noexcept override;

    /**
     * @brief Compare two VTG messages for equality.
     *
     * Equality delegates to Message0183::operator==.
     *
     * @param other The VTG message to compare with.
     * @return bool true if equal, false otherwise.
     */
    bool operator==(const VTG& other) const noexcept;

    /**
     * @brief Compare the parsed content of two VTG messages.
     *
     * Content comparison delegates to Message0183::hasEqualContent.
     *
     * @param other The VTG message to compare with.
     * @return bool true if content is equal, false otherwise.
     */
    bool hasEqualContent(const VTG& other) const noexcept;

private:
    double courseOverGroundTrue_{};
    char courseOverGroundTrueType_{};
    double courseOverGroundMagnetic_{};
    char courseOverGroundMagneticType_{};
    double speedOverGroundKnots_{};
    char speedOverGroundKnotsType_{};
    double speedOverGroundKph_{};
    char speedOverGroundKphType_{};
    std::optional<char> faaModeIndicator_{};
    bool legacyFormat_{};

    VTG() = default;

    VTG(Message0183 baseMessage,
        double courseOverGroundTrue,
        char courseOverGroundTrueType,
        double courseOverGroundMagnetic,
        char courseOverGroundMagneticType,
        double speedOverGroundKnots,
        char speedOverGroundKnotsType,
        double speedOverGroundKph,
        char speedOverGroundKphType,
        std::optional<char> faaModeIndicator,
        bool legacyFormat
    ) noexcept;

    static std::unique_ptr<VTG> create(std::unique_ptr<Message0183> baseMessage);
    static std::string composeRaw(const std::string& talkerId,
                                  double courseOverGroundTrue,
                                  double courseOverGroundMagnetic,
                                  double speedOverGroundKnots,
                                  double speedOverGroundKph,
                                  std::optional<char> faaModeIndicator,
                                  bool legacyFormat);

    friend class Nmea0183Factory;
    friend class MessageRegistry;
};

} // namespace nmea0183
} // namespace nmealib
