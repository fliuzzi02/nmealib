#include "messageRegistry.hpp"
#include "nmealib/nmea0183/apb.h"
#include "nmealib/nmea0183/dbt.h"
#include "nmealib/nmea0183/gga.h"
#include "nmealib/nmea0183/gll.h"
#include "nmealib/nmea0183/gsa.h"
#include "nmealib/nmea0183/hdm.h"
#include "nmealib/nmea0183/mtw.h"
#include "nmealib/nmea0183/mwv.h"
#include "nmealib/nmea0183/rmc.h"
#include "nmealib/nmea0183/vhw.h"
#include "nmealib/nmea0183/vtg.h"
#include "nmealib/nmea0183/vwr.h"
#include "nmealib/nmea0183/zda.h"

namespace nmealib {
namespace nmea0183 {

MessageRegistry& MessageRegistry::instance() {
    static MessageRegistry registry;
    return registry;
}

std::unique_ptr<Message0183> MessageRegistry::create(const std::string& sentenceType,
                                                      std::unique_ptr<Message0183> baseMessage) {
    // TODO: Consider using a map of string to function pointer for better scalability if many sentence types are supported
    if (sentenceType == "APB") {
        return APB::create(std::move(baseMessage));
    } else if (sentenceType == "DBT") {
        return DBT::create(std::move(baseMessage));
    } else if (sentenceType == "GGA") {
        return GGA::create(std::move(baseMessage));
    } else if (sentenceType == "GLL") {
        return GLL::create(std::move(baseMessage));
    } else if (sentenceType == "GSA") {
        return GSA::create(std::move(baseMessage));
    } else if (sentenceType == "HDM") {
        return HDM::create(std::move(baseMessage));
    } else if (sentenceType == "MTW") {
        return MTW::create(std::move(baseMessage));
    } else if (sentenceType == "MWV") {
        return MWV::create(std::move(baseMessage));
    } else if (sentenceType == "RMC") {
        return RMC::create(std::move(baseMessage));
    } else if (sentenceType == "VHW") {
        return VHW::create(std::move(baseMessage));
    } else if (sentenceType == "VTG") {
        return VTG::create(std::move(baseMessage));
    } else if (sentenceType == "VWR") {
        return VWR::create(std::move(baseMessage));
    } else if (sentenceType == "ZDA") {
        return ZDA::create(std::move(baseMessage));
    } else {
        // Return generic Message0183 if type not registered
        return baseMessage;
    }
}

} // namespace nmea0183
} // namespace nmealib
