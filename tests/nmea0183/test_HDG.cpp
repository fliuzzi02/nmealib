#include <gtest/gtest.h>

#include <memory>
#include <string>

#include <nmealib.h>

using namespace nmealib::nmea0183;

static const std::string HDG_SENTENCE = "$IIHDG,274.1,1.2,E,3.4,W*58\r\n";
static const std::string HDG_SENTENCE_NO_CHECKSUM = "$IIHDG,274.1,1.2,E,3.4,W\r\n";
static const std::string INCOMPLETE_HDG_SENTENCE = "$IIHDG,,,,,\r\n";
static const std::string NOT_HDG_SENTENCE = "$GPZDA,160012.71,11,03,2004,-1,00*7D\r\n";
static const std::string MALFORMED_HDG_SENTENCE = "$IIHDG,274.1,1.2,E\r\n";

TEST(HDG, CreateFromMessage0183Factory)
{
    auto msg = Nmea0183Factory::create(HDG_SENTENCE);
    ASSERT_NE(msg, nullptr);

    auto hdgMsg = dynamic_cast<HDG*>(msg.get());
    ASSERT_NE(hdgMsg, nullptr);

    EXPECT_EQ(hdgMsg->getTalker(), "II");
    EXPECT_EQ(hdgMsg->getSentenceType(), "HDG");
    EXPECT_DOUBLE_EQ(hdgMsg->getMagneticHeading(), 274.1);
    EXPECT_DOUBLE_EQ(hdgMsg->getMagneticDeviation(), 1.2);
    EXPECT_EQ(hdgMsg->getDeviationDirection(), 'E');
    EXPECT_DOUBLE_EQ(hdgMsg->getMagneticVariation(), 3.4);
    EXPECT_EQ(hdgMsg->getVariationDirection(), 'W');

    msg = Nmea0183Factory::create(HDG_SENTENCE_NO_CHECKSUM);
    ASSERT_NE(msg, nullptr);
    hdgMsg = dynamic_cast<HDG*>(msg.get());
    ASSERT_NE(hdgMsg, nullptr);

    EXPECT_DOUBLE_EQ(hdgMsg->getMagneticHeading(), 274.1);
    EXPECT_DOUBLE_EQ(hdgMsg->getMagneticDeviation(), 1.2);
    EXPECT_EQ(hdgMsg->getDeviationDirection(), 'E');
    EXPECT_DOUBLE_EQ(hdgMsg->getMagneticVariation(), 3.4);
    EXPECT_EQ(hdgMsg->getVariationDirection(), 'W');
}

TEST(HDG, CreateFromFields)
{
    HDG hdg("II", 274.1, 1.2, 'E', 3.4, 'W');

    EXPECT_EQ(hdg.getTalker(), "II");
    EXPECT_EQ(hdg.getSentenceType(), "HDG");
    EXPECT_DOUBLE_EQ(hdg.getMagneticHeading(), 274.1);
    EXPECT_DOUBLE_EQ(hdg.getMagneticDeviation(), 1.2);
    EXPECT_EQ(hdg.getDeviationDirection(), 'E');
    EXPECT_DOUBLE_EQ(hdg.getMagneticVariation(), 3.4);
    EXPECT_EQ(hdg.getVariationDirection(), 'W');
}

TEST(HDG, SerializeRoundTripFromFields)
{
    HDG source("II", 274.1, 1.2, 'E', 3.4, 'W');

    auto parsedBase = Nmea0183Factory::create(source.serialize());
    ASSERT_NE(parsedBase, nullptr);
    auto parsed = dynamic_cast<HDG*>(parsedBase.get());
    ASSERT_NE(parsed, nullptr);

    EXPECT_DOUBLE_EQ(parsed->getMagneticHeading(), 274.1);
    EXPECT_DOUBLE_EQ(parsed->getMagneticDeviation(), 1.2);
    EXPECT_EQ(parsed->getDeviationDirection(), 'E');
    EXPECT_DOUBLE_EQ(parsed->getMagneticVariation(), 3.4);
    EXPECT_EQ(parsed->getVariationDirection(), 'W');

    const std::string wire = source.serialize();
    EXPECT_NE(wire.find("$IIHDG,274.1,1.2,E,3.4,W"), std::string::npos);
}

