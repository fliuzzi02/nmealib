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

    // Getters
    Type getType() const noexcept { return type_; }
    const std::string& getRawData() const noexcept { return rawData_; }
    TimePoint getTimestamp() const noexcept { return timestamp_; }

    // Polymorphic copy
    virtual std::unique_ptr<Message> clone() const = 0;

    // Produce wire-format representation
    virtual std::string serialize() const = 0;

    // Validate message contents (checksum, length, etc.)
    virtual bool validate() const = 0;

protected:
    std::string rawData_;
    Type type_{Type::Unknown};
    TimePoint timestamp_{};

    void setType(Type t) noexcept { type_ = t; }
    void setRaw(std::string r) noexcept { rawData_ = std::move(r); }
    void setTimestamp(TimePoint ts) noexcept { timestamp_ = ts; }
};

} // namespace nmealib