#pragma once

#include "nmea0183.hpp"

namespace nmealib {
namespace nmea0183 {

class NotMWVException : public NmeaException {
public:
    explicit NotMWVException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "The sentence is not an MWV sentence", details) {}
};

class MWV : public Message0183 {
public:
    MWV(std::string talkerId,
        double windAngle,
        char reference,
        double windSpeed,
        char windSpeedUnits,
        char status
    );

    MWV(const MWV&) = default;
    MWV& operator=(const MWV&) = default;
    MWV(MWV&&) noexcept = default;
    MWV& operator=(MWV&&) noexcept = default;

    ~MWV() override = default;

    std::unique_ptr<nmealib::Message> clone() const override;

    double getWindAngle() const noexcept;
    char getReference() const noexcept;
    double getWindSpeed() const noexcept;
    char getWindSpeedUnits() const noexcept;
    char getStatus() const noexcept;

    std::string getStringContent(bool verbose) const noexcept override;
    bool operator==(const MWV& other) const noexcept;
    bool hasEqualContent(const MWV& other) const noexcept;

private:
    double windAngle_;
    char reference_;
    double windSpeed_;
    char windSpeedUnits_;
    char status_;

    MWV() = default;

    MWV(Message0183 baseMessage,
        double windAngle,
        char reference,
        double windSpeed,
        char windSpeedUnits,
        char status
    ) noexcept;

    static std::unique_ptr<MWV> create(std::unique_ptr<Message0183> baseMessage);
    static std::string composeRaw(std::string talkerId,
                                  double windAngle,
                                  char reference,
                                  double windSpeed,
                                  char windSpeedUnits,
                                  char status);

    friend class Nmea0183Factory;
    friend class MessageRegistry;
};

} // namespace nmea0183
} // namespace nmealib
