#pragma once

#include <nmealib/nmea0183.h>

namespace nmealib {
namespace nmea0183 {

/**
 * @brief Exception thrown when a sentence is not a valid DPT sentence.
 *
 * This exception is raised during parsing when the sentence type is not "DPT"
 * or the payload does not conform to the expected DPT format.
 */
class NotDPTException : public NmeaException {
public:
    /**
     * @brief Construct a NotDPTException with context and optional details.
     *
     * @param context  A short string identifying where the exception was thrown.
     * @param details  Optional additional information about the failure.
     */
    explicit NotDPTException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "The sentence is not a DPT sentence", details) {}
};

/**
 * @brief Represents a parsed NMEA 0183 DPT (Depth of Water) sentence.
 *
 * The DPT sentence reports water depth relative to the transducer, the
 * transducer offset, and optionally the maximum range scale in use.
 *
 * Sentence format:
 * @code
 * $--DPT,x.x,x.x,x.x*hh<CR><LF>
 * @endcode
 */
class DPT : public Message0183 {
public:
    /**
     * @brief Construct a DPT message from individual field values.
     *
     * @param talkerId           Two-character talker identifier.
     * @param waterDepthMeters   Depth relative to transducer, meters.
     * @param transducerOffset   Transducer offset, meters.
     * @param maxRangeScale      Maximum range scale, meters.
     */
    DPT(std::string talkerId,
        double waterDepthMeters,
        double transducerOffset,
        double maxRangeScale
    );

    DPT(const DPT&) = default;
    DPT& operator=(const DPT&) = default;
    DPT(DPT&&) noexcept = default;
    DPT& operator=(DPT&&) noexcept = default;

    ~DPT() override = default;

    /**
     * @brief Create a polymorphic copy of this DPT message.
     *
     * @return std::unique_ptr<nmealib::Message> A heap-allocated copy of this object.
     */
    std::unique_ptr<nmealib::Message> clone() const override;

    /** @brief Get water depth in meters. */
    double getWaterDepthMeters() const noexcept;
    /** @brief Get transducer offset in meters. */
    double getTransducerOffset() const noexcept;
    /** @brief Get maximum range scale in meters. */
    double getMaxRangeScale() const noexcept;

    /**
     * @brief Return a human-readable string representation of this message.
     *
     * @param verbose When true, returns a multi-line detailed representation.
     * @return std::string Formatted string content.
     */
    std::string getStringContent(bool verbose) const noexcept override;

    /**
     * @brief Compare two DPT messages for equality.
     *
     * Equality delegates to Message0183::operator==.
     *
     * @param other The DPT message to compare with.
     * @return bool true if equal, false otherwise.
     */
    bool operator==(const DPT& other) const noexcept;

private:
    double waterDepthMeters_{};
    double transducerOffset_{};
    double maxRangeScale_{};

    DPT() = delete;

    DPT(Message0183 baseMessage,
        double waterDepthMeters,
        double transducerOffset,
        double maxRangeScale
    ) noexcept;

    static std::unique_ptr<DPT> create(std::unique_ptr<Message0183> baseMessage);
    static std::string composeRaw(const std::string& talkerId,
                                  double waterDepthMeters,
                                  double transducerOffset,
                                  double maxRangeScale);

    friend class Nmea0183Factory;
    friend class MessageRegistry;
};

} // namespace nmea0183
} // namespace nmealib
