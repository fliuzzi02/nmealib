#pragma once

#include <nmealib/nmea2000.h>

#include <cstdint>
#include <memory>

namespace nmealib {
namespace nmea2000 {

class MessageRegistry {
public:
    static MessageRegistry& instance();

    std::unique_ptr<Message2000> create(std::uint32_t pgn,
                                        std::unique_ptr<Message2000> baseMessage);

private:
    MessageRegistry() = default;

    friend class PGN129029;
    friend class PGN127250;
};

} // namespace nmea2000
} // namespace nmealib
