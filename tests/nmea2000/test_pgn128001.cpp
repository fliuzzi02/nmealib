#include <gtest/gtest.h>

#include <chrono>
#include <limits>
#include <memory>
#include <string>

#include <nmealib.h>

using namespace nmealib::nmea2000;

static const std::string FRAME_STANDARD = "01F40100:050A00FBFF6400FF";
static const std::string FRAME_EXTREMES = "01F40100:010080FF7F000001";
static const std::string WRONG_MESSAGE = "01F80100:0000000000000000";
static const std::string FRAME_TOO_LONG = "01F40100:000000000000000000";

static PGN128001 makeTypical() {
    return PGN128001(5,
                     Acceleration::fromRaw(10),
                     Acceleration::fromRaw(-5),
                     Acceleration::fromRaw(100),
                     Byte::fromValue(255));
}

TEST(PGN128001, GettersReturnCorrectValues) {
    auto pgn = makeTypical();

    EXPECT_EQ(pgn.getType(), nmealib::Message::Type::NMEA2000);
    EXPECT_EQ(pgn.getPgn(), 128001U);

    EXPECT_EQ(pgn.getSequenceId(), 5U);
    EXPECT_EQ(pgn.getLongitudinalAcceleration(), Acceleration::fromRaw(10));
    EXPECT_EQ(pgn.getTransverseAcceleration(), Acceleration::fromRaw(-5));
    EXPECT_EQ(pgn.getVerticalAcceleration(), Acceleration::fromRaw(100));

    EXPECT_NEAR(pgn.getLongitudinalAccelerationG(), pgn.getLongitudinalAcceleration().getValue() / 9.80665f, 1e-6f);
    EXPECT_NEAR(pgn.getTransverseAccelerationG(), pgn.getTransverseAcceleration().getValue() / 9.80665f, 1e-6f);
    EXPECT_NEAR(pgn.getVerticalAccelerationG(), pgn.getVerticalAcceleration().getValue() / 9.80665f, 1e-6f);

    EXPECT_TRUE(pgn.validate());
    EXPECT_EQ(pgn.getCanFrameLength(), 8U);
    EXPECT_EQ(pgn.getCanFrame().size(), 8U);
}

TEST(PGN128001, DataFieldLimits) {
    auto min = PGN128001(1,
                         Acceleration::fromRaw(std::numeric_limits<int16_t>::min()),
                         Acceleration::fromRaw(std::numeric_limits<int16_t>::min()),
                         Acceleration::fromRaw(std::numeric_limits<int16_t>::min()),
                         Byte::fromRaw(std::numeric_limits<uint8_t>::min()));

    auto max = PGN128001(1,
                         Acceleration::fromRaw(std::numeric_limits<int16_t>::max()),
                         Acceleration::fromRaw(std::numeric_limits<int16_t>::max()),
                         Acceleration::fromRaw(std::numeric_limits<int16_t>::max()),
                         Byte::fromRaw(std::numeric_limits<uint8_t>::max()));

    EXPECT_EQ(min.getLongitudinalAcceleration(), Acceleration::fromRaw(std::numeric_limits<int16_t>::min()));
    EXPECT_EQ(max.getLongitudinalAcceleration(), Acceleration::fromRaw(std::numeric_limits<int16_t>::max()));
}

TEST(PGN128001, FactoryConstruction) {
    auto msg = Nmea2000Factory::create(FRAME_STANDARD);
    ASSERT_NE(msg, nullptr);

    auto* pgn = dynamic_cast<PGN128001*>(msg.get());
    ASSERT_NE(pgn, nullptr);

    EXPECT_EQ(pgn->getSequenceId(), 5U);
    EXPECT_EQ(pgn->getLongitudinalAcceleration(), Acceleration::fromRaw(10));
    EXPECT_EQ(pgn->getTransverseAcceleration(), Acceleration::fromRaw(-5));
    EXPECT_EQ(pgn->getVerticalAcceleration(), Acceleration::fromRaw(100));

    auto wrongMsg = Nmea2000Factory::create(WRONG_MESSAGE);
    ASSERT_NE(wrongMsg, nullptr);
    auto* wrongPgn = dynamic_cast<PGN128001*>(wrongMsg.get());
    EXPECT_EQ(wrongPgn, nullptr);

    EXPECT_THROW(Nmea2000Factory::create(FRAME_TOO_LONG), InvalidCanFrameException);
}

