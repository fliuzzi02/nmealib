#pragma once

/**
 * @file nmealib.h
 * @brief Umbrella header for the nmealib library.
 *
 * Including this single header provides access to all public types and
 * functions exported by the library:
 *  - nmealib::Message              (base message class)
 *  - nmealib::NmeaException        (base exception class)
 *  - nmealib::nmea0183::Message0183 and its associated exceptions
 *  - nmealib::nmea0183::DBT, GGA, GLL, GSA, MWV, RMC, VHW, VTG, ZDA (typed sentence classes)
 *  - nmealib::nmea0183::Nmea0183Factory (factory for creating typed messages)
 *  - nmealib::nmea2000::Message2000 and its associated exceptions
 *  - nmealib::nmea2000::Nmea2000Factory (factory for creating typed PGN messages)
 *
 * Example usage:
 * @code
 * #include <nmealib.h>
 *
 * auto msg = nmealib::nmea0183::Nmea0183Factory::create("$GPGGA,...*47\r\n");
 * @endcode
 */

#include "nmealib/message.h"
#include "nmealib/nmeaException.h"
#include "nmealib/nmea0183.h"
#include "nmealib/nmea2000.h"
#include "nmealib/nmea0183/nmea0183Factory.h"
#include "nmealib/nmea0183/dbt.h"
#include "nmealib/nmea0183/rmc.h"
#include "nmealib/nmea0183/gga.h"
#include "nmealib/nmea0183/gll.h"
#include "nmealib/nmea0183/gsa.h"
#include "nmealib/nmea0183/mwv.h"
#include "nmealib/nmea0183/vhw.h"
#include "nmealib/nmea0183/vtg.h"
#include "nmealib/nmea0183/zda.h"
#include "nmealib/nmea2000/nmea2000Factory.h"
