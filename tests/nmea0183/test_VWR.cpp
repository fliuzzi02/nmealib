#include <gtest/gtest.h>

#include <memory>
#include <string>

#include <nmealib.h>

using namespace nmealib::nmea0183;

static const std::string VWR_SENTENCE = "$IIVWR,45.0,R,10.5,N,5.4,M,19.4,K*55\r\n";
static const std::string VWR_SENTENCE_NO_CHECKSUM = "$IIVWR,45.0,R,10.5,N,5.4,M,19.4,K\r\n";
static const std::string INCOMPLETE_VWR_SENTENCE = "$IIVWR,,L,,N,,M,,K\r\n";
static const std::string NOT_VWR_SENTENCE = "$GPZDA,160012.71,11,03,2004,-1,00*7D\r\n";
static const std::string MALFORMED_VWR_SENTENCE = "$IIVWR,45.0,R,10.5\r\n";

TEST(VWR, CreateFromMessage0183Factory)
{
    auto msg = Nmea0183Factory::create(VWR_SENTENCE);
    ASSERT_NE(msg, nullptr);

    auto vwrMsg = dynamic_cast<VWR*>(msg.get());
    ASSERT_NE(vwrMsg, nullptr);

    EXPECT_EQ(vwrMsg->getTalker(), "II");
    EXPECT_EQ(vwrMsg->getSentenceType(), "VWR");
    EXPECT_DOUBLE_EQ(vwrMsg->getWindAngle(), 45.0);
    EXPECT_EQ(vwrMsg->getWindSide(), 'R');
    EXPECT_DOUBLE_EQ(vwrMsg->getSpeedKnots(), 10.5);
    EXPECT_EQ(vwrMsg->getSpeedKnotsUnit(), 'N');
    EXPECT_DOUBLE_EQ(vwrMsg->getSpeedMps(), 5.4);
    EXPECT_EQ(vwrMsg->getSpeedMpsUnit(), 'M');
    EXPECT_DOUBLE_EQ(vwrMsg->getSpeedKph(), 19.4);
    EXPECT_EQ(vwrMsg->getSpeedKphUnit(), 'K');

    msg = Nmea0183Factory::create(VWR_SENTENCE_NO_CHECKSUM);
    ASSERT_NE(msg, nullptr);
    vwrMsg = dynamic_cast<VWR*>(msg.get());
    ASSERT_NE(vwrMsg, nullptr);

    EXPECT_DOUBLE_EQ(vwrMsg->getWindAngle(), 45.0);
    EXPECT_EQ(vwrMsg->getWindSide(), 'R');
    EXPECT_DOUBLE_EQ(vwrMsg->getSpeedKnots(), 10.5);
    EXPECT_DOUBLE_EQ(vwrMsg->getSpeedMps(), 5.4);
    EXPECT_DOUBLE_EQ(vwrMsg->getSpeedKph(), 19.4);
}

TEST(VWR, CreateFromFields)
{
    VWR vwr("II", 45.0, 'R', 10.5, 5.4, 19.4);

    EXPECT_EQ(vwr.getTalker(), "II");
    EXPECT_EQ(vwr.getSentenceType(), "VWR");
    EXPECT_DOUBLE_EQ(vwr.getWindAngle(), 45.0);
    EXPECT_EQ(vwr.getWindSide(), 'R');
    EXPECT_DOUBLE_EQ(vwr.getSpeedKnots(), 10.5);
    EXPECT_EQ(vwr.getSpeedKnotsUnit(), 'N');
    EXPECT_DOUBLE_EQ(vwr.getSpeedMps(), 5.4);
    EXPECT_EQ(vwr.getSpeedMpsUnit(), 'M');
    EXPECT_DOUBLE_EQ(vwr.getSpeedKph(), 19.4);
    EXPECT_EQ(vwr.getSpeedKphUnit(), 'K');
}

