#pragma once

#include <nmealib/nmea2000.hpp>

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

    std::unique_ptr<Message2000> createPGN129029(std::unique_ptr<Message2000> baseMessage);

    friend class PGN129029;
};

} // namespace nmea2000
} // namespace nmealib
