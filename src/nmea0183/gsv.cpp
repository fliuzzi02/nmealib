#include "nmealib/nmea0183/gsv.h"

#include "nmealib/detail/errorSupport.h"
#include "nmealib/detail/parse.h"

#include <sstream>
#include <vector>

namespace nmealib {
namespace nmea0183 {

std::unique_ptr<GSV> GSV::create(std::unique_ptr<Message0183> baseMessage) {
    constexpr std::size_t GSV_HEADER_FIELDS = 3U;
    constexpr std::size_t GSV_FIELDS_PER_SATELLITE = 4U;
    constexpr std::size_t GSV_OPTIONAL_SIGNAL_ID_REMAINDER = 1U;

    std::string context = "GSV::create";
    if (baseMessage->getSentenceType() != "GSV") {
        NMEALIB_RETURN_ERROR(NotGSVException(context, "Expected sentence type 'GSV', got " + baseMessage->getSentenceType()));
    }

    std::string payload = baseMessage->getPayload();
    std::istringstream ss(payload);
    std::string token;
    std::vector<std::string> fields;

    while (std::getline(ss, token, ',')) {
        fields.push_back(token);
    }

    if (!payload.empty() && payload.back() == ',') {
        fields.push_back("");
    }

    if (!fields.empty()) {
        fields.erase(fields.begin());
    }

    if (fields.size() < GSV_HEADER_FIELDS) {
        NMEALIB_RETURN_ERROR(NotGSVException(context, "Invalid fields in GSV payload: expected at least 3, got " + std::to_string(fields.size()) + ". Payload: " + payload));
    }

    unsigned int totalSentences = 0U;
    unsigned int sentenceNumber = 0U;
    unsigned int satellitesInView = 0U;
    if (!detail::parseOptionalUnsigned(fields[0], totalSentences) ||
        !detail::parseOptionalUnsigned(fields[1], sentenceNumber) ||
        !detail::parseOptionalUnsigned(fields[2], satellitesInView)) {
        NMEALIB_RETURN_ERROR(NmeaException(context, "Error parsing GSV fields"));
    }

    const std::size_t remainingAfterHeader = fields.size() - GSV_HEADER_FIELDS;
    const std::size_t remainder = remainingAfterHeader % GSV_FIELDS_PER_SATELLITE;

    if (remainder != 0U && remainder != GSV_OPTIONAL_SIGNAL_ID_REMAINDER) {
        NMEALIB_RETURN_ERROR(NotGSVException(context, "Invalid fields in GSV payload: expected satellite quadruplets with optional trailing signal ID. Payload: " + payload));
    }

    std::size_t satelliteFieldEnd = fields.size();
    std::optional<unsigned int> signalId = std::nullopt;
    if (remainder == GSV_OPTIONAL_SIGNAL_ID_REMAINDER) {
        satelliteFieldEnd = fields.size() - 1U;
        if (!fields.back().empty()) {
            unsigned int parsedSignalId = 0U;
            if (!detail::parseUnsigned(fields.back(), parsedSignalId)) {
                NMEALIB_RETURN_ERROR(NmeaException(context, "Error parsing GSV fields"));
            }
            signalId = parsedSignalId;
        }
    }

    std::vector<SatelliteInfo> satellites;
    for (std::size_t index = GSV_HEADER_FIELDS; index < satelliteFieldEnd; index += GSV_FIELDS_PER_SATELLITE) {
        SatelliteInfo satellite;

        if (!detail::parseOptionalUnsigned(fields[index], satellite.satelliteId)) {
            NMEALIB_RETURN_ERROR(NmeaException(context, "Error parsing GSV fields"));
        }

        if ((index + 1U) < satelliteFieldEnd) {
            if (!detail::parseOptionalInt(fields[index + 1U], satellite.elevation)) {
                NMEALIB_RETURN_ERROR(NmeaException(context, "Error parsing GSV fields"));
            }
        }

        if ((index + 2U) < satelliteFieldEnd) {
            if (!detail::parseOptionalUnsigned(fields[index + 2U], satellite.azimuth)) {
                NMEALIB_RETURN_ERROR(NmeaException(context, "Error parsing GSV fields"));
            }
        }

        if ((index + 3U) < satelliteFieldEnd && !fields[index + 3U].empty()) {
            unsigned int parsedSnr = 0U;
            if (!detail::parseUnsigned(fields[index + 3U], parsedSnr)) {
                NMEALIB_RETURN_ERROR(NmeaException(context, "Error parsing GSV fields"));
            }
            satellite.snr = parsedSnr;
        }

        satellites.push_back(std::move(satellite));
    }

    return std::unique_ptr<GSV>(new GSV(std::move(*baseMessage),
                                        totalSentences,
                                        sentenceNumber,
                                        satellitesInView,
                                        std::move(satellites),
                                        signalId));
}

GSV::GSV(Message0183 baseMessage,
         unsigned int totalSentences,
         unsigned int sentenceNumber,
         unsigned int satellitesInView,
         std::vector<SatelliteInfo> satellites,
         std::optional<unsigned int> signalId) noexcept
    : Message0183(std::move(baseMessage)),
      totalSentences_(totalSentences),
      sentenceNumber_(sentenceNumber),
      satellitesInView_(satellitesInView),
      satellites_(std::move(satellites)),
      signalId_(signalId) {}

GSV::GSV(std::string talkerId,
         unsigned int totalSentences,
         unsigned int sentenceNumber,
         unsigned int satellitesInView,
         std::vector<SatelliteInfo> satellites,
         std::optional<unsigned int> signalId)
    : Message0183(*Message0183::create(composeRaw(talkerId,
                                                  totalSentences,
                                                  sentenceNumber,
                                                  satellitesInView,
                                                  satellites,
                                                  signalId))),
      totalSentences_(totalSentences),
      sentenceNumber_(sentenceNumber),
      satellitesInView_(satellitesInView),
      satellites_(std::move(satellites)),
      signalId_(signalId) {}

std::unique_ptr<nmealib::Message> GSV::clone() const {
    return std::unique_ptr<GSV>(new GSV(*this));
}

std::string GSV::getStringContent(bool verbose) const noexcept {
    std::ostringstream ss;
    ss << this->toString(verbose);

    if (verbose) {
        ss << "\tTotal Sentences: " << totalSentences_ << "\n";
        ss << "\tSentence Number: " << sentenceNumber_ << "\n";
        ss << "\tSatellites in View: " << satellitesInView_ << "\n";
        ss << "\tSatellites Reported: " << satellites_.size();
        if (signalId_.has_value()) {
            ss << "\n\tSignal ID: " << signalId_.value();
        }
        ss << "\n";
    } else {
        ss << "Total=" << totalSentences_
           << ", Number=" << sentenceNumber_
           << ", InView=" << satellitesInView_
           << ", Reported=" << satellites_.size();
        if (signalId_.has_value()) {
            ss << ", SignalID=" << signalId_.value();
        }
    }

    return ss.str();
}

std::string GSV::composeRaw(const std::string& talkerId,
                            unsigned int totalSentences,
                            unsigned int sentenceNumber,
                            unsigned int satellitesInView,
                            const std::vector<SatelliteInfo>& satellites,
                            std::optional<unsigned int> signalId) {
    std::ostringstream payloadStream;
    payloadStream << talkerId << "GSV," << totalSentences << "," << sentenceNumber << "," << satellitesInView;

    for (const SatelliteInfo& satellite : satellites) {
        payloadStream << "," << satellite.satelliteId
                      << "," << satellite.elevation
                      << "," << satellite.azimuth
                      << ",";
        if (satellite.snr.has_value()) {
            payloadStream << satellite.snr.value();
        }
    }

    if (signalId.has_value()) {
        payloadStream << "," << signalId.value();
    }

    std::string payload = payloadStream.str();
    return "$" + payload + "\r\n";
}

unsigned int GSV::getTotalSentences() const noexcept {
    return totalSentences_;
}

unsigned int GSV::getSentenceNumber() const noexcept {
    return sentenceNumber_;
}

unsigned int GSV::getSatellitesInView() const noexcept {
    return satellitesInView_;
}

const std::vector<GSV::SatelliteInfo>& GSV::getSatellites() const noexcept {
    return satellites_;
}

std::size_t GSV::getSatelliteCount() const noexcept {
    return satellites_.size();
}

const GSV::SatelliteInfo* GSV::getSatellite(std::size_t index) const noexcept {
    if (index >= satellites_.size()) {
        return nullptr;
    }
    return &satellites_[index];
}

bool GSV::hasSignalId() const noexcept {
    return signalId_.has_value();
}

std::optional<unsigned int> GSV::getSignalId() const noexcept {
    return signalId_;
}

bool GSV::operator==(const GSV& other) const noexcept {
    return Message0183::operator==(other);
}

} // namespace nmea0183
} // namespace nmealib
