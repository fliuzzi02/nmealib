#include "messageRegistry.hpp"

#include "nmealib/nmea2000/PGN128259.h"
#include "nmealib/nmea2000/PGN127250.h"


namespace nmealib {
namespace nmea2000 {

MessageRegistry& MessageRegistry::instance() {
    static MessageRegistry registry;
    return registry;
}

std::unique_ptr<Message2000> MessageRegistry::create(std::uint32_t pgn,
                                                      std::unique_ptr<Message2000> baseMessage) {
    if (pgn == 128259) {
        return PGN128259::create(std::move(baseMessage));
    } else if (pgn == 127250) {
        return PGN127250::create(std::move(baseMessage));

    } else {
        return baseMessage;
    }
}

} // namespace nmea2000
} // namespace nmealib
