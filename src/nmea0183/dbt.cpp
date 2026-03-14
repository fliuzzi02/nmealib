#include "nmealib/nmea0183/dbt.h"

#include <iomanip>
#include <sstream>
#include <vector>

namespace nmealib {
namespace nmea0183 {

std::unique_ptr<DBT> DBT::create(std::unique_ptr<Message0183> baseMessage) {
    std::string context = "DBT::create";
    if (baseMessage->getSentenceType() != "DBT") {
        throw NotDBTException(context, "Expected sentence type 'DBT', got " + baseMessage->getSentenceType());
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

    if (fields.size() != 6) {
        throw NotDBTException(context, "Invalid fields in DBT payload: expected 6, got " + std::to_string(fields.size()) + ". Payload: " + payload);
    }

    try {
        double depthFeet = fields[0].empty() ? 0.0 : std::stod(fields[0]);
        char feetUnit = fields[1].empty() ? '\0' : fields[1][0];
        double depthMeters = fields[2].empty() ? 0.0 : std::stod(fields[2]);
        char metersUnit = fields[3].empty() ? '\0' : fields[3][0];
        double depthFathoms = fields[4].empty() ? 0.0 : std::stod(fields[4]);
        char fathomsUnit = fields[5].empty() ? '\0' : fields[5][0];

        return std::unique_ptr<DBT>(new DBT(std::move(*baseMessage),
                                            depthFeet,
                                            feetUnit,
                                            depthMeters,
                                            metersUnit,
                                            depthFathoms,
                                            fathomsUnit));
    } catch (const std::exception& e) {
        throw NmeaException(context, "Error parsing DBT fields: " + std::string(e.what()));
    }
}

DBT::DBT(Message0183 baseMessage,
        double depthFeet,
        char feetUnit,
        double depthMeters,
        char metersUnit,
        double depthFathoms,
        char fathomsUnit) noexcept : 
        Message0183(std::move(baseMessage)),
        depthFeet_(depthFeet),
        feetUnit_(feetUnit),
        depthMeters_(depthMeters),
        metersUnit_(metersUnit),
        depthFathoms_(depthFathoms),
        fathomsUnit_(fathomsUnit) {}

DBT::DBT(std::string talkerId,
        double depthFeet,
        char feetUnit,
        double depthMeters,
        char metersUnit,
        double depthFathoms,
        char fathomsUnit) : 
        Message0183(*Message0183::create(composeRaw(talkerId,
                                                depthFeet,
                                                feetUnit,
                                                depthMeters,
                                                metersUnit,
                                                depthFathoms,
                                                fathomsUnit))),
        depthFeet_(depthFeet),
        feetUnit_(feetUnit),
        depthMeters_(depthMeters),
        metersUnit_(metersUnit),
        depthFathoms_(depthFathoms),
        fathomsUnit_(fathomsUnit) {}

std::unique_ptr<nmealib::Message> DBT::clone() const {
    return std::unique_ptr<DBT>(new DBT(*this));
}

std::string DBT::getStringContent(bool verbose) const noexcept {
    std::ostringstream ss;
    ss << this->toString(verbose);
    ss << std::fixed << std::setprecision(2);

    if (verbose) {
        ss << "\tDepth: " << depthFeet_ << " " << feetUnit_ << "\n";
        ss << "\tDepth (meters): " << depthMeters_ << " " << metersUnit_ << "\n";
        ss << "\tDepth (fathoms): " << depthFathoms_ << " " << fathomsUnit_;
        ss << "\n";
    } else {
        ss << "Depth=" << depthFeet_ << feetUnit_
           << ", " << depthMeters_ << metersUnit_
           << ", " << depthFathoms_ << fathomsUnit_;
    }
    return ss.str();
}

std::string DBT::composeRaw(const std::string& talkerId,
                             double depthFeet,
                             char feetUnit,
                             double depthMeters,
                             char metersUnit,
                             double depthFathoms,
                             char fathomsUnit) {
    std::ostringstream payloadStream;
    payloadStream << talkerId << "DBT,";
    payloadStream << std::fixed << std::setprecision(1) << depthFeet << ",";
    payloadStream << feetUnit << ",";
    payloadStream << std::fixed << std::setprecision(1) << depthMeters << ",";
    payloadStream << metersUnit << ",";
    payloadStream << std::fixed << std::setprecision(1) << depthFathoms << ",";
    payloadStream << fathomsUnit;

    std::string payload = payloadStream.str();
    return "$" + payload + "\r\n";
}

double DBT::getDepthFeet() const noexcept {
    return depthFeet_;
}

char DBT::getFeetUnit() const noexcept {
    return feetUnit_;
}

double DBT::getDepthMeters() const noexcept {
    return depthMeters_;
}

char DBT::getMetersUnit() const noexcept {
    return metersUnit_;
}

double DBT::getDepthFathoms() const noexcept {
    return depthFathoms_;
}

char DBT::getFathomsUnit() const noexcept {
    return fathomsUnit_;
}

bool DBT::operator==(const DBT& other) const noexcept {
    return Message0183::operator==(other);
}

} // namespace nmea0183
} // namespace nmealib
