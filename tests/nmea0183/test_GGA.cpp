#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "gga.hpp"
#include "nmea0183Factory.hpp"

using namespace nmealib::nmea0183;

static const std::string GGA_SENTENCE = "$GNGGA,062735.00,3150.788156,N,11711.922383,E,1,12,2.0,90.0,M,,M,,*55\r\n";
static const std::string GGA_SENTENCE_NO_CHECKSUM = "$GNGGA,062735.00,3150.788156,N,11711.922383,E,1,12,2.0,90.0,M,,M,,\r\n";
static const std::string INCOMPLETE_GGA_SENTENCE = "$GNGGA,062735.00,,N,11711.922383,E,1,12,2.0,,M,,M,,*7A\r\n";
static const std::string NOT_GGA_SENTENCE = "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W,A,V*6A\r\n";
static const std::string MALFORMED_GGA_SENTENCE = "$GPGGA,062735.00,3150.788156,N\r\n";

TEST(GGA, CreateFromMessage0183Factory)
{
    auto msg = Nmea0183Factory::create(GGA_SENTENCE);
    ASSERT_NE(msg, nullptr);

    auto ggaMsg = dynamic_cast<GGA*>(msg.get());
    ASSERT_NE(ggaMsg, nullptr);

    EXPECT_EQ(ggaMsg->getTalker(), "GN");
    EXPECT_EQ(ggaMsg->getSentenceType(), "GGA");
    EXPECT_DOUBLE_EQ(ggaMsg->getTimestamp(), 62735.0);
    EXPECT_NEAR(ggaMsg->getLatitude(), 31.8464692667, 1e-9);
    EXPECT_EQ(ggaMsg->getLatitudeDirection(), 'N');
    EXPECT_NEAR(ggaMsg->getLongitude(), 117.1987063833, 1e-9);
    EXPECT_EQ(ggaMsg->getLongitudeDirection(), 'E');
    EXPECT_EQ(ggaMsg->getGpsQuality(), 1u);
    EXPECT_EQ(ggaMsg->getSatellites(), 12u);
    EXPECT_DOUBLE_EQ(ggaMsg->getHdop(), 2.0);
    EXPECT_DOUBLE_EQ(ggaMsg->getAltitude(), 90.0);
    EXPECT_EQ(ggaMsg->getAltitudeUnits(), 'M');
    EXPECT_DOUBLE_EQ(ggaMsg->getGeoidalSeparation(), 0.0);
    EXPECT_EQ(ggaMsg->getGeoidalSeparationUnits(), 'M');
    EXPECT_DOUBLE_EQ(ggaMsg->getDgpsAge(), 0.0);
    EXPECT_EQ(ggaMsg->getDgpsReferenceStationId(), "");

    msg = Nmea0183Factory::create(GGA_SENTENCE_NO_CHECKSUM);
    ASSERT_NE(msg, nullptr);
    ggaMsg = dynamic_cast<GGA*>(msg.get());
    ASSERT_NE(ggaMsg, nullptr);

    EXPECT_EQ(ggaMsg->getTalker(), "GN");
    EXPECT_EQ(ggaMsg->getSentenceType(), "GGA");
    EXPECT_DOUBLE_EQ(ggaMsg->getTimestamp(), 62735.0);
    EXPECT_NEAR(ggaMsg->getLatitude(), 31.8464692667, 1e-9);
    EXPECT_EQ(ggaMsg->getLatitudeDirection(), 'N');
    EXPECT_NEAR(ggaMsg->getLongitude(), 117.1987063833, 1e-9);
    EXPECT_EQ(ggaMsg->getLongitudeDirection(), 'E');
}

TEST(GGA, CreateFromFields)
{
    GGA gga("GN", 62735.0, 31.8464692667, 'N', 117.1987063833, 'E', 1, 12, 2.0, 90.0, 'M', 0.0, 'M', 0.0, "");

    EXPECT_EQ(gga.getTalker(), "GN");
    EXPECT_EQ(gga.getSentenceType(), "GGA");
    EXPECT_DOUBLE_EQ(gga.getTimestamp(), 62735.0);
    EXPECT_NEAR(gga.getLatitude(), 31.8464692667, 1e-9);
    EXPECT_EQ(gga.getLatitudeDirection(), 'N');
    EXPECT_NEAR(gga.getLongitude(), 117.1987063833, 1e-9);
    EXPECT_EQ(gga.getLongitudeDirection(), 'E');
    EXPECT_EQ(gga.getGpsQuality(), 1u);
    EXPECT_EQ(gga.getSatellites(), 12u);
    EXPECT_DOUBLE_EQ(gga.getHdop(), 2.0);
    EXPECT_DOUBLE_EQ(gga.getAltitude(), 90.0);
    EXPECT_EQ(gga.getAltitudeUnits(), 'M');
    EXPECT_DOUBLE_EQ(gga.getGeoidalSeparation(), 0.0);
    EXPECT_EQ(gga.getGeoidalSeparationUnits(), 'M');
    EXPECT_DOUBLE_EQ(gga.getDgpsAge(), 0.0);
    EXPECT_EQ(gga.getDgpsReferenceStationId(), "");
}

