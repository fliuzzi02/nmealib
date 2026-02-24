#include "nmealib/message.hpp"

namespace nmealib {

std::string Message::typeToString(Type t) {
    switch (t) {
        case Type::Unknown: return "Unknown";
        case Type::NMEA0183: return "NMEA0183";
        case Type::NMEA2000: return "NMEA2000";
        default: return "InvalidType";
    }
}

Message::Message() = default;

Message::Message(std::string raw, Type type, TimePoint ts) noexcept
    : rawData_(std::move(raw)), type_(type), timestamp_(ts) {}

Message::~Message() = default;

Message::Type Message::getType() const noexcept {
    return type_;
}

const std::string& Message::getRawData() const noexcept {
    return rawData_;
}

Message::TimePoint Message::getTimestamp() const noexcept {
    return timestamp_;
}

bool Message::operator==(const Message& other) const noexcept {
    return type_ == other.type_ &&
           rawData_ == other.rawData_ &&
           timestamp_ == other.timestamp_;
}

void Message::setType(Type t) noexcept {
    type_ = t;
}

void Message::setRaw(std::string r) noexcept {
    rawData_ = std::move(r);
}

void Message::setTimestamp(TimePoint ts) noexcept {
    timestamp_ = ts;
}

} // namespace nmealib
