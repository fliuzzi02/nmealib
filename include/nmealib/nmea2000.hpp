#pragma once

#include "message.hpp"
#include "nmeaException.hpp"

#include <chrono>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace nmealib {
namespace nmea2000 {

class InvalidPgnException : public NmeaException {
public:
    explicit InvalidPgnException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "NMEA 2000 PGN is invalid", details) {}
};

class Message2000 : public nmealib::Message {
public:
    Message2000(const Message2000&) = default;
    Message2000& operator=(const Message2000&) = default;
    Message2000(Message2000&&) noexcept = default;
    Message2000& operator=(Message2000&&) noexcept = default;
    ~Message2000() override = default;

    std::unique_ptr<nmealib::Message> clone() const override;

    std::uint32_t getPgn() const noexcept;
    bool hasCanId() const noexcept;
    std::uint32_t getCanId() const noexcept;
    std::uint8_t getPriority() const noexcept;
    std::uint8_t getDataPage() const noexcept;
    std::uint8_t getPduFormat() const noexcept;
    std::uint8_t getPduSpecific() const noexcept;
    std::uint8_t getSourceAddress() const noexcept;
    std::string getPayload() const noexcept;
    const std::vector<std::uint8_t>& getPayloadBytes() const noexcept;

    virtual std::string getStringContent(bool verbose) const noexcept;

    std::string serialize() const override;

    bool operator==(const Message2000& other) const noexcept;
    virtual bool hasEqualContent(const Message2000& other) const noexcept;

    bool validate() const override;

protected:
    std::uint32_t pgn_;
    bool hasCanId_{false};
    std::uint32_t canId_{0};
    std::uint8_t priority_{0};
    std::uint8_t dataPage_{0};
    std::uint8_t pduFormat_{0};
    std::uint8_t pduSpecific_{0};
    std::uint8_t sourceAddress_{0};
    std::string payload_;
    std::vector<std::uint8_t> payloadBytes_;

    static std::unique_ptr<Message2000> create(std::string raw, TimePoint ts = std::chrono::system_clock::now());

private:
    explicit Message2000(std::string raw,
                        TimePoint ts,
                        std::uint32_t pgn,
                        bool hasCanId,
                        std::uint32_t canId,
                        std::uint8_t priority,
                        std::uint8_t dataPage,
                        std::uint8_t pduFormat,
                        std::uint8_t pduSpecific,
                        std::uint8_t sourceAddress,
                        std::string payload,
                        std::vector<std::uint8_t> payloadBytes) noexcept;

    static std::uint32_t parsePgn(const std::string& context, const std::string& token);
    static std::uint32_t parseCanId(const std::string& context, const std::string& token);
    static std::uint32_t extractPgnFromCanId(std::uint32_t canId);
    static void decodeCanIdFields(std::uint32_t canId,
                                  std::uint8_t& priority,
                                  std::uint8_t& dataPage,
                                  std::uint8_t& pduFormat,
                                  std::uint8_t& pduSpecific,
                                  std::uint8_t& sourceAddress);
    static std::vector<std::uint8_t> parsePayloadBytes(const std::string& context, const std::string& payloadToken);
    static std::string bytesToHex(const std::vector<std::uint8_t>& bytes);
    static std::string bytesToSpacedHex(const std::vector<std::uint8_t>& bytes);
    static void validateFormat(const std::string& context, const std::string& raw);

    friend class Nmea2000Factory;
};

} // namespace nmea2000
} // namespace nmealib
