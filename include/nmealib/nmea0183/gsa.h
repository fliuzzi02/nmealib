#pragma once

#include <nmealib/nmea0183.h>

#include <array>
#include <optional>

namespace nmealib {
namespace nmea0183 {

/**
 * @brief Exception thrown when a sentence is not a valid GSA sentence.
 *
 * This exception is raised during parsing when the sentence type is not "GSA"
 * or the payload does not conform to the expected GSA format.
 */
class NotGSAException : public NmeaException {
public:
    /**
     * @brief Construct a NotGSAException with context and optional details.
     *
     * @param context  A short string identifying where the exception was thrown.
     * @param details  Optional additional information about the failure.
     */
    explicit NotGSAException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "The sentence is not a GSA sentence", details) {}
};

/**
 * @brief Represents a parsed NMEA 0183 GSA (GNSS DOP and Active Satellites) sentence.
 *
 * GSA contains selection mode, fix dimension, active satellite IDs, and DOP metrics.
 * The message may optionally include a GNSS system ID as a trailing field.
 *
 * Sentence format:
 * @code
 * $--GSA,a,x,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,p.p,h.h,v.v[,s]*hh<CR><LF>
 * @endcode
 */
class GSA : public Message0183 {
public:
    /**
     * @brief Construct a GSA message from individual field values.
     *
     * @param talkerId        Two-character talker identifier.
     * @param selectionMode   Selection mode ('M' manual or 'A' automatic).
     * @param mode            Fix dimension mode (1=no fix, 2=2D, 3=3D).
     * @param satelliteIds    Up to 12 satellite identifiers (0 for empty slots).
     * @param pdop            Position dilution of precision.
     * @param hdop            Horizontal dilution of precision.
     * @param vdop            Vertical dilution of precision.
     * @param systemId        Optional GNSS system ID.
     */
    GSA(std::string talkerId,
        char selectionMode,
        unsigned int mode,
        std::array<unsigned int, 12> satelliteIds,
        double pdop,
        double hdop,
        double vdop,
        std::optional<unsigned int> systemId = std::nullopt
    );

    GSA(const GSA&) = default;
    GSA& operator=(const GSA&) = default;
    GSA(GSA&&) noexcept = default;
    GSA& operator=(GSA&&) noexcept = default;

    ~GSA() override = default;

    /**
     * @brief Create a polymorphic copy of this GSA message.
     *
     * @return std::unique_ptr<nmealib::Message> A heap-allocated copy of this object.
     */
    std::unique_ptr<nmealib::Message> clone() const override;

    /** @brief Get selection mode indicator. */
    char getSelectionMode() const noexcept;
    /** @brief Get fix dimension mode. */
    unsigned int getMode() const noexcept;
    /** @brief Get all 12 satellite ID slots (0 for empty). */
    std::array<unsigned int, 12> getSatelliteIds() const noexcept;
    /**
     * @brief Get one satellite ID by slot index.
     *
     * @param index Zero-based index in [0, 11].
     * @return unsigned int Satellite ID, or 0 when index is out of range.
     */
    unsigned int getSatelliteId(size_t index) const noexcept;
    /** @brief Get PDOP value. */
    double getPdop() const noexcept;
    /** @brief Get HDOP value. */
    double getHdop() const noexcept;
    /** @brief Get VDOP value. */
    double getVdop() const noexcept;
    /** @brief Return whether an optional system ID is present. */
    bool hasSystemId() const noexcept;
    /** @brief Get optional GNSS system ID. */
    std::optional<unsigned int> getSystemId() const noexcept;

    /**
     * @brief Return a human-readable string representation of this message.
     *
     * @param verbose When true, returns a multi-line detailed representation.
     * @return std::string Formatted string content.
     */
    std::string getStringContent(bool verbose) const noexcept override;

    /**
     * @brief Compare two GSA messages for equality.
     *
     * Equality delegates to Message0183::operator==.
     *
     * @param other The GSA message to compare with.
     * @return bool true if equal, false otherwise.
     */
    bool operator==(const GSA& other) const noexcept;

    /**
     * @brief Compare the parsed content of two GSA messages.
     *
     * Content comparison delegates to Message0183::hasEqualContent.
     *
     * @param other The GSA message to compare with.
     * @return bool true if content is equal, false otherwise.
     */
    bool hasEqualContent(const GSA& other) const noexcept;

private:
    char selectionMode_{};
    unsigned int mode_{};
    std::array<unsigned int, 12> satelliteIds_{};
    double pdop_{};
    double hdop_{};
    double vdop_{};
    std::optional<unsigned int> systemId_{};

    GSA() = default;

    GSA(Message0183 baseMessage,
        char selectionMode,
        unsigned int mode,
        std::array<unsigned int, 12> satelliteIds,
        double pdop,
        double hdop,
        double vdop,
        std::optional<unsigned int> systemId
    ) noexcept;

    static std::unique_ptr<GSA> create(std::unique_ptr<Message0183> baseMessage);
    static std::string composeRaw(const std::string& talkerId,
                                  char selectionMode,
                                  unsigned int mode,
                                  std::array<unsigned int, 12> satelliteIds,
                                  double pdop,
                                  double hdop,
                                  double vdop,
                                  std::optional<unsigned int> systemId);

    friend class Nmea0183Factory;
    friend class MessageRegistry;
};

} // namespace nmea0183
} // namespace nmealib
