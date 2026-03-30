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
    EXPECT_EQ(pgn.getSequenceId(), 1U);
    EXPECT_NEAR(pgn.getWindSpeed().getValue(), 5.0f, 0.02f);
    EXPECT_NEAR(pgn.getWindDirection().getValue(), 1.5707f, 0.02f);
    EXPECT_EQ(pgn.getWindReference(), HalfByte::fromValue(1));

    // Test custom getters
    EXPECT_NEAR(pgn.getWindDirectionDegrees(), 90.0f, 0.02f);
    EXPECT_NEAR(pgn.getWindSpeedKnots(), 9.72f, 0.02f);
    
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
        HalfByte::fromRaw(0xF));
    
    // Test minimum values
    EXPECT_NEAR(min.getWindSpeed().getValue(), 0.0f, 0.03f);
    EXPECT_NEAR(min.getWindDirection().getValue(), 0.0f, 0.03f);
    EXPECT_EQ(min.getWindReference().getValue(), 0U);

    // Test maximum values
    EXPECT_NEAR(max.getWindSpeed().getValue(), 655.35f, 0.03f);
    EXPECT_NEAR(max.getWindDirection().getValue(), 6.2831f, 0.03f);
    EXPECT_EQ(max.getWindReference().getValue(), 15U);
}

TEST(PGN130306, FactoryConstruction) {
    std::string VALID_MESSAGE = "19FD0201:8C2C010479FAFFFF";
    std::string WRONG_MESSAGE = "01F50300:01F4012C01000000";
    std::string LONG_MESSAGE  = "19FD0201:970000FFFF00FFFFFF"; // 9 bytes, invalid

    auto msg = Nmea2000Factory::create(
        VALID_MESSAGE
    );
    ASSERT_NE(msg, nullptr);
    auto* pgn = dynamic_cast<PGN130306*>(msg.get());
    ASSERT_NE(pgn, nullptr);

    // Test standard getters
    EXPECT_EQ(pgn->getSequenceId(), 140U);

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

    std::string expectedVerbose = "--------------------------------\nProtocol:    NMEA2000\nPriority:    0\nData Page:   1\nPDU Format:  0xfd (PDU2 - broadcast)\nDestination: 255 (global)\nSource Addr: 0\nPGN:         130306 (0x1fd02)\nFrame Len:   8 bytes\nFrame Data:  01 f4 01 fe 3f 01 00 00\nFields:\n\tSequence ID: 1\n\tWind Speed: 5.00 knots\n\tWind Direction: 1.5706rad, 89.9904\xC2\xB0\n\tWind Reference: Magnetic(North)\n";
    std::string expectedNonVerbose = "[OK] NMEA2000 PGN130306: SeqID=1 WindSpeed=4.99977 knots WindDirection=89.9904° WindReference=1";

    EXPECT_EQ(pgn.getStringContent(true), expectedVerbose);
    EXPECT_EQ(pgn.getStringContent(false), expectedNonVerbose);
}