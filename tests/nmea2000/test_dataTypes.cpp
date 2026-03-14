#include <gtest/gtest.h>

#include <chrono>
#include <memory>
#include <string>
#include <limits>

#include <nmealib.h>

using namespace nmealib::nmea2000;

TEST(DataType, Angle) {
    auto min = Angle::fromRaw(std::numeric_limits<uint16_t>::min());
    EXPECT_EQ(min.getRaw(), std::numeric_limits<uint16_t>::min());
    EXPECT_EQ(min.getValue(), 0.0f);

    auto max = Angle::fromRaw(std::numeric_limits<uint16_t>::max());
    EXPECT_EQ(max.getRaw(), std::numeric_limits<uint16_t>::max());
    EXPECT_NEAR(max.getValue(), 6.2832f, 0.02f);

    auto minValue = Angle::fromValue(0.0f);
    EXPECT_EQ(minValue.getRaw(), std::numeric_limits<uint16_t>::min());
    EXPECT_EQ(minValue.getValue(), 0.0f);

    auto maxValue = Angle::fromValue(6.2832f);
    EXPECT_EQ(maxValue.getRaw(), std::numeric_limits<uint16_t>::max());
    EXPECT_NEAR(maxValue.getValue(), 6.2832f, 0.02f);

    // Test operators
    EXPECT_EQ(max, maxValue);
    EXPECT_EQ(min, minValue);
    EXPECT_TRUE(max > min);
    EXPECT_TRUE(min < max);
    EXPECT_TRUE(max >= min);
    EXPECT_TRUE(min <= max);
}

TEST(DataType, SignedAngle) {
    auto min = SignedAngle::fromRaw(std::numeric_limits<uint16_t>::min());
    EXPECT_EQ(min.getRaw(), std::numeric_limits<uint16_t>::min());
    EXPECT_NEAR(min.getValue(), -3.1416f, 0.02f);

    auto max = SignedAngle::fromRaw(std::numeric_limits<uint16_t>::max());
    EXPECT_EQ(max.getRaw(), std::numeric_limits<uint16_t>::max());
    EXPECT_NEAR(max.getValue(), 3.1416f, 0.02f);

    auto minValue = SignedAngle::fromValue(-3.1416f);
    EXPECT_EQ(minValue.getRaw(), std::numeric_limits<uint16_t>::min());
    EXPECT_NEAR(minValue.getValue(), -3.1416f, 0.02f);

    auto maxValue = SignedAngle::fromValue(3.1416f);
    EXPECT_EQ(maxValue.getRaw(), std::numeric_limits<uint16_t>::max());
    EXPECT_NEAR(maxValue.getValue(), 3.1416f, 0.02f);

    // Test operators
    EXPECT_EQ(max, maxValue);
    EXPECT_EQ(min, minValue);
    EXPECT_TRUE(max > min);
    EXPECT_TRUE(min < max);
    EXPECT_TRUE(max >= min);
    EXPECT_TRUE(min <= max);
}

TEST(DataType, Distance) {
    auto min = Distance::fromRaw(std::numeric_limits<uint32_t>::min());
    EXPECT_EQ(min.getRaw(), std::numeric_limits<uint32_t>::min());
    EXPECT_NEAR(min.getValue(), 0.0f, 0.02f);

    auto max = Distance::fromRaw(std::numeric_limits<uint32_t>::max());
    EXPECT_EQ(max.getRaw(), std::numeric_limits<uint32_t>::max());
    EXPECT_NEAR(max.getValue(), 42950000.0f, 0.02f);

    auto minValue = Distance::fromValue(0.0f);
    EXPECT_EQ(minValue.getRaw(), std::numeric_limits<uint32_t>::min());
    EXPECT_NEAR(minValue.getValue(), 0.0f, 0.02f);

    auto maxValue = Distance::fromValue(42950000.0f);
    EXPECT_EQ(maxValue.getRaw(), std::numeric_limits<uint32_t>::max());
    EXPECT_NEAR(maxValue.getValue(), 42950000.0f, 0.02f);

    // Test operators
    EXPECT_EQ(max, maxValue);
    EXPECT_EQ(min, minValue);
    EXPECT_TRUE(max > min);
    EXPECT_TRUE(min < max);
    EXPECT_TRUE(max >= min);
    EXPECT_TRUE(min <= max);
}

TEST(DataType, Speed) {
    auto min = Speed::fromRaw(std::numeric_limits<uint16_t>::min());
    EXPECT_EQ(min.getRaw(), std::numeric_limits<uint16_t>::min());
    EXPECT_NEAR(min.getValue(), 0.0f, 0.02f);

    auto max = Speed::fromRaw(std::numeric_limits<uint16_t>::max());
    EXPECT_EQ(max.getRaw(), std::numeric_limits<uint16_t>::max());
    EXPECT_NEAR(max.getValue(), 655.32f, 0.02f);

    auto minValue = Speed::fromValue(0.0f);
    EXPECT_EQ(minValue.getRaw(), std::numeric_limits<uint16_t>::min());
    EXPECT_NEAR(minValue.getValue(), 0.0f, 0.02f);

    auto maxValue = Speed::fromValue(655.32f);
    EXPECT_EQ(maxValue.getRaw(), std::numeric_limits<uint16_t>::max());
    EXPECT_NEAR(maxValue.getValue(), 655.32f, 0.02f);

    // Test operators
    EXPECT_EQ(max, maxValue);
    EXPECT_EQ(min, minValue);
    EXPECT_TRUE(max > min);
    EXPECT_TRUE(min < max);
    EXPECT_TRUE(max >= min);
    EXPECT_TRUE(min <= max);
}

