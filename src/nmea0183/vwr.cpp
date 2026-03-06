#include "nmealib/nmea0183/vwr.h"

#include <iomanip>
#include <sstream>
#include <vector>

namespace nmealib {
namespace nmea0183 {

std::unique_ptr<VWR> VWR::create(std::unique_ptr<Message0183> baseMessage) {
    std::string context = "VWR::create";
    if (baseMessage->getSentenceType() != "VWR") {
        throw NotVWRException(context, "Expected sentence type 'VWR', got " + baseMessage->getSentenceType());
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
        throw NotVWRException(context, "Invalid fields in VWR payload: expected 8, got " + std::to_string(fields.size()) + ". Payload: " + payload);
    }

    try {
        double windAngle    = fields[0].empty() ? 0.0  : std::stod(fields[0]);
        char windSide       = fields[1].empty() ? '\0' : fields[1][0];
        double speedKnots   = fields[2].empty() ? 0.0  : std::stod(fields[2]);
        char speedKnotsUnit = fields[3].empty() ? '\0' : fields[3][0];
        double speedMps     = fields[4].empty() ? 0.0  : std::stod(fields[4]);
        char speedMpsUnit   = fields[5].empty() ? '\0' : fields[5][0];
        double speedKph     = fields[6].empty() ? 0.0  : std::stod(fields[6]);
        char speedKphUnit   = fields[7].empty() ? '\0' : fields[7][0];

        return std::unique_ptr<VWR>(new VWR(std::move(*baseMessage),
                                            windAngle,
                                            windSide,
                                            speedKnots,
                                            speedKnotsUnit,
                                            speedMps,
                                            speedMpsUnit,
                                            speedKph,
                                            speedKphUnit));
    } catch (const std::exception& e) {
        throw NmeaException(context, "Error parsing VWR fields: " + std::string(e.what()));
    }
}

VWR::VWR(Message0183 baseMessage,
         double windAngle,
         char windSide,
         double speedKnots,
         char speedKnotsUnit,
         double speedMps,
         char speedMpsUnit,
         double speedKph,
         char speedKphUnit) noexcept
    : Message0183(std::move(baseMessage)),
      windAngle_(windAngle),
      windSide_(windSide),
      speedKnots_(speedKnots),
      speedKnotsUnit_(speedKnotsUnit),
      speedMps_(speedMps),
      speedMpsUnit_(speedMpsUnit),
      speedKph_(speedKph),
      speedKphUnit_(speedKphUnit) {}

VWR::VWR(std::string talkerId,
         double windAngle,
         char windSide,
         double speedKnots,
         double speedMps,
         double speedKph)
    : Message0183(*Message0183::create(composeRaw(talkerId,
                                                  windAngle,
                                                  windSide,
                                                  speedKnots,
                                                  speedMps,
                                                  speedKph))),
      windAngle_(windAngle),
      windSide_(windSide),
      speedKnots_(speedKnots),
      speedKnotsUnit_('N'),
      speedMps_(speedMps),
      speedMpsUnit_('M'),
      speedKph_(speedKph),
      speedKphUnit_('K') {}

std::unique_ptr<nmealib::Message> VWR::clone() const {
    return std::unique_ptr<VWR>(new VWR(*this));
}

std::string VWR::getStringContent(bool verbose) const noexcept {
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
        ss << "\tWind Angle: " << windAngle_ << "\n";
        ss << "\tWind Side: " << windSide_ << "\n";
        ss << "\tSpeed (Knots): " << speedKnots_;
        if (speedKnotsUnit_ != '\0') {
            ss << " " << speedKnotsUnit_;
        }
        ss << "\n"
           << "\tSpeed (m/s): " << speedMps_;
        if (speedMpsUnit_ != '\0') {
            ss << " " << speedMpsUnit_;
        }
        ss << "\n"
           << "\tSpeed (KPH): " << speedKph_;
        if (speedKphUnit_ != '\0') {
            ss << " " << speedKphUnit_;
        }
    } else {
        ss << "[" << validity << "] " << typeToString(type_) << " " << getTalker() << " " << getSentenceType() << ": "
           << "Angle=" << windAngle_ << windSide_
           << ", Knots=" << speedKnots_
           << ", m/s=" << speedMps_
           << ", KPH=" << speedKph_;
    }

    return ss.str();
}

std::string VWR::composeRaw(const std::string& talkerId,
                            double windAngle,
                            char windSide,
                            double speedKnots,
                            double speedMps,
                            double speedKph) {
    std::ostringstream payloadStream;
    payloadStream << talkerId << "VWR,";
    payloadStream << std::fixed << std::setprecision(1) << windAngle << ",";
    payloadStream << windSide << ",";
    payloadStream << std::fixed << std::setprecision(1) << speedKnots << ",N,";
    payloadStream << std::fixed << std::setprecision(1) << speedMps << ",M,";
    payloadStream << std::fixed << std::setprecision(1) << speedKph << ",K";

    std::string payload = payloadStream.str();
    return "$" + payload + "\r\n";
}

double VWR::getWindAngle() const noexcept {
    return windAngle_;
}

char VWR::getWindSide() const noexcept {
    return windSide_;
}

double VWR::getSpeedKnots() const noexcept {
    return speedKnots_;
}

char VWR::getSpeedKnotsUnit() const noexcept {
    return speedKnotsUnit_;
}

double VWR::getSpeedMps() const noexcept {
    return speedMps_;
}

char VWR::getSpeedMpsUnit() const noexcept {
    return speedMpsUnit_;
}

double VWR::getSpeedKph() const noexcept {
    return speedKph_;
}

char VWR::getSpeedKphUnit() const noexcept {
    return speedKphUnit_;
}

bool VWR::operator==(const VWR& other) const noexcept {
    return Message0183::operator==(other);
}

bool VWR::hasEqualContent(const VWR& other) const noexcept {
    return Message0183::hasEqualContent(other);
}

} // namespace nmea0183
} // namespace nmealib
