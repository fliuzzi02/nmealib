#include <gtest/gtest.h>

#include <chrono>
#include <memory>
#include <string>
#include <limits>

#include <nmealib.h>

using namespace nmealib::nmea2000;

// ---------------------------------------------------------------------------
// CAN frame constants for PGN 130306 (0x1F502)
// CAN ID = (130306 << 8) = 0x01F50200
//
// Byte layout (8 bytes total):
//   [0]     Sequence ID
//   [1-2]   Heading (Angle)
//   [3-4]   Deviation (SignedAngle)
//   [5-6]   Variation (SignedAngle)
//   [7]     6bits of Reserved and 4bits of Heading reference
// ---------------------------------------------------------------------------

TEST(PGN130306, GettersReturnCorrectValues) {
    auto pgn = PGN130306(1,
        Speed::fromValue(5.0f),
        Angle::fromValue(1.5707f),
        HalfByte::fromValue(1));

    EXPECT_EQ(pgn.getType(), nmealib::Message::Type::NMEA2000);
    EXPECT_EQ(pgn.getPgn(), 130306U);

    // Test standard getters
    EXPECT_EQ(pgn.getSequenceId(), 2U);

    // Test custom getters
    
    // Test parent class getters
    EXPECT_TRUE(pgn.validate());
    EXPECT_EQ(pgn.getCanFrameLength(), 8U);
    EXPECT_EQ(pgn.getCanFrame().size(), 8U);
}

TEST(PGN130306, DataFieldLimits) {
    auto min = PGN130306(2,
        Speed::fromRaw(std::numeric_limits<uint16_t>::min()),
        Angle::fromRaw(std::numeric_limits<uint16_t>::min()),
        HalfByte::fromRaw(std::numeric_limits<uint8_t>::min()));
    
    auto max = PGN130306(2,
        Speed::fromRaw(std::numeric_limits<uint16_t>::max()),
        Angle::fromRaw(std::numeric_limits<uint16_t>::max()),
        HalfByte::fromRaw(std::numeric_limits<uint8_t>::max()));
    
    // Test minimum values

    // Test maximum values
}

TEST(PGN130306, FactoryConstruction) {
    std::string VALID_MESSAGE = "";
    std::string WRONG_MESSAGE = "";
    std::string LONG_MESSAGE = ""; // 9 bytes, invalid

    auto msg = Nmea2000Factory::create(
        VALID_MESSAGE
    );
    ASSERT_NE(msg, nullptr);
    auto* pgn = dynamic_cast<PGN130306*>(msg.get());
    ASSERT_NE(pgn, nullptr);

    // Test standard getters
    EXPECT_EQ(pgn->getSequenceId(), 52U);

    auto wrongMsg = Nmea2000Factory::create(
        WRONG_MESSAGE
    );
    ASSERT_NE(wrongMsg, nullptr);
    auto* wrongPgn = dynamic_cast<PGN130306*>(wrongMsg.get());
    EXPECT_EQ(wrongPgn, nullptr);

    EXPECT_THROW(Nmea2000Factory::create(
        LONG_MESSAGE
    ), InvalidCanFrameException);
}

// Test clone() method
TEST(PGN130306, CloneCreatesEqualObject) {
    auto original = PGN130306(1,
        Speed::fromValue(5.0f),
        Angle::fromValue(1.5707f),
        HalfByte::fromValue(1));

    auto clone = original.clone();
    auto* clonedPgn = dynamic_cast<PGN130306*>(clone.get());
    ASSERT_NE(clonedPgn, nullptr);
    EXPECT_EQ(*clonedPgn, original);
}

// Test toStringContent() method
TEST(PGN130306, StringContent) {
    auto pgn = PGN130306(1,
        Speed::fromValue(5.0f),
        Angle::fromValue(1.5707f),
        HalfByte::fromValue(1));

    std::string expectedVerbose = "";
    std::string expectedNonVerbose = "";

    EXPECT_EQ(pgn.getStringContent(true), expectedVerbose);
    EXPECT_EQ(pgn.getStringContent(false), expectedNonVerbose);
}