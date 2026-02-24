#pragma once

/**
 * @file nmealib.hpp
 * @brief Umbrella header for the nmealib library.
 *
 * Including this single header provides access to all public types and
 * functions exported by the library:
 *  - nmealib::Message          (base message class)
 *  - nmealib::NmeaException    (base exception class)
 *  - nmealib::nmea0183::Message0183 and its associated exceptions
 *
 * Example usage:
 * @code
 * #include <nmealib/nmealib.hpp>
 *
 * auto msg = nmealib::nmea0183::Message0183::create("$GPGGA,...*47\r\n");
 * @endcode
 */

#include "message.hpp"
#include "nmeaException.hpp"
#include "nmea0183/nmea0183.hpp"
