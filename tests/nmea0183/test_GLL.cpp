#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "gll.hpp"
#include "nmea0183Factory.hpp"

using namespace nmealib::nmea0183;

static const std::string GLL_SENTENCE = "$GNGLL,3150.788156,N,11711.922383,E,062735.00,A,A*76\r\n";
static const std::string GLL_SENTENCE_NO_CHECKSUM = "$GNGLL,3150.788156,N,11711.922383,E,062735.00,A,A\r\n";
static const std::string INCOMPLETE_GLL_SENTENCE = "$GNGLL,,N,11711.922383,E,,V,N\r\n";
static const std::string NOT_GLL_SENTENCE = "$GNGGA,062735.00,3150.788156,N,11711.922383,E,1,12,2.0,90.0,M,,M,,*55\r\n";
static const std::string MALFORMED_GLL_SENTENCE = "$GPGLL,3150.788156,N,11711.922383,E\r\n";
static const std::string HIGH_PRECISION_GLL_SENTENCE_NO_CHECKSUM = "$GNGLL,3150.7881569,N,11711.9223839,E,062735.00,A,A\r\n";

TEST(GLL, CreateFromMessage0183Factory)
{
    auto msg = Nmea0183Factory::create(GLL_SENTENCE);
    ASSERT_NE(msg, nullptr);

    auto gllMsg = dynamic_cast<GLL*>(msg.get());
    ASSERT_NE(gllMsg, nullptr);

    EXPECT_EQ(gllMsg->getTalker(), "GN");
    EXPECT_EQ(gllMsg->getSentenceType(), "GLL");
    EXPECT_NEAR(gllMsg->getLatitude(), 31.8464692667, 1e-9);
    EXPECT_EQ(gllMsg->getLatitudeDirection(), 'N');
    EXPECT_NEAR(gllMsg->getLongitude(), 117.1987063833, 1e-9);
    EXPECT_EQ(gllMsg->getLongitudeDirection(), 'E');
    EXPECT_DOUBLE_EQ(gllMsg->getTimestamp(), 62735.0);
    EXPECT_EQ(gllMsg->getStatus(), 'A');
    EXPECT_EQ(gllMsg->getModeIndicator(), 'A');

    msg = Nmea0183Factory::create(GLL_SENTENCE_NO_CHECKSUM);
    ASSERT_NE(msg, nullptr);
    gllMsg = dynamic_cast<GLL*>(msg.get());
    ASSERT_NE(gllMsg, nullptr);

    EXPECT_EQ(gllMsg->getTalker(), "GN");
    EXPECT_EQ(gllMsg->getSentenceType(), "GLL");
    EXPECT_NEAR(gllMsg->getLatitude(), 31.8464692667, 1e-9);
    EXPECT_EQ(gllMsg->getLatitudeDirection(), 'N');
    EXPECT_NEAR(gllMsg->getLongitude(), 117.1987063833, 1e-9);
    EXPECT_EQ(gllMsg->getLongitudeDirection(), 'E');
    EXPECT_DOUBLE_EQ(gllMsg->getTimestamp(), 62735.0);
    EXPECT_EQ(gllMsg->getStatus(), 'A');
    EXPECT_EQ(gllMsg->getModeIndicator(), 'A');
}

TEST(GLL, CreateFromFields)
{
    GLL gll("GN", 31.8464692667, 'N', 117.1987063833, 'E', 62735.0, 'A', 'A');

    EXPECT_EQ(gll.getTalker(), "GN");
    EXPECT_EQ(gll.getSentenceType(), "GLL");
    EXPECT_NEAR(gll.getLatitude(), 31.8464692667, 1e-9);
    EXPECT_EQ(gll.getLatitudeDirection(), 'N');
    EXPECT_NEAR(gll.getLongitude(), 117.1987063833, 1e-9);
    EXPECT_EQ(gll.getLongitudeDirection(), 'E');
    EXPECT_DOUBLE_EQ(gll.getTimestamp(), 62735.0);
    EXPECT_EQ(gll.getStatus(), 'A');
    EXPECT_EQ(gll.getModeIndicator(), 'A');
}