TEST(VWR, SerializeRoundTripFromFields)
{
    VWR source("II", 45.0, 'R', 10.5, 5.4, 19.4);

    auto parsedBase = Nmea0183Factory::create(source.serialize());
    ASSERT_NE(parsedBase, nullptr);
    auto parsed = dynamic_cast<VWR*>(parsedBase.get());
    ASSERT_NE(parsed, nullptr);

    EXPECT_DOUBLE_EQ(parsed->getWindAngle(), 45.0);
    EXPECT_EQ(parsed->getWindSide(), 'R');
    EXPECT_DOUBLE_EQ(parsed->getSpeedKnots(), 10.5);
    EXPECT_EQ(parsed->getSpeedKnotsUnit(), 'N');
    EXPECT_DOUBLE_EQ(parsed->getSpeedMps(), 5.4);
    EXPECT_EQ(parsed->getSpeedMpsUnit(), 'M');
    EXPECT_DOUBLE_EQ(parsed->getSpeedKph(), 19.4);
    EXPECT_EQ(parsed->getSpeedKphUnit(), 'K');

    const std::string wire = source.serialize();
    EXPECT_NE(wire.find("$IIVWR,45.0,R,10.5,N,5.4,M,19.4,K"), std::string::npos);
}

TEST(VWR, CreateFromMessageFactoryWithIncompleteFieldsDefaults)
{
    auto msg = Nmea0183Factory::create(INCOMPLETE_VWR_SENTENCE);
    ASSERT_NE(msg, nullptr);
    auto vwrMsg = dynamic_cast<VWR*>(msg.get());
    ASSERT_NE(vwrMsg, nullptr);

    EXPECT_DOUBLE_EQ(vwrMsg->getWindAngle(), 0.0);
    EXPECT_EQ(vwrMsg->getWindSide(), 'L');
    EXPECT_DOUBLE_EQ(vwrMsg->getSpeedKnots(), 0.0);
    EXPECT_EQ(vwrMsg->getSpeedKnotsUnit(), 'N');
    EXPECT_DOUBLE_EQ(vwrMsg->getSpeedMps(), 0.0);
    EXPECT_EQ(vwrMsg->getSpeedMpsUnit(), 'M');
    EXPECT_DOUBLE_EQ(vwrMsg->getSpeedKph(), 0.0);
    EXPECT_EQ(vwrMsg->getSpeedKphUnit(), 'K');
}

TEST(VWR, ClonePreservesVwrContent)
{
    VWR source("II", 45.0, 'R', 10.5, 5.4, 19.4);

    std::unique_ptr<nmealib::Message> clonedBase = source.clone();
    ASSERT_NE(clonedBase, nullptr);
    auto cloned = dynamic_cast<VWR*>(clonedBase.get());
    ASSERT_NE(cloned, nullptr);

    EXPECT_TRUE(source == *cloned);
}

TEST(VWR, FactoryDoesNotPromoteNonVwrSentence)
{
    auto base = Nmea0183Factory::create(NOT_VWR_SENTENCE);
    ASSERT_NE(base, nullptr);
    auto vwr = dynamic_cast<VWR*>(base.get());
    EXPECT_EQ(vwr, nullptr);
}

TEST(VWR, FactoryThrowsOnMalformedVwrSentence)
{
    EXPECT_THROW(Nmea0183Factory::create(MALFORMED_VWR_SENTENCE), NotVWRException);
}

// Additional tests for full coverage
TEST(VWR, GetStringContent)
{
    VWR vwr("II", 45.0, 'R', 10.5, 5.4, 19.4);
    std::string expectedVerbose = "--------------------------------\nProtocol: NMEA0183\nTalker: II\nSentence Type: VWR\nChecksum: None\nFields:\n\tWind Angle: 45.00\n\tWind Side: R\n\tSpeed: 10.50kts\n\tSpeed: 5.40 m/s\n\tSpeed: 19.40 kph\n";
    std::string expectedOneLiner = "[OK] NMEA0183 II VWR: Angle=45.00R, Knots=10.50, m/s=5.40, KPH=19.40";
    std::string actualVerbose = vwr.getStringContent(true);
    std::string actualOneLiner = vwr.getStringContent(false);
    EXPECT_EQ(actualVerbose, expectedVerbose);
    EXPECT_EQ(actualOneLiner, expectedOneLiner);
}