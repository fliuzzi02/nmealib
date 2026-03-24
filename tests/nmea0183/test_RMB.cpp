#include <gtest/gtest.h>

#include <memory>
#include <string>

#include <nmealib.h>

using namespace nmealib::nmea0183;

static const std::string RMB_SENTENCE = "$GPRMB,A,0.66,L,003,004,4917.24,N,12309.57,W,1.3,52.5,0.5,V*04\r\n";
static const std::string RMB_SENTENCE_NO_CHECKSUM = "$GPRMB,A,0.66,L,003,004,4917.24,N,12309.57,W,1.3,52.5,0.5,V\r\n";
static const std::string RMB_SENTENCE_WITH_FAA = "$GPRMB,A,0.66,L,003,004,4917.24,N,12309.57,W,1.3,52.5,0.5,V,A*25\r\n";
static const std::string INCOMPLETE_RMB_SENTENCE = "$GPRMB,A,,R,ORIG,DEST,,N,,W,,,,A\r\n";
static const std::string NOT_RMB_SENTENCE = "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W,A,V*6A\r\n";
static const std::string MALFORMED_RMB_SENTENCE = "$GPRMB,A,0.66,L\r\n";

TEST(RMB, CreateFromMessage0183Factory)
{
    auto msg = Nmea0183Factory::create(RMB_SENTENCE);
    ASSERT_NE(msg, nullptr);

    auto rmbMsg = dynamic_cast<RMB*>(msg.get());
    ASSERT_NE(rmbMsg, nullptr);

    EXPECT_EQ(rmbMsg->getTalker(), "GP");
    EXPECT_EQ(rmbMsg->getSentenceType(), "RMB");
    EXPECT_EQ(rmbMsg->getStatus(), 'A');
    EXPECT_DOUBLE_EQ(rmbMsg->getCrossTrackErrorNm(), 0.66);
    EXPECT_EQ(rmbMsg->getDirectionToSteer(), 'L');
    EXPECT_EQ(rmbMsg->getOriginWaypointId(), "003");
    EXPECT_EQ(rmbMsg->getDestinationWaypointId(), "004");
    EXPECT_NEAR(rmbMsg->getDestinationLatitude(), 49.287333333333336, 1e-12);
    EXPECT_EQ(rmbMsg->getDestinationLatitudeHemisphere(), 'N');
    EXPECT_NEAR(rmbMsg->getDestinationLongitude(), 123.1595, 1e-12);
    EXPECT_EQ(rmbMsg->getDestinationLongitudeHemisphere(), 'W');
    EXPECT_DOUBLE_EQ(rmbMsg->getRangeToDestinationNm(), 1.3);
    EXPECT_DOUBLE_EQ(rmbMsg->getBearingToDestinationTrue(), 52.5);
    EXPECT_DOUBLE_EQ(rmbMsg->getDestinationClosingVelocityKnots(), 0.5);
    EXPECT_EQ(rmbMsg->getArrivalStatus(), 'V');
    EXPECT_FALSE(rmbMsg->hasFaaModeIndicator());
    EXPECT_EQ(rmbMsg->getFaaModeIndicator(), std::nullopt);
}

TEST(RMB, CreateFromMessage0183FactoryWithOptionalFaa)
{
    auto msg = Nmea0183Factory::create(RMB_SENTENCE_WITH_FAA);
    ASSERT_NE(msg, nullptr);

    auto rmbMsg = dynamic_cast<RMB*>(msg.get());
    ASSERT_NE(rmbMsg, nullptr);

    ASSERT_TRUE(rmbMsg->hasFaaModeIndicator());
    ASSERT_TRUE(rmbMsg->getFaaModeIndicator().has_value());
    EXPECT_EQ(rmbMsg->getFaaModeIndicator().value(), 'A');
}

TEST(RMB, CreateFromFields)
{
    RMB rmb("GP", 'A', 0.66, 'L', "003", "004", 49.287333333333336, 'N', 123.1595, 'W', 1.3, 52.5, 0.5, 'V');

    EXPECT_EQ(rmb.getTalker(), "GP");
    EXPECT_EQ(rmb.getSentenceType(), "RMB");
    EXPECT_EQ(rmb.getStatus(), 'A');
    EXPECT_DOUBLE_EQ(rmb.getCrossTrackErrorNm(), 0.66);
    EXPECT_EQ(rmb.getDirectionToSteer(), 'L');
    EXPECT_EQ(rmb.getOriginWaypointId(), "003");
    EXPECT_EQ(rmb.getDestinationWaypointId(), "004");
    EXPECT_NEAR(rmb.getDestinationLatitude(), 49.287333333333336, 1e-12);
    EXPECT_EQ(rmb.getDestinationLatitudeHemisphere(), 'N');
    EXPECT_NEAR(rmb.getDestinationLongitude(), 123.1595, 1e-12);
    EXPECT_EQ(rmb.getDestinationLongitudeHemisphere(), 'W');
    EXPECT_DOUBLE_EQ(rmb.getRangeToDestinationNm(), 1.3);
    EXPECT_DOUBLE_EQ(rmb.getBearingToDestinationTrue(), 52.5);
    EXPECT_DOUBLE_EQ(rmb.getDestinationClosingVelocityKnots(), 0.5);
    EXPECT_EQ(rmb.getArrivalStatus(), 'V');
    EXPECT_FALSE(rmb.hasFaaModeIndicator());
}

