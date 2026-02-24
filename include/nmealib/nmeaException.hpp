#pragma once

#include <string>
#include <stdexcept>

namespace nmealib {

/**
 * @brief Base exception class for all NMEA library errors.
 *
 * Derived from std::runtime_error. All NMEA-specific exceptions
 * inherit from this class, allowing callers to catch either the
 * base type or a more specific subtype.
 */
class NmeaException : public std::runtime_error {
public:
    /**
     * @brief Constructs an NmeaException with context, message, and optional details.
     *
     * @param context  A string identifying where the error occurred (e.g., function name).
     * @param message  A human-readable description of the error.
     * @param details  Optional extra information appended in parentheses.
     */
    explicit NmeaException(const std::string& context, const std::string& message, const std::string& details = "") : 
    std::runtime_error(context + ": " + message + (details.empty() ? "" : " (" + details + ")")) {}
};

} // namespace nmealib
