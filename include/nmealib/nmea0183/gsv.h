#pragma once

#include <nmealib/nmea0183.h>

#include <cstddef>
#include <optional>
#include <vector>

namespace nmealib {
namespace nmea0183 {

class NotGSVException : public NmeaException {
public:
    explicit NotGSVException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "The sentence is not a GSV sentence", details) {}
};

class GSV : public Message0183 {
public:
    struct SatelliteInfo {
        unsigned int satelliteId{};
        int elevation{};
        unsigned int azimuth{};
        std::optional<unsigned int> snr{};
    };

    GSV(std::string talkerId,
        unsigned int totalSentences,
        unsigned int sentenceNumber,
        unsigned int satellitesInView,
        std::vector<SatelliteInfo> satellites,
        std::optional<unsigned int> signalId = std::nullopt
    );

    GSV(const GSV&) = default;
    GSV& operator=(const GSV&) = default;
    GSV(GSV&&) noexcept = default;
    GSV& operator=(GSV&&) noexcept = default;

    ~GSV() override = default;

    std::unique_ptr<nmealib::Message> clone() const override;

    unsigned int getTotalSentences() const noexcept;
    unsigned int getSentenceNumber() const noexcept;
    unsigned int getSatellitesInView() const noexcept;
    const std::vector<SatelliteInfo>& getSatellites() const noexcept;
    std::size_t getSatelliteCount() const noexcept;
    const SatelliteInfo* getSatellite(std::size_t index) const noexcept;
    bool hasSignalId() const noexcept;
    std::optional<unsigned int> getSignalId() const noexcept;

    std::string getStringContent(bool verbose) const noexcept override;
    bool operator==(const GSV& other) const noexcept;

private:
    unsigned int totalSentences_{};
    unsigned int sentenceNumber_{};
    unsigned int satellitesInView_{};
    std::vector<SatelliteInfo> satellites_{};
    std::optional<unsigned int> signalId_{};

    GSV() = delete;

    GSV(Message0183 baseMessage,
        unsigned int totalSentences,
        unsigned int sentenceNumber,
        unsigned int satellitesInView,
        std::vector<SatelliteInfo> satellites,
        std::optional<unsigned int> signalId
    ) noexcept;

    static std::unique_ptr<GSV> create(std::unique_ptr<Message0183> baseMessage);
    static std::string composeRaw(const std::string& talkerId,
                                  unsigned int totalSentences,
                                  unsigned int sentenceNumber,
                                  unsigned int satellitesInView,
                                  const std::vector<SatelliteInfo>& satellites,
                                  std::optional<unsigned int> signalId);

    friend class Nmea0183Factory;
    friend class MessageRegistry;
};

} // namespace nmea0183
} // namespace nmealib
