#include <gtest/gtest.h>

#include <memory>
#include <string>

#include <nmealib.h>

using namespace nmealib::nmea0183;

static const std::string APB_SENTENCE = "$GPAPB,A,A,0.10,R,N,V,V,011,M,DEST,011,M,011,M*82\r\n";
static const std::string APB_SENTENCE_NO_CHECKSUM = "$GPAPB,A,A,0.10,R,N,V,V,011,M,DEST,011,M,011,M\r\n";
static const std::string INCOMPLETE_APB_SENTENCE = "$GPAPB,A,A,,L,N,,,,,DEST,,,," "\r\n";
static const std::string NOT_APB_SENTENCE = "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W,A,V*6A\r\n";
static const std::string MALFORMED_APB_SENTENCE = "$GPAPB,A,A,0.10,R,N,V,V,011,M,DEST,011,M\r\n";

TEST(APB, CreateFromMessage0183Factory)
{
    auto msg = Nmea0183Factory::create(APB_SENTENCE);
    ASSERT_NE(msg, nullptr);

    auto apbMsg = dynamic_cast<APB*>(msg.get());
    ASSERT_NE(apbMsg, nullptr);

    EXPECT_EQ(apbMsg->getTalker(), "GP");
    EXPECT_EQ(apbMsg->getSentenceType(), "APB");
    EXPECT_EQ(apbMsg->getStatus1(), 'A');
    EXPECT_EQ(apbMsg->getStatus2(), 'A');
    EXPECT_DOUBLE_EQ(apbMsg->getCrossTrackErrorMagnitude(), 0.10);
    EXPECT_EQ(apbMsg->getDirectionToSteer(), 'R');
    EXPECT_EQ(apbMsg->getCrossTrackUnits(), 'N');
    EXPECT_EQ(apbMsg->getArrivalCircleStatus(), 'V');
    EXPECT_EQ(apbMsg->getPerpendicularPassedAtWaypoint(), 'V');
    EXPECT_DOUBLE_EQ(apbMsg->getBearingOriginToDestination(), 11.0);
    EXPECT_EQ(apbMsg->getBearingOriginToDestinationType(), 'M');
    EXPECT_EQ(apbMsg->getDestinationWaypointId(), "DEST");
    EXPECT_DOUBLE_EQ(apbMsg->getBearingPresentToDestination(), 11.0);
    EXPECT_EQ(apbMsg->getBearingPresentToDestinationType(), 'M');
    EXPECT_DOUBLE_EQ(apbMsg->getHeadingToSteerToDestinationWaypoint(), 11.0);
    EXPECT_EQ(apbMsg->getHeadingToSteerToDestinationWaypointType(), 'M');

    msg = Nmea0183Factory::create(APB_SENTENCE_NO_CHECKSUM);
    ASSERT_NE(msg, nullptr);
    apbMsg = dynamic_cast<APB*>(msg.get());
    ASSERT_NE(apbMsg, nullptr);
    EXPECT_DOUBLE_EQ(apbMsg->getCrossTrackErrorMagnitude(), 0.10);
    EXPECT_EQ(apbMsg->getDestinationWaypointId(), "DEST");
}

TEST(APB, CreateFromFields)
{
    APB apb("GP", 'A', 'A', 0.10, 'R', 'N', 'V', 'V', 11.0, 'M', "DEST", 11.0, 'M', 11.0, 'M');

    EXPECT_EQ(apb.getTalker(), "GP");
    EXPECT_EQ(apb.getSentenceType(), "APB");
    EXPECT_EQ(apb.getStatus1(), 'A');
    EXPECT_EQ(apb.getStatus2(), 'A');
    EXPECT_DOUBLE_EQ(apb.getCrossTrackErrorMagnitude(), 0.10);
    EXPECT_EQ(apb.getDirectionToSteer(), 'R');
    EXPECT_EQ(apb.getCrossTrackUnits(), 'N');
    EXPECT_EQ(apb.getArrivalCircleStatus(), 'V');
    EXPECT_EQ(apb.getPerpendicularPassedAtWaypoint(), 'V');
    EXPECT_DOUBLE_EQ(apb.getBearingOriginToDestination(), 11.0);
    EXPECT_EQ(apb.getBearingOriginToDestinationType(), 'M');
    EXPECT_EQ(apb.getDestinationWaypointId(), "DEST");
    EXPECT_DOUBLE_EQ(apb.getBearingPresentToDestination(), 11.0);
    EXPECT_EQ(apb.getBearingPresentToDestinationType(), 'M');
    EXPECT_DOUBLE_EQ(apb.getHeadingToSteerToDestinationWaypoint(), 11.0);
    EXPECT_EQ(apb.getHeadingToSteerToDestinationWaypointType(), 'M');
}

