#include "nmealib/nmea0183/hdg.h"

#include "nmealib/detail/errorSupport.h"
#include "nmealib/detail/parse.h"

#include <iomanip>
#include <sstream>
#include <vector>

namespace nmealib {
namespace nmea0183 {

std::unique_ptr<HDG> HDG::create(std::unique_ptr<Message0183> baseMessage) {
    std::string context = "HDG::create";
    if (baseMessage->getSentenceType() != "HDG") {
        NMEALIB_RETURN_ERROR(NotHDGException(context, "Expected sentence type 'HDG', got " + baseMessage->getSentenceType()));
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

    if (fields.size() != 5) {
        NMEALIB_RETURN_ERROR(NotHDGException(context, "Invalid fields in HDG payload: expected 5, got " + std::to_string(fields.size()) + ". Payload: " + payload));
    }

    double magneticHeading = 0.0;
    double magneticDeviation = 0.0;
    double magneticVariation = 0.0;
    if (!detail::parseOptionalDouble(fields[0], magneticHeading) ||
        !detail::parseOptionalDouble(fields[1], magneticDeviation) ||
        !detail::parseOptionalDouble(fields[3], magneticVariation)) {
        NMEALIB_RETURN_ERROR(NmeaException(context, "Error parsing HDG fields"));
    }

    char deviationDirection = fields[2].empty() ? '\0' : fields[2][0];
    char variationDirection = fields[4].empty() ? '\0' : fields[4][0];

    return std::unique_ptr<HDG>(new HDG(std::move(*baseMessage),
                                        magneticHeading,
                                        magneticDeviation,
                                        deviationDirection,
                                        magneticVariation,
                                        variationDirection));
}

HDG::HDG(Message0183 baseMessage,
         double magneticHeading,
         double magneticDeviation,
         char deviationDirection,
         double magneticVariation,
         char variationDirection) noexcept
    : Message0183(std::move(baseMessage)),
      magneticHeading_(magneticHeading),
      magneticDeviation_(magneticDeviation),
      deviationDirection_(deviationDirection),
      magneticVariation_(magneticVariation),
      variationDirection_(variationDirection) {}

HDG::HDG(std::string talkerId,
         double magneticHeading,
         double magneticDeviation,
         char deviationDirection,
         double magneticVariation,
         char variationDirection)
    : Message0183(*Message0183::create(composeRaw(talkerId,
                                                  magneticHeading,
                                                  magneticDeviation,
                                                  deviationDirection,
                                                  magneticVariation,
                                                  variationDirection))),
      magneticHeading_(magneticHeading),
      magneticDeviation_(magneticDeviation),
      deviationDirection_(deviationDirection),
      magneticVariation_(magneticVariation),
      variationDirection_(variationDirection) {}

std::unique_ptr<nmealib::Message> HDG::clone() const {
    return std::unique_ptr<HDG>(new HDG(*this));
}

std::string HDG::getStringContent(bool verbose) const noexcept {
    std::ostringstream ss;
    ss << this->toString(verbose);
    ss << std::fixed << std::setprecision(1);
    const char deviationDirection = (deviationDirection_ == '\0') ? '-' : deviationDirection_;
    const char variationDirection = (variationDirection_ == '\0') ? '-' : variationDirection_;

    if (verbose) {
        ss << "\tMagnetic Heading: " << magneticHeading_ << "\n";
        ss << "\tMagnetic Deviation: " << magneticDeviation_ << " " << deviationDirection << "\n";
        ss << "\tMagnetic Variation: " << magneticVariation_ << " " << variationDirection << "\n";
    } else {
        ss << "HeadingM=" << magneticHeading_
           << ", Dev=" << magneticDeviation_ << deviationDirection
           << ", Var=" << magneticVariation_ << variationDirection;
    }

    return ss.str();
}

std::string HDG::composeRaw(const std::string& talkerId,
                            double magneticHeading,
                            double magneticDeviation,
                            char deviationDirection,
                            double magneticVariation,
                            char variationDirection) {
    std::ostringstream payloadStream;
    payloadStream << talkerId << "HDG,";
    payloadStream << std::fixed << std::setprecision(1) << magneticHeading << ",";
    payloadStream << std::fixed << std::setprecision(1) << magneticDeviation << ",";
    payloadStream << deviationDirection << ",";
    payloadStream << std::fixed << std::setprecision(1) << magneticVariation << ",";
    payloadStream << variationDirection;

    std::string payload = payloadStream.str();
    return "$" + payload + "\r\n";
}

double HDG::getMagneticHeading() const noexcept {
    return magneticHeading_;
}

double HDG::getMagneticDeviation() const noexcept {
    return magneticDeviation_;
}

char HDG::getDeviationDirection() const noexcept {
    return deviationDirection_;
}

double HDG::getMagneticVariation() const noexcept {
    return magneticVariation_;
}

char HDG::getVariationDirection() const noexcept {
    return variationDirection_;
}

bool HDG::operator==(const HDG& other) const noexcept {
    return Message0183::operator==(other);
}

} // namespace nmea0183
} // namespace nmealib
