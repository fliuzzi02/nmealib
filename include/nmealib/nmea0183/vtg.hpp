#pragma once

#include "nmea0183.hpp"

#include <optional>

namespace nmealib {
namespace nmea0183 {

class NotVTGException : public NmeaException {
public:
    explicit NotVTGException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "The sentence is not a VTG sentence", details) {}
};

class VTG : public Message0183 {
public:
    VTG(std::string talkerId,
        double courseOverGroundTrue,
        double courseOverGroundMagnetic,
        double speedOverGroundKnots,
        double speedOverGroundKph,
        std::optional<char> faaModeIndicator = std::nullopt,
        bool legacyFormat = false
    );

    VTG(const VTG&) = default;
    VTG& operator=(const VTG&) = default;
    VTG(VTG&&) noexcept = default;
    VTG& operator=(VTG&&) noexcept = default;

    ~VTG() override = default;

    std::unique_ptr<nmealib::Message> clone() const override;

    double getCourseOverGroundTrue() const noexcept;
    char getCourseOverGroundTrueType() const noexcept;
    double getCourseOverGroundMagnetic() const noexcept;
    char getCourseOverGroundMagneticType() const noexcept;
    double getSpeedOverGroundKnots() const noexcept;
    char getSpeedOverGroundKnotsType() const noexcept;
    double getSpeedOverGroundKph() const noexcept;
    char getSpeedOverGroundKphType() const noexcept;
    bool hasFaaModeIndicator() const noexcept;
    std::optional<char> getFaaModeIndicator() const noexcept;
    bool isLegacyFormat() const noexcept;

    std::string getStringContent(bool verbose) const noexcept override;
    bool operator==(const VTG& other) const noexcept;
    bool hasEqualContent(const VTG& other) const noexcept;

private:
    double courseOverGroundTrue_;
    char courseOverGroundTrueType_;
    double courseOverGroundMagnetic_;
    char courseOverGroundMagneticType_;
    double speedOverGroundKnots_;
    char speedOverGroundKnotsType_;
    double speedOverGroundKph_;
    char speedOverGroundKphType_;
    std::optional<char> faaModeIndicator_;
    bool legacyFormat_;

    VTG() = default;

    VTG(Message0183 baseMessage,
        double courseOverGroundTrue,
        char courseOverGroundTrueType,
        double courseOverGroundMagnetic,
        char courseOverGroundMagneticType,
        double speedOverGroundKnots,
        char speedOverGroundKnotsType,
        double speedOverGroundKph,
        char speedOverGroundKphType,
        std::optional<char> faaModeIndicator,
        bool legacyFormat
    ) noexcept;

    static std::unique_ptr<VTG> create(std::unique_ptr<Message0183> baseMessage);
    static std::string composeRaw(std::string talkerId,
                                  double courseOverGroundTrue,
                                  double courseOverGroundMagnetic,
                                  double speedOverGroundKnots,
                                  double speedOverGroundKph,
                                  std::optional<char> faaModeIndicator,
                                  bool legacyFormat);

    friend class Nmea0183Factory;
    friend class MessageRegistry;
};

} // namespace nmea0183
} // namespace nmealib