TEST(DataType, Byte) {
    auto min = Byte::fromRaw(std::numeric_limits<uint8_t>::min());
    EXPECT_EQ(min.getRaw(), std::numeric_limits<uint8_t>::min());
    EXPECT_EQ(min.getValue(), 0);

    auto mean = Byte::fromRaw(127);
    EXPECT_EQ(mean.getRaw(), 127);
    EXPECT_EQ(mean.getValue(), 127);

    auto max = Byte::fromRaw(std::numeric_limits<uint8_t>::max());
    EXPECT_EQ(max.getRaw(), std::numeric_limits<uint8_t>::max());
    EXPECT_EQ(max.getValue(), 255);

    auto minValue = Byte::fromValue(0);
    EXPECT_EQ(minValue.getRaw(), std::numeric_limits<uint8_t>::min());
    EXPECT_EQ(minValue.getValue(), 0);

    auto meanValue = Byte::fromValue(127);
    EXPECT_EQ(meanValue.getRaw(), 127);
    EXPECT_EQ(meanValue.getValue(), 127);

    auto maxValue = Byte::fromValue(255);
    EXPECT_EQ(maxValue.getRaw(), std::numeric_limits<uint8_t>::max());
    EXPECT_EQ(maxValue.getValue(), 255);

    // Test operators
    EXPECT_EQ(max, maxValue);
    EXPECT_EQ(min, minValue);
    EXPECT_TRUE(max > min);
    EXPECT_TRUE(min < max);
    EXPECT_TRUE(max >= min);
    EXPECT_TRUE(min <= max);
}

TEST(DataType, HalfByte) {
    auto min = HalfByte::fromRaw(0);
    EXPECT_EQ(min.getRaw(), 0);
    EXPECT_EQ(min.getValue(), 0);

    auto mean = HalfByte::fromRaw(7);
    EXPECT_EQ(mean.getRaw(), 7);
    EXPECT_EQ(mean.getValue(), 7);

    auto max = HalfByte::fromRaw(15);
    EXPECT_EQ(max.getRaw(), 15);
    EXPECT_EQ(max.getValue(), 15);

    auto minValue = HalfByte::fromValue(0);
    EXPECT_EQ(minValue.getRaw(), 0);
    EXPECT_EQ(minValue.getValue(), 0);

    auto meanValue = HalfByte::fromValue(7);
    EXPECT_EQ(meanValue.getRaw(), 7);
    EXPECT_EQ(meanValue.getValue(), 7);

    auto maxValue = HalfByte::fromValue(15);
    EXPECT_EQ(maxValue.getRaw(), 15);
    EXPECT_EQ(maxValue.getValue(), 0b1111);

    // Test operators
    EXPECT_EQ(max, maxValue);
    EXPECT_EQ(min, minValue);
    EXPECT_TRUE(max > min);
    EXPECT_TRUE(min < max);
    EXPECT_TRUE(max >= min);
    EXPECT_TRUE(min <= max);
}

TEST(DataType, Latitude) {
    auto min = Latitude::fromRaw(-90 * static_cast<int32_t>(1e7));
    EXPECT_EQ(min.getRaw(), -90 * static_cast<int32_t>(1e7));
    EXPECT_NEAR(min.getValue(), -90.0f, 1e-4f);

    auto max = Latitude::fromRaw(90 * static_cast<int32_t>(1e7));
    EXPECT_EQ(max.getRaw(), 90 * static_cast<int32_t>(1e7));
    EXPECT_NEAR(max.getValue(), 90.0f, 1e-4f);

    auto minValue = Latitude::fromValue(-90.0f);
    EXPECT_EQ(minValue.getRaw(), -90 * static_cast<int32_t>(1e7));
    EXPECT_NEAR(minValue.getValue(), -90.0f, 1e-4f);

    auto maxValue = Latitude::fromValue(90.0f);
    EXPECT_EQ(maxValue.getRaw(), 90 * static_cast<int32_t>(1e7));
    EXPECT_NEAR(maxValue.getValue(), 90.0f, 1e-4f);

    // Test operators
    EXPECT_EQ(max, maxValue);
    EXPECT_EQ(min, minValue);
    EXPECT_TRUE(max > min);
    EXPECT_TRUE(min < max);
    EXPECT_TRUE(max >= min);
    EXPECT_TRUE(min <= max);
}

TEST(DataType, Longitude) {
    auto min = Longitude::fromRaw(-180 * static_cast<int32_t>(1e7));
    EXPECT_EQ(min.getRaw(), -180 * static_cast<int32_t>(1e7));
    EXPECT_NEAR(min.getValue(), -180.0f, 1e-4f);

    auto max = Longitude::fromRaw(180 * static_cast<int32_t>(1e7));
    EXPECT_EQ(max.getRaw(), 180 * static_cast<int32_t>(1e7));
    EXPECT_NEAR(max.getValue(), 180.0f, 1e-4f);

    auto minValue = Longitude::fromValue(-180.0f);
    EXPECT_EQ(minValue.getRaw(), -180 * static_cast<int32_t>(1e7));
    EXPECT_NEAR(minValue.getValue(), -180.0f, 1e-4f);

    auto maxValue = Longitude::fromValue(180.0f);
    EXPECT_EQ(maxValue.getRaw(), 180 * static_cast<int32_t>(1e7));
    EXPECT_NEAR(maxValue.getValue(), 180.0f, 1e-4f);

    // Test operators
    EXPECT_EQ(max, maxValue);
    EXPECT_EQ(min, minValue);
    EXPECT_TRUE(max > min);
    EXPECT_TRUE(min < max);
    EXPECT_TRUE(max >= min);
    EXPECT_TRUE(min <= max);
}