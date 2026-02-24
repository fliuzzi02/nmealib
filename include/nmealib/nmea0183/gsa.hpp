#pragma once

#include "nmea0183.hpp"

#include <array>
#include <optional>

namespace nmealib {
namespace nmea0183 {

class NotGSAException : public NmeaException {
public:
    explicit NotGSAException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "The sentence is not a GSA sentence", details) {}
};

class GSA : public Message0183 {
public:
    GSA(std::string talkerId,
        char selectionMode,
        unsigned int mode,
        std::array<unsigned int, 12> satelliteIds,
        double pdop,
        double hdop,
        double vdop,
        std::optional<unsigned int> systemId = std::nullopt
    );

    GSA(const GSA&) = default;
    GSA& operator=(const GSA&) = default;
    GSA(GSA&&) noexcept = default;
    GSA& operator=(GSA&&) noexcept = default;

    ~GSA() override = default;

    std::unique_ptr<nmealib::Message> clone() const override;

    char getSelectionMode() const noexcept { return selectionMode_; }
    unsigned int getMode() const noexcept { return mode_; }
    std::array<unsigned int, 12> getSatelliteIds() const noexcept { return satelliteIds_; }
    unsigned int getSatelliteId(size_t index) const noexcept { return index < satelliteIds_.size() ? satelliteIds_[index] : 0u; }
    double getPdop() const noexcept { return pdop_; }
    double getHdop() const noexcept { return hdop_; }
    double getVdop() const noexcept { return vdop_; }
    bool hasSystemId() const noexcept { return systemId_.has_value(); }
    std::optional<unsigned int> getSystemId() const noexcept { return systemId_; }

    std::string getStringContent(bool verbose) const noexcept override;
    bool operator==(const GSA& other) const noexcept {
        return Message0183::operator==(other);
    }

    bool hasEqualContent(const GSA& other) const noexcept {
        return Message0183::hasEqualContent(other);
    }

private:
    char selectionMode_;
    unsigned int mode_;
    std::array<unsigned int, 12> satelliteIds_;
    double pdop_;
    double hdop_;
    double vdop_;
    std::optional<unsigned int> systemId_;

    GSA() = default;

    GSA(Message0183 baseMessage,
        char selectionMode,
        unsigned int mode,
        std::array<unsigned int, 12> satelliteIds,
        double pdop,
        double hdop,
        double vdop,
        std::optional<unsigned int> systemId
    ) noexcept;

    static std::unique_ptr<GSA> create(std::unique_ptr<Message0183> baseMessage);
    static std::string composeRaw(std::string talkerId,
                                  char selectionMode,
                                  unsigned int mode,
                                  std::array<unsigned int, 12> satelliteIds,
                                  double pdop,
                                  double hdop,
                                  double vdop,
                                  std::optional<unsigned int> systemId);

    friend class Nmea0183Factory;
    friend class MessageRegistry;
};

} // namespace nmea0183
} // namespace nmealib
