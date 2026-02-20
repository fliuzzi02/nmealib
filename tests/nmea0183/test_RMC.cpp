#include <gtest/gtest.h>
#include <chrono>
#include <memory>
#include <string>
#include "nmea0183Factory.hpp"
#include "rmc.hpp"

using namespace nmealib::nmea0183;

static const std::string RMC_SENTENCE = "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W,A,V*6A\r\n";
static const std::string RMC_SENTENCE_NO_CHECKSUM = "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W,A,V\r\n";
static const std::string INCOMPLETE_RMC_SENTENCE = "$GPRMC,123519,A,,N,01131.000,E,022.4,,,,W,A,V*6A\r\n";
static const std::string NOT_RMC_SENTENCE = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n";
static const std::string MALFORMED_RMC_SENTENCE = "$GPRMC,123519,A\r\n";
static const std::string HIGH_PRECISION_RMC_SENTENCE_NO_CHECKSUM = "$GPRMC,123519,A,4807.0381234,N,01131.0009876,E,022.4,084.4,230394,003.1,W,A,V\r\n";

// Test RMC creation through parent class factory method
TEST(RMC, CreateFromMessage0183Factory)
{
    auto msg = Nmea0183Factory::create(RMC_SENTENCE);
    ASSERT_NE(msg, nullptr);
    // Attempt to downcast to RMC
    auto rmcMsg = dynamic_cast<RMC*>(msg.get());
    ASSERT_NE(rmcMsg, nullptr);
    EXPECT_EQ(rmcMsg->getTalker(), "GP");
    EXPECT_EQ(rmcMsg->getSentenceType(), "RMC");
    EXPECT_EQ(rmcMsg->getUtcFix(), 123519u);
    EXPECT_EQ(rmcMsg->getStatus(), 'A');
    EXPECT_DOUBLE_EQ(rmcMsg->getLatitude(), 48.1173);
    EXPECT_EQ(rmcMsg->getLatitudeDirection(), 'N');
    EXPECT_DOUBLE_EQ(rmcMsg->getLongitude(), 11.516666666666667);
    EXPECT_EQ(rmcMsg->getLongitudeDirection(), 'E');
    EXPECT_DOUBLE_EQ(rmcMsg->getSpeedOverGround(), 22.4);
    EXPECT_DOUBLE_EQ(rmcMsg->getCourseOverGround(), 84.4);
    EXPECT_EQ(rmcMsg->getDate(), 230394u);
    EXPECT_DOUBLE_EQ(rmcMsg->getMagneticVariation(), 3.1);
    EXPECT_EQ(rmcMsg->getMagneticVariationDirection(), 'W');
    EXPECT_EQ(rmcMsg->getModeIndicator(), 'A');
    EXPECT_EQ(rmcMsg->getNavigationStatus(), 'V');

    msg = Nmea0183Factory::create(RMC_SENTENCE_NO_CHECKSUM);
    ASSERT_NE(msg, nullptr);
    rmcMsg = dynamic_cast<RMC*>(msg.get());
    ASSERT_NE(rmcMsg, nullptr);
    EXPECT_EQ(rmcMsg->getTalker(), "GP");
    EXPECT_EQ(rmcMsg->getSentenceType(), "RMC");
    EXPECT_EQ(rmcMsg->getUtcFix(), 123519u);
    EXPECT_EQ(rmcMsg->getStatus(), 'A');
    EXPECT_DOUBLE_EQ(rmcMsg->getLatitude(), 48.1173);
    EXPECT_EQ(rmcMsg->getLatitudeDirection(), 'N');
    EXPECT_DOUBLE_EQ(rmcMsg->getLongitude(), 11.516666666666667);
    EXPECT_EQ(rmcMsg->getLongitudeDirection(), 'E');
    EXPECT_DOUBLE_EQ(rmcMsg->getSpeedOverGround(), 22.4);
    EXPECT_DOUBLE_EQ(rmcMsg->getCourseOverGround(), 84.4);
    EXPECT_EQ(rmcMsg->getDate(), 230394u);
    EXPECT_DOUBLE_EQ(rmcMsg->getMagneticVariation(), 3.1);
    EXPECT_EQ(rmcMsg->getMagneticVariationDirection(), 'W');
    EXPECT_EQ(rmcMsg->getModeIndicator(), 'A');
    EXPECT_EQ(rmcMsg->getNavigationStatus(), 'V');

    msg = Nmea0183Factory::create(INCOMPLETE_RMC_SENTENCE);
    ASSERT_NE(msg, nullptr);
    rmcMsg = dynamic_cast<RMC*>(msg.get());
    EXPECT_EQ(rmcMsg->getUtcFix(), 123519u);
    EXPECT_EQ(rmcMsg->getStatus(), 'A');
    EXPECT_DOUBLE_EQ(rmcMsg->getLatitude(), 0.0); // Empty latitude should default to 0.0
    EXPECT_EQ(rmcMsg->getLatitudeDirection(), 'N');
    EXPECT_DOUBLE_EQ(rmcMsg->getLongitude(), 11.516666666666667);
    EXPECT_EQ(rmcMsg->getLongitudeDirection(), 'E');
    EXPECT_DOUBLE_EQ(rmcMsg->getSpeedOverGround(), 22.4);
    EXPECT_DOUBLE_EQ(rmcMsg->getCourseOverGround(), 0.0); // Empty course should default to 0.0
    EXPECT_EQ(rmcMsg->getDate(), 0u); // Empty date should default to 0
    EXPECT_DOUBLE_EQ(rmcMsg->getMagneticVariation(), 0.0); // Empty magnetic variation should default to 0.0
    EXPECT_EQ(rmcMsg->getMagneticVariationDirection(), 'W');
    EXPECT_EQ(rmcMsg->getModeIndicator(), 'A');
    EXPECT_EQ(rmcMsg->getNavigationStatus(), 'V');
}

