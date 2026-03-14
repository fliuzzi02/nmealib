#include "nmealib/nmea0183/zda.h"

#include <cmath>
#include <iomanip>
#include <sstream>
#include <vector>

namespace nmealib {
namespace nmea0183 {

std::unique_ptr<ZDA> ZDA::create(std::unique_ptr<Message0183> baseMessage) {
    std::string context = "ZDA::create";
    if (baseMessage->getSentenceType() != "ZDA") {
        throw NotZDAException(context, "Expected sentence type 'ZDA', got " + baseMessage->getSentenceType());
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
        throw NotZDAException(context, "Invalid fields in ZDA payload: expected 6, got " + std::to_string(fields.size()) + ". Payload: " + payload);
    }

    try {
        double utcTime = fields[0].empty() ? 0.0 : std::stod(fields[0]);
        unsigned int day = fields[1].empty() ? 0u : std::stoul(fields[1]);
        unsigned int month = fields[2].empty() ? 0u : std::stoul(fields[2]);
        unsigned int year = fields[3].empty() ? 0u : std::stoul(fields[3]);
        int localZoneHours = fields[4].empty() ? 0 : std::stoi(fields[4]);
        int localZoneMinutes = fields[5].empty() ? 0 : std::stoi(fields[5]);

        return std::unique_ptr<ZDA>(new ZDA(std::move(*baseMessage),
                                            utcTime,
                                            day,
                                            month,
                                            year,
                                            localZoneHours,
                                            localZoneMinutes));
    } catch (const std::exception& e) {
        throw NmeaException(context, "Error parsing ZDA fields: " + std::string(e.what()));
    }
}

ZDA::ZDA(Message0183 baseMessage,
         double utcTime,
         unsigned int day,
         unsigned int month,
         unsigned int year,
         int localZoneHours,
         int localZoneMinutes) noexcept
    : Message0183(std::move(baseMessage)),
      utcTime_(utcTime),
      day_(day),
      month_(month),
      year_(year),
      localZoneHours_(localZoneHours),
      localZoneMinutes_(localZoneMinutes) {}

ZDA::ZDA(std::string talkerId,
         double utcTime,
         unsigned int day,
         unsigned int month,
         unsigned int year,
         int localZoneHours,
         int localZoneMinutes)
    : Message0183(*Message0183::create(composeRaw(talkerId,
                                                  utcTime,
                                                  day,
                                                  month,
                                                  year,
                                                  localZoneHours,
                                                  localZoneMinutes))),
      utcTime_(utcTime),
      day_(day),
      month_(month),
      year_(year),
      localZoneHours_(localZoneHours),
      localZoneMinutes_(localZoneMinutes) {}

std::unique_ptr<nmealib::Message> ZDA::clone() const {
    return std::unique_ptr<ZDA>(new ZDA(*this));
}

std::string ZDA::getStringContent(bool verbose) const noexcept {
    std::ostringstream ss;
    ss << this->toString(verbose);

    if (verbose) {
        ss << "\tUTC Time: " << utcTime_ << "\n";
        ss << "\tDay: " << day_ << "\n";
        ss << "\tMonth: " << month_ << "\n";
        ss << "\tYear: " << year_ << "\n";
        ss << "\tLocal Zone Hours: " << localZoneHours_ << "\n";
        ss << "\tLocal Zone Minutes: " << localZoneMinutes_;
        ss << "\n";
    } else {
        ss << "UTC=" << utcTime_
           << ", Date=" << day_ << "/" << month_ << "/" << year_
           << ", Zone=" << localZoneHours_ << ":";
        ss << std::setw(2) << std::setfill('0') << std::abs(localZoneMinutes_);
    }

    return ss.str();
}

std::string ZDA::composeRaw(const std::string& talkerId,
                            double utcTime,
                            unsigned int day,
                            unsigned int month,
                            unsigned int year,
                            int localZoneHours,
                            int localZoneMinutes) {
    std::ostringstream payloadStream;
    payloadStream << talkerId << "ZDA,";
    payloadStream << std::fixed << std::setprecision(2) << std::setw(9) << std::setfill('0') << utcTime << ",";
    payloadStream << std::setw(2) << std::setfill('0') << day << ",";
    payloadStream << std::setw(2) << std::setfill('0') << month << ",";
    payloadStream << std::setw(4) << std::setfill('0') << year << ",";
    payloadStream << localZoneHours << ",";
    payloadStream << std::setw(2) << std::setfill('0') << std::abs(localZoneMinutes);

    std::string payload = payloadStream.str();
    return "$" + payload + "\r\n";
}

double ZDA::getUtcTime() const noexcept {
    return utcTime_;
}

unsigned int ZDA::getDay() const noexcept {
    return day_;
}

unsigned int ZDA::getMonth() const noexcept {
    return month_;
}

unsigned int ZDA::getYear() const noexcept {
    return year_;
}

int ZDA::getLocalZoneHours() const noexcept {
    return localZoneHours_;
}

int ZDA::getLocalZoneMinutes() const noexcept {
    return localZoneMinutes_;
}

bool ZDA::operator==(const ZDA& other) const noexcept {
    return Message0183::operator==(other);
}

} // namespace nmea0183
} // namespace nmealib
