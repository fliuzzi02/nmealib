#include "nmealib/nmea0183/dpt.h"

#include "nmealib/detail/errorSupport.h"
#include "nmealib/detail/parse.h"

#include <iomanip>
#include <sstream>
#include <vector>

namespace nmealib {
namespace nmea0183 {

std::unique_ptr<DPT> DPT::create(std::unique_ptr<Message0183> baseMessage) {
    std::string context = "DPT::create";
    if (baseMessage->getSentenceType() != "DPT") {
        NMEALIB_RETURN_ERROR(NotDPTException(context, "Expected sentence type 'DPT', got " + baseMessage->getSentenceType()));
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

    if (fields.size() != 3) {
        NMEALIB_RETURN_ERROR(NotDPTException(context, "Invalid fields in DPT payload: expected 3, got " + std::to_string(fields.size()) + ". Payload: " + payload));
    }

    double waterDepthMeters = 0.0;
    double transducerOffset = 0.0;
    double maxRangeScale = 0.0;
    if (!detail::parseOptionalDouble(fields[0], waterDepthMeters) ||
        !detail::parseOptionalDouble(fields[1], transducerOffset) ||
        !detail::parseOptionalDouble(fields[2], maxRangeScale)) {
        NMEALIB_RETURN_ERROR(NmeaException(context, "Error parsing DPT fields"));
    }

    return std::unique_ptr<DPT>(new DPT(std::move(*baseMessage),
                                        waterDepthMeters,
                                        transducerOffset,
                                        maxRangeScale));
}

DPT::DPT(Message0183 baseMessage,
         double waterDepthMeters,
         double transducerOffset,
         double maxRangeScale) noexcept :
         Message0183(std::move(baseMessage)),
         waterDepthMeters_(waterDepthMeters),
         transducerOffset_(transducerOffset),
         maxRangeScale_(maxRangeScale) {}

DPT::DPT(std::string talkerId,
         double waterDepthMeters,
         double transducerOffset,
         double maxRangeScale) :
         Message0183(*Message0183::create(composeRaw(talkerId,
                                                     waterDepthMeters,
                                                     transducerOffset,
                                                     maxRangeScale))),
         waterDepthMeters_(waterDepthMeters),
         transducerOffset_(transducerOffset),
         maxRangeScale_(maxRangeScale) {}

std::unique_ptr<nmealib::Message> DPT::clone() const {
    return std::unique_ptr<DPT>(new DPT(*this));
}

std::string DPT::getStringContent(bool verbose) const noexcept {
    std::ostringstream ss;
    ss << this->toString(verbose);
    ss << std::fixed << std::setprecision(2);

    if (verbose) {
        ss << "\tWater depth: " << waterDepthMeters_ << " m\n";
        ss << "\tTransducer offset: " << transducerOffset_ << " m\n";
        ss << "\tMax range scale: " << maxRangeScale_ << " m\n";
    } else {
        ss << "Depth=" << waterDepthMeters_ << "m"
           << ", Offset=" << transducerOffset_ << "m"
           << ", MaxRange=" << maxRangeScale_ << "m";
    }
    return ss.str();
}

std::string DPT::composeRaw(const std::string& talkerId,
                            double waterDepthMeters,
                            double transducerOffset,
                            double maxRangeScale) {
    std::ostringstream payloadStream;
    payloadStream << talkerId << "DPT,";
    payloadStream << std::fixed << std::setprecision(1) << waterDepthMeters << ",";
    payloadStream << std::fixed << std::setprecision(1) << transducerOffset << ",";
    payloadStream << std::fixed << std::setprecision(1) << maxRangeScale;

    std::string payload = payloadStream.str();
    return "$" + payload + "\r\n";
}

double DPT::getWaterDepthMeters() const noexcept {
    return waterDepthMeters_;
}

double DPT::getTransducerOffset() const noexcept {
    return transducerOffset_;
}

double DPT::getMaxRangeScale() const noexcept {
    return maxRangeScale_;
}

bool DPT::operator==(const DPT& other) const noexcept {
    return Message0183::operator==(other) &&
           waterDepthMeters_ == other.waterDepthMeters_ &&
           transducerOffset_ == other.transducerOffset_ &&
           maxRangeScale_ == other.maxRangeScale_;
}

} // namespace nmea0183
} // namespace nmealib
