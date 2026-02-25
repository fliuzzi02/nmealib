#include <gtest/gtest.h>

#include <memory>
#include <string>

#include <nmealib/nmealib.hpp>

using namespace nmealib::nmea2000;

static const std::string PGN129029_MESSAGE =
    "18F80523 1D 01 23 45 67 FE DC BA 98 00 01 86 A0 12 00 04 E2 0C";

static const std::string PGN129029_MESSAGE_COMPACT =
    "18F805231D01234567FEDCBA98000186A0120004E20C";

static const std::string INCOMPLETE_PGN129029_MESSAGE =
    "18F80523 1D 01 23";

static const std::string NOT_129029_MESSAGE =
    "18FDEF23 01 02 03";

TEST(PGN129029, CreateFromMessage2000Factory)
{
    auto msg = Nmea2000Factory::create(PGN129029_MESSAGE);
    ASSERT_NE(msg, nullptr);

    auto pgn = dynamic_cast<PGN129029*>(msg.get());
    ASSERT_NE(pgn, nullptr);

    EXPECT_EQ(pgn->getPgn(), 129029u);
    EXPECT_TRUE(pgn->hasCanId());
    EXPECT_EQ(pgn->getCanId(), 0x18F80523u);
    EXPECT_EQ(pgn->getSid(), 0x1Du);
    EXPECT_DOUBLE_EQ(pgn->getLatitude(), 1.9088743);
    EXPECT_DOUBLE_EQ(pgn->getLongitude(), -1.9088744);
    EXPECT_DOUBLE_EQ(pgn->getAltitude(), 100.0);
    EXPECT_EQ(pgn->getFixStatus(), 0x12u);
    EXPECT_EQ(pgn->getFixType(), 0x02u);
    EXPECT_EQ(pgn->getStatusFlags(), 0x02u);
    EXPECT_DOUBLE_EQ(pgn->getUtcTime(), 32.0012);
}

TEST(PGN129029, CreateFromMessage2000FactoryCompact)
{
    auto msg = Nmea2000Factory::create(PGN129029_MESSAGE_COMPACT);
    ASSERT_NE(msg, nullptr);

    auto pgn = dynamic_cast<PGN129029*>(msg.get());
    ASSERT_NE(pgn, nullptr);

    EXPECT_EQ(pgn->getPgn(), 129029u);
    EXPECT_TRUE(pgn->hasCanId());
    EXPECT_EQ(pgn->getCanId(), 0x18F80523u);
    EXPECT_EQ(pgn->getSid(), 0x1Du);
    EXPECT_DOUBLE_EQ(pgn->getLatitude(), 1.9088743);
    EXPECT_DOUBLE_EQ(pgn->getLongitude(), -1.9088744);
    EXPECT_DOUBLE_EQ(pgn->getAltitude(), 100.0);
    EXPECT_EQ(pgn->getFixStatus(), 0x12u);
    EXPECT_DOUBLE_EQ(pgn->getUtcTime(), 32.0012);
}

TEST(PGN129029, CreateFromFields)
{
    PGN129029 pgn(0x1D, 1.9088743, -1.9088744, 100.0, 0x12, 32.0012, 0x23, 6);

    EXPECT_EQ(pgn.getPgn(), 129029u);
    EXPECT_EQ(pgn.getSid(), 0x1Du);
    EXPECT_DOUBLE_EQ(pgn.getLatitude(), 1.9088743);
    EXPECT_DOUBLE_EQ(pgn.getLongitude(), -1.9088744);
    EXPECT_DOUBLE_EQ(pgn.getAltitude(), 100.0);
    EXPECT_EQ(pgn.getFixStatus(), 0x12u);
    EXPECT_EQ(pgn.getFixType(), 0x02u);
    EXPECT_EQ(pgn.getStatusFlags(), 0x02u);
    EXPECT_DOUBLE_EQ(pgn.getUtcTime(), 32.0012);
}

TEST(PGN129029, SerializeRoundTripFromFields)
{
    PGN129029 source(0x1D, 1.9088743, -1.9088744, 100.0, 0x12, 32.0012, 0x23, 6);

    auto parsedBase = Nmea2000Factory::create(source.serialize());
    ASSERT_NE(parsedBase, nullptr);
    auto parsed = dynamic_cast<PGN129029*>(parsedBase.get());
    ASSERT_NE(parsed, nullptr);

    EXPECT_EQ(parsed->getSid(), 0x1Du);
    EXPECT_NEAR(parsed->getLatitude(), 1.9088743, 1e-7);
    EXPECT_NEAR(parsed->getLongitude(), -1.9088744, 1e-7);
    EXPECT_NEAR(parsed->getAltitude(), 100.0, 1e-3);
    EXPECT_EQ(parsed->getFixStatus(), 0x12u);
    EXPECT_NEAR(parsed->getUtcTime(), 32.0012, 1e-4);

    const std::string wire = source.serialize();
    EXPECT_EQ(wire, PGN129029_MESSAGE);
}

TEST(PGN129029, ClonePreservesContent)
{
    PGN129029 source(0x1D, 1.9088743, -1.9088744, 100.0, 0x12, 32.0012, 0x23, 6);

    std::unique_ptr<nmealib::Message> clonedBase = source.clone();
    ASSERT_NE(clonedBase, nullptr);
    auto cloned = dynamic_cast<PGN129029*>(clonedBase.get());
    ASSERT_NE(cloned, nullptr);

    EXPECT_TRUE(source.hasEqualContent(*cloned));
    EXPECT_TRUE(source == *cloned);
}

TEST(PGN129029, FactoryDoesNotPromoteNon129029Message)
{
    auto base = Nmea2000Factory::create(NOT_129029_MESSAGE);
    ASSERT_NE(base, nullptr);
    auto pgn = dynamic_cast<PGN129029*>(base.get());
    EXPECT_EQ(pgn, nullptr);
}

TEST(PGN129029, FactoryThrowsOnMalformed129029Message)
{
    EXPECT_THROW(Nmea2000Factory::create(INCOMPLETE_PGN129029_MESSAGE), NotPGN129029Exception);
}
