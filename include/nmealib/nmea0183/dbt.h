#pragma once

#include <nmealib/nmea0183.h>

namespace nmealib {
namespace nmea0183 {

/**
 * @brief Exception thrown when a sentence is not a valid DBT sentence.
 *
 * This exception is raised during parsing when the sentence type is not "DBT"
 * or the payload does not conform to the expected DBT format.
 */
class NotDBTException : public NmeaException {
public:
    /**
     * @brief Construct a NotDBTException with context and optional details.
     *
     * @param context  A short string identifying where the exception was thrown.
     * @param details  Optional additional information about the failure.
     */
    explicit NotDBTException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "The sentence is not a DBT sentence", details) {}
};

/**
 * @brief Represents a parsed NMEA 0183 DBT (Depth Below Transducer) sentence.
 *
 * The DBT sentence reports measured depth in three units:
 * feet, meters, and fathoms.
 *
 * Sentence format:
 * @code
 * $--DBT,x.x,f,x.x,M,x.x,F*hh<CR><LF>
 * @endcode
 */
class DBT : public Message0183 {
public:
    /**
     * @brief Construct a DBT message from individual field values.
     *
     * @param talkerId      Two-character talker identifier.
     * @param depthFeet     Depth value in feet.
     * @param feetUnit      Unit indicator for feet field (typically 'f').
     * @param depthMeters   Depth value in meters.
     * @param metersUnit    Unit indicator for meter field (typically 'M').
     * @param depthFathoms  Depth value in fathoms.
     * @param fathomsUnit   Unit indicator for fathom field (typically 'F').
     */
    DBT(std::string talkerId,
        double depthFeet,
        char feetUnit,
        double depthMeters,
        char metersUnit,
        double depthFathoms,
        char fathomsUnit
    );

    DBT(const DBT&) = default;
    DBT& operator=(const DBT&) = default;
    DBT(DBT&&) noexcept = default;
    DBT& operator=(DBT&&) noexcept = default;

    ~DBT() override = default;

    /**
     * @brief Create a polymorphic copy of this DBT message.
     *
     * @return std::unique_ptr<nmealib::Message> A heap-allocated copy of this object.
     */
    std::unique_ptr<nmealib::Message> clone() const override;

    /**
     * @brief Get the depth in feet.
     *
     * @return double Depth value in feet.
     */
    double getDepthFeet() const noexcept;

    /**
     * @brief Get the feet unit indicator.
     *
     * @return char Unit indicator for feet (typically 'f').
     */
    char getFeetUnit() const noexcept;

    /**
     * @brief Get the depth in meters.
     *
     * @return double Depth value in meters.
     */
    double getDepthMeters() const noexcept;

    /**
     * @brief Get the meters unit indicator.
     *
     * @return char Unit indicator for meters (typically 'M').
     */
    char getMetersUnit() const noexcept;

    /**
     * @brief Get the depth in fathoms.
     *
     * @return double Depth value in fathoms.
     */
    double getDepthFathoms() const noexcept;

    /**
     * @brief Get the fathoms unit indicator.
     *
     * @return char Unit indicator for fathoms (typically 'F').
     */
    char getFathomsUnit() const noexcept;

    /**
     * @brief Return a human-readable string representation of this message.
     *
     * @param verbose When true, returns a multi-line detailed representation.
     * @return std::string Formatted string content.
     */
    std::string getStringContent(bool verbose) const noexcept override;

    /**
     * @brief Compare two DBT messages for equality.
     *
     * Equality delegates to Message0183::operator==.
     *
     * @param other The DBT message to compare with.
     * @return bool true if equal, false otherwise.
     */
    bool operator==(const DBT& other) const noexcept;

    /**
     * @brief Compare the parsed content of two DBT messages.
     *
     * Content comparison delegates to Message0183::hasEqualContent.
     *
     * @param other The DBT message to compare with.
     * @return bool true if content is equal, false otherwise.
     */
    bool hasEqualContent(const DBT& other) const noexcept;

private:
    double depthFeet_{};
    char feetUnit_{};
    double depthMeters_{};
    char metersUnit_{};
    double depthFathoms_{};
    char fathomsUnit_{};

    DBT() = default;

    DBT(Message0183 baseMessage,
        double depthFeet,
        char feetUnit,
        double depthMeters,
        char metersUnit,
        double depthFathoms,
        char fathomsUnit
    ) noexcept;

    static std::unique_ptr<DBT> create(std::unique_ptr<Message0183> baseMessage);
    static std::string composeRaw(const std::string& talkerId,
                                  double depthFeet,
                                  char feetUnit,
                                  double depthMeters,
                                  char metersUnit,
                                  double depthFathoms,
                                  char fathomsUnit);

    friend class Nmea0183Factory;
    friend class MessageRegistry;
};

} // namespace nmea0183
} // namespace nmealib
