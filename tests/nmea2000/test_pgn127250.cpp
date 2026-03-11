#include <gtest/gtest.h>

#include <chrono>
#include <memory>
#include <string>
#include <limits>

#include <nmealib.h>

using namespace nmealib::nmea2000;

// ---------------------------------------------------------------------------
// CAN frame constants for PGN 127250 (0x1F502)
// CAN ID = (127250 << 8) = 0x01F50200
//
// Byte layout (8 bytes total):
//   [0]     Sequence ID
//   [1-2]   Heading (Angle)
//   [3-4]   Deviation (SignedAngle)
//   [5-6]   Variation (SignedAngle)
//   [7]     6bits of Reserved and 4bits of Heading reference
// ---------------------------------------------------------------------------

TEST(PGN127250DirectConstruction, GettersReturnCorrectValues) {
    auto pgn = PGN127250(2,
        Angle::fromValue(1.5707f),
        SignedAngle::fromValue(-0.7853f),
        SignedAngle::fromValue(0.7853f),
        HalfByte::fromValue(1),
        Byte::fromValue(0));

    EXPECT_EQ(pgn.getType(), nmealib::Message::Type::NMEA2000);
    EXPECT_EQ(pgn.getPgn(), 127250U);

    EXPECT_EQ(pgn.getSequenceId(), 2U);
    EXPECT_NEAR(pgn.getHeading().getValue(), 1.5707f, 0.02f);
    EXPECT_NEAR(pgn.getDeviation().getValue(), -0.7853f, 0.02f);
    EXPECT_NEAR(pgn.getVariation().getValue(), 0.7853f, 0.02f);
    EXPECT_EQ(pgn.getHeadingReference(), HalfByte::fromValue(1));

    // Test degrees conversion for heading
    EXPECT_NEAR(pgn.getHeadingDegrees(), 90.0f, 0.02f);
    EXPECT_NEAR(pgn.getDeviationDegrees(), -45.0f, 0.02f);
    EXPECT_NEAR(pgn.getVariationDegrees(), 45.0f, 0.02f);
    
    // Test parent class getters
    EXPECT_TRUE(pgn.validate());
    EXPECT_EQ(pgn.getCanFrameLength(), 8U);
    EXPECT_EQ(pgn.getCanFrame().size(), 8U);
}

TEST(PGN127250DirectConstruction, DataFieldLimits) {
    auto min = PGN127250(2,
        Angle::fromRaw(std::numeric_limits<uint16_t>::min()),
        SignedAngle::fromRaw(std::numeric_limits<uint16_t>::min()),
        SignedAngle::fromRaw(std::numeric_limits<uint16_t>::min()),
        HalfByte::fromRaw(std::numeric_limits<uint8_t>::min()),
        Byte::fromRaw(std::numeric_limits<uint8_t>::min()));
    
    auto max = PGN127250(2,
        Angle::fromRaw(std::numeric_limits<uint16_t>::max()),
        SignedAngle::fromRaw(std::numeric_limits<uint16_t>::max()),
        SignedAngle::fromRaw(std::numeric_limits<uint16_t>::max()),
        HalfByte::fromRaw(15),
        Byte::fromRaw(std::numeric_limits<uint8_t>::max()));

    EXPECT_NEAR(min.getHeading().getValue(), 0.0f, 0.02f);
    EXPECT_NEAR(min.getDeviation().getValue(), -3.1415f, 0.02f);
    EXPECT_NEAR(min.getVariation().getValue(), -3.1415f, 0.02f);

    EXPECT_NEAR(max.getHeading().getValue(), 6.2832f, 0.02f);
    EXPECT_NEAR(max.getDeviation().getValue(), 3.1415f, 0.02f);
    EXPECT_NEAR(max.getVariation().getValue(), 3.1415f, 0.02f);
}