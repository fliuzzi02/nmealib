#pragma once

#include <string>
#include <chrono>
#include <memory>

namespace nmealib {

/**
 * @brief Defines a base class for NMEA messages, encapsulating common properties and behaviors.
 * This class serves as an abstract interface for specific NMEA message types (e.g., NMEA0183, NMEA2000),
 * 
 */
class Message {
public:
    /**
     * @brief Defines the type of NMEA message, allowing for differentiation between various standards.
     * 
     */
    enum class Type { Unknown, NMEA0183, NMEA2000 };

    /**
     * @brief Converts a Message::Type enum value to its string representation.
     *
     * @param t The message type to convert.
     * @return std::string A human-readable string for the given type (e.g., "NMEA0183", "Unknown").
     */
    static std::string typeToString(Type t) {
        switch (t) {
            case Type::Unknown: return "Unknown";
            case Type::NMEA0183: return "NMEA0183";
            case Type::NMEA2000: return "NMEA2000";
            default: return "InvalidType";
        }
    }

    using TimePoint = std::chrono::system_clock::time_point;

    Message() = default;
    explicit Message(std::string raw,
                     Type type = Type::Unknown,
                     TimePoint ts = std::chrono::system_clock::now()) noexcept
        : rawData_(std::move(raw)), type_(type), timestamp_(ts) {}

    virtual ~Message() = default;
    
    Message(const Message&) = default;
    Message& operator=(const Message&) = default;
    Message(Message&&) noexcept = default;
    Message& operator=(Message&&) noexcept = default;

    /**
     * @brief Returns the message type.
     *
     * @return Type The type of this NMEA message.
     */
    Type getType() const noexcept { return type_; }

    /**
     * @brief Returns the raw, unmodified data string as received.
     *
     * @return const std::string& A const reference to the raw data string.
     */
    const std::string& getRawData() const noexcept { return rawData_; }

    /**
     * @brief Returns the timestamp associated with this message.
     *
     * @return TimePoint The time point at which this message was created or received.
     */
    TimePoint getTimestamp() const noexcept { return timestamp_; }

    /**
     * @brief Creates a polymorphic deep copy of this message.
     *
     * @return std::unique_ptr<Message> A unique pointer to the cloned message.
     */
    virtual std::unique_ptr<Message> clone() const = 0;

    /**
     * @brief Produces the wire-format representation of this message.
     *
     * @return std::string The serialized string suitable for transmission.
     */
    virtual std::string serialize() const = 0;

    /**
     * @brief Validates the message contents (e.g., checksum, length).
     *
     * @return true  If the message is valid.
     * @return false If the message fails validation.
     */
    virtual bool validate() const = 0;

    /**
     * @brief Compares two Message objects for equality based on their content.
     *
     * @param other The other Message to compare against.
     * @return true  If type, raw data, and timestamp are all equal.
     * @return false Otherwise.
     */
    bool operator==(const Message& other) const noexcept {
        return type_ == other.type_ &&
               rawData_ == other.rawData_ &&
               timestamp_ == other.timestamp_;
    }

protected:
    std::string rawData_;
    Type type_{Type::Unknown};
    TimePoint timestamp_{};

    /**
     * @brief Sets the message type.
     *
     * @param t The new message type.
     */
    void setType(Type t) noexcept { type_ = t; }

    /**
     * @brief Sets the raw data string.
     *
     * @param r The new raw data string.
     */
    void setRaw(std::string r) noexcept { rawData_ = std::move(r); }

    /**
     * @brief Sets the message timestamp.
     *
     * @param ts The new timestamp.
     */
    void setTimestamp(TimePoint ts) noexcept { timestamp_ = ts; }
};

} // namespace nmealib
