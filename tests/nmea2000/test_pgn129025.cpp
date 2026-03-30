#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <limits>

#include <nmealib.h>

using namespace nmealib::nmea2000;

TEST(PGN129025, GettersReturnCorrectValues) {
    auto pgn = PGN129025(
        Latitude::fromValue(45.1234567f),
        Longitude::fromValue(9.7654321f));

    EXPECT_EQ(pgn.getType(), nmealib::Message::Type::NMEA2000);
    EXPECT_EQ(pgn.getPgn(), 129025U);

    EXPECT_NEAR(pgn.getLatitude().getValue(), 45.1234567f, 0.02f);
    EXPECT_NEAR(pgn.getLongitude().getValue(), 9.7654321f, 0.02f);

    EXPECT_TRUE(pgn.validate());
    EXPECT_EQ(pgn.getCanFrameLength(), 8U);
    EXPECT_EQ(pgn.getCanFrame().size(), 8U);
}

TEST(PGN129025, DataFieldLimits) {
    auto min = PGN129025(
        Latitude::fromRaw(-90 * static_cast<int32_t>(1e7)),
        Longitude::fromRaw(-180 * static_cast<int32_t>(1e7)));

    auto max = PGN129025(
        Latitude::fromRaw(90 * static_cast<int32_t>(1e7)),
        Longitude::fromRaw(180 * static_cast<int32_t>(1e7)));

    EXPECT_NEAR(min.getLatitude().getValue(), -90.0f, 0.02f);
    EXPECT_NEAR(min.getLongitude().getValue(), -180.0f, 0.02f);

    EXPECT_NEAR(max.getLatitude().getValue(), 90.0f, 0.02f);
    EXPECT_NEAR(max.getLongitude().getValue(), 180.0f, 0.02f);
}

TEST(PGN129025, FactoryConstruction) {
    std::string VALID_MESSAGE = "01F80100:0000000000000000";
    std::string WRONG_MESSAGE = "01F50300:01F4012C01000000";
    std::string LONG_MESSAGE = "01F80100:000000000000000000"; // 9 bytes, invalid

    auto msg = Nmea2000Factory::create(VALID_MESSAGE);
    ASSERT_NE(msg, nullptr);
    auto* pgn = dynamic_cast<PGN129025*>(msg.get());
    ASSERT_NE(pgn, nullptr);

    EXPECT_NEAR(pgn->getLatitude().getValue(), 0.0f, 0.02f);
    EXPECT_NEAR(pgn->getLongitude().getValue(), 0.0f, 0.02f);

    auto wrongMsg = Nmea2000Factory::create(WRONG_MESSAGE);
    ASSERT_NE(wrongMsg, nullptr);
    auto* wrongPgn = dynamic_cast<PGN129025*>(wrongMsg.get());
    EXPECT_EQ(wrongPgn, nullptr);

    EXPECT_THROW(Nmea2000Factory::create(LONG_MESSAGE), InvalidCanFrameException);
}

TEST(PGN129025, CloneCreatesEqualObject) {
    auto original = PGN129025(
        Latitude::fromValue(12.345678f),
        Longitude::fromValue(-45.678901f));

    auto clone = original.clone();
    auto* clonedPgn = dynamic_cast<PGN129025*>(clone.get());
    ASSERT_NE(clonedPgn, nullptr);
    EXPECT_EQ(*clonedPgn, original);
}

TEST(PGN129025, StringContent) {
    auto pgn = PGN129025(
        Latitude::fromRaw(0),
        Longitude::fromRaw(0));

    std::string expectedVerbose = "--------------------------------\nProtocol:    NMEA2000\nPriority:    0\nData Page:   1\nPDU Format:  0xf8 (PDU2 - broadcast)\nDestination: 255 (global)\nSource Addr: 0\nPGN:         129025 (0x1f801)\nFrame Len:   8 bytes\nFrame Data:  00 00 00 00 00 00 00 00\nFields:\n\tLatitude: 0.0000000\xC2\xB0\n\tLongitude: 0.0000000\xC2\xB0\n";
    std::string expectedNonVerbose = "[OK] NMEA2000 PGN129025: Latitude=0.0000000° Longitude=0.0000000°";

    EXPECT_EQ(pgn.getStringContent(true), expectedVerbose);
    EXPECT_EQ(pgn.getStringContent(false), expectedNonVerbose);
}