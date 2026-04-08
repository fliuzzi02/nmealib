#include <gtest/gtest.h>

#include <limits>
#include <memory>
#include <string>

#include <nmealib.h>

using namespace nmealib::nmea2000;

TEST(PGN127245, GettersReturnCorrectValues) {
    auto pgn = PGN127245(1,
        HalfByte::fromValue(1),
        SignedAngle::fromValue(1.5707f),
        SignedAngle::fromValue(-0.7853f));

    EXPECT_EQ(pgn.getType(), nmealib::Message::Type::NMEA2000);
    EXPECT_EQ(pgn.getPgn(), 127245U);

    EXPECT_EQ(pgn.getRudderId(), 1U);
    EXPECT_EQ(pgn.getDirection(), HalfByte::fromValue(1));
    EXPECT_EQ(pgn.getDirectionString(), "starboard");
    EXPECT_NEAR(pgn.getAngleOrder().getValue(), 1.5707f, 0.02f);
    EXPECT_NEAR(pgn.getPosition().getValue(), -0.7853f, 0.02f);

    EXPECT_NEAR(pgn.getAngleOrderDegrees(), 90.0f, 0.02f);
    EXPECT_NEAR(pgn.getPositionDegrees(), -45.0f, 0.02f);

    EXPECT_TRUE(pgn.validate());
    EXPECT_EQ(pgn.getCanFrameLength(), 8U);
    EXPECT_EQ(pgn.getCanFrame().size(), 8U);
}

TEST(PGN127245, DataFieldLimits) {
    auto min = PGN127245(0,
        HalfByte::fromRaw(std::numeric_limits<uint8_t>::min()),
        SignedAngle::fromRaw(std::numeric_limits<uint16_t>::min()),
        SignedAngle::fromRaw(std::numeric_limits<uint16_t>::min()));

    auto max = PGN127245(255,
        HalfByte::fromRaw(7),
        SignedAngle::fromRaw(std::numeric_limits<uint16_t>::max()),
        SignedAngle::fromRaw(std::numeric_limits<uint16_t>::max()));

    EXPECT_EQ(min.getDirection(), HalfByte::fromValue(0));
    EXPECT_EQ(max.getDirection(), HalfByte::fromValue(7));

    EXPECT_NEAR(min.getAngleOrder().getValue(), -3.1415f, 0.02f);
    EXPECT_NEAR(min.getPosition().getValue(), -3.1415f, 0.02f);
    EXPECT_NEAR(max.getAngleOrder().getValue(), 3.1415f, 0.02f);
    EXPECT_NEAR(max.getPosition().getValue(), 3.1415f, 0.02f);
}

TEST(PGN127245, FactoryConstruction) {
    std::string VALID_MESSAGE = "01F10D00:0101FEBF0060FFFF";
    std::string WRONG_MESSAGE = "01F50300:01F4012C01000000";
    std::string LONG_MESSAGE = "01F10D00:0101FEBF60FEFFFF00"; // 9 bytes, invalid

    auto msg = Nmea2000Factory::create(VALID_MESSAGE);
    ASSERT_NE(msg, nullptr);
    auto* pgn = dynamic_cast<PGN127245*>(msg.get());
    ASSERT_NE(pgn, nullptr);

    EXPECT_EQ(pgn->getRudderId(), 1U);
    EXPECT_EQ(pgn->getDirection(), HalfByte::fromValue(1));
    EXPECT_EQ(pgn->getDirectionString(), "starboard");
    EXPECT_NEAR(pgn->getAngleOrder().getValue(), 1.5707f, 0.02f);
    EXPECT_NEAR(pgn->getPosition().getValue(), -0.7854f, 0.02f);

    auto wrongMsg = Nmea2000Factory::create(WRONG_MESSAGE);
    ASSERT_NE(wrongMsg, nullptr);
    auto* wrongPgn = dynamic_cast<PGN127245*>(wrongMsg.get());
    EXPECT_EQ(wrongPgn, nullptr);

    EXPECT_THROW(Nmea2000Factory::create(LONG_MESSAGE), InvalidCanFrameException);
}

TEST(PGN127245, CloneCreatesEqualObject) {
    auto original = PGN127245(1,
        HalfByte::fromValue(2),
        SignedAngle::fromValue(0.5f),
        SignedAngle::fromValue(0.45f));

    auto clone = original.clone();
    auto* clonedPgn = dynamic_cast<PGN127245*>(clone.get());
    ASSERT_NE(clonedPgn, nullptr);
    EXPECT_EQ(*clonedPgn, original);
}

TEST(PGN127245, StringContent) {
    auto pgn = PGN127245(1,
        HalfByte::fromValue(1),
        SignedAngle::fromValue(1.5707f),
        SignedAngle::fromValue(-0.7853f));

    std::string expectedVerbose = "--------------------------------\nProtocol:    NMEA2000\nPriority:    0\nData Page:   1\nPDU Format:  0xf1 (PDU2 - broadcast)\nDestination: 255 (global)\nSource Addr: 0\nPGN:         127245 (0x1f10d)\nFrame Len:   8 bytes\nFrame Data:  01 01 fe bf 00 60 ff ff\nFields:\n\tRudder ID: 1\n\tDirection: starboard (1)\n\tAngle Order: 1.5707rad, 89.9931\xC2\xB0\n\tPosition: -0.7854rad, -44.9979\xC2\xB0\n";
    std::string expectedNonVerbose = "[OK] NMEA2000 PGN127245: RudderID=1 Dir=starboard AngleOrder=89.9931° Position=-44.9979°";

    EXPECT_EQ(pgn.getStringContent(true), expectedVerbose);
    EXPECT_EQ(pgn.getStringContent(false), expectedNonVerbose);
}

TEST(PGN127245, SerializeRoundTrip) {
    std::string VALID_MESSAGE = "01F10D00:0101FEBF0060FFFF";
    auto msg = Nmea2000Factory::create(VALID_MESSAGE);
    ASSERT_NE(msg, nullptr);
    auto* pgn = dynamic_cast<PGN127245*>(msg.get());
    ASSERT_NE(pgn, nullptr);

    auto toSerialize = PGN127245(pgn->getRudderId(),
                                 pgn->getDirection(),
                                 pgn->getAngleOrder(),
                                 pgn->getPosition());

    EXPECT_EQ(pgn->serialize(), VALID_MESSAGE);
    EXPECT_EQ(toSerialize.serialize(), VALID_MESSAGE);
}
