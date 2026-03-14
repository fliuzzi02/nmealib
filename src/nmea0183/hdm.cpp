#include "nmealib/nmea0183/hdm.h"

#include <iomanip>
#include <sstream>
#include <vector>

namespace nmealib {
namespace nmea0183 {

std::unique_ptr<HDM> HDM::create(std::unique_ptr<Message0183> baseMessage) {
    std::string context = "HDM::create";
    if (baseMessage->getSentenceType() != "HDM") {
        throw NotHDMException(context, "Expected sentence type 'HDM', got " + baseMessage->getSentenceType());
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

    if (fields.size() != 2) {
        throw NotHDMException(context, "Invalid fields in HDM payload: expected 2, got " + std::to_string(fields.size()) + ". Payload: " + payload);
    }

    try {
        double heading = fields[0].empty() ? 0.0 : std::stod(fields[0]);
        char indicator = fields[1].empty() ? '\0' : fields[1][0];

        return std::unique_ptr<HDM>(new HDM(std::move(*baseMessage),
                                            heading,
                                            indicator));
    } catch (const std::exception& e) {
        throw NmeaException(context, "Error parsing HDM fields: " + std::string(e.what()));
    }
}

HDM::HDM(Message0183 baseMessage,
         double heading,
         char indicator) noexcept
    : Message0183(std::move(baseMessage)),
      heading_(heading),
      indicator_(indicator) {}

HDM::HDM(std::string talkerId,
         double heading,
         char indicator)
    : Message0183(*Message0183::create(composeRaw(talkerId, heading, indicator))),
      heading_(heading),
      indicator_(indicator) {}

std::unique_ptr<nmealib::Message> HDM::clone() const {
    return std::unique_ptr<HDM>(new HDM(*this));
}

std::string HDM::getStringContent(bool verbose) const noexcept {
    std::ostringstream ss;
    ss << this->toString(verbose);
    ss << std::fixed << std::setprecision(1);

    if (verbose) {
        ss << "\tHeading: " << heading_ << " " << indicator_;
        ss << "\n";
    } else {
        ss << "Heading=" << heading_ << indicator_;
    }

    return ss.str();
}

std::string HDM::composeRaw(const std::string& talkerId,
                             double heading,
                             char indicator) {
    std::ostringstream payloadStream;
    payloadStream << talkerId << "HDM,";
    payloadStream << std::fixed << std::setprecision(1) << heading << ",";
    payloadStream << indicator;

    std::string payload = payloadStream.str();
    return "$" + payload + "\r\n";
}

double HDM::getHeading() const noexcept {
    return heading_;
}

char HDM::getIndicator() const noexcept {
    return indicator_;
}

bool HDM::operator==(const HDM& other) const noexcept {
    return Message0183::operator==(other);
}

} // namespace nmea0183
} // namespace nmealib
