#include <gtest/gtest.h>

#include <memory>
#include <string>

#include <nmealib.h>

using namespace nmealib::nmea0183;

static const std::string RMA_SENTENCE = "$GPRMA,A,4917.24,N,12309.57,W,0.0,0.0,0.5,54.7,20.3,E*67\r\n";
static const std::string RMA_SENTENCE_NO_CHECKSUM = "$GPRMA,A,4917.24,N,12309.57,W,0.0,0.0,0.5,54.7,20.3,E\r\n";
static const std::string INCOMPLETE_RMA_SENTENCE = "$GPRMA,V,,S,,E,,,,,,W\r\n";
static const std::string RMA_SENTENCE_WITH_TRAILING_MODE = "$IIRMA,A,5800.617,N,01145.801,E,,,2.3,22.5,0.0,E,A\r\n";
static const std::string NOT_RMA_SENTENCE = "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W,A,V*6A\r\n";
static const std::string MALFORMED_RMA_SENTENCE = "$GPRMA,A,4917.24,N\r\n";

TEST(RMA, CreateFromMessage0183Factory)
{
    auto msg = Nmea0183Factory::create(RMA_SENTENCE);
    ASSERT_NE(msg, nullptr);

    auto rmaMsg = dynamic_cast<RMA*>(msg.get());
    ASSERT_NE(rmaMsg, nullptr);

    EXPECT_EQ(rmaMsg->getTalker(), "GP");
    EXPECT_EQ(rmaMsg->getSentenceType(), "RMA");
    EXPECT_EQ(rmaMsg->getStatus(), 'A');
    EXPECT_NEAR(rmaMsg->getLatitude(), 49.287333333333336, 1e-12);
    EXPECT_EQ(rmaMsg->getLatitudeDirection(), 'N');
    EXPECT_NEAR(rmaMsg->getLongitude(), 123.1595, 1e-12);
    EXPECT_EQ(rmaMsg->getLongitudeDirection(), 'W');
    EXPECT_DOUBLE_EQ(rmaMsg->getTimeDifferenceA(), 0.0);
    EXPECT_DOUBLE_EQ(rmaMsg->getTimeDifferenceB(), 0.0);
    EXPECT_DOUBLE_EQ(rmaMsg->getSpeedOverGround(), 0.5);
    EXPECT_DOUBLE_EQ(rmaMsg->getTrackMadeGood(), 54.7);
    EXPECT_DOUBLE_EQ(rmaMsg->getMagneticVariation(), 20.3);
    EXPECT_EQ(rmaMsg->getVariationDirection(), 'E');

    msg = Nmea0183Factory::create(RMA_SENTENCE_NO_CHECKSUM);
    ASSERT_NE(msg, nullptr);
    rmaMsg = dynamic_cast<RMA*>(msg.get());
    ASSERT_NE(rmaMsg, nullptr);
    EXPECT_EQ(rmaMsg->getSentenceType(), "RMA");
}

TEST(RMA, CreateFromMessageFactoryWithIncompleteFieldsDefaults)
{
    auto msg = Nmea0183Factory::create(INCOMPLETE_RMA_SENTENCE);
    ASSERT_NE(msg, nullptr);
    auto rmaMsg = dynamic_cast<RMA*>(msg.get());
    ASSERT_NE(rmaMsg, nullptr);

    EXPECT_EQ(rmaMsg->getStatus(), 'V');
    EXPECT_DOUBLE_EQ(rmaMsg->getLatitude(), 0.0);
    EXPECT_EQ(rmaMsg->getLatitudeDirection(), 'S');
    EXPECT_DOUBLE_EQ(rmaMsg->getLongitude(), 0.0);
    EXPECT_EQ(rmaMsg->getLongitudeDirection(), 'E');
    EXPECT_DOUBLE_EQ(rmaMsg->getTimeDifferenceA(), 0.0);
    EXPECT_DOUBLE_EQ(rmaMsg->getTimeDifferenceB(), 0.0);
    EXPECT_DOUBLE_EQ(rmaMsg->getSpeedOverGround(), 0.0);
    EXPECT_DOUBLE_EQ(rmaMsg->getTrackMadeGood(), 0.0);
    EXPECT_DOUBLE_EQ(rmaMsg->getMagneticVariation(), 0.0);
    EXPECT_EQ(rmaMsg->getVariationDirection(), 'W');
}