TEST(GLL, SerializeRoundTripFromFields)
{
    GLL source("GN", 31.8464692667, 'N', 117.1987063833, 'E', 62735.0, 'A', 'A');

    auto parsedBase = Nmea0183Factory::create(source.serialize());
    ASSERT_NE(parsedBase, nullptr);
    auto parsed = dynamic_cast<GLL*>(parsedBase.get());
    ASSERT_NE(parsed, nullptr);

    EXPECT_NEAR(parsed->getLatitude(), 31.8464692667, 1e-6);
    EXPECT_EQ(parsed->getLatitudeDirection(), 'N');
    EXPECT_NEAR(parsed->getLongitude(), 117.1987063833, 1e-6);
    EXPECT_EQ(parsed->getLongitudeDirection(), 'E');
    EXPECT_DOUBLE_EQ(parsed->getTimestamp(), 62735.0);
    EXPECT_EQ(parsed->getStatus(), 'A');
    EXPECT_EQ(parsed->getModeIndicator(), 'A');

    const std::string wire = source.serialize();
    EXPECT_NE(wire.find("$GNGLL,3150.788156,N,11711.922383,E,062735.00,A,A"), std::string::npos);
}

TEST(GLL, CreateFromMessageFactoryWithIncompleteFieldsDefaults)
{
    auto msg = Nmea0183Factory::create(INCOMPLETE_GLL_SENTENCE);
    ASSERT_NE(msg, nullptr);
    auto gllMsg = dynamic_cast<GLL*>(msg.get());
    ASSERT_NE(gllMsg, nullptr);

    EXPECT_DOUBLE_EQ(gllMsg->getLatitude(), 0.0);
    EXPECT_EQ(gllMsg->getLatitudeDirection(), 'N');
    EXPECT_NEAR(gllMsg->getLongitude(), 117.1987063833, 1e-9);
    EXPECT_EQ(gllMsg->getLongitudeDirection(), 'E');
    EXPECT_DOUBLE_EQ(gllMsg->getTimestamp(), 0.0);
    EXPECT_EQ(gllMsg->getStatus(), 'V');
    EXPECT_EQ(gllMsg->getModeIndicator(), 'N');
}

TEST(GLL, ClonePreservesGllContent)
{
    GLL source("GN", 31.8464692667, 'N', 117.1987063833, 'E', 62735.0, 'A', 'A');

    std::unique_ptr<nmealib::Message> clonedBase = source.clone();
    ASSERT_NE(clonedBase, nullptr);
    auto cloned = dynamic_cast<GLL*>(clonedBase.get());
    ASSERT_NE(cloned, nullptr);

    EXPECT_TRUE(source.hasEqualContent(*cloned));
    EXPECT_TRUE(source == *cloned);
}

TEST(GLL, FactoryDoesNotPromoteNonGllSentence)
{
    auto base = Nmea0183Factory::create(NOT_GLL_SENTENCE);
    ASSERT_NE(base, nullptr);
    auto gll = dynamic_cast<GLL*>(base.get());
    EXPECT_EQ(gll, nullptr);
}

TEST(GLL, FactoryThrowsOnMalformedGllSentence)
{
    EXPECT_THROW(Nmea0183Factory::create(MALFORMED_GLL_SENTENCE), NotGLLException);
}

TEST(GLL, ConvertsCoordinatesWithHighPrecision)
{
    auto msg = Nmea0183Factory::create(HIGH_PRECISION_GLL_SENTENCE_NO_CHECKSUM);
    ASSERT_NE(msg, nullptr);
    auto gllMsg = dynamic_cast<GLL*>(msg.get());
    ASSERT_NE(gllMsg, nullptr);

    const double expectedLatitude = 31.0 + (50.7881569 / 60.0);
    const double expectedLongitude = 117.0 + (11.9223839 / 60.0);

    EXPECT_NEAR(gllMsg->getLatitude(), expectedLatitude, 1e-12);
    EXPECT_NEAR(gllMsg->getLongitude(), expectedLongitude, 1e-12);
}
