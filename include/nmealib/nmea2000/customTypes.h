#pragma once

#include <cstdint>
#include <string>
#include <iomanip>
#include <sstream>
#include <cmath>

namespace nmealib {
namespace nmea2000 {

/**
 * @brief Strongly typed numeric wrapper for NMEA 2000 custom field types.
 *
 * This template models NMEA 2000 custom numeric types where a physical value
 * is represented with:
 * - a valid range (`MIN`, `MAX`),
 * - a fixed resolution (`RESOLUTION`),
 * - conversion to/from raw integer representation.
 *
 * Internally, values are stored as `double` and normalized to the configured
 * resolution in the constructor.
 *
 * @tparam Traits Traits type defining bounds and resolution.
 * 
 * Traits must define:
 * - static constexpr double MIN, MAX, RESOLUTION;
 *
 * ```cpp
 * struct DistanceTraits {
 *     static constexpr double MIN = 0.0;
 *     static constexpr double MAX = 4.295e7;
 *     static constexpr double RESOLUTION = 1e-2;
 * };
 *
 * using Distance = nmealib::nmea2000::CustomType<DistanceTraits>;
 *
 * // Construct from physical value (meters), auto-rounded to 0.01
 * Distance d(1234.5678);
 *
 * // Convert to raw integer for wire format (123456)
 * std::int64_t raw = d.toRawValue();
 *
 * // Reconstruct from raw integer
 * Distance parsed = Distance::fromRawValue(raw);
 *
 * // Readable formatted value and comparisons
 * std::string text = parsed.toString(); // "1234.57"
 * bool farther = parsed > Distance(1000.0);
 * ```
 */
template<typename Traits>
class CustomType {
public:
    /** @brief Minimum representable value for this type. */
    static constexpr double MIN = Traits::MIN;
    /** @brief Maximum representable value for this type. */
    static constexpr double MAX = Traits::MAX;
    /** @brief Resolution (least significant physical unit step). */
    static constexpr double RESOLUTION = Traits::RESOLUTION;

    /**
     * @brief Constructs a value initialized at @ref MIN.
     */
    constexpr CustomType() noexcept : value_(MIN) {}

    /**
     * @brief Constructs from a physical value.
     *
     * The input is clamped to [`MIN`, `MAX`] and rounded to the nearest
     * multiple of `RESOLUTION`.
     *
     * @param value Physical value.
     */
    explicit constexpr CustomType(double value) noexcept 
        : value_(clampAndRound(value)) {}

    /**
     * @brief Checks whether the stored value is within configured bounds.
     * @return `true` if `MIN <= value <= MAX`, otherwise `false`.
     */
    constexpr bool isValid() const noexcept {
        return value_ >= MIN && value_ <= MAX;
    }

    /**
     * @brief Returns the normalized physical value.
     * @return Value in physical units.
     */
    constexpr double get() const noexcept { return value_; }
    
    /**
     * @brief Converts physical value to raw integer representation.
     *
     * The conversion follows: `raw = round(value / RESOLUTION)`.
     *
     * @return Raw integer value suitable for payload encoding.
     */
    int64_t toRawValue() const noexcept {
        return static_cast<int64_t>(std::round(value_ / RESOLUTION));
    }
    
    /**
     * @brief Creates a value from raw integer representation.
     *
     * The conversion follows: `value = raw * RESOLUTION`.
     *
     * @param raw Raw integer value from payload.
     * @return Normalized custom type value.
     */
    static constexpr CustomType fromRawValue(int64_t raw) noexcept {
        return CustomType(raw * RESOLUTION);
    }
    
    /**
     * @brief Converts the value to a fixed-point string.
     *
     * The number of decimal digits is derived from `RESOLUTION`.
     *
     * @return String representation of the physical value.
     */
    std::string toString() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(getPrecision()) << value_;
        return oss.str();
    }

    /** @brief Equality comparison. */
    constexpr bool operator==(const CustomType& other) const noexcept {
        return value_ == other.value_;
    }
    /** @brief Inequality comparison. */
    constexpr bool operator!=(const CustomType& other) const noexcept {
        return value_ != other.value_;
    }
    /** @brief Strict less-than comparison. */
    constexpr bool operator<(const CustomType& other) const noexcept {
        return value_ < other.value_;
    }
    /** @brief Less-than-or-equal comparison. */
    constexpr bool operator<=(const CustomType& other) const noexcept {
        return value_ <= other.value_;
    }
    /** @brief Strict greater-than comparison. */
    constexpr bool operator>(const CustomType& other) const noexcept {
        return value_ > other.value_;
    }
    /** @brief Greater-than-or-equal comparison. */
    constexpr bool operator>=(const CustomType& other) const noexcept {
        return value_ >= other.value_;
    }

private:
    /** @brief Stored normalized physical value. */
    double value_;

    /**
     * @brief Clamps to range and rounds to the configured resolution.
     * @param val Input physical value.
     * @return Normalized physical value.
     */
    static constexpr double clampAndRound(double val) noexcept {
        double clamped = (val < MIN) ? MIN : (val > MAX) ? MAX : val;
        if constexpr (RESOLUTION != 1.0) {
            return std::round(clamped / RESOLUTION) * RESOLUTION;
        }
        return clamped;
    }

    /**
     * @brief Computes decimal precision from @ref RESOLUTION.
     * @return Number of fractional digits to print in @ref toString.
     */
    static constexpr int getPrecision() noexcept {
        if constexpr (RESOLUTION >= 1.0) return 0;
        int p = 0;
        double r = RESOLUTION;
        while (r < 1.0 && p < 15) { r *= 10; p++; }
        return p;
    }
};

/**
 * @brief Example traits for a distance quantity.
 *
 * Units and semantic interpretation are decided by the message field using it.
 */
struct DistanceTraits { static constexpr double MIN = 0, MAX = 4.295e7, RESOLUTION = 1e-2; };

/** @brief Distance custom type based on @ref DistanceTraits. */
using Distance = CustomType<DistanceTraits>;

} // namespace nmea2000
} // namespace nmealib