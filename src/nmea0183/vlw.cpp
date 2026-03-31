#include "nmealib/nmea0183/vlw.h"

#include "nmealib/detail/errorSupport.h"
#include "nmealib/detail/parse.h"

#include <iomanip>
#include <sstream>
#include <vector>

namespace nmealib {
namespace nmea0183 {

std::unique_ptr<VLW> VLW::create(std::unique_ptr<Message0183> baseMessage) {
    std::string context = "VLW::create";
    if (baseMessage->getSentenceType() != "VLW") {
        NMEALIB_RETURN_ERROR(NotVLWException(context, "Expected sentence type 'VLW', got " + baseMessage->getSentenceType()));
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

    if (fields.size() != 4 && fields.size() != 8) {
        NMEALIB_RETURN_ERROR(NotVLWException(context, "Invalid fields in VLW payload: expected 4 or 8, got " + std::to_string(fields.size()) + ". Payload: " + payload));
    }

    double totalWaterDistanceNm = 0.0;
    double waterDistanceSinceResetNm = 0.0;
    if (!detail::parseOptionalDouble(fields[0], totalWaterDistanceNm) ||
        !detail::parseOptionalDouble(fields[2], waterDistanceSinceResetNm)) {
        NMEALIB_RETURN_ERROR(NmeaException(context, "Error parsing VLW water-distance fields"));
    }

    char totalWaterDistanceUnit = fields[1].empty() ? '\0' : fields[1][0];
    char waterDistanceSinceResetUnit = fields[3].empty() ? '\0' : fields[3][0];

    bool hasGroundDistanceData = fields.size() == 8;
    double totalGroundDistanceNm = 0.0;
    char totalGroundDistanceUnit = '\0';
    double groundDistanceSinceResetNm = 0.0;
    char groundDistanceSinceResetUnit = '\0';

    if (hasGroundDistanceData) {
        if (!detail::parseOptionalDouble(fields[4], totalGroundDistanceNm) ||
            !detail::parseOptionalDouble(fields[6], groundDistanceSinceResetNm)) {
            NMEALIB_RETURN_ERROR(NmeaException(context, "Error parsing VLW ground-distance fields"));
        }

        totalGroundDistanceUnit = fields[5].empty() ? '\0' : fields[5][0];
        groundDistanceSinceResetUnit = fields[7].empty() ? '\0' : fields[7][0];
    }

    return std::unique_ptr<VLW>(new VLW(std::move(*baseMessage),
                                        totalWaterDistanceNm,
                                        totalWaterDistanceUnit,
                                        waterDistanceSinceResetNm,
                                        waterDistanceSinceResetUnit,
                                        hasGroundDistanceData,
                                        totalGroundDistanceNm,
                                        totalGroundDistanceUnit,
                                        groundDistanceSinceResetNm,
                                        groundDistanceSinceResetUnit));
}

VLW::VLW(Message0183 baseMessage,
         double totalWaterDistanceNm,
         char totalWaterDistanceUnit,
         double waterDistanceSinceResetNm,
         char waterDistanceSinceResetUnit,
         bool hasGroundDistanceData,
         double totalGroundDistanceNm,
         char totalGroundDistanceUnit,
         double groundDistanceSinceResetNm,
         char groundDistanceSinceResetUnit) noexcept
    : Message0183(std::move(baseMessage)),
      totalWaterDistanceNm_(totalWaterDistanceNm),
      totalWaterDistanceUnit_(totalWaterDistanceUnit),
      waterDistanceSinceResetNm_(waterDistanceSinceResetNm),
      waterDistanceSinceResetUnit_(waterDistanceSinceResetUnit),
      hasGroundDistanceData_(hasGroundDistanceData),
      totalGroundDistanceNm_(totalGroundDistanceNm),
      totalGroundDistanceUnit_(totalGroundDistanceUnit),
      groundDistanceSinceResetNm_(groundDistanceSinceResetNm),
      groundDistanceSinceResetUnit_(groundDistanceSinceResetUnit) {}

VLW::VLW(std::string talkerId,
         double totalWaterDistanceNm,
         double waterDistanceSinceResetNm,
         std::optional<double> totalGroundDistanceNm,
         std::optional<double> groundDistanceSinceResetNm)
    : Message0183(*Message0183::create(composeRaw(talkerId,
                                                  totalWaterDistanceNm,
                                                  waterDistanceSinceResetNm,
                                                  totalGroundDistanceNm,
                                                  groundDistanceSinceResetNm))),
      totalWaterDistanceNm_(totalWaterDistanceNm),
      totalWaterDistanceUnit_('N'),
      waterDistanceSinceResetNm_(waterDistanceSinceResetNm),
      waterDistanceSinceResetUnit_('N'),
      hasGroundDistanceData_(totalGroundDistanceNm.has_value() && groundDistanceSinceResetNm.has_value()),
      totalGroundDistanceNm_(totalGroundDistanceNm.value_or(0.0)),
      totalGroundDistanceUnit_(hasGroundDistanceData_ ? 'N' : '\0'),
      groundDistanceSinceResetNm_(groundDistanceSinceResetNm.value_or(0.0)),
      groundDistanceSinceResetUnit_(hasGroundDistanceData_ ? 'N' : '\0') {}

std::unique_ptr<nmealib::Message> VLW::clone() const {
    return std::unique_ptr<VLW>(new VLW(*this));
}

std::string VLW::getStringContent(bool verbose) const noexcept {
    std::ostringstream ss;
    ss << this->toString(verbose);
    ss << std::fixed << std::setprecision(2);

    if (verbose) {
        ss << "\tTotal Water Distance: " << totalWaterDistanceNm_ << " " << totalWaterDistanceUnit_ << "\n";
        ss << "\tWater Distance Since Reset: " << waterDistanceSinceResetNm_ << " " << waterDistanceSinceResetUnit_;
        if (hasGroundDistanceData_) {
            ss << "\n"
               << "\tTotal Ground Distance: " << totalGroundDistanceNm_ << " " << totalGroundDistanceUnit_ << "\n"
               << "\tGround Distance Since Reset: " << groundDistanceSinceResetNm_ << " " << groundDistanceSinceResetUnit_;
        }
        ss << "\n";
    } else {
        ss << "WaterTotal=" << totalWaterDistanceNm_
           << ", WaterReset=" << waterDistanceSinceResetNm_;
        if (hasGroundDistanceData_) {
            ss << ", GroundTotal=" << totalGroundDistanceNm_
               << ", GroundReset=" << groundDistanceSinceResetNm_;
        }
    }

    return ss.str();
}

std::string VLW::composeRaw(const std::string& talkerId,
                            double totalWaterDistanceNm,
                            double waterDistanceSinceResetNm,
                            std::optional<double> totalGroundDistanceNm,
                            std::optional<double> groundDistanceSinceResetNm) {
    const bool hasGroundDistanceData = totalGroundDistanceNm.has_value() && groundDistanceSinceResetNm.has_value();

    std::ostringstream payloadStream;
    payloadStream << talkerId << "VLW,";
    payloadStream << std::fixed << std::setprecision(1) << totalWaterDistanceNm << ",N,";
    payloadStream << std::fixed << std::setprecision(1) << waterDistanceSinceResetNm << ",N";

    if (hasGroundDistanceData) {
        payloadStream << ",";
        payloadStream << std::fixed << std::setprecision(1) << totalGroundDistanceNm.value() << ",N,";
        payloadStream << std::fixed << std::setprecision(1) << groundDistanceSinceResetNm.value() << ",N";
    }

    std::string payload = payloadStream.str();
    return "$" + payload + "\r\n";
}

double VLW::getTotalWaterDistanceNm() const noexcept {
    return totalWaterDistanceNm_;
}

char VLW::getTotalWaterDistanceUnit() const noexcept {
    return totalWaterDistanceUnit_;
}

double VLW::getWaterDistanceSinceResetNm() const noexcept {
    return waterDistanceSinceResetNm_;
}

char VLW::getWaterDistanceSinceResetUnit() const noexcept {
    return waterDistanceSinceResetUnit_;
}

bool VLW::hasGroundDistanceData() const noexcept {
    return hasGroundDistanceData_;
}

double VLW::getTotalGroundDistanceNm() const noexcept {
    return totalGroundDistanceNm_;
}

char VLW::getTotalGroundDistanceUnit() const noexcept {
    return totalGroundDistanceUnit_;
}

double VLW::getGroundDistanceSinceResetNm() const noexcept {
    return groundDistanceSinceResetNm_;
}

char VLW::getGroundDistanceSinceResetUnit() const noexcept {
    return groundDistanceSinceResetUnit_;
}

bool VLW::operator==(const VLW& other) const noexcept {
    return Message0183::operator==(other);
}

} // namespace nmea0183
} // namespace nmealib
