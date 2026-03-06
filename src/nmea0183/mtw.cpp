#include "nmealib/nmea0183/mtw.h"

#include <iomanip>
#include <sstream>
#include <vector>

namespace nmealib {
namespace nmea0183 {

std::unique_ptr<MTW> MTW::create(std::unique_ptr<Message0183> baseMessage) {
    std::string context = "MTW::create";
    if (baseMessage->getSentenceType() != "MTW") {
        throw NotMTWException(context, "Expected sentence type 'MTW', got " + baseMessage->getSentenceType());
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
        throw NotMTWException(context, "Invalid fields in MTW payload: expected 2, got " + std::to_string(fields.size()) + ". Payload: " + payload);
    }

    try {
        double temperature = fields[0].empty() ? 0.0 : std::stod(fields[0]);
        char unit = fields[1].empty() ? '\0' : fields[1][0];

        return std::unique_ptr<MTW>(new MTW(std::move(*baseMessage),
                                            temperature,
                                            unit));
    } catch (const std::exception& e) {
        throw NmeaException(context, "Error parsing MTW fields: " + std::string(e.what()));
    }
}

MTW::MTW(Message0183 baseMessage,
         double temperature,
         char unit) noexcept
    : Message0183(std::move(baseMessage)),
      temperature_(temperature),
      unit_(unit) {}

MTW::MTW(std::string talkerId,
         double temperature,
         char unit)
    : Message0183(*Message0183::create(composeRaw(talkerId, temperature, unit))),
      temperature_(temperature),
      unit_(unit) {}

std::unique_ptr<nmealib::Message> MTW::clone() const {
    return std::unique_ptr<MTW>(new MTW(*this));
}

std::string MTW::getStringContent(bool verbose) const noexcept {
    std::ostringstream ss;
    std::string validity = "KO";
    if (validate()) {
        validity = "OK";
    }
    ss << std::fixed << std::setprecision(2);

    if (verbose) {
        ss << "Protocol: " << typeToString(type_) << "\n";
        ss << "Talker: " << getTalker() << "\n";
        ss << "Sentence Type: " << getSentenceType() << "\n";
        ss << "Checksum: " << (checksumStr_.empty() ? "None" : validity) << "\n";
        ss << "Fields:\n";
        ss << "\tTemperature: " << temperature_ << "\n";
        ss << "\tUnit: " << unit_;
    } else {
        ss << "[" << validity << "] " << typeToString(type_) << " " << getTalker() << " " << getSentenceType() << ": "
           << "Temperature=" << temperature_ << unit_;
    }

    return ss.str();
}

std::string MTW::composeRaw(const std::string& talkerId,
                             double temperature,
                             char unit) {
    std::ostringstream payloadStream;
    payloadStream << talkerId << "MTW,";
    payloadStream << std::fixed << std::setprecision(1) << temperature << ",";
    payloadStream << unit;

    std::string payload = payloadStream.str();
    return "$" + payload + "\r\n";
}

double MTW::getTemperature() const noexcept {
    return temperature_;
}

char MTW::getUnit() const noexcept {
    return unit_;
}

bool MTW::operator==(const MTW& other) const noexcept {
    return Message0183::operator==(other);
}

bool MTW::hasEqualContent(const MTW& other) const noexcept {
    return Message0183::hasEqualContent(other);
}

} // namespace nmea0183
} // namespace nmealib
