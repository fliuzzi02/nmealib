#pragma once

#include <nmealib/nmea0183.h>

namespace nmealib {
namespace nmea0183 {

class NotRMAException : public NmeaException {
public:
    explicit NotRMAException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "The sentence is not an RMA sentence", details) {}
};

/**
 * @brief Represents a parsed NMEA 0183 RMA (Recommended Minimum Navigation Information) sentence.
 *
 * Sentence format:
 * @code
 * $--RMA,A,llll.ll,a,yyyyy.yy,a,x.x,x.x,x.x,x.x,x.x,a*hh<CR><LF>
 * @endcode
 */
class RMA : public Message0183 {
public:
    RMA(std::string talkerId,
        char status,
        double latitude,
        char latitudeDirection,
        double longitude,
        char longitudeDirection,
        double timeDifferenceA,
        double timeDifferenceB,
        double speedOverGround,
        double trackMadeGood,
        double magneticVariation,
        char variationDirection);

    RMA(const RMA&) = default;
    RMA& operator=(const RMA&) = default;
    RMA(RMA&&) noexcept = default;
    RMA& operator=(RMA&&) noexcept = default;

    ~RMA() override = default;

    std::unique_ptr<nmealib::Message> clone() const override;

    char getStatus() const noexcept;
    double getLatitude() const noexcept;
    char getLatitudeDirection() const noexcept;
    double getLongitude() const noexcept;
    char getLongitudeDirection() const noexcept;
    double getTimeDifferenceA() const noexcept;
    double getTimeDifferenceB() const noexcept;
    double getSpeedOverGround() const noexcept;
    double getTrackMadeGood() const noexcept;
    double getMagneticVariation() const noexcept;
    char getVariationDirection() const noexcept;

    std::string getStringContent(bool verbose) const noexcept override;
    bool operator==(const RMA& other) const noexcept;

private:
    char status_{};
    double latitude_{};
    char latitudeDirection_{};
    double longitude_{};
    char longitudeDirection_{};
    double timeDifferenceA_{};
    double timeDifferenceB_{};
    double speedOverGround_{};
    double trackMadeGood_{};
    double magneticVariation_{};
    char variationDirection_{};

    RMA() = delete;

    RMA(Message0183 baseMessage,
        char status,
        double latitude,
        char latitudeDirection,
        double longitude,
        char longitudeDirection,
        double timeDifferenceA,
        double timeDifferenceB,
        double speedOverGround,
        double trackMadeGood,
        double magneticVariation,
        char variationDirection) noexcept;

    static std::unique_ptr<RMA> create(std::unique_ptr<Message0183> baseMessage);
    static std::string composeRaw(const std::string& talkerId,
                                  char status,
                                  double latitude,
                                  char latitudeDirection,
                                  double longitude,
                                  char longitudeDirection,
                                  double timeDifferenceA,
                                  double timeDifferenceB,
                                  double speedOverGround,
                                  double trackMadeGood,
                                  double magneticVariation,
                                  char variationDirection);

    friend class Nmea0183Factory;
    friend class MessageRegistry;
};

} // namespace nmea0183
} // namespace nmealib
