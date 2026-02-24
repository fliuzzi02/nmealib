#pragma once

#include "nmea0183.hpp"

namespace nmealib {
namespace nmea0183 {

class NotGLLException : public NmeaException {
public:
    explicit NotGLLException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "The sentence is not a GLL sentence", details) {}
};

class GLL : public Message0183 {
public:
    // Public constructor from parameters
    GLL(std::string talkerId,
        double latitude,
        char latitudeDirection,
        double longitude,
        char longitudeDirection,
        double timestamp,
        char status,
        char modeIndicator
    );

    // Accessory constructors
    GLL(const GLL&) = default;
    GLL& operator=(const GLL&) = default;
    GLL(GLL&&) noexcept = default;
    GLL& operator=(GLL&&) noexcept = default;

    // Destructor
    ~GLL() override = default;

    // Polymorphic copy
    std::unique_ptr<nmealib::Message> clone() const override;

    // GLL-specific getters
    double getLatitude() const noexcept;
    char getLatitudeDirection() const noexcept;
    double getLongitude() const noexcept;
    char getLongitudeDirection() const noexcept;
    double getTimestamp() const noexcept;
    char getStatus() const noexcept;
    char getModeIndicator() const noexcept;

    // Overridden methods
    std::string getStringContent(bool verbose) const noexcept override;
    bool operator==(const GLL& other) const noexcept;
    bool hasEqualContent(const GLL& other) const noexcept;

private:
    double latitude_;
    char latitudeDirection_;
    double longitude_;
    char longitudeDirection_;
    double timestamp_;
    char status_;
    char modeIndicator_;

    GLL() = default;

    // Private constructor used by the factory method
    GLL(Message0183 baseMessage,
        double latitude,
        char latitudeDirection,
        double longitude,
        char longitudeDirection,
        double timestamp,
        char status,
        char modeIndicator
    ) noexcept;

    // Private internal factory
    static std::unique_ptr<GLL> create(std::unique_ptr<Message0183> baseMessage);
    static std::string composeRaw(std::string talkerId,
                                  double latitude,
                                  char latitudeDirection,
                                  double longitude,
                                  char longitudeDirection,
                                  double timestamp,
                                  char status,
                                  char modeIndicator);

    friend class Nmea0183Factory;
    friend class MessageRegistry;
};

} // namespace nmea0183
} // namespace nmealib
