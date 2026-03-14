#include <gtest/gtest.h>

#include <memory>
#include <string>

#include <nmealib.h>

using namespace nmealib::nmea0183;

static const std::string HDM_SENTENCE = "$IIHDM,45.0,M*13\r\n";
static const std::string HDM_SENTENCE_NO_CHECKSUM = "$IIHDM,45.0,M\r\n";
static const std::string INCOMPLETE_HDM_SENTENCE = "$IIHDM,,M\r\n";
static const std::string NOT_HDM_SENTENCE = "$GPZDA,160012.71,11,03,2004,-1,00*7D\r\n";
static const std::string MALFORMED_HDM_SENTENCE = "$IIHDM,45.0\r\n";

TEST(HDM, CreateFromMessage0183Factory)
{
    auto msg = Nmea0183Factory::create(HDM_SENTENCE);
    ASSERT_NE(msg, nullptr);

    auto hdmMsg = dynamic_cast<HDM*>(msg.get());
    ASSERT_NE(hdmMsg, nullptr);

    EXPECT_EQ(hdmMsg->getTalker(), "II");
    EXPECT_EQ(hdmMsg->getSentenceType(), "HDM");
    EXPECT_DOUBLE_EQ(hdmMsg->getHeading(), 45.0);
    EXPECT_EQ(hdmMsg->getIndicator(), 'M');

    msg = Nmea0183Factory::create(HDM_SENTENCE_NO_CHECKSUM);
    ASSERT_NE(msg, nullptr);
    hdmMsg = dynamic_cast<HDM*>(msg.get());
    ASSERT_NE(hdmMsg, nullptr);

    EXPECT_DOUBLE_EQ(hdmMsg->getHeading(), 45.0);
    EXPECT_EQ(hdmMsg->getIndicator(), 'M');
}

TEST(HDM, CreateFromFields)
{
    HDM hdm("II", 45.0, 'M');

    EXPECT_EQ(hdm.getTalker(), "II");
    EXPECT_EQ(hdm.getSentenceType(), "HDM");
    EXPECT_DOUBLE_EQ(hdm.getHeading(), 45.0);
    EXPECT_EQ(hdm.getIndicator(), 'M');
}

TEST(HDM, SerializeRoundTripFromFields)
{
    HDM source("II", 45.0, 'M');

    auto parsedBase = Nmea0183Factory::create(source.serialize());
    ASSERT_NE(parsedBase, nullptr);
    auto parsed = dynamic_cast<HDM*>(parsedBase.get());
    ASSERT_NE(parsed, nullptr);

    EXPECT_DOUBLE_EQ(parsed->getHeading(), 45.0);
    EXPECT_EQ(parsed->getIndicator(), 'M');

    const std::string wire = source.serialize();
    EXPECT_NE(wire.find("$IIHDM,45.0,M"), std::string::npos);
}

TEST(HDM, CreateFromMessageFactoryWithIncompleteFieldsDefaults)
{
    auto msg = Nmea0183Factory::create(INCOMPLETE_HDM_SENTENCE);
    ASSERT_NE(msg, nullptr);
    auto hdmMsg = dynamic_cast<HDM*>(msg.get());
    ASSERT_NE(hdmMsg, nullptr);

    EXPECT_DOUBLE_EQ(hdmMsg->getHeading(), 0.0);
    EXPECT_EQ(hdmMsg->getIndicator(), 'M');
}

TEST(HDM, ClonePreservesHdmContent)
{
    HDM source("II", 45.0, 'M');

    std::unique_ptr<nmealib::Message> clonedBase = source.clone();
    ASSERT_NE(clonedBase, nullptr);
    auto cloned = dynamic_cast<HDM*>(clonedBase.get());
    ASSERT_NE(cloned, nullptr);

    EXPECT_TRUE(source == *cloned);
}

TEST(HDM, FactoryDoesNotPromoteNonHdmSentence)
{
    auto base = Nmea0183Factory::create(NOT_HDM_SENTENCE);
    ASSERT_NE(base, nullptr);
    auto hdm = dynamic_cast<HDM*>(base.get());
    EXPECT_EQ(hdm, nullptr);
}

TEST(HDM, FactoryThrowsOnMalformedHdmSentence)
{
    EXPECT_THROW(Nmea0183Factory::create(MALFORMED_HDM_SENTENCE), NotHDMException);
}

TEST(HDM, BoundaryHeadingValues)
{
    HDM hdmZero("II", 0.0, 'M');
    EXPECT_DOUBLE_EQ(hdmZero.getHeading(), 0.0);
    EXPECT_EQ(hdmZero.getIndicator(), 'M');

    HDM hdmMax("II", 359.9, 'M');
    EXPECT_DOUBLE_EQ(hdmMax.getHeading(), 359.9);
    EXPECT_EQ(hdmMax.getIndicator(), 'M');
}

// Additional tests for full coverage
TEST(HDM, GetStringContent)
{
    HDM hdm("II", 45.0, 'M');
    std::string expectedVerbose = "--------------------------------\nProtocol: NMEA0183\nTalker: II\nSentence Type: HDM\nChecksum: None\nFields:\n\tHeading: 45.0 M\n";
    std::string expectedOneLiner = "[OK] NMEA0183 II HDM: Heading=45.0M";
    std::string actualVerbose = hdm.getStringContent(true);
    std::string actualOneLiner = hdm.getStringContent(false);
    EXPECT_EQ(actualVerbose, expectedVerbose);
    EXPECT_EQ(actualOneLiner, expectedOneLiner);
}