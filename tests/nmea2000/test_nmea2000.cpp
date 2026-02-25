#include <gtest/gtest.h>

#include <chrono>
#include <memory>
#include <string>

#include <nmealib/nmealib.hpp>

using namespace nmealib::nmea2000;

static const std::string SAMPLE_PGN129029 =
    "18F80523 1D 01 23 45 67 FE DC BA 98 00 01 86 A0 12 00 04 E2 0C";

static const std::string SAMPLE_PGN129029_COMPACT =
    "18F805231D01234567FEDCBA98000186A0120004E20C";

static const std::string SAMPLE_UNKNOWN_PGN =
    "18FDEF23 01 02 03";

static const std::string SAMPLE_INVALID_PGN =
    "XYZ12345 01 02 03";

TEST(Message2000, CreateSucceedsAndAccessors)
{
    auto msg = Nmea2000Factory::create(SAMPLE_PGN129029);
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getPgn(), 129029u);
    EXPECT_TRUE(msg->hasCanId());
    EXPECT_EQ(msg->getCanId(), 0x18F80523u);
    EXPECT_EQ(msg->getPriority(), 6u);
    EXPECT_EQ(msg->getDataPage(), 0u);
    EXPECT_EQ(msg->getPduFormat(), 0xF8u);
    EXPECT_EQ(msg->getPduSpecific(), 0x05u);
    EXPECT_EQ(msg->getSourceAddress(), 0x23u);
    EXPECT_EQ(msg->getPayloadBytes().size(), 18u);
    EXPECT_EQ(msg->serialize(), SAMPLE_PGN129029);
}

TEST(Message2000, CreateSucceedsWithCompactRawFrame)
{
    auto msg = Nmea2000Factory::create(SAMPLE_PGN129029_COMPACT);
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getPgn(), 129029u);
    EXPECT_TRUE(msg->hasCanId());
    EXPECT_EQ(msg->getCanId(), 0x18F80523u);
    EXPECT_EQ(msg->getPayloadBytes().size(), 18u);
    EXPECT_EQ(msg->serialize(), SAMPLE_PGN129029);
}

TEST(Message2000, CreateThrowsOnInvalidPgn)
{
    EXPECT_THROW(Nmea2000Factory::create(SAMPLE_INVALID_PGN), InvalidPgnException);
}

TEST(Message2000, CloneProducesEqualObject)
{
    auto msg = Nmea2000Factory::create(SAMPLE_UNKNOWN_PGN);
    ASSERT_NE(msg, nullptr);

    std::unique_ptr<nmealib::Message> baseClone = msg->clone();
    ASSERT_NE(baseClone, nullptr);

    auto derivedClone = dynamic_cast<Message2000*>(baseClone.get());
    ASSERT_NE(derivedClone, nullptr);
    EXPECT_TRUE(msg->hasEqualContent(*derivedClone));
}

TEST(Message2000, EqualityAndHasEqualContent)
{
    using Clock = std::chrono::system_clock;
    auto ts = Clock::now();

    auto m1 = Nmea2000Factory::create(SAMPLE_UNKNOWN_PGN, ts);
    auto m2 = Nmea2000Factory::create(SAMPLE_UNKNOWN_PGN, ts);
    ASSERT_NE(m1, nullptr);
    ASSERT_NE(m2, nullptr);

    EXPECT_TRUE(*m1 == *m2);

    auto m3 = Nmea2000Factory::create(SAMPLE_UNKNOWN_PGN);
    ASSERT_NE(m3, nullptr);
    EXPECT_TRUE(m1->hasEqualContent(*m3));
}

TEST(Message2000, ValidateAlwaysTrueForNow)
{
    auto msg = Nmea2000Factory::create(SAMPLE_UNKNOWN_PGN);
    ASSERT_NE(msg, nullptr);
    EXPECT_TRUE(msg->validate());
}
