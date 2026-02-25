#pragma once

#include <nmealib/nmea2000.hpp>

#include <cstdint>
#include <memory>
#include <string>
namespace nmealib {
namespace nmea2000 {

class NotPGN129029Exception : public NmeaException {
public:
    explicit NotPGN129029Exception(const std::string& context, const std::string& details = "") :
    NmeaException(context, "The message is not PGN 129029", details) {}
};

class PGN129029 : public Message2000 {
public:
    PGN129029(std::uint8_t sid,
              double latitude,
              double longitude,
              double altitude,
              std::uint8_t fixStatus,
              double utcTime,
              std::uint8_t sourceAddress = 0x00,
              std::uint8_t priority = 3);

    PGN129029(const PGN129029&) = default;
    PGN129029& operator=(const PGN129029&) = default;
    PGN129029(PGN129029&&) noexcept = default;
    PGN129029& operator=(PGN129029&&) noexcept = default;
    ~PGN129029() override = default;

    std::unique_ptr<nmealib::Message> clone() const override;

    std::uint8_t getSid() const noexcept;
    double getLatitude() const noexcept;
    double getLongitude() const noexcept;
    double getAltitude() const noexcept;
    std::uint8_t getFixStatus() const noexcept;
    std::uint8_t getFixType() const noexcept;
    std::uint8_t getStatusFlags() const noexcept;
    double getUtcTime() const noexcept;

    std::string getStringContent(bool verbose) const noexcept override;
    bool operator==(const PGN129029& other) const noexcept;

    bool hasEqualContent(const PGN129029& other) const noexcept;

private:
    std::uint8_t sid_;
    double latitude_;
    double longitude_;
    double altitude_;
    std::uint8_t fixStatus_;
    double utcTime_;

    PGN129029() = default;

    PGN129029(Message2000 baseMessage,
              std::uint8_t sid,
              double latitude,
              double longitude,
              double altitude,
              std::uint8_t fixStatus,
              double utcTime) noexcept;

    static std::unique_ptr<PGN129029> create(std::unique_ptr<Message2000> baseMessage);
    static std::string composeRaw(std::uint8_t sid,
                                  double latitude,
                                  double longitude,
                                  double altitude,
                                  std::uint8_t fixStatus,
                                  double utcTime,
                                  std::uint8_t sourceAddress,
                                  std::uint8_t priority);

    friend class Nmea2000Factory;
    friend class MessageRegistry;
};

} // namespace nmea2000
} // namespace nmealib
