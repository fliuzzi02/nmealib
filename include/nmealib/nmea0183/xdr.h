#pragma once

#include <nmealib/nmea0183.h>

#include <cstddef>
#include <vector>

namespace nmealib {
namespace nmea0183 {

/**
 * @brief Exception thrown when a sentence is not a valid XDR sentence.
 *
 * This exception is raised during parsing when the sentence type is not "XDR"
 * or the payload does not conform to the expected XDR format.
 */
class NotXDRException : public NmeaException {
public:
    /**
     * @brief Construct a NotXDRException with context and optional details.
     *
     * @param context  A short string identifying where the exception was thrown.
     * @param details  Optional additional information about the failure.
     */
    explicit NotXDRException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "The sentence is not an XDR sentence", details) {}
};

/**
 * @brief Represents a parsed NMEA 0183 XDR (Transducer Measurement) sentence.
 *
 * XDR carries one or more transducer measurement quadruplets:
 * @code
 * $--XDR,a,x.x,a,c--c, ..... *hh<CR><LF>
 * @endcode
 *
 * Each quadruplet is composed of:
 *  - transducer type (single character),
 *  - measurement value,
 *  - units code,
 *  - transducer name.
 */
class XDR : public Message0183 {
public:
    /**
     * @brief One transducer measurement quadruplet in an XDR sentence.
     */
    struct TransducerMeasurement {
        /** @brief Single-character transducer type identifier. */
        char transducerType{};
        /** @brief Numeric measurement value. */
        double measurement{};
        /** @brief Single-character units code. */
        char units{};
        /** @brief Free-form transducer name/label. */
        std::string name;
    };

    /**
     * @brief Construct an XDR message from field values.
     *
     * @param talkerId    Two-character talker identifier.
     * @param transducers Sequence of one or more transducer measurements.
     */
    XDR(std::string talkerId, std::vector<TransducerMeasurement> transducers);

    XDR(const XDR&) = default;
    XDR& operator=(const XDR&) = default;
    XDR(XDR&&) noexcept = default;
    XDR& operator=(XDR&&) noexcept = default;

    ~XDR() override = default;

    /**
     * @brief Create a polymorphic copy of this XDR message.
     *
     * @return std::unique_ptr<nmealib::Message> A heap-allocated copy of this object.
     */
    std::unique_ptr<nmealib::Message> clone() const override;

    /**
     * @brief Get all transducer measurements in this sentence.
     *
     * @return const std::vector<TransducerMeasurement>& Ordered transducer list.
     */
    const std::vector<TransducerMeasurement>& getTransducers() const noexcept;

    /**
     * @brief Get the number of transducer measurements in this sentence.
     *
     * @return std::size_t Number of transducer quadruplets.
     */
    std::size_t getTransducerCount() const noexcept;

    /**
     * @brief Get a transducer measurement by index.
     *
     * @param index Zero-based transducer index.
     * @return const TransducerMeasurement* Pointer to transducer data, or nullptr if out of range.
     */
    const TransducerMeasurement* getTransducer(std::size_t index) const noexcept;

    /**
     * @brief Return a human-readable string representation of this message.
     *
     * @param verbose When true, returns a multi-line detailed representation.
     * @return std::string Formatted string content.
     */
    std::string getStringContent(bool verbose) const noexcept override;

    /**
     * @brief Compare two XDR messages for equality.
     *
     * Equality delegates to Message0183::operator==.
     *
     * @param other The XDR message to compare with.
     * @return bool true if equal, false otherwise.
     */
    bool operator==(const XDR& other) const noexcept;

private:
    std::vector<TransducerMeasurement> transducers_;

    XDR() = delete;

    XDR(Message0183 baseMessage, std::vector<TransducerMeasurement> transducers) noexcept;

    static std::unique_ptr<XDR> create(std::unique_ptr<Message0183> baseMessage);
    static std::string composeRaw(const std::string& talkerId,
                                  const std::vector<TransducerMeasurement>& transducers);

    friend class Nmea0183Factory;
    friend class MessageRegistry;
};

} // namespace nmea0183
} // namespace nmealib
