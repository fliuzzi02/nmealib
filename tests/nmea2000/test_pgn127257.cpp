#include <gtest/gtest.h>

#include <limits>
#include <memory>
#include <string>

#include <nmealib.h>

using namespace nmealib::nmea2000;

TEST(PGN127257, GettersReturnCorrectValues) {
    auto pgn = PGN127257(2,
        SignedAngle::fromValue(1.5707f),
        SignedAngle::fromValue(-0.7853f),
        SignedAngle::fromValue(0.7853f),
        Byte::fromValue(0));

    EXPECT_EQ(pgn.getType(), nmealib::Message::Type::NMEA2000);
    EXPECT_EQ(pgn.getPgn(), 127257U);

    EXPECT_EQ(pgn.getSequenceId(), 2U);
    EXPECT_NEAR(pgn.getYaw().getValue(), 1.5707f, 0.02f);
    EXPECT_NEAR(pgn.getPitch().getValue(), -0.7853f, 0.02f);
    EXPECT_NEAR(pgn.getRoll().getValue(), 0.7853f, 0.02f);
    EXPECT_EQ(pgn.getReserved(), Byte::fromValue(0));

    EXPECT_NEAR(pgn.getYawDegrees(), 90.0f, 0.02f);
    EXPECT_NEAR(pgn.getPitchDegrees(), -45.0f, 0.02f);
    EXPECT_NEAR(pgn.getRollDegrees(), 45.0f, 0.02f);

    EXPECT_TRUE(pgn.validate());
    EXPECT_EQ(pgn.getCanFrameLength(), 8U);
    EXPECT_EQ(pgn.getCanFrame().size(), 8U);
}

TEST(PGN127257, DataFieldLimits) {
    auto min = PGN127257(2,
        SignedAngle::fromRaw(std::numeric_limits<uint16_t>::min()),
        SignedAngle::fromRaw(std::numeric_limits<uint16_t>::min()),
        SignedAngle::fromRaw(std::numeric_limits<uint16_t>::min()),
        Byte::fromRaw(std::numeric_limits<uint8_t>::min()));

    auto max = PGN127257(2,
        SignedAngle::fromRaw(std::numeric_limits<uint16_t>::max()),
        SignedAngle::fromRaw(std::numeric_limits<uint16_t>::max()),
        SignedAngle::fromRaw(std::numeric_limits<uint16_t>::max()),
        Byte::fromRaw(std::numeric_limits<uint8_t>::max()));

    EXPECT_NEAR(min.getYaw().getValue(), -3.1415f, 0.02f);
    EXPECT_NEAR(min.getPitch().getValue(), -3.1415f, 0.02f);
    EXPECT_NEAR(min.getRoll().getValue(), -3.1415f, 0.02f);

    EXPECT_NEAR(max.getYaw().getValue(), 3.1415f, 0.02f);
    EXPECT_NEAR(max.getPitch().getValue(), 3.1415f, 0.02f);
    EXPECT_NEAR(max.getRoll().getValue(), 3.1415f, 0.02f);
}

TEST(PGN127257, FactoryConstruction) {
    std::string VALID_MESSAGE = "01F11900:02FEBF0060FE9F00";
    std::string WRONG_MESSAGE = "01F50300:01F4012C01000000";
    std::string LONG_MESSAGE = "01F11900:02FE3F0060FE9F0000"; // 9 bytes, invalid

    auto msg = Nmea2000Factory::create(VALID_MESSAGE);
    ASSERT_NE(msg, nullptr);
    auto* pgn = dynamic_cast<PGN127257*>(msg.get());
    ASSERT_NE(pgn, nullptr);

    EXPECT_EQ(pgn->getSequenceId(), 2U);
    EXPECT_NEAR(pgn->getYaw().getValue(), 1.5707f, 0.02f);
    EXPECT_NEAR(pgn->getPitch().getValue(), -0.7854f, 0.02f);
    EXPECT_NEAR(pgn->getRoll().getValue(), 0.7853f, 0.02f);
    EXPECT_EQ(pgn->getReserved(), Byte::fromValue(0));

    auto wrongMsg = Nmea2000Factory::create(WRONG_MESSAGE);
    ASSERT_NE(wrongMsg, nullptr);
    auto* wrongPgn = dynamic_cast<PGN127257*>(wrongMsg.get());
    EXPECT_EQ(wrongPgn, nullptr);

    EXPECT_THROW(Nmea2000Factory::create(LONG_MESSAGE), InvalidCanFrameException);
}

TEST(PGN127257, CloneCreatesEqualObject) {
    auto original = PGN127257(2,
        SignedAngle::fromValue(1.5707f),
        SignedAngle::fromValue(-0.7853f),
        SignedAngle::fromValue(0.7853f),
        Byte::fromValue(0));

    auto clone = original.clone();
    auto* clonedPgn = dynamic_cast<PGN127257*>(clone.get());
    ASSERT_NE(clonedPgn, nullptr);
    EXPECT_EQ(*clonedPgn, original);
}

TEST(PGN127257, StringContent) {
    auto pgn = PGN127257(2,
        SignedAngle::fromValue(1.5707f),
        SignedAngle::fromValue(-0.7853f),
        SignedAngle::fromValue(0.7853f),
        Byte::fromValue(0));

    std::string expectedVerbose = "--------------------------------\nProtocol:    NMEA2000\nPriority:    0\nData Page:   1\nPDU Format:  0xf1 (PDU2 - broadcast)\nDestination: 255 (global)\nSource Addr: 0\nPGN:         127257 (0x1f119)\nFrame Len:   8 bytes\nFrame Data:  02 fe bf 00 60 fe 9f 00\nFields:\n\tSequence ID: 2\n\tYaw: 1.5707rad, 89.9931\xC2\xB0\n\tPitch: -0.7854rad, -44.9979\xC2\xB0\n\tRoll: 0.7853rad, 44.9924\xC2\xB0\n";
    std::string expectedNonVerbose = "[OK] NMEA2000 PGN127257: SeqID=2 Yaw=89.9931° Pitch=-44.9979° Roll=44.9924°";

    EXPECT_EQ(pgn.getStringContent(true), expectedVerbose);
    EXPECT_EQ(pgn.getStringContent(false), expectedNonVerbose);
}

TEST(PGN127257, SerializeRoundTrip) {
    std::string VALID_MESSAGE = "01F11900:02FEBF0060FE9F00";
    auto msg = Nmea2000Factory::create(VALID_MESSAGE);
    ASSERT_NE(msg, nullptr);
    auto* pgn = dynamic_cast<PGN127257*>(msg.get());
    ASSERT_NE(pgn, nullptr);

    auto toSerialize = PGN127257(pgn->getSequenceId(),
                                 pgn->getYaw(),
                                 pgn->getPitch(),
                                 pgn->getRoll(),
                                 pgn->getReserved());

    EXPECT_EQ(pgn->serialize(), VALID_MESSAGE);
    EXPECT_EQ(toSerialize.serialize(), VALID_MESSAGE);
}