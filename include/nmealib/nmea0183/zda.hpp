#pragma once

#include "nmea0183.hpp"

namespace nmealib {
namespace nmea0183 {

class NotZDAException : public NmeaException {
public:
    explicit NotZDAException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "The sentence is not a ZDA sentence", details) {}
};

class ZDA : public Message0183 {
public:
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

    std::unique_ptr<nmealib::Message> clone() const override;

    double getUtcTime() const noexcept { return utcTime_; }
    unsigned int getDay() const noexcept { return day_; }
    unsigned int getMonth() const noexcept { return month_; }
    unsigned int getYear() const noexcept { return year_; }
    int getLocalZoneHours() const noexcept { return localZoneHours_; }
    int getLocalZoneMinutes() const noexcept { return localZoneMinutes_; }

    std::string getStringContent(bool verbose) const noexcept override;
    bool operator==(const ZDA& other) const noexcept {
        return Message0183::operator==(other);
    }

    bool hasEqualContent(const ZDA& other) const noexcept {
        return Message0183::hasEqualContent(other);
    }

private:
    double utcTime_;
    unsigned int day_;
    unsigned int month_;
    unsigned int year_;
    int localZoneHours_;
    int localZoneMinutes_;

    ZDA() = default;

    ZDA(Message0183 baseMessage,
        double utcTime,
        unsigned int day,
        unsigned int month,
        unsigned int year,
        int localZoneHours,
        int localZoneMinutes
    ) noexcept;

    static std::unique_ptr<ZDA> create(std::unique_ptr<Message0183> baseMessage);
    static std::string composeRaw(std::string talkerId,
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