TEST(GGA, SerializeRoundTripFromFields)
{
    GGA source("GN", 62735.0, 31.8464692667, 'N', 117.1987063833, 'E', 1, 12, 2.0, 90.0, 'M', 0.0, 'M', 0.0, "");

    auto parsedBase = Nmea0183Factory::create(source.serialize());
    ASSERT_NE(parsedBase, nullptr);
    auto parsed = dynamic_cast<GGA*>(parsedBase.get());
    ASSERT_NE(parsed, nullptr);

    EXPECT_DOUBLE_EQ(parsed->getTimestamp(), 62735.0);
    EXPECT_NEAR(parsed->getLatitude(), 31.8464692667, 1e-6);
    EXPECT_EQ(parsed->getLatitudeDirection(), 'N');
    EXPECT_NEAR(parsed->getLongitude(), 117.1987063833, 1e-6);
    EXPECT_EQ(parsed->getLongitudeDirection(), 'E');
    EXPECT_EQ(parsed->getGpsQuality(), 1u);
    EXPECT_EQ(parsed->getSatellites(), 12u);

    const std::string wire = source.serialize();
    EXPECT_NE(wire.find("$GNGGA,062735.00,"), std::string::npos);
    EXPECT_NE(wire.find(",3150.788156,N,11711.922383,E,"), std::string::npos);
    EXPECT_NE(wire.find(",1,12,2.0,90.0,M,,M,,"), std::string::npos);
}

TEST(GGA, CreateFromMessageFactoryWithIncompleteFieldsDefaults)
{
    auto msg = Nmea0183Factory::create(INCOMPLETE_GGA_SENTENCE);
    ASSERT_NE(msg, nullptr);
    auto ggaMsg = dynamic_cast<GGA*>(msg.get());
    ASSERT_NE(ggaMsg, nullptr);

    EXPECT_DOUBLE_EQ(ggaMsg->getTimestamp(), 62735.0);
    EXPECT_DOUBLE_EQ(ggaMsg->getLatitude(), 0.0);
    EXPECT_EQ(ggaMsg->getLatitudeDirection(), 'N');
    EXPECT_NEAR(ggaMsg->getLongitude(), 117.1987063833, 1e-9);
    EXPECT_EQ(ggaMsg->getGpsQuality(), 1u);
    EXPECT_EQ(ggaMsg->getSatellites(), 12u);
    EXPECT_DOUBLE_EQ(ggaMsg->getAltitude(), 0.0);
    EXPECT_EQ(ggaMsg->getAltitudeUnits(), 'M');
    EXPECT_DOUBLE_EQ(ggaMsg->getDgpsAge(), 0.0);
    EXPECT_EQ(ggaMsg->getDgpsReferenceStationId(), "");
}

TEST(GGA, ClonePreservesGgaContent)
{
    GGA source("GN", 62735.0, 31.8464692667, 'N', 117.1987063833, 'E', 1, 12, 2.0, 90.0, 'M', 0.0, 'M', 0.0, "");

    std::unique_ptr<nmealib::Message> clonedBase = source.clone();
    ASSERT_NE(clonedBase, nullptr);
    auto cloned = dynamic_cast<GGA*>(clonedBase.get());
    ASSERT_NE(cloned, nullptr);

    EXPECT_TRUE(source.hasEqualContent(*cloned));
    EXPECT_TRUE(source == *cloned);
}

TEST(GGA, FactoryDoesNotPromoteNonGgaSentence)
{
    auto base = Nmea0183Factory::create(NOT_GGA_SENTENCE);
    ASSERT_NE(base, nullptr);
    auto gga = dynamic_cast<GGA*>(base.get());
    EXPECT_EQ(gga, nullptr);
}

TEST(GGA, FactoryThrowsOnMalformedGgaSentence)
{
    EXPECT_THROW(Nmea0183Factory::create(MALFORMED_GGA_SENTENCE), NotGGAException);
}