TEST(APB, SerializeRoundTripFromFields)
{
    APB source("GP", 'A', 'A', 0.10, 'R', 'N', 'V', 'V', 11.0, 'M', "DEST", 11.0, 'M', 11.0, 'M');

    auto parsedBase = Nmea0183Factory::create(source.serialize());
    ASSERT_NE(parsedBase, nullptr);
    auto parsed = dynamic_cast<APB*>(parsedBase.get());
    ASSERT_NE(parsed, nullptr);

    EXPECT_EQ(parsed->getStatus1(), 'A');
    EXPECT_EQ(parsed->getStatus2(), 'A');
    EXPECT_DOUBLE_EQ(parsed->getCrossTrackErrorMagnitude(), 0.10);
    EXPECT_EQ(parsed->getDirectionToSteer(), 'R');
    EXPECT_EQ(parsed->getCrossTrackUnits(), 'N');
    EXPECT_EQ(parsed->getArrivalCircleStatus(), 'V');
    EXPECT_EQ(parsed->getPerpendicularPassedAtWaypoint(), 'V');
    EXPECT_DOUBLE_EQ(parsed->getBearingOriginToDestination(), 11.0);
    EXPECT_EQ(parsed->getBearingOriginToDestinationType(), 'M');
    EXPECT_EQ(parsed->getDestinationWaypointId(), "DEST");
    EXPECT_DOUBLE_EQ(parsed->getBearingPresentToDestination(), 11.0);
    EXPECT_EQ(parsed->getBearingPresentToDestinationType(), 'M');
    EXPECT_DOUBLE_EQ(parsed->getHeadingToSteerToDestinationWaypoint(), 11.0);
    EXPECT_EQ(parsed->getHeadingToSteerToDestinationWaypointType(), 'M');
}

TEST(APB, CreateFromMessageFactoryWithIncompleteFieldsDefaults)
{
    auto msg = Nmea0183Factory::create(INCOMPLETE_APB_SENTENCE);
    ASSERT_NE(msg, nullptr);
    auto apbMsg = dynamic_cast<APB*>(msg.get());
    ASSERT_NE(apbMsg, nullptr);

    EXPECT_EQ(apbMsg->getStatus1(), 'A');
    EXPECT_EQ(apbMsg->getStatus2(), 'A');
    EXPECT_DOUBLE_EQ(apbMsg->getCrossTrackErrorMagnitude(), 0.0);
    EXPECT_EQ(apbMsg->getDirectionToSteer(), 'L');
    EXPECT_EQ(apbMsg->getCrossTrackUnits(), 'N');
    EXPECT_EQ(apbMsg->getArrivalCircleStatus(), '\0');
    EXPECT_EQ(apbMsg->getPerpendicularPassedAtWaypoint(), '\0');
    EXPECT_DOUBLE_EQ(apbMsg->getBearingOriginToDestination(), 0.0);
    EXPECT_EQ(apbMsg->getBearingOriginToDestinationType(), '\0');
    EXPECT_EQ(apbMsg->getDestinationWaypointId(), "DEST");
    EXPECT_DOUBLE_EQ(apbMsg->getBearingPresentToDestination(), 0.0);
    EXPECT_EQ(apbMsg->getBearingPresentToDestinationType(), '\0');
    EXPECT_DOUBLE_EQ(apbMsg->getHeadingToSteerToDestinationWaypoint(), 0.0);
    EXPECT_EQ(apbMsg->getHeadingToSteerToDestinationWaypointType(), '\0');
}

TEST(APB, ClonePreservesApbContent)
{
    APB source("GP", 'A', 'A', 0.10, 'R', 'N', 'V', 'V', 11.0, 'M', "DEST", 11.0, 'M', 11.0, 'M');

    std::unique_ptr<nmealib::Message> clonedBase = source.clone();
    ASSERT_NE(clonedBase, nullptr);
    auto cloned = dynamic_cast<APB*>(clonedBase.get());
    ASSERT_NE(cloned, nullptr);

    EXPECT_TRUE(source == *cloned);
}

TEST(APB, FactoryDoesNotPromoteNonApbSentence)
{
    auto base = Nmea0183Factory::create(NOT_APB_SENTENCE);
    ASSERT_NE(base, nullptr);
    auto apb = dynamic_cast<APB*>(base.get());
    EXPECT_EQ(apb, nullptr);
}

TEST(APB, FactoryThrowsOnMalformedApbSentence)
{
    EXPECT_THROW(Nmea0183Factory::create(MALFORMED_APB_SENTENCE), NotAPBException);
}

TEST(APB, GetStringContent)
{
    APB apb("GP", 'A', 'A', 0.10, 'R', 'N', 'V', 'V', 11.0, 'M', "DEST", 11.0, 'M', 11.0, 'M');
    std::string expectedOneLiner = "[OK] NMEA0183 GP APB: Status1=A, Status2=A, XTE=0.10N, Steer=R, Arrival=V, Perpendicular=V, BOD=11.00M, Dest=DEST, BPD=11.00M, Heading=11.00M";
    std::string actualOneLiner = apb.getStringContent(false);
    EXPECT_EQ(actualOneLiner, expectedOneLiner);
}
