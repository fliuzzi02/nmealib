#pragma once

#include "nmea0183.hpp"

namespace nmealib {
namespace nmea0183 {

class NotRMCException : public NmeaException {
public:
    explicit NotRMCException(const std::string& context, const std::string& details = "") : 
    NmeaException(context, "The sentence is not an RMC sentence", details) {}
};

class RMC : public Message0183 {
public:
    // Accessory constructors
    RMC(const RMC&) = default;
    RMC& operator=(const RMC&) = default;
    RMC(RMC&&) noexcept = default;
    RMC& operator=(RMC&&) noexcept = default;

    // Destructor
    ~RMC() override = default;

    // Polymorphic copy
    std::unique_ptr<nmealib::Message> clone() const override;

    // RMC-specific getters
    unsigned int getUtcFix() const noexcept { return utcFix_; }
    char getStatus() const noexcept { return status_; }
    double getLatitude() const noexcept { return latitude_; }
    double getLongitude() const noexcept { return longitude_; }
    double getSpeedOverGround() const noexcept { return speedOverGround_; }
    double getCourseOverGround() const noexcept { return courseOverGround_; }
    unsigned int getDate() const noexcept { return date_; }
    double getMagneticVariation() const noexcept { return magneticVariation_; }

    // Overridden methods
    std::string getStringContent(bool verbose) const noexcept override;
    bool operator==(const RMC& other) const noexcept {
        return Message0183::operator==(other);
    }

    bool hasEqualContent(const RMC& other) const noexcept {
        return Message0183::hasEqualContent(other);
    }

private:
    unsigned int utcFix_;
    char status_;
    double latitude_;
    double longitude_;
    double speedOverGround_;
    double courseOverGround_;
    unsigned int date_;
    double magneticVariation_;

    RMC(Message0183 baseMessage, 
        unsigned int utcFix, 
        char status, 
        double latitude, 
        double longitude, 
        double speedOverGround, 
        double courseOverGround, 
        unsigned int date, 
        double magneticVariation) noexcept;

    // Private internal factory
    static std::unique_ptr<RMC> create(std::unique_ptr<Message0183> baseMessage);
    
    friend class Nmea0183Factory;
};

} // namespace nmea0183
} // namespace nmealib