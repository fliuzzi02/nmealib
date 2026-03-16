#pragma once

#include <nmealib/nmea0183.h>

namespace nmealib {
namespace nmea0183 {

/**
 * @brief Exception thrown when a sentence is not a valid ZDA sentence.
 *
 * This exception is raised during parsing when the sentence type is not "ZDA"
 * or the payload does not conform to the expected ZDA format.
 */
class NotZDAException : public NmeaException {
public:
    /**
     * @brief Construct a NotZDAException with context and optional details.
     *
     * @param context  A short string identifying where the exception was thrown.
     * @param details  Optional additional information about the failure.
     */
    explicit NotZDAException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "The sentence is not a ZDA sentence", details) {}
};

/**
 * @brief Represents a parsed NMEA 0183 ZDA (Time and Date) sentence.
 *
 * ZDA carries UTC time, calendar date, and local zone offset.
 *
 * Sentence format:
 * @code
 * $--ZDA,hhmmss.ss,dd,mm,yyyy,zh,zm*hh<CR><LF>
 * @endcode
 */
class ZDA : public Message0183 {
public:
    /**
     * @brief Construct a ZDA message from individual field values.
     *
     * @param talkerId          Two-character talker identifier.
     * @param utcTime           UTC time in hhmmss.ss numeric form.
     * @param day               Day of month.
     * @param month             Month number.
     * @param year              Four-digit year.
     * @param localZoneHours    Local zone offset hours.
     * @param localZoneMinutes  Local zone offset minutes.
     */
    ZDA(std::string talkerId,
        double utcTime,
        unsigned int day,
        unsigned int month,
        unsigned int year,
        int localZoneHours,
        int localZoneMinutes
    );

    ZDA(const ZDA&) = default;
    ZDA& operator=(const ZDA&) = default;
    ZDA(ZDA&&) noexcept = default;
    ZDA& operator=(ZDA&&) noexcept = default;

    ~ZDA() override = default;

    /**
     * @brief Create a polymorphic copy of this ZDA message.
     *
     * @return std::unique_ptr<nmealib::Message> A heap-allocated copy of this object.
     */
    std::unique_ptr<nmealib::Message> clone() const override;

    /** @brief Get UTC time in hhmmss.ss numeric form. */
    double getUtcTime() const noexcept;
    /** @brief Get day of month. */
    unsigned int getDay() const noexcept;
    /** @brief Get month number. */
    unsigned int getMonth() const noexcept;
    /** @brief Get four-digit year. */
    unsigned int getYear() const noexcept;
    /** @brief Get local zone offset hours. */
    int getLocalZoneHours() const noexcept;
    /** @brief Get local zone offset minutes. */
    int getLocalZoneMinutes() const noexcept;

    /**
     * @brief Return a human-readable string representation of this message.
     *
     * @param verbose When true, returns a multi-line detailed representation.
     * @return std::string Formatted string content.
     */
    std::string getStringContent(bool verbose) const noexcept override;

    /**
     * @brief Compare two ZDA messages for equality.
     *
     * Equality delegates to Message0183::operator==.
     *
     * @param other The ZDA message to compare with.
     * @return bool true if equal, false otherwise.
     */
    bool operator==(const ZDA& other) const noexcept;

private:
    double utcTime_{};
    unsigned int day_{};
    unsigned int month_{};
    unsigned int year_{};
    int localZoneHours_{};
    int localZoneMinutes_{};

    ZDA() = delete;

    ZDA(Message0183 baseMessage,
        double utcTime,
        unsigned int day,
        unsigned int month,
        unsigned int year,
        int localZoneHours,
        int localZoneMinutes
    ) noexcept;

    static std::unique_ptr<ZDA> create(std::unique_ptr<Message0183> baseMessage);
    static std::string composeRaw(const std::string& talkerId,
                                  double utcTime,
                                  unsigned int day,
                                  unsigned int month,
                                  unsigned int year,
                                  int localZoneHours,
                                  int localZoneMinutes);

    friend class Nmea0183Factory;
    friend class MessageRegistry;
};

} // namespace nmea0183
} // namespace nmealib
