#pragma once

#include <nmealib/nmea0183.h>

#include <optional>

namespace nmealib {
namespace nmea0183 {

class NotVLWException : public NmeaException {
public:
    explicit NotVLWException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "The sentence is not a VLW sentence", details) {}
};

class VLW : public Message0183 {
public:
    VLW(std::string talkerId,
        double totalWaterDistanceNm,
        double waterDistanceSinceResetNm,
        std::optional<double> totalGroundDistanceNm = std::nullopt,
        std::optional<double> groundDistanceSinceResetNm = std::nullopt);

    VLW(const VLW&) = default;
    VLW& operator=(const VLW&) = default;
    VLW(VLW&&) noexcept = default;
    VLW& operator=(VLW&&) noexcept = default;

    ~VLW() override = default;

    std::unique_ptr<nmealib::Message> clone() const override;

    double getTotalWaterDistanceNm() const noexcept;
    char getTotalWaterDistanceUnit() const noexcept;
    double getWaterDistanceSinceResetNm() const noexcept;
    char getWaterDistanceSinceResetUnit() const noexcept;

    bool hasGroundDistanceData() const noexcept;
    double getTotalGroundDistanceNm() const noexcept;
    char getTotalGroundDistanceUnit() const noexcept;
    double getGroundDistanceSinceResetNm() const noexcept;
    char getGroundDistanceSinceResetUnit() const noexcept;

    std::string getStringContent(bool verbose) const noexcept override;
    bool operator==(const VLW& other) const noexcept;

private:
    double totalWaterDistanceNm_{};
    char totalWaterDistanceUnit_{};
    double waterDistanceSinceResetNm_{};
    char waterDistanceSinceResetUnit_{};
    bool hasGroundDistanceData_{};
    double totalGroundDistanceNm_{};
    char totalGroundDistanceUnit_{};
    double groundDistanceSinceResetNm_{};
    char groundDistanceSinceResetUnit_{};

    VLW() = delete;

    VLW(Message0183 baseMessage,
        double totalWaterDistanceNm,
        char totalWaterDistanceUnit,
        double waterDistanceSinceResetNm,
        char waterDistanceSinceResetUnit,
        bool hasGroundDistanceData,
        double totalGroundDistanceNm,
        char totalGroundDistanceUnit,
        double groundDistanceSinceResetNm,
        char groundDistanceSinceResetUnit) noexcept;

    static std::unique_ptr<VLW> create(std::unique_ptr<Message0183> baseMessage);
    static std::string composeRaw(const std::string& talkerId,
                                  double totalWaterDistanceNm,
                                  double waterDistanceSinceResetNm,
                                  std::optional<double> totalGroundDistanceNm,
                                  std::optional<double> groundDistanceSinceResetNm);

    friend class Nmea0183Factory;
    friend class MessageRegistry;
};

} // namespace nmea0183
} // namespace nmealib
