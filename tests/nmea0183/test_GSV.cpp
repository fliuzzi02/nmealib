#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <vector>

#include <nmealib.h>

using namespace nmealib::nmea0183;

static const std::string GSV_SENTENCE = "$GPGSV,3,1,11,03,03,111,00,04,15,270,00,06,01,010,00,13,06,292,00*74\r\n";
static const std::string GSV_SENTENCE_NO_CHECKSUM = "$GPGSV,3,2,11,14,25,170,00,16,57,208,39,18,67,296,40,19,40,246,00\r\n";
static const std::string GSV_SENTENCE_WITH_EMPTY_SNR = "$GPGSV,3,3,11,22,42,067,42,24,14,311,43,27,05,244,00,,,,*4D\r\n";
static const std::string GSV_SENTENCE_WITH_SIGNAL_ID = "$GPGSV,1,1,01,10,45,123,40,6*41\r\n";
static const std::string NOT_GSV_SENTENCE = "$GPGLL,4916.45,N,12311.12,W,225444,A,*1D\r\n";
static const std::string MALFORMED_GSV_SENTENCE = "$GPGSV,1,1,01,10,45\r\n";

TEST(GSV, CreateFromMessage0183Factory)
{
    auto msg = Nmea0183Factory::create(GSV_SENTENCE);
    ASSERT_NE(msg, nullptr);

    auto gsvMsg = dynamic_cast<GSV*>(msg.get());
    ASSERT_NE(gsvMsg, nullptr);

    EXPECT_EQ(gsvMsg->getTalker(), "GP");
    EXPECT_EQ(gsvMsg->getSentenceType(), "GSV");
    EXPECT_EQ(gsvMsg->getTotalSentences(), 3u);
    EXPECT_EQ(gsvMsg->getSentenceNumber(), 1u);
    EXPECT_EQ(gsvMsg->getSatellitesInView(), 11u);
    ASSERT_EQ(gsvMsg->getSatelliteCount(), 4u);

    const auto* firstSatellite = gsvMsg->getSatellite(0);
    ASSERT_NE(firstSatellite, nullptr);
    EXPECT_EQ(firstSatellite->satelliteId, 3u);
    EXPECT_EQ(firstSatellite->elevation, 3);
    EXPECT_EQ(firstSatellite->azimuth, 111u);
    ASSERT_TRUE(firstSatellite->snr.has_value());
    EXPECT_EQ(firstSatellite->snr.value(), 0u);

    msg = Nmea0183Factory::create(GSV_SENTENCE_NO_CHECKSUM);
    ASSERT_NE(msg, nullptr);
    gsvMsg = dynamic_cast<GSV*>(msg.get());
    ASSERT_NE(gsvMsg, nullptr);
    EXPECT_EQ(gsvMsg->getSentenceNumber(), 2u);
    ASSERT_EQ(gsvMsg->getSatelliteCount(), 4u);

    const auto* secondSatellite = gsvMsg->getSatellite(1);
    ASSERT_NE(secondSatellite, nullptr);
    ASSERT_TRUE(secondSatellite->snr.has_value());
    EXPECT_EQ(secondSatellite->snr.value(), 39u);
}

TEST(GSV, CreateFromMessage0183FactoryWithEmptySnr)
{
    auto msg = Nmea0183Factory::create(GSV_SENTENCE_WITH_EMPTY_SNR);
    ASSERT_NE(msg, nullptr);

    auto gsvMsg = dynamic_cast<GSV*>(msg.get());
    ASSERT_NE(gsvMsg, nullptr);

    ASSERT_EQ(gsvMsg->getSatelliteCount(), 4u);

    const auto* thirdSatellite = gsvMsg->getSatellite(2);
    ASSERT_NE(thirdSatellite, nullptr);
    EXPECT_EQ(thirdSatellite->satelliteId, 27u);
    ASSERT_TRUE(thirdSatellite->snr.has_value());
    EXPECT_EQ(thirdSatellite->snr.value(), 0u);

    const auto* fourthSatellite = gsvMsg->getSatellite(3);
    ASSERT_NE(fourthSatellite, nullptr);
    EXPECT_EQ(fourthSatellite->satelliteId, 0u);
    EXPECT_EQ(fourthSatellite->elevation, 0);
    EXPECT_EQ(fourthSatellite->azimuth, 0u);
    EXPECT_FALSE(fourthSatellite->snr.has_value());
}

