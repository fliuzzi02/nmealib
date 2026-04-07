#include <gtest/gtest.h>

#include <chrono>
#include <memory>
#include <string>
#include <limits>

#include <nmealib.h>

using namespace nmealib::nmea2000;

static const std::string FRAME_STANDARD = "01F80200:01010040F4010000";
static const std::string FRAME_WITH_RESERVED = "01F80200:07AA0040F40112AB";
static const std::string WRONG_MESSAGE = "01F80100:0000000000000000";
static const std::string FRAME_TOO_LONG = "01F80200:01010040F401000000";

static PGN129026 makeTypical() {
    return PGN129026(7,
                     HalfByte::fromValue(1),
                     Byte::fromValue(0),
                     Angle::fromValue(1.5707f),
                     Speed::fromValue(5.0f),
                     Byte::fromValue(0x12),
                     Byte::fromValue(0x34));
}

TEST(PGN129026, GettersReturnCorrectValues) {
    auto pgn = makeTypical();

    EXPECT_EQ(pgn.getType(), nmealib::Message::Type::NMEA2000);
    EXPECT_EQ(pgn.getPgn(), 129026U);

    EXPECT_EQ(pgn.getSequenceId(), 7U);
    EXPECT_EQ(pgn.getCogReference(), HalfByte::fromValue(1));
    EXPECT_EQ(pgn.getReserved1(), Byte::fromValue(0));
    EXPECT_NEAR(pgn.getCog().getValue(), 1.5707f, 0.02f);
    EXPECT_NEAR(pgn.getSog().getValue(), 5.0f, 0.02f);
    EXPECT_EQ(pgn.getReserved2(), Byte::fromValue(0x12));
    EXPECT_EQ(pgn.getReserved3(), Byte::fromValue(0x34));

    EXPECT_NEAR(pgn.getCogDegrees(), 90.0f, 0.05f);
    EXPECT_NEAR(pgn.getSogKnots(), 9.72f, 0.05f);

    EXPECT_TRUE(pgn.validate());
    EXPECT_EQ(pgn.getCanFrameLength(), 8U);
    EXPECT_EQ(pgn.getCanFrame().size(), 8U);
}

TEST(PGN129026, DataFieldLimits) {
    auto min = PGN129026(1,
                         HalfByte::fromRaw(std::numeric_limits<uint8_t>::min()),
                         Byte::fromValue(0),
                         Angle::fromRaw(std::numeric_limits<uint16_t>::min()),
                         Speed::fromRaw(std::numeric_limits<uint16_t>::min()),
                         Byte::fromRaw(std::numeric_limits<uint8_t>::min()),
                         Byte::fromRaw(std::numeric_limits<uint8_t>::min()));

    auto max = PGN129026(1,
                         HalfByte::fromRaw(0x0F),
                         Byte::fromValue(63),
                         Angle::fromRaw(std::numeric_limits<uint16_t>::max()),
                         Speed::fromRaw(std::numeric_limits<uint16_t>::max()),
                         Byte::fromRaw(std::numeric_limits<uint8_t>::max()),
                         Byte::fromRaw(std::numeric_limits<uint8_t>::max()));

    EXPECT_NEAR(min.getCog().getValue(), 0.0f, 0.02f);
    EXPECT_NEAR(min.getSog().getValue(), 0.0f, 0.02f);

    EXPECT_NEAR(max.getCog().getValue(), 2.0f * static_cast<float>(M_PI), 0.02f);
    EXPECT_NEAR(max.getSog().getValue(), 655.32f, 0.05f);
    EXPECT_EQ(max.getReserved1(), Byte::fromValue(63));
}

TEST(PGN129026, FactoryConstruction) {
    auto msg = Nmea2000Factory::create(FRAME_STANDARD);
    ASSERT_NE(msg, nullptr);

    auto* pgn = dynamic_cast<PGN129026*>(msg.get());
    ASSERT_NE(pgn, nullptr);

    EXPECT_EQ(pgn->getSequenceId(), 1U);
    EXPECT_EQ(pgn->getCogReference(), HalfByte::fromValue(1));
    EXPECT_EQ(pgn->getReserved1(), Byte::fromValue(0));
    EXPECT_EQ(pgn->getCog(), Angle::fromRaw(0x4000));
    EXPECT_EQ(pgn->getSog(), Speed::fromRaw(0x01F4));
    EXPECT_EQ(pgn->getReserved2(), Byte::fromValue(0));
    EXPECT_EQ(pgn->getReserved3(), Byte::fromValue(0));

    auto wrongMsg = Nmea2000Factory::create(WRONG_MESSAGE);
    ASSERT_NE(wrongMsg, nullptr);
    auto* wrongPgn = dynamic_cast<PGN129026*>(wrongMsg.get());
    EXPECT_EQ(wrongPgn, nullptr);

    EXPECT_THROW(Nmea2000Factory::create(FRAME_TOO_LONG), InvalidCanFrameException);
}

