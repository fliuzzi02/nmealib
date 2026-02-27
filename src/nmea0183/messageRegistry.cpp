#include "nmealib/nmea0183/messageRegistry.hpp"
#include "nmealib/nmea0183/dbt.hpp"
#include "nmealib/nmea0183/gga.hpp"
#include "nmealib/nmea0183/gll.hpp"
#include "nmealib/nmea0183/gsa.hpp"
#include "nmealib/nmea0183/mwv.hpp"
#include "nmealib/nmea0183/rmc.hpp"
#include "nmealib/nmea0183/vhw.hpp"
#include "nmealib/nmea0183/vtg.hpp"
#include "nmealib/nmea0183/zda.hpp"

namespace nmealib {
namespace nmea0183 {

MessageRegistry& MessageRegistry::instance() {
    static MessageRegistry registry;
    return registry;
}

std::unique_ptr<Message0183> MessageRegistry::create(const std::string& sentenceType,
                                                      std::unique_ptr<Message0183> baseMessage) {
    // TODO: Consider using a map of string to function pointer for better scalability if many sentence types are supported
    if (sentenceType == "DBT") {
        return createDBT(std::move(baseMessage));
    } else if (sentenceType == "RMC") {
        return createRMC(std::move(baseMessage));
    } else if (sentenceType == "GGA") {
        return createGGA(std::move(baseMessage));
    } else if (sentenceType == "GSA") {
        return createGSA(std::move(baseMessage));
    } else if (sentenceType == "GLL") {
        return createGLL(std::move(baseMessage));
    } else if (sentenceType == "MWV") {
        return createMWV(std::move(baseMessage));
    } else if (sentenceType == "VHW") {
        return createVHW(std::move(baseMessage));
    } else if (sentenceType == "VTG") {
        return createVTG(std::move(baseMessage));
    } else if (sentenceType == "ZDA") {
        return createZDA(std::move(baseMessage));
    } else {
        // Return generic Message0183 if type not registered
        return baseMessage;
    }
}

std::unique_ptr<Message0183> MessageRegistry::createDBT(std::unique_ptr<Message0183> baseMessage) {
    return DBT::create(std::move(baseMessage));
}

std::unique_ptr<Message0183> MessageRegistry::createRMC(std::unique_ptr<Message0183> baseMessage) {
    return RMC::create(std::move(baseMessage));
}

std::unique_ptr<Message0183> MessageRegistry::createGGA(std::unique_ptr<Message0183> baseMessage) {
    return GGA::create(std::move(baseMessage));
}

std::unique_ptr<Message0183> MessageRegistry::createGSA(std::unique_ptr<Message0183> baseMessage) {
    return GSA::create(std::move(baseMessage));
}

std::unique_ptr<Message0183> MessageRegistry::createGLL(std::unique_ptr<Message0183> baseMessage) {
    return GLL::create(std::move(baseMessage));
}

std::unique_ptr<Message0183> MessageRegistry::createMWV(std::unique_ptr<Message0183> baseMessage) {
    return MWV::create(std::move(baseMessage));
}

std::unique_ptr<Message0183> MessageRegistry::createVHW(std::unique_ptr<Message0183> baseMessage) {
    return VHW::create(std::move(baseMessage));
}

std::unique_ptr<Message0183> MessageRegistry::createVTG(std::unique_ptr<Message0183> baseMessage) {
    return VTG::create(std::move(baseMessage));
}

std::unique_ptr<Message0183> MessageRegistry::createZDA(std::unique_ptr<Message0183> baseMessage) {
    return ZDA::create(std::move(baseMessage));
}

} // namespace nmea0183
} // namespace nmealib