TEST(RMA, CreateFromFieldsAndRoundTrip)
{
    RMA source("GP", 'A', 49.287333333333336, 'N', 123.1595, 'W', 0.0, 0.0, 0.5, 54.7, 20.3, 'E');

    EXPECT_EQ(source.getTalker(), "GP");
    EXPECT_EQ(source.getSentenceType(), "RMA");
    EXPECT_EQ(source.getStatus(), 'A');
    EXPECT_NEAR(source.getLatitude(), 49.287333333333336, 1e-12);
    EXPECT_EQ(source.getLatitudeDirection(), 'N');
    EXPECT_NEAR(source.getLongitude(), 123.1595, 1e-12);
    EXPECT_EQ(source.getLongitudeDirection(), 'W');
    EXPECT_DOUBLE_EQ(source.getSpeedOverGround(), 0.5);
    EXPECT_DOUBLE_EQ(source.getTrackMadeGood(), 54.7);
    EXPECT_DOUBLE_EQ(source.getMagneticVariation(), 20.3);
    EXPECT_EQ(source.getVariationDirection(), 'E');

    auto parsedBase = Nmea0183Factory::create(source.serialize());
    ASSERT_NE(parsedBase, nullptr);
    auto parsed = dynamic_cast<RMA*>(parsedBase.get());
    ASSERT_NE(parsed, nullptr);
    EXPECT_NEAR(parsed->getLatitude(), 49.287333333333336, 1e-4);
    EXPECT_NEAR(parsed->getLongitude(), 123.1595, 1e-4);
    EXPECT_DOUBLE_EQ(parsed->getSpeedOverGround(), 0.5);
    EXPECT_DOUBLE_EQ(parsed->getTrackMadeGood(), 54.7);
}

TEST(RMA, ClonePreservesContent)
{
    RMA source("GP", 'A', 49.287333333333336, 'N', 123.1595, 'W', 0.0, 0.0, 0.5, 54.7, 20.3, 'E');

    std::unique_ptr<nmealib::Message> clonedBase = source.clone();
    ASSERT_NE(clonedBase, nullptr);
    auto cloned = dynamic_cast<RMA*>(clonedBase.get());
    ASSERT_NE(cloned, nullptr);

    EXPECT_TRUE(source == *cloned);
}

TEST(RMA, FactoryDoesNotPromoteNonRmaSentence)
{
    auto base = Nmea0183Factory::create(NOT_RMA_SENTENCE);
    ASSERT_NE(base, nullptr);
    auto rma = dynamic_cast<RMA*>(base.get());
    EXPECT_EQ(rma, nullptr);
}

TEST(RMA, FactoryDoesNotPromoteRmaSentenceToOtherTypes)
{
    auto base = Nmea0183Factory::create(RMA_SENTENCE);
    ASSERT_NE(base, nullptr);

    auto rmc = dynamic_cast<RMC*>(base.get());
    auto rmb = dynamic_cast<RMB*>(base.get());
    EXPECT_EQ(rmc, nullptr);
    EXPECT_EQ(rmb, nullptr);
}

TEST(RMA, FactoryThrowsOnMalformedRmaSentence)
{
    EXPECT_THROW(Nmea0183Factory::create(MALFORMED_RMA_SENTENCE), NotRMAException);
}

TEST(RMA, AcceptsSentenceWithTrailingOptionalField)
{
    auto msg = Nmea0183Factory::create(RMA_SENTENCE_WITH_TRAILING_MODE);
    ASSERT_NE(msg, nullptr);

    auto rmaMsg = dynamic_cast<RMA*>(msg.get());
    ASSERT_NE(rmaMsg, nullptr);
    EXPECT_EQ(rmaMsg->getStatus(), 'A');
    EXPECT_NEAR(rmaMsg->getLatitude(), 58.0102833333, 1e-9);
    EXPECT_EQ(rmaMsg->getLatitudeDirection(), 'N');
    EXPECT_NEAR(rmaMsg->getLongitude(), 11.76335, 1e-9);
    EXPECT_EQ(rmaMsg->getLongitudeDirection(), 'E');
    EXPECT_DOUBLE_EQ(rmaMsg->getSpeedOverGround(), 2.3);
    EXPECT_DOUBLE_EQ(rmaMsg->getTrackMadeGood(), 22.5);
    EXPECT_DOUBLE_EQ(rmaMsg->getMagneticVariation(), 0.0);
    EXPECT_EQ(rmaMsg->getVariationDirection(), 'E');
}
