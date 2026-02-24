#pragma once

/**
 * @file nmealib.hpp
 * @brief Umbrella header for the nmealib library.
 *
 * Including this single header provides access to all public types and
 * functions exported by the library:
 *  - nmealib::Message              (base message class)
 *  - nmealib::NmeaException        (base exception class)
 *  - nmealib::nmea0183::Message0183 and its associated exceptions
 *  - nmealib::nmea0183::GGA, GLL, GSA, MWV, RMC, VTG, ZDA (typed sentence classes)
 *  - nmealib::nmea0183::Nmea0183Factory (factory for creating typed messages)
 *
 * Example usage:
 * @code
 * #include <nmealib/nmealib.hpp>
 *
 * auto msg = nmealib::nmea0183::Nmea0183Factory::create("$GPGGA,...*47\r\n");
 * @endcode
 */

#include "message.hpp"
#include "nmeaException.hpp"
#include "nmea0183/nmea0183Factory.hpp"
#include "nmea0183/rmc.hpp"
#include "nmea0183/gga.hpp"
#include "nmea0183/gll.hpp"
#include "nmea0183/gsa.hpp"
#include "nmea0183/mwv.hpp"
#include "nmea0183/vtg.hpp"
#include "nmea0183/zda.hpp"
