#pragma once

#include <nmealib/nmea0183.h>
#include <map>
#include <memory>
#include <string>

namespace nmealib {
namespace nmea0183 {

/**
 * @brief Registry for message type creators.
 *
 * The MessageRegistry implements the factory pattern for NMEA 0183 message types.
 * Instead of hard-coding a dispatch if-else chain in the factory, message types
 * register themselves with the registry at library initialization time.
 *
 * This design scales efficiently for 50+ message types without compilation overhead.
 */
class MessageRegistry {
public:
    /**
     * @brief Get the singleton instance of the MessageRegistry.
     *
     * @return MessageRegistry& The global message registry instance.
     */
    static MessageRegistry& instance();

    /**
     * @brief Create a typed message from a generic Message0183.
     *
     * Dispatches to the appropriate creator method for the sentence type.
     * If the sentence type is not supported, returns the base Message0183 unchanged.
     *
     * @param sentenceType The three-character sentence type code.
     * @param baseMessage The generic Message0183 to convert.
     * @return std::unique_ptr<Message0183> The typed message, or baseMessage if type not registered.
     */
    std::unique_ptr<Message0183> create(const std::string& sentenceType,
                                        std::unique_ptr<Message0183> baseMessage);

private:
    /**
     * @brief Private constructor for singleton pattern.
     */
    MessageRegistry() = default;

    // Creator methods that have access to private create() functions through friendship
    std::unique_ptr<Message0183> createDBT(std::unique_ptr<Message0183> baseMessage);
    std::unique_ptr<Message0183> createRMC(std::unique_ptr<Message0183> baseMessage);
    std::unique_ptr<Message0183> createGGA(std::unique_ptr<Message0183> baseMessage);
    std::unique_ptr<Message0183> createGSA(std::unique_ptr<Message0183> baseMessage);
    std::unique_ptr<Message0183> createGLL(std::unique_ptr<Message0183> baseMessage);
    std::unique_ptr<Message0183> createMWV(std::unique_ptr<Message0183> baseMessage);
    std::unique_ptr<Message0183> createVHW(std::unique_ptr<Message0183> baseMessage);
    std::unique_ptr<Message0183> createVTG(std::unique_ptr<Message0183> baseMessage);
    std::unique_ptr<Message0183> createVWR(std::unique_ptr<Message0183> baseMessage);
    std::unique_ptr<Message0183> createZDA(std::unique_ptr<Message0183> baseMessage);

    // Forward declarations for friend declarations in message classes
    friend class DBT;
    friend class RMC;
    friend class GGA;
    friend class GSA;
    friend class GLL;
    friend class MWV;
    friend class VHW;
    friend class VTG;
    friend class VWR;
    friend class ZDA;
};

} // namespace nmea0183
} // namespace nmealib