TEST(RMB, SerializeRoundTripFromFieldsWithFaa)
{
    RMB source("GP", 'A', 0.66, 'L', "003", "004", 49.287333333333336, 'N', 123.1595, 'W', 1.3, 52.5, 0.5, 'V', 'A');

    auto parsedBase = Nmea0183Factory::create(source.serialize());
    ASSERT_NE(parsedBase, nullptr);
    auto parsed = dynamic_cast<RMB*>(parsedBase.get());
    ASSERT_NE(parsed, nullptr);

    EXPECT_EQ(parsed->getStatus(), 'A');
    EXPECT_DOUBLE_EQ(parsed->getCrossTrackErrorNm(), 0.66);
    EXPECT_EQ(parsed->getDirectionToSteer(), 'L');
    EXPECT_EQ(parsed->getOriginWaypointId(), "003");
    EXPECT_EQ(parsed->getDestinationWaypointId(), "004");
    EXPECT_NEAR(parsed->getDestinationLatitude(), 49.287333333333336, 1e-4);
    EXPECT_EQ(parsed->getDestinationLatitudeHemisphere(), 'N');
    EXPECT_NEAR(parsed->getDestinationLongitude(), 123.1595, 1e-4);
    EXPECT_EQ(parsed->getDestinationLongitudeHemisphere(), 'W');
    EXPECT_DOUBLE_EQ(parsed->getRangeToDestinationNm(), 1.3);
    EXPECT_DOUBLE_EQ(parsed->getBearingToDestinationTrue(), 52.5);
    EXPECT_DOUBLE_EQ(parsed->getDestinationClosingVelocityKnots(), 0.5);
    EXPECT_EQ(parsed->getArrivalStatus(), 'V');
    ASSERT_TRUE(parsed->getFaaModeIndicator().has_value());
    EXPECT_EQ(parsed->getFaaModeIndicator().value(), 'A');
}

TEST(RMB, CreateFromMessageFactoryWithIncompleteFieldsDefaults)
{
    auto msg = Nmea0183Factory::create(INCOMPLETE_RMB_SENTENCE);
    ASSERT_NE(msg, nullptr);
    auto rmbMsg = dynamic_cast<RMB*>(msg.get());
    ASSERT_NE(rmbMsg, nullptr);

    EXPECT_EQ(rmbMsg->getStatus(), 'A');
    EXPECT_DOUBLE_EQ(rmbMsg->getCrossTrackErrorNm(), 0.0);
    EXPECT_EQ(rmbMsg->getDirectionToSteer(), 'R');
    EXPECT_EQ(rmbMsg->getOriginWaypointId(), "ORIG");
    EXPECT_EQ(rmbMsg->getDestinationWaypointId(), "DEST");
    EXPECT_DOUBLE_EQ(rmbMsg->getDestinationLatitude(), 0.0);
    EXPECT_EQ(rmbMsg->getDestinationLatitudeHemisphere(), 'N');
    EXPECT_DOUBLE_EQ(rmbMsg->getDestinationLongitude(), 0.0);
    EXPECT_EQ(rmbMsg->getDestinationLongitudeHemisphere(), 'W');
    EXPECT_DOUBLE_EQ(rmbMsg->getRangeToDestinationNm(), 0.0);
    EXPECT_DOUBLE_EQ(rmbMsg->getBearingToDestinationTrue(), 0.0);
    EXPECT_DOUBLE_EQ(rmbMsg->getDestinationClosingVelocityKnots(), 0.0);
    EXPECT_EQ(rmbMsg->getArrivalStatus(), 'A');
    EXPECT_FALSE(rmbMsg->hasFaaModeIndicator());
}

TEST(RMB, FactoryDoesNotPromoteNonRmbSentence)
{
    auto base = Nmea0183Factory::create(NOT_RMB_SENTENCE);
    ASSERT_NE(base, nullptr);
    auto rmb = dynamic_cast<RMB*>(base.get());
    EXPECT_EQ(rmb, nullptr);
}

TEST(RMB, FactoryThrowsOnMalformedRmbSentence)
{
    EXPECT_THROW(Nmea0183Factory::create(MALFORMED_RMB_SENTENCE), NotRMBException);
}
