#include "nmealib/nmea0183/vtg.h"

#include "nmealib/detail/errorSupport.h"
#include "nmealib/detail/parse.h"

#include <iomanip>
#include <sstream>
#include <vector>

namespace nmealib {
namespace nmea0183 {

std::unique_ptr<VTG> VTG::create(std::unique_ptr<Message0183> baseMessage) {
    std::string context = "VTG::create";
    if (baseMessage->getSentenceType() != "VTG") {
        NMEALIB_RETURN_ERROR(NotVTGException(context, "Expected sentence type 'VTG', got " + baseMessage->getSentenceType()));
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

    if (fields.size() != 4 && fields.size() != 8 && fields.size() != 9) {
        NMEALIB_RETURN_ERROR(NotVTGException(context, "Invalid fields in VTG payload: expected 4, 8 or 9, got " + std::to_string(fields.size()) + ". Payload: " + payload));
    }

    bool legacyFormat = fields.size() == 4;
    if (!legacyFormat && !fields[1].empty() && fields[1] != "T") {
        legacyFormat = true;
    }

    if (legacyFormat) {
        if (fields.size() != 4) {
            NMEALIB_RETURN_ERROR(NotVTGException(context, "Legacy VTG format requires exactly 4 fields"));
        }

        double courseOverGroundTrue = 0.0;
        double courseOverGroundMagnetic = 0.0;
        double speedOverGroundKnots = 0.0;
        double speedOverGroundKph = 0.0;
        if (!detail::parseOptionalDouble(fields[0], courseOverGroundTrue) ||
            !detail::parseOptionalDouble(fields[1], courseOverGroundMagnetic) ||
            !detail::parseOptionalDouble(fields[2], speedOverGroundKnots) ||
            !detail::parseOptionalDouble(fields[3], speedOverGroundKph)) {
            NMEALIB_RETURN_ERROR(NmeaException(context, "Error parsing VTG fields"));
        }

        return std::unique_ptr<VTG>(new VTG(std::move(*baseMessage),
                                            courseOverGroundTrue,
                                            '\0',
                                            courseOverGroundMagnetic,
                                            '\0',
                                            speedOverGroundKnots,
                                            '\0',
                                            speedOverGroundKph,
                                            '\0',
                                            std::nullopt,
                                            true));
    }

    double courseOverGroundTrue = 0.0;
    double courseOverGroundMagnetic = 0.0;
    double speedOverGroundKnots = 0.0;
    double speedOverGroundKph = 0.0;
    if (!detail::parseOptionalDouble(fields[0], courseOverGroundTrue) ||
        !detail::parseOptionalDouble(fields[2], courseOverGroundMagnetic) ||
        !detail::parseOptionalDouble(fields[4], speedOverGroundKnots) ||
        !detail::parseOptionalDouble(fields[6], speedOverGroundKph)) {
        NMEALIB_RETURN_ERROR(NmeaException(context, "Error parsing VTG fields"));
    }

    char courseOverGroundTrueType = fields[1].empty() ? '\0' : fields[1][0];
    char courseOverGroundMagneticType = fields[3].empty() ? '\0' : fields[3][0];
    char speedOverGroundKnotsType = fields[5].empty() ? '\0' : fields[5][0];
    char speedOverGroundKphType = fields[7].empty() ? '\0' : fields[7][0];
    std::optional<char> faaModeIndicator = std::nullopt;

    if (fields.size() == 9 && !fields[8].empty()) {
        faaModeIndicator = fields[8][0];
    }

    return std::unique_ptr<VTG>(new VTG(std::move(*baseMessage),
                                        courseOverGroundTrue,
                                        courseOverGroundTrueType,
                                        courseOverGroundMagnetic,
                                        courseOverGroundMagneticType,
                                        speedOverGroundKnots,
                                        speedOverGroundKnotsType,
                                        speedOverGroundKph,
                                        speedOverGroundKphType,
                                        faaModeIndicator,
                                        false));
}

VTG::VTG(Message0183 baseMessage,
         double courseOverGroundTrue,
         char courseOverGroundTrueType,
         double courseOverGroundMagnetic,
         char courseOverGroundMagneticType,
         double speedOverGroundKnots,
         char speedOverGroundKnotsType,
         double speedOverGroundKph,
         char speedOverGroundKphType,
         std::optional<char> faaModeIndicator,
         bool legacyFormat) noexcept
    : Message0183(std::move(baseMessage)),
      courseOverGroundTrue_(courseOverGroundTrue),
      courseOverGroundTrueType_(courseOverGroundTrueType),
      courseOverGroundMagnetic_(courseOverGroundMagnetic),
      courseOverGroundMagneticType_(courseOverGroundMagneticType),
      speedOverGroundKnots_(speedOverGroundKnots),
      speedOverGroundKnotsType_(speedOverGroundKnotsType),
      speedOverGroundKph_(speedOverGroundKph),
      speedOverGroundKphType_(speedOverGroundKphType),
      faaModeIndicator_(faaModeIndicator),
      legacyFormat_(legacyFormat) {}

VTG::VTG(std::string talkerId,
         double courseOverGroundTrue,
         double courseOverGroundMagnetic,
         double speedOverGroundKnots,
         double speedOverGroundKph,
         std::optional<char> faaModeIndicator,
         bool legacyFormat)
    : Message0183(*Message0183::create(composeRaw(talkerId,
                                                  courseOverGroundTrue,
                                                  courseOverGroundMagnetic,
                                                  speedOverGroundKnots,
                                                  speedOverGroundKph,
                                                  faaModeIndicator,
                                                  legacyFormat))),
      courseOverGroundTrue_(courseOverGroundTrue),
      courseOverGroundTrueType_(legacyFormat ? '\0' : 'T'),
      courseOverGroundMagnetic_(courseOverGroundMagnetic),
      courseOverGroundMagneticType_(legacyFormat ? '\0' : 'M'),
      speedOverGroundKnots_(speedOverGroundKnots),
      speedOverGroundKnotsType_(legacyFormat ? '\0' : 'N'),
      speedOverGroundKph_(speedOverGroundKph),
      speedOverGroundKphType_(legacyFormat ? '\0' : 'K'),
      faaModeIndicator_(legacyFormat ? std::nullopt : faaModeIndicator),
      legacyFormat_(legacyFormat) {}

std::unique_ptr<nmealib::Message> VTG::clone() const {
    return std::unique_ptr<VTG>(new VTG(*this));
}

std::string VTG::getStringContent(bool verbose) const noexcept {
    std::ostringstream ss;
    ss << this->toString(verbose);
    ss << std::fixed << std::setprecision(2);

    if (verbose) {
        ss << "\tLegacy Format: " << (legacyFormat_ ? "Yes" : "No") << "\n";
        ss << "\tCourse Over Ground: " << courseOverGroundTrue_ << "T\n";
        ss << "\tCourse Over Ground: " << courseOverGroundMagnetic_ << "M\n";
        ss << "\tSpeed Over Ground: " << speedOverGroundKnots_ << "kts\n";
        ss << "\tSpeed Over Ground: " << speedOverGroundKph_ << "kph";
        if (faaModeIndicator_.has_value()) {
            ss << "\n"
               << "\tFAA Mode Indicator: " << faaModeIndicator_.value();
        }
        ss << "\n";
    } else {
        ss << "True=" << courseOverGroundTrue_
           << ", Magnetic=" << courseOverGroundMagnetic_
           << ", Knots=" << speedOverGroundKnots_
           << ", KPH=" << speedOverGroundKph_;
        if (faaModeIndicator_.has_value()) {
            ss << ", FAA=" << faaModeIndicator_.value();
        }
        ss << ", Legacy=" << (legacyFormat_ ? "Y" : "N");
    }

    return ss.str();
}

std::string VTG::composeRaw(const std::string& talkerId,
                            double courseOverGroundTrue,
                            double courseOverGroundMagnetic,
                            double speedOverGroundKnots,
                            double speedOverGroundKph,
                            std::optional<char> faaModeIndicator,
                            bool legacyFormat) {
    std::ostringstream payloadStream;
    payloadStream << talkerId << "VTG,";

    if (legacyFormat) {
        payloadStream << std::fixed << std::setprecision(2) << courseOverGroundTrue << ",";
        payloadStream << std::fixed << std::setprecision(2) << courseOverGroundMagnetic << ",";
        payloadStream << std::fixed << std::setprecision(3) << speedOverGroundKnots << ",";
        payloadStream << std::fixed << std::setprecision(3) << speedOverGroundKph;
    } else {
        payloadStream << std::fixed << std::setprecision(2) << courseOverGroundTrue << ",T,";
        payloadStream << std::fixed << std::setprecision(2) << courseOverGroundMagnetic << ",M,";
        payloadStream << std::fixed << std::setprecision(3) << speedOverGroundKnots << ",N,";
        payloadStream << std::fixed << std::setprecision(3) << speedOverGroundKph << ",K";

        if (faaModeIndicator.has_value()) {
            payloadStream << "," << faaModeIndicator.value();
        }
    }

    std::string payload = payloadStream.str();
    return "$" + payload + "\r\n";
}

double VTG::getCourseOverGroundTrue() const noexcept {
    return courseOverGroundTrue_;
}

char VTG::getCourseOverGroundTrueType() const noexcept {
    return courseOverGroundTrueType_;
}

double VTG::getCourseOverGroundMagnetic() const noexcept {
    return courseOverGroundMagnetic_;
}

char VTG::getCourseOverGroundMagneticType() const noexcept {
    return courseOverGroundMagneticType_;
}

double VTG::getSpeedOverGroundKnots() const noexcept {
    return speedOverGroundKnots_;
}

char VTG::getSpeedOverGroundKnotsType() const noexcept {
    return speedOverGroundKnotsType_;
}

double VTG::getSpeedOverGroundKph() const noexcept {
    return speedOverGroundKph_;
}

char VTG::getSpeedOverGroundKphType() const noexcept {
    return speedOverGroundKphType_;
}

bool VTG::hasFaaModeIndicator() const noexcept {
    return faaModeIndicator_.has_value();
}

std::optional<char> VTG::getFaaModeIndicator() const noexcept {
    return faaModeIndicator_;
}

bool VTG::isLegacyFormat() const noexcept {
    return legacyFormat_;
}

bool VTG::operator==(const VTG& other) const noexcept {
    return Message0183::operator==(other);
}

} // namespace nmea0183
} // namespace nmealib
