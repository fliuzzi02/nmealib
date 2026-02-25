#pragma once

#include <nmealib/nmea0183.hpp>

namespace nmealib {
namespace nmea0183 {

class NotDBTException : public NmeaException {
public:
    explicit NotDBTException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "The sentence is not a DBT sentence", details) {}
};

class DBT : public Message0183 {
public:
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

    std::unique_ptr<nmealib::Message> clone() const override;

    double getDepthFeet() const noexcept;
    char getFeetUnit() const noexcept;
    double getDepthMeters() const noexcept;
    char getMetersUnit() const noexcept;
    double getDepthFathoms() const noexcept;
    char getFathomsUnit() const noexcept;

    std::string getStringContent(bool verbose) const noexcept override;
    bool operator==(const DBT& other) const noexcept;
    bool hasEqualContent(const DBT& other) const noexcept;

private:
    double depthFeet_;
    char feetUnit_;
    double depthMeters_;
    char metersUnit_;
    double depthFathoms_;
    char fathomsUnit_;

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
    static std::string composeRaw(std::string talkerId,
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
