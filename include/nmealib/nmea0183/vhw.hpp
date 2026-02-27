#pragma once

#include <nmealib/nmea0183.hpp>

namespace nmealib {
namespace nmea0183 {

class NotVHWException : public NmeaException {
public:
    explicit NotVHWException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "The sentence is not a VHW sentence", details) {}
};

class VHW : public Message0183 {
public:
    VHW(std::string talkerId,
        double headingTrue,
        double headingMagnetic,
        double speedKnots,
        double speedKph
    );

    VHW(const VHW&) = default;
    VHW& operator=(const VHW&) = default;
    VHW(VHW&&) noexcept = default;
    VHW& operator=(VHW&&) noexcept = default;

    ~VHW() override = default;

    std::unique_ptr<nmealib::Message> clone() const override;

    double getHeadingTrue() const noexcept;
    char getHeadingTrueType() const noexcept;
    double getHeadingMagnetic() const noexcept;
    char getHeadingMagneticType() const noexcept;
    double getSpeedKnots() const noexcept;
    char getSpeedKnotsType() const noexcept;
    double getSpeedKph() const noexcept;
    char getSpeedKphType() const noexcept;

    std::string getStringContent(bool verbose) const noexcept override;
    bool operator==(const VHW& other) const noexcept;
    bool hasEqualContent(const VHW& other) const noexcept;

private:
    double headingTrue_{};
    char headingTrueType_{};
    double headingMagnetic_{};
    char headingMagneticType_{};
    double speedKnots_{};
    char speedKnotsType_{};
    double speedKph_{};
    char speedKphType_{};

    VHW() = default;

    VHW(Message0183 baseMessage,
        double headingTrue,
        char headingTrueType,
        double headingMagnetic,
        char headingMagneticType,
        double speedKnots,
        char speedKnotsType,
        double speedKph,
        char speedKphType
    ) noexcept;

    static std::unique_ptr<VHW> create(std::unique_ptr<Message0183> baseMessage);
    static std::string composeRaw(const std::string& talkerId,
                                  double headingTrue,
                                  double headingMagnetic,
                                  double speedKnots,
                                  double speedKph);

    friend class Nmea0183Factory;
    friend class MessageRegistry;
};

} // namespace nmea0183
} // namespace nmealib
