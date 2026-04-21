#include "messageRegistry.hpp"

#include "nmealib/nmea2000/PGN128001.h"
#include "nmealib/nmea2000/PGN128259.h"
#include "nmealib/nmea2000/PGN127245.h"
#include "nmealib/nmea2000/PGN127250.h"
#include "nmealib/nmea2000/PGN127251.h"
#include "nmealib/nmea2000/PGN127257.h"
#include "nmealib/nmea2000/PGN129025.h"
#include "nmealib/nmea2000/PGN129026.h"
#include "nmealib/nmea2000/PGN130306.h"

namespace nmealib {
namespace nmea2000 {

MessageRegistry& MessageRegistry::instance() {
    static MessageRegistry registry;
    return registry;
}

std::unique_ptr<Message2000> MessageRegistry::create(std::uint32_t pgn,
                                                      std::unique_ptr<Message2000> baseMessage) {
    if (pgn == 128001) {
        return PGN128001::create(std::move(baseMessage));
    } else if (pgn == 128259) {
        return PGN128259::create(std::move(baseMessage));
    } else if (pgn == 127245) {
        return PGN127245::create(std::move(baseMessage));
    } else if (pgn == 127250) {
        return PGN127250::create(std::move(baseMessage));
    } else if (pgn == 127251) {
        return PGN127251::create(std::move(baseMessage));
    } else if (pgn == 127257) {
        return PGN127257::create(std::move(baseMessage));
    } else if (pgn == 129025) {
        return PGN129025::create(std::move(baseMessage));
    } else if (pgn == 129026) {
        return PGN129026::create(std::move(baseMessage));
    } else if (pgn == 130306) {
        return PGN130306::create(std::move(baseMessage));
    } else {
        return baseMessage;
    }
}

} // namespace nmea2000
} // namespace nmealib
