#pragma once

#include <nmealib/nmea0183.hpp>

namespace nmealib {
namespace nmea0183 {

class NotRMCException : public NmeaException {
public:
    explicit NotRMCException(const std::string& context, const std::string& details = "") : 
    NmeaException(context, "The sentence is not an RMC sentence", details) {}
};

class RMC : public Message0183 {
public:
    // Public constructor from parameters
    /**
     * @brief Construct a new RMC object from data
     * 
     * @param talkerId The talker ID to use in the payload, e.g. "GP", "II", etc.
     * @param utcFix hhmmss.ss format, e.g. 123519 for 12:35:19 UTC
     * @param status A or V
     * @param latitude Expected in the format ddmm.mmmm, positive
     * @param latitudeDirection N or S
     * @param longitude Expected in the format dddmm.mmmm, positive
     * @param longitudeDirection W or E
     * @param speedOverGround In knots
     * @param courseOverGround In degrees
     * @param date ddmmyy format, e.g. 230394 for 23 March 1994
     * @param magneticVariation In degrees
     * @param magneticVariationDirection E or W 
     * @param modeIndicator D, A, E or N
     * @param navigationStatus S, C, U or V
     */
    RMC(std::string talkerId,
        unsigned int utcFix, 
        char status, 
        double latitude,
        char latitudeDirection, 
        double longitude,
        char longitudeDirection,
        double speedOverGround, 
        double courseOverGround, 
        unsigned int date, 
        double magneticVariation,
        char magneticVariationDirection,
        char modeIndicator,
        char navigationStatus
    );

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
    unsigned int getUtcFix() const noexcept;
    char getStatus() const noexcept;
    double getLatitude() const noexcept;
    char getLatitudeDirection() const noexcept;
    double getLongitude() const noexcept;
    char getLongitudeDirection() const noexcept;
    double getSpeedOverGround() const noexcept;
    double getCourseOverGround() const noexcept;
    unsigned int getDate() const noexcept;
    double getMagneticVariation() const noexcept;
    char getMagneticVariationDirection() const noexcept;
    char getModeIndicator() const noexcept;
    char getNavigationStatus() const noexcept;

    // Overridden methods
    std::string getStringContent(bool verbose) const noexcept override;
    bool operator==(const RMC& other) const noexcept;

    bool hasEqualContent(const RMC& other) const noexcept;

private:
    unsigned int utcFix_;
    char status_;
    double latitude_;
    char latitudeDirection_;
    double longitude_;
    char longitudeDirection_;
    double speedOverGround_;
    double courseOverGround_;
    unsigned int date_;
    double magneticVariation_;
    char magneticVariationDirection_;
    char modeIndicator_;
    char navigationStatus_;

    RMC() = default;

    // Private constructor used by the factory method
    RMC(Message0183 baseMessage, 
        unsigned int utcFix, 
        char status, 
        double latitude,
        char latitudeDirection, 
        double longitude,
        char longitudeDirection,
        double speedOverGround, 
        double courseOverGround, 
        unsigned int date, 
        double magneticVariation,
        char magneticVariationDirection,
        char modeIndicator,
        char navigationStatus
    ) noexcept;

    // Private internal factory
    static std::unique_ptr<RMC> create(std::unique_ptr<Message0183> baseMessage);
    static std::string composeRaw(std::string talkerId, unsigned int utcFix, 
                            char status, 
                            double latitude,
                            char latitudeDirection, 
                            double longitude,
                            char longitudeDirection,
                            double speedOverGround, 
                            double courseOverGround, 
                            unsigned int date, 
                            double magneticVariation,
                            char magneticVariationDirection,
                            char modeIndicator,
                            char navigationStatus);
    
    friend class Nmea0183Factory;
    friend class MessageRegistry;
};

} // namespace nmea0183
} // namespace nmealib