#include <gtest/gtest.h>

#include <memory>
#include <string>

#include <nmealib.h>

using namespace nmealib::nmea0183;

static const std::string XTE_SENTENCE = "$GPXTE,A,A,0.7,L,N*6F\r\n";
static const std::string XTE_SENTENCE_WITH_MODE = "$GPXTE,V,V,,,N,S*43\r\n";
static const std::string XTE_SENTENCE_NO_CHECKSUM = "$GPXTE,A,A,0.7,L,N\r\n";
static const std::string NOT_XTE_SENTENCE = "$GPZDA,160012.71,11,03,2004,-1,00*7D\r\n";
static const std::string MALFORMED_XTE_SENTENCE = "$GPXTE,A,A,0.7\r\n";

TEST(XTE, CreateFromMessage0183Factory)
{
    auto msg = Nmea0183Factory::create(XTE_SENTENCE);
    ASSERT_NE(msg, nullptr);

    auto xteMsg = dynamic_cast<XTE*>(msg.get());
    ASSERT_NE(xteMsg, nullptr);

    EXPECT_EQ(xteMsg->getTalker(), "GP");
    EXPECT_EQ(xteMsg->getSentenceType(), "XTE");
    EXPECT_EQ(xteMsg->getStatus1(), 'A');
    EXPECT_EQ(xteMsg->getStatus2(), 'A');
    EXPECT_DOUBLE_EQ(xteMsg->getCrossTrackError(), 0.7);
    EXPECT_EQ(xteMsg->getSteerDirection(), 'L');
    EXPECT_EQ(xteMsg->getCrossTrackUnits(), 'N');
    EXPECT_FALSE(xteMsg->hasFaaModeIndicator());

    msg = Nmea0183Factory::create(XTE_SENTENCE_NO_CHECKSUM);
    ASSERT_NE(msg, nullptr);
    xteMsg = dynamic_cast<XTE*>(msg.get());
    ASSERT_NE(xteMsg, nullptr);

    EXPECT_EQ(xteMsg->getStatus1(), 'A');
    EXPECT_EQ(xteMsg->getStatus2(), 'A');
    EXPECT_DOUBLE_EQ(xteMsg->getCrossTrackError(), 0.7);
    EXPECT_EQ(xteMsg->getSteerDirection(), 'L');
    EXPECT_EQ(xteMsg->getCrossTrackUnits(), 'N');
    EXPECT_FALSE(xteMsg->hasFaaModeIndicator());
}

TEST(XTE, CreateFromMessage0183FactoryWithMode)
{
    auto msg = Nmea0183Factory::create(XTE_SENTENCE_WITH_MODE);
    ASSERT_NE(msg, nullptr);

    auto xteMsg = dynamic_cast<XTE*>(msg.get());
    ASSERT_NE(xteMsg, nullptr);

    EXPECT_EQ(xteMsg->getStatus1(), 'V');
    EXPECT_EQ(xteMsg->getStatus2(), 'V');
    EXPECT_DOUBLE_EQ(xteMsg->getCrossTrackError(), 0.0);
    EXPECT_EQ(xteMsg->getSteerDirection(), '\0');
    EXPECT_EQ(xteMsg->getCrossTrackUnits(), 'N');
    ASSERT_TRUE(xteMsg->hasFaaModeIndicator());
    ASSERT_TRUE(xteMsg->getFaaModeIndicator().has_value());
    EXPECT_EQ(xteMsg->getFaaModeIndicator().value(), 'S');
}

TEST(XTE, CreateFromFields)
{
    XTE xte("GP", 'A', 'A', 0.7, 'L', 'N');

    EXPECT_EQ(xte.getTalker(), "GP");
    EXPECT_EQ(xte.getSentenceType(), "XTE");
    EXPECT_EQ(xte.getStatus1(), 'A');
    EXPECT_EQ(xte.getStatus2(), 'A');
    EXPECT_DOUBLE_EQ(xte.getCrossTrackError(), 0.7);
    EXPECT_EQ(xte.getSteerDirection(), 'L');
    EXPECT_EQ(xte.getCrossTrackUnits(), 'N');
    EXPECT_FALSE(xte.hasFaaModeIndicator());
}

TEST(XTE, SerializeRoundTripFromFields)
{
    XTE source("GP", 'A', 'A', 0.7, 'L', 'N', 'S');

    auto parsedBase = Nmea0183Factory::create(source.serialize());
    ASSERT_NE(parsedBase, nullptr);
    auto parsed = dynamic_cast<XTE*>(parsedBase.get());
    ASSERT_NE(parsed, nullptr);

    EXPECT_EQ(parsed->getStatus1(), 'A');
    EXPECT_EQ(parsed->getStatus2(), 'A');
    EXPECT_DOUBLE_EQ(parsed->getCrossTrackError(), 0.7);
    EXPECT_EQ(parsed->getSteerDirection(), 'L');
    EXPECT_EQ(parsed->getCrossTrackUnits(), 'N');
    ASSERT_TRUE(parsed->hasFaaModeIndicator());
    EXPECT_EQ(parsed->getFaaModeIndicator().value(), 'S');

    const std::string wire = source.serialize();
    EXPECT_NE(wire.find("$GPXTE,A,A,0.7,L,N,S"), std::string::npos);
}

TEST(XTE, ClonePreservesXteContent)
{
    XTE source("GP", 'A', 'A', 0.7, 'L', 'N', 'S');

    std::unique_ptr<nmealib::Message> clonedBase = source.clone();
    ASSERT_NE(clonedBase, nullptr);
    auto cloned = dynamic_cast<XTE*>(clonedBase.get());
    ASSERT_NE(cloned, nullptr);

    EXPECT_TRUE(source == *cloned);
}

TEST(XTE, FactoryDoesNotPromoteNonXteSentence)
{
    auto base = Nmea0183Factory::create(NOT_XTE_SENTENCE);
    ASSERT_NE(base, nullptr);
    auto xte = dynamic_cast<XTE*>(base.get());
    EXPECT_EQ(xte, nullptr);
}

TEST(XTE, FactoryThrowsOnMalformedXteSentence)
{
    EXPECT_THROW(Nmea0183Factory::create(MALFORMED_XTE_SENTENCE), NotXTEException);
}

TEST(XTE, GetStringContent)
{
    XTE xte("GP", 'A', 'A', 0.7, 'L', 'N', 'S');
    std::string expectedVerbose = "--------------------------------\nProtocol: NMEA0183\nTalker: GP\nSentence Type: XTE\nChecksum: None\nFields:\n\tStatus1: A\n\tStatus2: A\n\tCross Track Error: 0.70 N\n\tSteer Direction: L\n\tFAA Mode Indicator: S\n";
    std::string expectedOneLiner = "[OK] NMEA0183 GP XTE: Status1=A, Status2=A, XTE=0.70N, Steer=L, FAA=S";
    std::string actualVerbose = xte.getStringContent(true);
    std::string actualOneLiner = xte.getStringContent(false);
    EXPECT_EQ(actualVerbose, expectedVerbose);
    EXPECT_EQ(actualOneLiner, expectedOneLiner);
}
