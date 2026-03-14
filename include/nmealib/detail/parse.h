#pragma once

#include <charconv>
#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <string>
#include <type_traits>

namespace nmealib {
namespace detail {

template<typename T>
bool parseUnsigned(const std::string& text, T& value, int base = 10) noexcept {
    static_assert(std::is_integral_v<T> && std::is_unsigned_v<T>);

    if (text.empty()) {
        return false;
    }

    T parsed{};
    const char* begin = text.data();
    const char* end = text.data() + text.size();
    const auto result = std::from_chars(begin, end, parsed, base);
    if (result.ec != std::errc() || result.ptr != end) {
        return false;
    }

    value = parsed;
    return true;
}

template<typename T>
bool parseSigned(const std::string& text, T& value, int base = 10) noexcept {
    static_assert(std::is_integral_v<T> && std::is_signed_v<T>);

    if (text.empty()) {
        return false;
    }

    T parsed{};
    const char* begin = text.data();
    const char* end = text.data() + text.size();
    const auto result = std::from_chars(begin, end, parsed, base);
    if (result.ec != std::errc() || result.ptr != end) {
        return false;
    }

    value = parsed;
    return true;
}

inline bool parseDouble(const std::string& text, double& value) noexcept {
    if (text.empty()) {
        return false;
    }

    char* end = nullptr;
    errno = 0;
    const double parsed = std::strtod(text.c_str(), &end);
    if (errno == ERANGE || end != text.c_str() + text.size()) {
        return false;
    }

    value = parsed;
    return true;
}

inline bool parseLongDouble(const std::string& text, long double& value) noexcept {
    if (text.empty()) {
        return false;
    }

    char* end = nullptr;
    errno = 0;
    const long double parsed = std::strtold(text.c_str(), &end);
    if (errno == ERANGE || end != text.c_str() + text.size()) {
        return false;
    }

    value = parsed;
    return true;
}

inline bool parseOptionalDouble(const std::string& text, double& value) noexcept {
    if (text.empty()) {
        value = 0.0;
        return true;
    }
    return parseDouble(text, value);
}

inline bool parseOptionalLongDouble(const std::string& text, long double& value) noexcept {
    if (text.empty()) {
        value = 0.0L;
        return true;
    }
    return parseLongDouble(text, value);
}

inline bool parseOptionalUnsigned(const std::string& text, unsigned int& value) noexcept {
    if (text.empty()) {
        value = 0U;
        return true;
    }
    return parseUnsigned(text, value);
}

inline bool parseOptionalInt(const std::string& text, int& value) noexcept {
    if (text.empty()) {
        value = 0;
        return true;
    }
    return parseSigned(text, value);
}

inline bool parseNmeaCoordinate(const std::string& text, double& value) noexcept {
    if (text.empty()) {
        value = 0.0;
        return true;
    }

    long double raw = 0.0L;
    if (!parseLongDouble(text, raw)) {
        return false;
    }

    const long double degrees = std::floor(raw / 100.0L);
    const long double minutes = raw - (degrees * 100.0L);
    value = static_cast<double>(degrees + (minutes / 60.0L));
    return true;
}

} // namespace detail
} // namespace nmealib