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

TEST(PGN127250, GettersReturnCorrectValues) {
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

TEST(PGN127250, DataFieldLimits) {
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

TEST(PGN127250, FactoryConstruction) {
    std::string VALID_MESSAGE = "09F11260:342C71FF7FFF7FFD";
    std::string WRONG_MESSAGE = "01F50300:01F4012C01000000";
    std::string LONG_MESSAGE = "09F11260:342C71FF7FFF7FFD00"; // 9 bytes, invalid

    auto msg = Nmea2000Factory::create(
        VALID_MESSAGE
    );
    ASSERT_NE(msg, nullptr);
    auto* pgn = dynamic_cast<PGN127250*>(msg.get());
    ASSERT_NE(pgn, nullptr);

    EXPECT_EQ(pgn->getSequenceId(), 52U);
    EXPECT_NEAR(pgn->getHeading().getValue(), 2.7777f, 0.02f);
    EXPECT_NEAR(pgn->getDeviation().getValue(), 0.0f, 0.02f);
    EXPECT_NEAR(pgn->getVariation().getValue(), 0.0f, 0.02f);
    EXPECT_EQ(pgn->getHeadingReference(), HalfByte::fromValue(1));

    auto wrongMsg = Nmea2000Factory::create(
        WRONG_MESSAGE
    );
    ASSERT_NE(wrongMsg, nullptr);
    auto* wrongPgn = dynamic_cast<PGN127250*>(wrongMsg.get());
    EXPECT_EQ(wrongPgn, nullptr);

    EXPECT_THROW(Nmea2000Factory::create(
        LONG_MESSAGE
    ), InvalidCanFrameException);
}

// Test clone() method
TEST(PGN127250, CloneCreatesEqualObject) {
    auto original = PGN127250(2,
        Angle::fromValue(1.5707f),
        SignedAngle::fromValue(-0.7853f),
        SignedAngle::fromValue(0.7853f),
        HalfByte::fromValue(1),
        Byte::fromValue(0));

    auto clone = original.clone();
    auto* clonedPgn = dynamic_cast<PGN127250*>(clone.get());
    ASSERT_NE(clonedPgn, nullptr);
    EXPECT_EQ(*clonedPgn, original);
}

// Test toStringContent() method
TEST(PGN127250, StringContent) {
    auto pgn = PGN127250(2,
        Angle::fromValue(1.5707f),
        SignedAngle::fromValue(-0.7853f),
        SignedAngle::fromValue(0.7853f),
        HalfByte::fromValue(1),
        Byte::fromValue(0));

    std::string expectedVerbose = "--------------------------------\nProtocol:    NMEA2000\nPriority:    0\nData Page:   1\nPDU Format:  0xf1 (PDU2 - broadcast)\nDestination: 255 (global)\nSource Addr: 0\nPGN:         127250 (0x1f112)\nFrame Len:   8 bytes\nFrame Data:  02 fe 3f 00 60 fe 9f 01\nFields:\n\tSequence ID: 2\n\tHeading: 1.5706rad, 89.9904\xC2\xB0\n\tDeviation: -0.7854rad, -44.9979\xC2\xB0\n\tVariation: 0.7853rad, 44.9924\xC2\xB0\n\tHeading Reference: Magnetic\n";
    std::string expectedNonVerbose = "[OK] NMEA2000 PGN127250: SeqID=2 Heading=89.9904° Deviation=-44.9979° Variation=44.9924° HeadingRef=1";

    EXPECT_EQ(pgn.getStringContent(true), expectedVerbose);
    EXPECT_EQ(pgn.getStringContent(false), expectedNonVerbose);
}

// Test Round-trip serialization
TEST(PGN127250, SerializeRoundTrip) {
    std::string VALID_MESSAGE = "01F11200:342C71FF7FFF7FC1";
    auto msg = Nmea2000Factory::create(
        VALID_MESSAGE
    );
    ASSERT_NE(msg, nullptr);
    auto* pgn = dynamic_cast<PGN127250*>(msg.get());
    ASSERT_NE(pgn, nullptr);

    auto toSerialize = PGN127250(pgn->getSequenceId(),
                                 pgn->getHeading(),
                                 pgn->getDeviation(),
                                 pgn->getVariation(),
                                 pgn->getHeadingReference(),
                                 Byte::fromRaw((pgn->getCanFrame()[7] >> 6) & 0x3F));

    EXPECT_EQ(pgn->serialize(), VALID_MESSAGE);
    EXPECT_EQ(toSerialize.serialize(), VALID_MESSAGE);
}