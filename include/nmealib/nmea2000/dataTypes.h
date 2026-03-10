#pragma once

#include <cstdint>
#include <string>
#include <iomanip>
#include <sstream>
#include <cmath>

namespace nmealib {
namespace nmea2000 {

class OutOfRangeException : public NmeaException {
public:
    explicit OutOfRangeException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "The value exceeds the valid range: ", details) {}
};

/**
 * @brief Strongly typed numeric wrapper for NMEA 2000 custom data types.
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
 * using Distance = nmealib::nmea2000::DataType<DistanceTraits>;
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
class DataType {
public:
    /** @brief Minimum representable value for this type. */
    static constexpr double MIN = Traits::MIN;
    /** @brief Maximum representable value for this type. */
    static constexpr double MAX = Traits::MAX;
    /** @brief Resolution (least significant physical unit step). */
    static constexpr double RESOLUTION = Traits::RESOLUTION;
    /** @brief Raw external type. */
    using RawType = typename Traits::RawType;
    /** @brief Target type for higher-level fields that expect a specific physical type. */
    using TargetType = typename Traits::TargetType;

    /**
     * @brief Constructs from a physical value.
     *
     * The input is clamped to [`MIN`, `MAX`] and rounded to the nearest
     * multiple of `RESOLUTION`.
     *
     * @param value Physical value.
     */
    static constexpr DataType fromValue(TargetType value) {
        if (value < MIN || value > MAX) {
            throw OutOfRangeException("DataType::fromValue", "["+std::to_string(MIN)+", "+std::to_string(MAX)+"]");
        }
        // Normalize to resolution by rounding to nearest multiple of RESOLUTION
        return DataType(static_cast<TargetType>(std::round(value / RESOLUTION) * RESOLUTION));
    }

    /**
     * @brief Constructs from a raw value.
     * 
     * The raw value is converted to physical units by multiplying with `RESOLUTION`.
     * 
     * @param raw Raw integer value representing the physical quantity. 
     */
    static constexpr DataType fromRaw(RawType raw) noexcept {
        // Convert raw value to physical units
        TargetType physicalValue = static_cast<TargetType>(raw) * RESOLUTION;
        // Normalize to resolution by rounding to nearest multiple of RESOLUTION
        TargetType normalizedValue = static_cast<TargetType>(std::round(physicalValue / RESOLUTION) * RESOLUTION);
        return DataType(normalizedValue);
    }

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
    constexpr TargetType getValue() const noexcept { return value_; }
    
    /**
     * @brief Converts physical value to raw integer representation.
     *
     * The conversion follows: `raw = round(value / RESOLUTION)`.
     *
     * @return Raw integer value suitable for payload encoding.
     */
    RawType getRaw() const noexcept {
        return static_cast<RawType>(std::round(value_ / RESOLUTION));
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

    constexpr bool operator==(const DataType& other) const noexcept {
        return value_ == other.value_;
    }
    constexpr bool operator!=(const DataType& other) const noexcept {
        return value_ != other.value_;
    }
    constexpr bool operator<(const DataType& other) const noexcept {
        return value_ < other.value_;
    }
    constexpr bool operator<=(const DataType& other) const noexcept {
        return value_ <= other.value_;
    }
    constexpr bool operator>(const DataType& other) const noexcept {
        return value_ > other.value_;
    }
    constexpr bool operator>=(const DataType& other) const noexcept {
        return value_ >= other.value_;
    }

private:
    /** @brief Stored normalized physical value. */
    TargetType value_;

    // Block default construction without parameters to enforce explicit initialization
    constexpr DataType(TargetType value) noexcept : value_(value) {}

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

struct DistanceTraits { static constexpr double MIN = 0, MAX = 4.295e7, RESOLUTION = 1e-2;  using RawType = uint32_t; using TargetType = float;};
struct SpeedTraits { static constexpr double MIN = 0, MAX = 655.32, RESOLUTION = 1e-2; using RawType = uint16_t; using TargetType = float;};
struct HalfByteTraits { static constexpr double MIN = 0, MAX = 15, RESOLUTION = 1; using RawType = uint8_t; using TargetType = uint8_t;};
struct ByteTraits { static constexpr double MIN = 0, MAX = 255, RESOLUTION = 1; using RawType = uint8_t; using TargetType = uint8_t; };

/**
 * @brief Custom type representing distances in meters.
 * 
 * - Range: [0, 4.295e7] m
 * 
 * - Resolution: 0.01 m
 * 
 * - Raw type: uint32_t
 * 
 * - Target type: float (for use in higher-level fields that expect a distance value)
 */
using Distance = DataType<DistanceTraits>;

/**
 * @brief Custom type representing speeds in meters per second.
 * 
 * - Range: [0, 655.32] m/s
 * 
 * - Resolution: 0.01 m/s
 * 
 * - Raw type: uint16_t
 * 
 * - Target type: float (for use in higher-level fields that expect a speed value)
 */
using Speed = DataType<SpeedTraits>;

/**
 * @brief Custom type representing 4 bits of data (0-15).
 * 
 * - Range: [0, 15]
 * 
 * - Resolution: 1
 * 
 * - Raw type: uint8_t
 * 
 * - Target type: uint8_t (for use in higher-level fields that expect a nibble value)
 */
using HalfByte = DataType<HalfByteTraits>;

/**
 * @brief Custom type representing 8 bits of data (0-255).
 * 
 * - Range: [0, 255]
 * 
 * - Resolution: 1
 * 
 * - Raw type: uint8_t
 * 
 * - Target type: uint8_t (for use in higher-level fields that expect a byte value)
 */
using Byte = DataType<ByteTraits>;

} // namespace nmea2000
} // namespace nmealib