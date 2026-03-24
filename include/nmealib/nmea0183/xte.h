#pragma once

#include <nmealib/nmea0183.h>

#include <optional>

namespace nmealib {
namespace nmea0183 {

/**
 * @brief Exception thrown when a sentence is not a valid XTE sentence.
 *
 * This exception is raised during parsing when the sentence type is not "XTE"
 * or the payload does not conform to one of the supported XTE layouts.
 */
class NotXTEException : public NmeaException {
public:
    /**
     * @brief Construct a NotXTEException with context and optional details.
     *
     * @param context  A short string identifying where the exception was thrown.
     * @param details  Optional additional information about the failure.
     */
    explicit NotXTEException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "The sentence is not an XTE sentence", details) {}
};

/**
 * @brief Represents a parsed NMEA 0183 XTE (Cross-Track Error, Measured) sentence.
 *
 * XTE supports the classic format and the NMEA 2.3+ format that includes an
 * optional FAA mode indicator.
 *
 * Classic sentence format:
 * @code
 * $--XTE,A,A,x.x,a,N*hh<CR><LF>
 * @endcode
 *
 * NMEA 2.3+ sentence format:
 * @code
 * $--XTE,A,A,x.x,a,N,m*hh<CR><LF>
 * @endcode
 */
class XTE : public Message0183 {
public:
    /**
     * @brief Construct an XTE message from individual field values.
     *
     * @param talkerId          Two-character talker identifier.
     * @param status1           First status field ('A' valid, 'V' warning).
     * @param status2           Second status field ('A' valid, 'V' warning).
     * @param crossTrackError   Cross-track error magnitude in nautical miles.
     * @param steerDirection    Direction to steer ('L' or 'R').
     * @param crossTrackUnits   Cross-track units (typically 'N').
     * @param faaModeIndicator  Optional FAA mode indicator (NMEA 2.3+).
     */
    XTE(std::string talkerId,
        char status1,
        char status2,
        double crossTrackError,
        char steerDirection,
        char crossTrackUnits,
        std::optional<char> faaModeIndicator = std::nullopt
    );

    XTE(const XTE&) = default;
    XTE& operator=(const XTE&) = default;
    XTE(XTE&&) noexcept = default;
    XTE& operator=(XTE&&) noexcept = default;

    ~XTE() override = default;

    /**
     * @brief Create a polymorphic copy of this XTE message.
     *
     * @return std::unique_ptr<nmealib::Message> A heap-allocated copy of this object.
     */
    std::unique_ptr<nmealib::Message> clone() const override;

    /** @brief Get the first status field. */
    char getStatus1() const noexcept;
    /** @brief Get the second status field. */
    char getStatus2() const noexcept;
    /** @brief Get the cross-track error magnitude in nautical miles. */
    double getCrossTrackError() const noexcept;
    /** @brief Get the steer direction indicator ('L' or 'R'). */
    char getSteerDirection() const noexcept;
    /** @brief Get the cross-track units indicator (typically 'N'). */
    char getCrossTrackUnits() const noexcept;
    /** @brief Return whether FAA mode indicator is present. */
    bool hasFaaModeIndicator() const noexcept;
    /** @brief Get optional FAA mode indicator. */
    std::optional<char> getFaaModeIndicator() const noexcept;

    /**
     * @brief Return a human-readable string representation of this message.
     *
     * @param verbose When true, returns a multi-line detailed representation.
     * @return std::string Formatted string content.
     */
    std::string getStringContent(bool verbose) const noexcept override;

    /**
     * @brief Compare two XTE messages for equality.
     *
     * Equality delegates to Message0183::operator==.
     *
     * @param other The XTE message to compare with.
     * @return bool true if equal, false otherwise.
     */
    bool operator==(const XTE& other) const noexcept;

private:
    char status1_{};
    char status2_{};
    double crossTrackError_{};
    char steerDirection_{};
    char crossTrackUnits_{};
    std::optional<char> faaModeIndicator_{};

    XTE() = delete;

    XTE(Message0183 baseMessage,
        char status1,
        char status2,
        double crossTrackError,
        char steerDirection,
        char crossTrackUnits,
        std::optional<char> faaModeIndicator
    ) noexcept;

    static std::unique_ptr<XTE> create(std::unique_ptr<Message0183> baseMessage);
    static std::string composeRaw(const std::string& talkerId,
                                  char status1,
                                  char status2,
                                  double crossTrackError,
                                  char steerDirection,
                                  char crossTrackUnits,
                                  std::optional<char> faaModeIndicator);

    friend class Nmea0183Factory;
    friend class MessageRegistry;
};

} // namespace nmea0183
} // namespace nmealib
