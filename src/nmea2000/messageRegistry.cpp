#include "nmealib/nmea2000/messageRegistry.hpp"

#include "nmealib/nmea2000/pgn129029.hpp"

namespace nmealib {
namespace nmea2000 {

MessageRegistry& MessageRegistry::instance() {
    static MessageRegistry registry;
    return registry;
}

std::unique_ptr<Message2000> MessageRegistry::create(std::uint32_t pgn,
                                                      std::unique_ptr<Message2000> baseMessage) {
    if (pgn == 129029) {
        return createPGN129029(std::move(baseMessage));
    }

    return baseMessage;
}

std::unique_ptr<Message2000> MessageRegistry::createPGN129029(std::unique_ptr<Message2000> baseMessage) {
    return PGN129029::create(std::move(baseMessage));
}

} // namespace nmea2000
} // namespace nmealib
