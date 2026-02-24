#pragma once

#include <nmealib/nmea0183.hpp>

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
    double getTimestamp() const noexcept;
    double getLatitude() const noexcept;
    char getLatitudeDirection() const noexcept;
    double getLongitude() const noexcept;
    char getLongitudeDirection() const noexcept;
    unsigned int getGpsQuality() const noexcept;
    unsigned int getSatellites() const noexcept;
    double getHdop() const noexcept;
    double getAltitude() const noexcept;
    char getAltitudeUnits() const noexcept;
    double getGeoidalSeparation() const noexcept;
    char getGeoidalSeparationUnits() const noexcept;
    double getDgpsAge() const noexcept;
    std::string getDgpsReferenceStationId() const noexcept;

    // Overridden methods
    std::string getStringContent(bool verbose) const noexcept override;
    bool operator==(const GGA& other) const noexcept;

    bool hasEqualContent(const GGA& other) const noexcept;

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
    friend class MessageRegistry;
};

} // namespace nmea0183
} // namespace nmealib