TEST(GSV, CreateFromMessage0183FactoryWithSignalId)
{
    auto msg = Nmea0183Factory::create(GSV_SENTENCE_WITH_SIGNAL_ID);
    ASSERT_NE(msg, nullptr);

    auto gsvMsg = dynamic_cast<GSV*>(msg.get());
    ASSERT_NE(gsvMsg, nullptr);

    EXPECT_EQ(gsvMsg->getTotalSentences(), 1u);
    EXPECT_EQ(gsvMsg->getSentenceNumber(), 1u);
    EXPECT_EQ(gsvMsg->getSatellitesInView(), 1u);
    ASSERT_EQ(gsvMsg->getSatelliteCount(), 1u);
    ASSERT_TRUE(gsvMsg->hasSignalId());
    ASSERT_TRUE(gsvMsg->getSignalId().has_value());
    EXPECT_EQ(gsvMsg->getSignalId().value(), 6u);
}

TEST(GSV, CreateFromFields)
{
    std::vector<GSV::SatelliteInfo> satellites = {
        {3u, 3, 111u, 0u},
        {4u, 15, 270u, 0u},
        {6u, 1, 10u, 0u},
        {13u, 6, 292u, 0u}
    };

    GSV gsv("GP", 3u, 1u, 11u, satellites);

    EXPECT_EQ(gsv.getTalker(), "GP");
    EXPECT_EQ(gsv.getSentenceType(), "GSV");
    EXPECT_EQ(gsv.getTotalSentences(), 3u);
    EXPECT_EQ(gsv.getSentenceNumber(), 1u);
    EXPECT_EQ(gsv.getSatellitesInView(), 11u);
    ASSERT_EQ(gsv.getSatelliteCount(), 4u);
    EXPECT_FALSE(gsv.hasSignalId());
}

TEST(GSV, SerializeRoundTripFromFields)
{
    std::vector<GSV::SatelliteInfo> satellites = {
        {10u, 45, 123u, 40u}
    };

    GSV source("GP", 1u, 1u, 1u, satellites, 6u);

    auto parsedBase = Nmea0183Factory::create(source.serialize());
    ASSERT_NE(parsedBase, nullptr);
    auto parsed = dynamic_cast<GSV*>(parsedBase.get());
    ASSERT_NE(parsed, nullptr);

    EXPECT_EQ(parsed->getTotalSentences(), 1u);
    EXPECT_EQ(parsed->getSentenceNumber(), 1u);
    EXPECT_EQ(parsed->getSatellitesInView(), 1u);
    ASSERT_EQ(parsed->getSatelliteCount(), 1u);
    ASSERT_TRUE(parsed->hasSignalId());
    EXPECT_EQ(parsed->getSignalId().value(), 6u);

    const auto* satellite = parsed->getSatellite(0);
    ASSERT_NE(satellite, nullptr);
    EXPECT_EQ(satellite->satelliteId, 10u);
    EXPECT_EQ(satellite->elevation, 45);
    EXPECT_EQ(satellite->azimuth, 123u);
    ASSERT_TRUE(satellite->snr.has_value());
    EXPECT_EQ(satellite->snr.value(), 40u);

    const std::string wire = source.serialize();
    EXPECT_NE(wire.find("$GPGSV,1,1,1,10,45,123,40,6"), std::string::npos);
}

TEST(GSV, ClonePreservesGsvContent)
{
    std::vector<GSV::SatelliteInfo> satellites = {
        {10u, 45, 123u, 40u}
    };
    GSV source("GP", 1u, 1u, 1u, satellites, 6u);

    std::unique_ptr<nmealib::Message> clonedBase = source.clone();
    ASSERT_NE(clonedBase, nullptr);
    auto cloned = dynamic_cast<GSV*>(clonedBase.get());
    ASSERT_NE(cloned, nullptr);

    EXPECT_TRUE(source == *cloned);
}

TEST(GSV, FactoryDoesNotPromoteNonGsvSentence)
{
    auto base = Nmea0183Factory::create(NOT_GSV_SENTENCE);
    ASSERT_NE(base, nullptr);
    auto gsv = dynamic_cast<GSV*>(base.get());
    EXPECT_EQ(gsv, nullptr);
}

TEST(GSV, FactoryThrowsOnMalformedGsvSentence)
{
    EXPECT_THROW(Nmea0183Factory::create(MALFORMED_GSV_SENTENCE), NotGSVException);
}
