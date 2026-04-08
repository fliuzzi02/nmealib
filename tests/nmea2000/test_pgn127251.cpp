#include <gtest/gtest.h>

#include <limits>
#include <memory>
#include <string>

#include <nmealib.h>

using namespace nmealib::nmea2000;

static const std::string FRAME_STANDARD = "01F11300:0200000000000000";
static const std::string FRAME_WITH_RESERVED = "01F11300:0711223344556677";
static const std::string WRONG_MESSAGE = "01F50300:0000000000000000";
static const std::string FRAME_TOO_LONG = "01F11300:020000000000000000";

static PGN127251 makeTypical() {
    return PGN127251(7,
                     AngularRate::fromRaw(0x44332211),
                     Byte::fromValue(0xAA),
                     Byte::fromValue(0xBB),
                     Byte::fromValue(0xCC));
}

TEST(PGN127251, GettersReturnCorrectValues) {
    auto pgn = makeTypical();

    EXPECT_EQ(pgn.getType(), nmealib::Message::Type::NMEA2000);
    EXPECT_EQ(pgn.getPgn(), 127251U);

    EXPECT_EQ(pgn.getSequenceId(), 7U);
    EXPECT_EQ(pgn.getRate(), AngularRate::fromRaw(0x44332211));
    EXPECT_EQ(pgn.getReserved1(), Byte::fromValue(0xAA));
    EXPECT_EQ(pgn.getReserved2(), Byte::fromValue(0xBB));
    EXPECT_EQ(pgn.getReserved3(), Byte::fromValue(0xCC));

    EXPECT_TRUE(pgn.validate());
    EXPECT_EQ(pgn.getCanFrameLength(), 8U);
    EXPECT_EQ(pgn.getCanFrame().size(), 8U);
}

TEST(PGN127251, DataFieldLimits) {
    auto min = PGN127251(1,
                         AngularRate::fromRaw(std::numeric_limits<int32_t>::min()),
                         Byte::fromRaw(std::numeric_limits<uint8_t>::min()),
                         Byte::fromRaw(std::numeric_limits<uint8_t>::min()),
                         Byte::fromRaw(std::numeric_limits<uint8_t>::min()));

    auto max = PGN127251(1,
                         AngularRate::fromRaw(std::numeric_limits<int32_t>::max()),
                         Byte::fromRaw(std::numeric_limits<uint8_t>::max()),
                         Byte::fromRaw(std::numeric_limits<uint8_t>::max()),
                         Byte::fromRaw(std::numeric_limits<uint8_t>::max()));

    EXPECT_NEAR(min.getRate().getValue(), -67.0, 0.0001);
    EXPECT_NEAR(max.getRate().getValue(), 67.0, 0.0001);
}

TEST(PGN127251, FactoryConstruction) {
    auto msg = Nmea2000Factory::create(FRAME_STANDARD);
    ASSERT_NE(msg, nullptr);

    auto* pgn = dynamic_cast<PGN127251*>(msg.get());
    ASSERT_NE(pgn, nullptr);

    EXPECT_EQ(pgn->getSequenceId(), 2U);
    EXPECT_EQ(pgn->getRate(), AngularRate::fromRaw(0));
    EXPECT_EQ(pgn->getReserved1(), Byte::fromValue(0));
    EXPECT_EQ(pgn->getReserved2(), Byte::fromValue(0));
    EXPECT_EQ(pgn->getReserved3(), Byte::fromValue(0));

    auto wrongMsg = Nmea2000Factory::create(WRONG_MESSAGE);
    ASSERT_NE(wrongMsg, nullptr);
    auto* wrongPgn = dynamic_cast<PGN127251*>(wrongMsg.get());
    EXPECT_EQ(wrongPgn, nullptr);

    EXPECT_THROW(Nmea2000Factory::create(FRAME_TOO_LONG), InvalidCanFrameException);
}

TEST(PGN127251, FactoryParsesRateAndReservedFields) {
    auto msg = Nmea2000Factory::create(FRAME_WITH_RESERVED);
    ASSERT_NE(msg, nullptr);

    auto* pgn = dynamic_cast<PGN127251*>(msg.get());
    ASSERT_NE(pgn, nullptr);

    EXPECT_EQ(pgn->getSequenceId(), 7U);
    EXPECT_EQ(pgn->getRate(), AngularRate::fromRaw(0x44332211));
    EXPECT_EQ(pgn->getReserved1(), Byte::fromValue(0x55));
    EXPECT_EQ(pgn->getReserved2(), Byte::fromValue(0x66));
    EXPECT_EQ(pgn->getReserved3(), Byte::fromValue(0x77));
}

TEST(PGN127251, CloneCreatesEqualObject) {
    auto original = makeTypical();

    auto clone = original.clone();
    auto* clonedPgn = dynamic_cast<PGN127251*>(clone.get());
    ASSERT_NE(clonedPgn, nullptr);
    EXPECT_EQ(*clonedPgn, original);
}

TEST(PGN127251, StringContent) {
    auto pgn = PGN127251(1,
                         AngularRate::fromRaw(0),
                         Byte::fromValue(0),
                         Byte::fromValue(0),
                         Byte::fromValue(0));

    const std::string verbose = pgn.getStringContent(true);
    const std::string nonVerbose = pgn.getStringContent(false);

    EXPECT_NE(verbose.find("PGN:         127251"), std::string::npos);
    EXPECT_NE(verbose.find("Rate:"), std::string::npos);
    EXPECT_NE(nonVerbose.find("PGN127251"), std::string::npos);
    EXPECT_NE(nonVerbose.find("Rate="), std::string::npos);
}

TEST(PGN127251, SerializeRoundTrip) {
    auto msg = Nmea2000Factory::create(FRAME_WITH_RESERVED);
    ASSERT_NE(msg, nullptr);
    auto* pgn = dynamic_cast<PGN127251*>(msg.get());
    ASSERT_NE(pgn, nullptr);

    auto toSerialize = PGN127251(pgn->getSequenceId(),
                                 pgn->getRate(),
                                 pgn->getReserved1(),
                                 pgn->getReserved2(),
                                 pgn->getReserved3());

    EXPECT_EQ(pgn->serialize(), FRAME_WITH_RESERVED);
    EXPECT_EQ(toSerialize.serialize(), FRAME_WITH_RESERVED);
}