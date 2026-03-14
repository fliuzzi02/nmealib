#include "nmealib/nmea0183/vhw.h"

#include "nmealib/detail/errorSupport.h"
#include "nmealib/detail/parse.h"

#include <iomanip>
#include <sstream>
#include <vector>

namespace nmealib {
namespace nmea0183 {

std::unique_ptr<VHW> VHW::create(std::unique_ptr<Message0183> baseMessage) {
    std::string context = "VHW::create";
    if (baseMessage->getSentenceType() != "VHW") {
        NMEALIB_RETURN_ERROR(NotVHWException(context, "Expected sentence type 'VHW', got " + baseMessage->getSentenceType()));
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

    if (fields.size() != 8) {
        NMEALIB_RETURN_ERROR(NotVHWException(context, "Invalid fields in VHW payload: expected 8, got " + std::to_string(fields.size()) + ". Payload: " + payload));
    }

    double headingTrue = 0.0;
    double headingMagnetic = 0.0;
    double speedKnots = 0.0;
    double speedKph = 0.0;
    if (!detail::parseOptionalDouble(fields[0], headingTrue) ||
        !detail::parseOptionalDouble(fields[2], headingMagnetic) ||
        !detail::parseOptionalDouble(fields[4], speedKnots) ||
        !detail::parseOptionalDouble(fields[6], speedKph)) {
        NMEALIB_RETURN_ERROR(NmeaException(context, "Error parsing VHW fields"));
    }

    char headingTrueType = fields[1].empty() ? '\0' : fields[1][0];
    char headingMagneticType = fields[3].empty() ? '\0' : fields[3][0];
    char speedKnotsType = fields[5].empty() ? '\0' : fields[5][0];
    char speedKphType = fields[7].empty() ? '\0' : fields[7][0];

    return std::unique_ptr<VHW>(new VHW(std::move(*baseMessage),
                                        headingTrue,
                                        headingTrueType,
                                        headingMagnetic,
                                        headingMagneticType,
                                        speedKnots,
                                        speedKnotsType,
                                        speedKph,
                                        speedKphType));
}

VHW::VHW(Message0183 baseMessage,
         double headingTrue,
         char headingTrueType,
         double headingMagnetic,
         char headingMagneticType,
         double speedKnots,
         char speedKnotsType,
         double speedKph,
         char speedKphType) noexcept
    : Message0183(std::move(baseMessage)),
      headingTrue_(headingTrue),
      headingTrueType_(headingTrueType),
      headingMagnetic_(headingMagnetic),
      headingMagneticType_(headingMagneticType),
      speedKnots_(speedKnots),
      speedKnotsType_(speedKnotsType),
      speedKph_(speedKph),
      speedKphType_(speedKphType) {}

VHW::VHW(std::string talkerId,
         double headingTrue,
         double headingMagnetic,
         double speedKnots,
         double speedKph)
    : Message0183(*Message0183::create(composeRaw(talkerId,
                                                  headingTrue,
                                                  headingMagnetic,
                                                  speedKnots,
                                                  speedKph))),
      headingTrue_(headingTrue),
      headingTrueType_('T'),
      headingMagnetic_(headingMagnetic),
      headingMagneticType_('M'),
      speedKnots_(speedKnots),
      speedKnotsType_('N'),
      speedKph_(speedKph),
      speedKphType_('K') {}

std::unique_ptr<nmealib::Message> VHW::clone() const {
    return std::unique_ptr<VHW>(new VHW(*this));
}

std::string VHW::getStringContent(bool verbose) const noexcept {
    std::ostringstream ss;
    ss << this->toString(verbose);
    ss << std::fixed << std::setprecision(2);

    if (verbose) {
        ss << "\tHeading: " << headingTrue_ << " " << headingTrueType_ << "\n";
        ss << "\tHeading: " << headingMagnetic_ << " " << headingMagneticType_ << "\n";
        ss << "\tSpeed: " << speedKnots_ << "kts\n"
           << "\tSpeed: " << speedKph_ << "kph";
        ss << "\n";        
    } else {
        ss << "True=" << headingTrue_
           << ", Magnetic=" << headingMagnetic_
           << ", Knots=" << speedKnots_
           << ", KPH=" << speedKph_;
    }

    return ss.str();
}

std::string VHW::composeRaw(const std::string& talkerId,
                            double headingTrue,
                            double headingMagnetic,
                            double speedKnots,
                            double speedKph) {
    std::ostringstream payloadStream;
    payloadStream << talkerId << "VHW,";
    payloadStream << std::fixed << std::setprecision(2) << headingTrue << ",T,";
    payloadStream << std::fixed << std::setprecision(2) << headingMagnetic << ",M,";
    payloadStream << std::fixed << std::setprecision(3) << speedKnots << ",N,";
    payloadStream << std::fixed << std::setprecision(3) << speedKph << ",K";

    std::string payload = payloadStream.str();
    return "$" + payload + "\r\n";
}

double VHW::getHeadingTrue() const noexcept {
    return headingTrue_;
}

char VHW::getHeadingTrueType() const noexcept {
    return headingTrueType_;
}

double VHW::getHeadingMagnetic() const noexcept {
    return headingMagnetic_;
}

char VHW::getHeadingMagneticType() const noexcept {
    return headingMagneticType_;
}

double VHW::getSpeedKnots() const noexcept {
    return speedKnots_;
}

char VHW::getSpeedKnotsType() const noexcept {
    return speedKnotsType_;
}

double VHW::getSpeedKph() const noexcept {
    return speedKph_;
}

char VHW::getSpeedKphType() const noexcept {
    return speedKphType_;
}

bool VHW::operator==(const VHW& other) const noexcept {
    return Message0183::operator==(other);
}

} // namespace nmea0183
} // namespace nmealib