TEST(PGN128001, FactoryParsesExtremeValues) {
    auto msg = Nmea2000Factory::create(FRAME_EXTREMES);
    ASSERT_NE(msg, nullptr);

    auto* pgn = dynamic_cast<PGN128001*>(msg.get());
    ASSERT_NE(pgn, nullptr);

    EXPECT_EQ(pgn->getSequenceId(), 1U);
    EXPECT_EQ(pgn->getLongitudinalAcceleration(), Acceleration::fromRaw(std::numeric_limits<int16_t>::min()));
    EXPECT_EQ(pgn->getTransverseAcceleration(), Acceleration::fromRaw(std::numeric_limits<int16_t>::max()));
    EXPECT_EQ(pgn->getVerticalAcceleration(), Acceleration::fromRaw(0));
}

TEST(PGN128001, CloneCreatesEqualObject) {
    auto original = makeTypical();

    auto clone = original.clone();
    auto* clonedPgn = dynamic_cast<PGN128001*>(clone.get());
    ASSERT_NE(clonedPgn, nullptr);
    EXPECT_EQ(*clonedPgn, original);
}

TEST(PGN128001, EqualityDependsOnPayload) {
    using Clock = std::chrono::system_clock;
    auto ts = Clock::now();

    auto msg1 = Nmea2000Factory::create(FRAME_STANDARD, ts);
    auto msg2 = Nmea2000Factory::create(FRAME_STANDARD, ts);
    ASSERT_NE(msg1, nullptr);
    ASSERT_NE(msg2, nullptr);

    auto* p1 = dynamic_cast<PGN128001*>(msg1.get());
    auto* p2 = dynamic_cast<PGN128001*>(msg2.get());
    ASSERT_NE(p1, nullptr);
    ASSERT_NE(p2, nullptr);
    EXPECT_TRUE(*p1 == *p2);

    msg2 = Nmea2000Factory::create(FRAME_EXTREMES, ts);
    ASSERT_NE(msg2, nullptr);
    p2 = dynamic_cast<PGN128001*>(msg2.get());
    ASSERT_NE(p2, nullptr);
    EXPECT_FALSE(*p1 == *p2);
}

TEST(PGN128001, StringContent) {
    auto pgn = makeTypical();

    const std::string verbose = pgn.getStringContent(true);
    const std::string nonVerbose = pgn.getStringContent(false);

    EXPECT_NE(verbose.find("Fields:"), std::string::npos);
    EXPECT_NE(verbose.find("Longitudinal Acceleration"), std::string::npos);
    EXPECT_NE(nonVerbose.find("PGN128001"), std::string::npos);
    EXPECT_NE(nonVerbose.find("SeqID=5"), std::string::npos);
}

TEST(PGN128001, SerializeRoundTrip) {
    auto msg = Nmea2000Factory::create(FRAME_STANDARD);
    ASSERT_NE(msg, nullptr);
    auto* pgn = dynamic_cast<PGN128001*>(msg.get());
    ASSERT_NE(pgn, nullptr);

    auto toSerialize = PGN128001(pgn->getSequenceId(),
                                 pgn->getLongitudinalAcceleration(),
                                 pgn->getTransverseAcceleration(),
                                 pgn->getVerticalAcceleration());

    EXPECT_EQ(pgn->serialize(), FRAME_STANDARD);
    EXPECT_EQ(toSerialize.serialize(), FRAME_STANDARD);
}
