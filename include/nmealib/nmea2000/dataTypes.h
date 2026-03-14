#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <limits>
#include <sstream>
#include <string>

namespace nmealib {
namespace nmea2000 {

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
    static DataType fromValue(TargetType value) {
        const double physicalValue = static_cast<double>(value);
        const double clampedValue = std::clamp(physicalValue, MIN, MAX);

        double scaled = clampedValue - MIN;
        scaled /= (MAX - MIN);
        scaled *= (static_cast<double>(Traits::RAW_MAX) - static_cast<double>(Traits::RAW_MIN));
        scaled += static_cast<double>(Traits::RAW_MIN);
        scaled = std::round(scaled / RESOLUTION) * RESOLUTION;

        return DataType(static_cast<RawType>(scaled));
    }

    /**
     * @brief Constructs from a raw value.
     * 
     * The raw value is converted to physical units by multiplying with `RESOLUTION`.
     * 
     * @param raw Raw integer value representing the physical quantity. 
     */
    static constexpr DataType fromRaw(RawType raw) noexcept {
        return DataType(raw);
    }

    /**
     * @brief Checks whether the stored value is within configured bounds.
     * @return `true` if `MIN <= value <= MAX`, otherwise `false`.
     */
    constexpr bool isValid() const noexcept {
        return value_ >= Traits::RAW_MIN && value_ <= Traits::RAW_MAX;
    }

    /**
     * @brief Returns the normalized physical value.
     * @return Value in physical units.
     */
    constexpr TargetType getValue() const noexcept {
        double scaled = static_cast<double>(value_) - static_cast<double>(Traits::RAW_MIN);
        scaled /= (static_cast<double>(Traits::RAW_MAX) - static_cast<double>(Traits::RAW_MIN));
        scaled *= (MAX - MIN);
        scaled += MIN;
        return static_cast<TargetType>(scaled);
    }
    
    /**
     * @brief Converts physical value to raw integer representation.
     *
     * The conversion follows: `raw = round(value / RESOLUTION)`.
     *
     * @return Raw integer value suitable for payload encoding.
     */
    RawType getRaw() const noexcept {
        return value_;
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
        oss << std::fixed << std::setprecision(getPrecision()) << getValue();
        return oss.str();
    }

    // Operators
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
    RawType value_;

    // Block default construction without parameters to enforce explicit initialization
    constexpr explicit DataType(RawType value) noexcept : value_(value) {}

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

struct DistanceTraits {
    static constexpr double MIN = 0;
    static constexpr double MAX = 4.295e7;
    static constexpr double RESOLUTION = 1e-2;
    using RawType = uint32_t;
    using TargetType = float;
    static constexpr RawType RAW_MIN = std::numeric_limits<RawType>::min();
    static constexpr RawType RAW_MAX = std::numeric_limits<RawType>::max();
};
struct SpeedTraits {
    static constexpr double MIN = 0;
    static constexpr double MAX = 655.32;
    static constexpr double RESOLUTION = 1e-2;
    using RawType = uint16_t;
    using TargetType = float;
    static constexpr RawType RAW_MIN = std::numeric_limits<RawType>::min();
    static constexpr RawType RAW_MAX = std::numeric_limits<RawType>::max();
};
struct AngleTraits {
    static constexpr double MIN = 0;
    static constexpr double MAX = 2 * M_PI;
    static constexpr double RESOLUTION = 1e-4;
    using RawType = uint16_t;
    using TargetType = float;
    static constexpr RawType RAW_MIN = std::numeric_limits<RawType>::min();
    static constexpr RawType RAW_MAX = std::numeric_limits<RawType>::max();
};
struct SignedAngleTraits {
    static constexpr double MIN = -M_PI;
    static constexpr double MAX = M_PI;
    static constexpr double RESOLUTION = 1e-4;
    using RawType = uint16_t;
    using TargetType = float;
    static constexpr RawType RAW_MIN = std::numeric_limits<RawType>::min();
    static constexpr RawType RAW_MAX = std::numeric_limits<RawType>::max();
};
struct HalfByteTraits {
    static constexpr double MIN = 0;
    static constexpr double MAX = 15;
    static constexpr double RESOLUTION = 1;
    using RawType = uint8_t;
    using TargetType = uint8_t;
    static constexpr RawType RAW_MIN = 0;
    static constexpr RawType RAW_MAX = 15;
};
struct ByteTraits {
    static constexpr double MIN = 0;
    static constexpr double MAX = 255;
    static constexpr double RESOLUTION = 1;
    using RawType = uint8_t;
    using TargetType = uint8_t;
    static constexpr RawType RAW_MIN = std::numeric_limits<RawType>::min();
    static constexpr RawType RAW_MAX = std::numeric_limits<RawType>::max();
};
struct LatitudeTraits {
    static constexpr double MIN = -90;
    static constexpr double MAX = 90;
    static constexpr double RESOLUTION = 1e-7;
    using RawType = int32_t;
    using TargetType = float;
    static constexpr RawType RAW_MIN = std::numeric_limits<RawType>::min();
    static constexpr RawType RAW_MAX = std::numeric_limits<RawType>::max();
};
struct LongitudeTraits {
    static constexpr double MIN = -180;
    static constexpr double MAX = 180;
    static constexpr double RESOLUTION = 1e-7;
    using RawType = int32_t;
    using TargetType = float;
    static constexpr RawType RAW_MIN = std::numeric_limits<RawType>::min();
    static constexpr RawType RAW_MAX = std::numeric_limits<RawType>::max();
};

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

/**
 * @brief Custom type representing angles in radians.
 * 
 * - Range: [0, 2π] rad
 * 
 * - Resolution: 0.0001 rad
 * 
 * - Raw type: uint16_t
 * 
 * - Target type: float (for use in higher-level fields that expect an angle value)
 */
using Angle = DataType<AngleTraits>;

/**
 * @brief Custom type representing signed angles in radians.
 * 
 * - Range: [-π, π] rad
 * 
 * - Resolution: 0.0001 rad
 * 
 * - Raw type: uint16_t
 * 
 * - Target type: float (for use in higher-level fields that expect a signed angle value)
 */
using SignedAngle = DataType<SignedAngleTraits>;

/**
 * @brief Custom type representing latitudes in degrees.
 * 
 * - Range: [-90, 90] deg
 * 
 * - Resolution: 0.0000001 deg
 * 
 * - Raw type: int32_t
 * 
 * - Target type: float (for use in higher-level fields that expect a latitude value)
 */
using Latitude = DataType<LatitudeTraits>;

/**
 * @brief Custom type representing longitudes in degrees.
 * 
 * - Range: [-180, 180] deg
 * 
 * - Resolution: 0.0000001 deg
 * 
 * - Raw type: int32_t
 * 
 * - Target type: float (for use in higher-level fields that expect a longitude value)
 */
using Longitude = DataType<LongitudeTraits>;

} // namespace nmea2000
} // namespace nmealib