TEST(HDG, CreateFromMessageFactoryWithIncompleteFieldsDefaults)
{
    auto msg = Nmea0183Factory::create(INCOMPLETE_HDG_SENTENCE);
    ASSERT_NE(msg, nullptr);
    auto hdgMsg = dynamic_cast<HDG*>(msg.get());
    ASSERT_NE(hdgMsg, nullptr);

    EXPECT_DOUBLE_EQ(hdgMsg->getMagneticHeading(), 0.0);
    EXPECT_DOUBLE_EQ(hdgMsg->getMagneticDeviation(), 0.0);
    EXPECT_EQ(hdgMsg->getDeviationDirection(), '\0');
    EXPECT_DOUBLE_EQ(hdgMsg->getMagneticVariation(), 0.0);
    EXPECT_EQ(hdgMsg->getVariationDirection(), '\0');
}

TEST(HDG, ClonePreservesHdgContent)
{
    HDG source("II", 274.1, 1.2, 'E', 3.4, 'W');

    std::unique_ptr<nmealib::Message> clonedBase = source.clone();
    ASSERT_NE(clonedBase, nullptr);
    auto cloned = dynamic_cast<HDG*>(clonedBase.get());
    ASSERT_NE(cloned, nullptr);

    EXPECT_TRUE(source == *cloned);
}

TEST(HDG, FactoryDoesNotPromoteNonHdgSentence)
{
    auto base = Nmea0183Factory::create(NOT_HDG_SENTENCE);
    ASSERT_NE(base, nullptr);
    auto hdg = dynamic_cast<HDG*>(base.get());
    EXPECT_EQ(hdg, nullptr);
}

TEST(HDG, FactoryThrowsOnMalformedHdgSentence)
{
    EXPECT_THROW(Nmea0183Factory::create(MALFORMED_HDG_SENTENCE), NotHDGException);
}

TEST(HDG, BoundaryValues)
{
    HDG hdgMin("II", 0.0, 0.0, 'E', 0.0, 'W');
    EXPECT_DOUBLE_EQ(hdgMin.getMagneticHeading(), 0.0);
    EXPECT_DOUBLE_EQ(hdgMin.getMagneticDeviation(), 0.0);
    EXPECT_DOUBLE_EQ(hdgMin.getMagneticVariation(), 0.0);

    HDG hdgMax("II", 359.9, 99.9, 'W', 99.9, 'E');
    EXPECT_DOUBLE_EQ(hdgMax.getMagneticHeading(), 359.9);
    EXPECT_DOUBLE_EQ(hdgMax.getMagneticDeviation(), 99.9);
    EXPECT_DOUBLE_EQ(hdgMax.getMagneticVariation(), 99.9);
}

TEST(HDG, GetStringContent)
{
    HDG hdg("II", 274.1, 1.2, 'E', 3.4, 'W');
    std::string expectedVerbose = "--------------------------------\nProtocol: NMEA0183\nTalker: II\nSentence Type: HDG\nChecksum: None\nFields:\n\tMagnetic Heading: 274.1\n\tMagnetic Deviation: 1.2 E\n\tMagnetic Variation: 3.4 W\n";
    std::string expectedOneLiner = "[OK] NMEA0183 II HDG: HeadingM=274.1, Dev=1.2E, Var=3.4W";
    std::string actualVerbose = hdg.getStringContent(true);
    std::string actualOneLiner = hdg.getStringContent(false);
    EXPECT_EQ(actualVerbose, expectedVerbose);
    EXPECT_EQ(actualOneLiner, expectedOneLiner);
}
