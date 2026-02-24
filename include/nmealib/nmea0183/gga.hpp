#pragma once

#include "nmea0183.hpp"

namespace nmealib {
namespace nmea0183 {

class NotGGAException : public NmeaException {
public:
    explicit NotGGAException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "The sentence is not a GGA sentence", details) {}
};

class GGA : public Message0183 {
public:
    // Public constructor from parameters
    GGA(std::string talkerId,
        double timestamp,
        double latitude,
        char latitudeDirection,
        double longitude,
        char longitudeDirection,
        unsigned int gpsQuality,
        unsigned int satellites,
        double hdop,
        double altitude,
        char altitudeUnits,
        double geoidalSeparation,
        char geoidalSeparationUnits,
        double dgpsAge,
        std::string dgpsReferenceStationId
    );

    // Accessory constructors
    GGA(const GGA&) = default;
    GGA& operator=(const GGA&) = default;
    GGA(GGA&&) noexcept = default;
    GGA& operator=(GGA&&) noexcept = default;

    // Destructor
    ~GGA() override = default;

    // Polymorphic copy
    std::unique_ptr<nmealib::Message> clone() const override;

    // GGA-specific getters
    double getTimestamp() const noexcept { return timestamp_; }
    double getLatitude() const noexcept { return latitude_; }
    char getLatitudeDirection() const noexcept { return latitudeDirection_; }
    double getLongitude() const noexcept { return longitude_; }
    char getLongitudeDirection() const noexcept { return longitudeDirection_; }
    unsigned int getGpsQuality() const noexcept { return gpsQuality_; }
    unsigned int getSatellites() const noexcept { return satellites_; }
    double getHdop() const noexcept { return hdop_; }
    double getAltitude() const noexcept { return altitude_; }
    char getAltitudeUnits() const noexcept { return altitudeUnits_; }
    double getGeoidalSeparation() const noexcept { return geoidalSeparation_; }
    char getGeoidalSeparationUnits() const noexcept { return geoidalSeparationUnits_; }
    double getDgpsAge() const noexcept { return dgpsAge_; }
    std::string getDgpsReferenceStationId() const noexcept { return dgpsReferenceStationId_; }

    // Overridden methods
    std::string getStringContent(bool verbose) const noexcept override;
    bool operator==(const GGA& other) const noexcept {
        return Message0183::operator==(other);
    }

    bool hasEqualContent(const GGA& other) const noexcept {
        return Message0183::hasEqualContent(other);
    }

private:
    double timestamp_;
    double latitude_;
    char latitudeDirection_;
    double longitude_;
    char longitudeDirection_;
    unsigned int gpsQuality_;
    unsigned int satellites_;
    double hdop_;
    double altitude_;
    char altitudeUnits_;
    double geoidalSeparation_;
    char geoidalSeparationUnits_;
    double dgpsAge_;
    std::string dgpsReferenceStationId_;

    GGA() = default;

    // Private constructor used by the factory method
    GGA(Message0183 baseMessage,
        double timestamp,
        double latitude,
        char latitudeDirection,
        double longitude,
        char longitudeDirection,
        unsigned int gpsQuality,
        unsigned int satellites,
        double hdop,
        double altitude,
        char altitudeUnits,
        double geoidalSeparation,
        char geoidalSeparationUnits,
        double dgpsAge,
        std::string dgpsReferenceStationId
    ) noexcept;

    // Private internal factory
    static std::unique_ptr<GGA> create(std::unique_ptr<Message0183> baseMessage);
    static std::string composeRaw(std::string talkerId,
                                  double timestamp,
                                  double latitude,
                                  char latitudeDirection,
                                  double longitude,
                                  char longitudeDirection,
                                  unsigned int gpsQuality,
                                  unsigned int satellites,
                                  double hdop,
                                  double altitude,
                                  char altitudeUnits,
                                  double geoidalSeparation,
                                  char geoidalSeparationUnits,
                                  double dgpsAge,
                                  std::string dgpsReferenceStationId);

    friend class Nmea0183Factory;
};

} // namespace nmea0183
} // namespace nmealib
