#include "nmealib/nmea0183/vhw.hpp"

#include <iomanip>
#include <sstream>
#include <vector>

namespace nmealib {
namespace nmea0183 {

std::unique_ptr<VHW> VHW::create(std::unique_ptr<Message0183> baseMessage) {
    std::string context = "VHW::create";
    if (baseMessage->getSentenceType() != "VHW") {
        throw NotVHWException(context, "Expected sentence type 'VHW', got " + baseMessage->getSentenceType());
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
        throw NotVHWException(context, "Invalid fields in VHW payload: expected 8, got " + std::to_string(fields.size()) + ". Payload: " + payload);
    }

    try {
        double headingTrue = fields[0].empty() ? 0.0 : std::stod(fields[0]);
        char headingTrueType = fields[1].empty() ? '\0' : fields[1][0];
        double headingMagnetic = fields[2].empty() ? 0.0 : std::stod(fields[2]);
        char headingMagneticType = fields[3].empty() ? '\0' : fields[3][0];
        double speedKnots = fields[4].empty() ? 0.0 : std::stod(fields[4]);
        char speedKnotsType = fields[5].empty() ? '\0' : fields[5][0];
        double speedKph = fields[6].empty() ? 0.0 : std::stod(fields[6]);
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
    } catch (const std::exception& e) {
        throw NmeaException(context, "Error parsing VHW fields: " + std::string(e.what()));
    }
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
    std::string validity = "KO";
    if (validate()) {
        validity = "OK";
    }

    if (verbose) {
        ss << "Protocol: " << typeToString(type_) << "\n";
        ss << "Talker: " << getTalker() << "\n";
        ss << "Sentence Type: " << getSentenceType() << "\n";
        ss << "Checksum: " << (checksumStr_.empty() ? "None" : validity) << "\n";
        ss << "Fields:\n";
        ss << "\tHeading (True): " << headingTrue_;
        if (headingTrueType_ != '\0') {
            ss << " " << headingTrueType_;
        }
        ss << "\n"
           << "\tHeading (Magnetic): " << headingMagnetic_;
        if (headingMagneticType_ != '\0') {
            ss << " " << headingMagneticType_;
        }
        ss << "\n"
           << "\tSpeed (Knots): " << speedKnots_;
        if (speedKnotsType_ != '\0') {
            ss << " " << speedKnotsType_;
        }
        ss << "\n"
           << "\tSpeed (KPH): " << speedKph_;
        if (speedKphType_ != '\0') {
            ss << " " << speedKphType_;
        }
    } else {
        ss << "[" << validity << "] " << typeToString(type_) << " " << getTalker() << " " << getSentenceType() << ": "
           << "True=" << headingTrue_
           << ", Magnetic=" << headingMagnetic_
           << ", Knots=" << speedKnots_
           << ", KPH=" << speedKph_;
    }

    return ss.str();
}

std::string VHW::composeRaw(std::string talkerId,
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

bool VHW::hasEqualContent(const VHW& other) const noexcept {
    return Message0183::hasEqualContent(other);
}

} // namespace nmea0183
} // namespace nmealib