TEST(RMC, CreateFromFields) {
    RMC rmc("GP", 123519, 'A', 48.1173, 'N', 11.5166, 'E', 22.4, 84.4, 230394, 3.1, 'W', 'A', 'V');
    // Print the content of the RMC message for debugging
    EXPECT_EQ(rmc.getTalker(), "GP");
    EXPECT_EQ(rmc.getSentenceType(), "RMC");
    EXPECT_EQ(rmc.getUtcFix(), 123519u);
    EXPECT_EQ(rmc.getStatus(), 'A');
    EXPECT_DOUBLE_EQ(rmc.getLatitude(), 48.1173);
    EXPECT_EQ(rmc.getLatitudeDirection(), 'N');
    EXPECT_DOUBLE_EQ(rmc.getLongitude(), 11.5166);
    EXPECT_EQ(rmc.getLongitudeDirection(), 'E');
    EXPECT_DOUBLE_EQ(rmc.getSpeedOverGround(), 22.4);
    EXPECT_DOUBLE_EQ(rmc.getCourseOverGround(), 84.4);
    EXPECT_EQ(rmc.getDate(), 230394u);
    EXPECT_DOUBLE_EQ(rmc.getMagneticVariation(), 3.1);
    EXPECT_EQ(rmc.getMagneticVariationDirection(), 'W');
    EXPECT_EQ(rmc.getModeIndicator(), 'A');
    EXPECT_EQ(rmc.getNavigationStatus(), 'V');
}

TEST(RMC, SerializeRoundTripFromFields)
{
    RMC source("GP", 123519, 'A', 48.1173, 'N', 11.5166, 'E', 22.4, 84.4, 230394, 3.1, 'W', 'A', 'V');

    auto parsedBase = Nmea0183Factory::create(source.serialize());
    ASSERT_NE(parsedBase, nullptr);
    auto parsed = dynamic_cast<RMC*>(parsedBase.get());
    ASSERT_NE(parsed, nullptr);

    EXPECT_EQ(parsed->getTalker(), "GP");
    EXPECT_EQ(parsed->getSentenceType(), "RMC");
    EXPECT_EQ(parsed->getUtcFix(), 123519u);
    EXPECT_EQ(parsed->getStatus(), 'A');
    EXPECT_NEAR(parsed->getLatitude(), 48.1173, 1e-4);
    EXPECT_EQ(parsed->getLatitudeDirection(), 'N');
    EXPECT_NEAR(parsed->getLongitude(), 11.5166, 1e-4);
    EXPECT_EQ(parsed->getLongitudeDirection(), 'E');
    EXPECT_DOUBLE_EQ(parsed->getSpeedOverGround(), 22.4);
    EXPECT_DOUBLE_EQ(parsed->getCourseOverGround(), 84.4);
    EXPECT_EQ(parsed->getDate(), 230394u);
    EXPECT_DOUBLE_EQ(parsed->getMagneticVariation(), 3.1);
    EXPECT_EQ(parsed->getMagneticVariationDirection(), 'W');
    EXPECT_EQ(parsed->getModeIndicator(), 'A');
    EXPECT_EQ(parsed->getNavigationStatus(), 'V');
}

