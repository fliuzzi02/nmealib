#include "nmealib/nmeaException.h"

namespace nmealib {

NmeaException::NmeaException(const std::string& context, const std::string& message, const std::string& details)
    : std::runtime_error(context + ": " + message + (details.empty() ? "" : " (" + details + ")")) {
}

} // namespace nmealib
