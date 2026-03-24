#include "nmealib/nmea0183/xte.h"

#include "nmealib/detail/errorSupport.h"
#include "nmealib/detail/parse.h"

#include <iomanip>
#include <sstream>
#include <vector>

namespace nmealib {
namespace nmea0183 {

std::unique_ptr<XTE> XTE::create(std::unique_ptr<Message0183> baseMessage) {
    std::string context = "XTE::create";
    if (baseMessage->getSentenceType() != "XTE") {
        NMEALIB_RETURN_ERROR(NotXTEException(context, "Expected sentence type 'XTE', got " + baseMessage->getSentenceType()));
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

    if (fields.size() != 5 && fields.size() != 6) {
        NMEALIB_RETURN_ERROR(NotXTEException(context, "Invalid fields in XTE payload: expected 5 or 6, got " + std::to_string(fields.size()) + ". Payload: " + payload));
    }

    char status1 = fields[0].empty() ? '\0' : fields[0][0];
    char status2 = fields[1].empty() ? '\0' : fields[1][0];
    double crossTrackError = 0.0;
    if (!detail::parseOptionalDouble(fields[2], crossTrackError)) {
        NMEALIB_RETURN_ERROR(NmeaException(context, "Error parsing XTE fields"));
    }
    char steerDirection = fields[3].empty() ? '\0' : fields[3][0];
    char crossTrackUnits = fields[4].empty() ? '\0' : fields[4][0];
    std::optional<char> faaModeIndicator = std::nullopt;
    if (fields.size() == 6 && !fields[5].empty()) {
        faaModeIndicator = fields[5][0];
    }

    return std::unique_ptr<XTE>(new XTE(std::move(*baseMessage),
                                        status1,
                                        status2,
                                        crossTrackError,
                                        steerDirection,
                                        crossTrackUnits,
                                        faaModeIndicator));
}

XTE::XTE(Message0183 baseMessage,
         char status1,
         char status2,
         double crossTrackError,
         char steerDirection,
         char crossTrackUnits,
         std::optional<char> faaModeIndicator) noexcept
    : Message0183(std::move(baseMessage)),
      status1_(status1),
      status2_(status2),
      crossTrackError_(crossTrackError),
      steerDirection_(steerDirection),
      crossTrackUnits_(crossTrackUnits),
      faaModeIndicator_(faaModeIndicator) {}

XTE::XTE(std::string talkerId,
         char status1,
         char status2,
         double crossTrackError,
         char steerDirection,
         char crossTrackUnits,
         std::optional<char> faaModeIndicator)
    : Message0183(*Message0183::create(composeRaw(talkerId,
                                                  status1,
                                                  status2,
                                                  crossTrackError,
                                                  steerDirection,
                                                  crossTrackUnits,
                                                  faaModeIndicator))),
      status1_(status1),
      status2_(status2),
      crossTrackError_(crossTrackError),
      steerDirection_(steerDirection),
      crossTrackUnits_(crossTrackUnits),
      faaModeIndicator_(faaModeIndicator) {}

std::unique_ptr<nmealib::Message> XTE::clone() const {
    return std::unique_ptr<XTE>(new XTE(*this));
}

std::string XTE::getStringContent(bool verbose) const noexcept {
    std::ostringstream ss;
    ss << this->toString(verbose);
    ss << std::fixed << std::setprecision(2);

    if (verbose) {
        ss << "\tStatus1: " << status1_ << "\n";
        ss << "\tStatus2: " << status2_ << "\n";
        ss << "\tCross Track Error: " << crossTrackError_ << " " << crossTrackUnits_ << "\n";
        ss << "\tSteer Direction: " << steerDirection_;
        if (faaModeIndicator_.has_value()) {
            ss << "\n\tFAA Mode Indicator: " << faaModeIndicator_.value();
        }
        ss << "\n";
    } else {
        ss << "Status1=" << status1_
           << ", Status2=" << status2_
           << ", XTE=" << crossTrackError_ << crossTrackUnits_
           << ", Steer=" << steerDirection_;
        if (faaModeIndicator_.has_value()) {
            ss << ", FAA=" << faaModeIndicator_.value();
        }
    }

    return ss.str();
}

std::string XTE::composeRaw(const std::string& talkerId,
                            char status1,
                            char status2,
                            double crossTrackError,
                            char steerDirection,
                            char crossTrackUnits,
                            std::optional<char> faaModeIndicator) {
    std::ostringstream payloadStream;
    payloadStream << talkerId << "XTE,";
    payloadStream << status1 << ",";
    payloadStream << status2 << ",";
    payloadStream << std::fixed << std::setprecision(1) << crossTrackError << ",";
    payloadStream << steerDirection << ",";
    payloadStream << crossTrackUnits;
    if (faaModeIndicator.has_value()) {
        payloadStream << "," << faaModeIndicator.value();
    }

    std::string payload = payloadStream.str();
    return "$" + payload + "\r\n";
}

char XTE::getStatus1() const noexcept {
    return status1_;
}

char XTE::getStatus2() const noexcept {
    return status2_;
}

double XTE::getCrossTrackError() const noexcept {
    return crossTrackError_;
}

char XTE::getSteerDirection() const noexcept {
    return steerDirection_;
}

char XTE::getCrossTrackUnits() const noexcept {
    return crossTrackUnits_;
}

bool XTE::hasFaaModeIndicator() const noexcept {
    return faaModeIndicator_.has_value();
}

std::optional<char> XTE::getFaaModeIndicator() const noexcept {
    return faaModeIndicator_;
}

bool XTE::operator==(const XTE& other) const noexcept {
    return Message0183::operator==(other);
}

} // namespace nmea0183
} // namespace nmealib