TEST(RMC, CreateFromFieldsSupportsHemisphereAndStatusVariants)
{
    RMC rmc("GP", 5, 'V', 12.5, 'S', 45.25, 'W', 0.0, 359.9, 101, 0.0, 'E', 'N', 'C');

    EXPECT_EQ(rmc.getUtcFix(), 5u);
    EXPECT_EQ(rmc.getStatus(), 'V');
    EXPECT_DOUBLE_EQ(rmc.getLatitude(), 12.5);
    EXPECT_EQ(rmc.getLatitudeDirection(), 'S');
    EXPECT_DOUBLE_EQ(rmc.getLongitude(), 45.25);
    EXPECT_EQ(rmc.getLongitudeDirection(), 'W');
    EXPECT_DOUBLE_EQ(rmc.getSpeedOverGround(), 0.0);
    EXPECT_DOUBLE_EQ(rmc.getCourseOverGround(), 359.9);
    EXPECT_EQ(rmc.getDate(), 101u);
    EXPECT_DOUBLE_EQ(rmc.getMagneticVariation(), 0.0);
    EXPECT_EQ(rmc.getMagneticVariationDirection(), 'E');
    EXPECT_EQ(rmc.getModeIndicator(), 'N');
    EXPECT_EQ(rmc.getNavigationStatus(), 'C');

    const std::string wire = rmc.serialize();
    EXPECT_NE(wire.find("$GPRMC,000005,V,"), std::string::npos);
    EXPECT_NE(wire.find(",S,"), std::string::npos);
    EXPECT_NE(wire.find(",W,"), std::string::npos);
    EXPECT_NE(wire.find(",000101,"), std::string::npos);
}

TEST(RMC, ClonePreservesRmcContent)
{
    RMC source("GP", 123519, 'A', 48.1173, 'N', 11.5166, 'E', 22.4, 84.4, 230394, 3.1, 'W', 'A', 'V');

    std::unique_ptr<nmealib::Message> clonedBase = source.clone();
    ASSERT_NE(clonedBase, nullptr);
    auto cloned = dynamic_cast<RMC*>(clonedBase.get());
    ASSERT_NE(cloned, nullptr);

    EXPECT_TRUE(source.hasEqualContent(*cloned));
    EXPECT_TRUE(source == *cloned);
}

TEST(RMC, StringContentFormatsAreStable)
{
    RMC rmc("GP", 123519, 'A', 48.1173, 'N', 11.5166, 'E', 22.4, 84.4, 230394, 3.1, 'W', 'A', 'V');

    const std::string compact = rmc.getStringContent(false);
    EXPECT_NE(compact.find("GP RMC: UTC Fix=123519"), std::string::npos);
    EXPECT_NE(compact.find("Lat=48.1173N"), std::string::npos);
    EXPECT_NE(compact.find("NavStatus=V"), std::string::npos);

    const std::string verbose = rmc.getStringContent(true);
    EXPECT_NE(verbose.find("Protocol: NMEA0183"), std::string::npos);
    EXPECT_NE(verbose.find("UTC Fix: 123519"), std::string::npos);
    EXPECT_NE(verbose.find("Latitude Direction: N"), std::string::npos);
    EXPECT_NE(verbose.find("Navigation Status: V"), std::string::npos);
}

TEST(RMC, FactoryDoesNotPromoteNonRmcSentence)
{
    auto base = Nmea0183Factory::create(NOT_RMC_SENTENCE);
    ASSERT_NE(base, nullptr);
    auto rmc = dynamic_cast<RMC*>(base.get());
    EXPECT_EQ(rmc, nullptr);
}

TEST(RMC, FactoryThrowsOnMalformedRmcSentence)
{
    EXPECT_THROW(Nmea0183Factory::create(MALFORMED_RMC_SENTENCE), NotRMCException);
}

TEST(RMC, ConvertsCoordinatesWithHighPrecision)
{
    auto msg = Nmea0183Factory::create(HIGH_PRECISION_RMC_SENTENCE_NO_CHECKSUM);
    ASSERT_NE(msg, nullptr);
    auto rmcMsg = dynamic_cast<RMC*>(msg.get());
    ASSERT_NE(rmcMsg, nullptr);

    const double expectedLatitude = 48.0 + (7.0381234 / 60.0);
    const double expectedLongitude = 11.0 + (31.0009876 / 60.0);

    EXPECT_NEAR(rmcMsg->getLatitude(), expectedLatitude, 1e-12);
    EXPECT_NEAR(rmcMsg->getLongitude(), expectedLongitude, 1e-12);
}