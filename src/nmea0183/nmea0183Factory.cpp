#include "nmea0183Factory.hpp"

namespace nmealib {
namespace nmea0183 {

std::unique_ptr<Message0183> Nmea0183Factory::create(const std::string& raw, Message::TimePoint ts) {
    auto baseMessage = Message0183::create(raw, ts);
    std::string sentenceType = baseMessage->getSentenceType();

    if (sentenceType == "RMC") {
        return RMC::create(std::move(baseMessage));
    } else if (sentenceType == "GGA") {
        return GGA::create(std::move(baseMessage));
    } else if (sentenceType == "GSA") {
        return GSA::create(std::move(baseMessage));
    } else if (sentenceType == "GLL") {
        return GLL::create(std::move(baseMessage));
    } else if (sentenceType == "VTG") {
        return VTG::create(std::move(baseMessage));
    } else {
        return std::move(baseMessage);
    }
}

} // namespace nmea0183
} // namespace nmealib