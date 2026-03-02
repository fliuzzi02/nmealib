#include "nmealib/nmea0183/nmea0183Factory.h"
#include "messageRegistry.hpp"

namespace nmealib {
namespace nmea0183 {

std::unique_ptr<Message0183> Nmea0183Factory::create(const std::string& raw, Message::TimePoint ts) {
    auto baseMessage = Message0183::create(raw, ts);
    std::string sentenceType = baseMessage->getSentenceType();

    // Dispatch through the message registry
    // The registry was populated with all known message types at library initialization
    return MessageRegistry::instance().create(sentenceType, std::move(baseMessage));
}

} // namespace nmea0183
} // namespace nmealib