TEST(PGN129026, FactoryParsesReservedAndReferenceFields) {
    auto msg = Nmea2000Factory::create(FRAME_WITH_RESERVED);
    ASSERT_NE(msg, nullptr);

    auto* pgn = dynamic_cast<PGN129026*>(msg.get());
    ASSERT_NE(pgn, nullptr);

    EXPECT_EQ(pgn->getSequenceId(), 7U);
    EXPECT_EQ(pgn->getCogReference(), HalfByte::fromValue(2));
    EXPECT_EQ(pgn->getReserved1(), Byte::fromValue(42));
    EXPECT_EQ(pgn->getReserved2(), Byte::fromValue(0x12));
    EXPECT_EQ(pgn->getReserved3(), Byte::fromValue(0xAB));
}

TEST(PGN129026, CloneCreatesEqualObject) {
    auto original = makeTypical();

    auto clone = original.clone();
    auto* clonedPgn = dynamic_cast<PGN129026*>(clone.get());
    ASSERT_NE(clonedPgn, nullptr);
    EXPECT_EQ(*clonedPgn, original);
}

TEST(PGN129026, EqualityDependsOnPayload) {
    using Clock = std::chrono::system_clock;
    auto ts = Clock::now();

    auto msg1 = Nmea2000Factory::create(FRAME_STANDARD, ts);
    auto msg2 = Nmea2000Factory::create(FRAME_STANDARD, ts);
    ASSERT_NE(msg1, nullptr);
    ASSERT_NE(msg2, nullptr);

    auto* p1 = dynamic_cast<PGN129026*>(msg1.get());
    auto* p2 = dynamic_cast<PGN129026*>(msg2.get());
    ASSERT_NE(p1, nullptr);
    ASSERT_NE(p2, nullptr);
    EXPECT_TRUE(*p1 == *p2);

    msg2 = Nmea2000Factory::create(FRAME_WITH_RESERVED, ts);
    ASSERT_NE(msg2, nullptr);
    p2 = dynamic_cast<PGN129026*>(msg2.get());
    ASSERT_NE(p2, nullptr);
    EXPECT_FALSE(*p1 == *p2);
}

TEST(PGN129026, StringContent) {
    auto pgn = PGN129026(1,
                         HalfByte::fromValue(0),
                         Byte::fromValue(0),
                         Angle::fromRaw(0),
                         Speed::fromRaw(0),
                         Byte::fromValue(0),
                         Byte::fromValue(0));

    std::string expectedVerbose = "--------------------------------\nProtocol:    NMEA2000\nPriority:    0\nData Page:   1\nPDU Format:  0xf8 (PDU2 - broadcast)\nDestination: 255 (global)\nSource Addr: 0\nPGN:         129026 (0x1f802)\nFrame Len:   8 bytes\nFrame Data:  01 00 00 00 00 00 00 00\nFields:\n\tSequence ID: 1\n\tCourse Over Ground: 0.0000rad, 0\xC2\xB0\n\tSpeed Over Ground: 0 knots\n\tCOG Reference: True\n";
    std::string expectedNonVerbose = "[OK] NMEA2000 PGN129026: SeqID=1 COG=0° SOG=0 knots COGRef=0";

    EXPECT_EQ(pgn.getStringContent(true), expectedVerbose);
    EXPECT_EQ(pgn.getStringContent(false), expectedNonVerbose);
}

// Test Round-trip serialization
TEST(PGN129026, SerializeRoundTrip) {
    auto msg = Nmea2000Factory::create(
        FRAME_STANDARD
    );
    ASSERT_NE(msg, nullptr);
    auto* pgn = dynamic_cast<PGN129026*>(msg.get());
    ASSERT_NE(pgn, nullptr);

    auto toSerialize = PGN129026(pgn->getSequenceId(),
                                 pgn->getCogReference(),
                                 pgn->getReserved1(),
                                 pgn->getCog(),
                                 pgn->getSog(),
                                 pgn->getReserved2(),
                                 pgn->getReserved3());
    
    EXPECT_EQ(pgn->serialize(), FRAME_STANDARD);
    EXPECT_EQ(toSerialize.serialize(), FRAME_STANDARD);
}