#include "nmealib/nmea2000/nmea2000Factory.hpp"

#include "nmealib/nmea2000/messageRegistry.hpp"

namespace nmealib {
namespace nmea2000 {

std::unique_ptr<Message2000> Nmea2000Factory::create(const std::string& raw, Message::TimePoint ts) {
    auto baseMessage = Message2000::create(raw, ts);
    std::uint32_t pgn = baseMessage->getPgn();

    // Dispatch through the message registry
    // The registry was populated with all known message types at library initialization
    return MessageRegistry::instance().create(pgn, std::move(baseMessage));
}

} // namespace nmea2000
} // namespace nmealib
