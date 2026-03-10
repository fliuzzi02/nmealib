#include <gtest/gtest.h>

#include <memory>
#include <string>

#include <nmealib.h>

using namespace nmealib::nmea0183;

static const std::string MTW_SENTENCE = "$IIMTW,23.5,C*1E\r\n";
static const std::string MTW_SENTENCE_NO_CHECKSUM = "$IIMTW,23.5,C\r\n";
static const std::string INCOMPLETE_MTW_SENTENCE = "$IIMTW,,C\r\n";
static const std::string NOT_MTW_SENTENCE = "$GPZDA,160012.71,11,03,2004,-1,00*7D\r\n";
static const std::string MALFORMED_MTW_SENTENCE = "$IIMTW,23.5\r\n";

TEST(MTW, CreateFromMessage0183Factory)
{
    auto msg = Nmea0183Factory::create(MTW_SENTENCE);
    ASSERT_NE(msg, nullptr);

    auto mtwMsg = dynamic_cast<MTW*>(msg.get());
    ASSERT_NE(mtwMsg, nullptr);

    EXPECT_EQ(mtwMsg->getTalker(), "II");
    EXPECT_EQ(mtwMsg->getSentenceType(), "MTW");
    EXPECT_DOUBLE_EQ(mtwMsg->getTemperature(), 23.5);
    EXPECT_EQ(mtwMsg->getUnit(), 'C');

    msg = Nmea0183Factory::create(MTW_SENTENCE_NO_CHECKSUM);
    ASSERT_NE(msg, nullptr);
    mtwMsg = dynamic_cast<MTW*>(msg.get());
    ASSERT_NE(mtwMsg, nullptr);

    EXPECT_DOUBLE_EQ(mtwMsg->getTemperature(), 23.5);
    EXPECT_EQ(mtwMsg->getUnit(), 'C');
}

TEST(MTW, CreateFromFields)
{
    MTW mtw("II", 23.5, 'C');

    EXPECT_EQ(mtw.getTalker(), "II");
    EXPECT_EQ(mtw.getSentenceType(), "MTW");
    EXPECT_DOUBLE_EQ(mtw.getTemperature(), 23.5);
    EXPECT_EQ(mtw.getUnit(), 'C');
}

TEST(MTW, SerializeRoundTripFromFields)
{
    MTW source("II", 23.5, 'C');

    auto parsedBase = Nmea0183Factory::create(source.serialize());
    ASSERT_NE(parsedBase, nullptr);
    auto parsed = dynamic_cast<MTW*>(parsedBase.get());
    ASSERT_NE(parsed, nullptr);

    EXPECT_DOUBLE_EQ(parsed->getTemperature(), 23.5);
    EXPECT_EQ(parsed->getUnit(), 'C');

    const std::string wire = source.serialize();
    EXPECT_NE(wire.find("$IIMTW,23.5,C"), std::string::npos);
}

TEST(MTW, CreateFromMessageFactoryWithIncompleteFieldsDefaults)
{
    auto msg = Nmea0183Factory::create(INCOMPLETE_MTW_SENTENCE);
    ASSERT_NE(msg, nullptr);
    auto mtwMsg = dynamic_cast<MTW*>(msg.get());
    ASSERT_NE(mtwMsg, nullptr);

    EXPECT_DOUBLE_EQ(mtwMsg->getTemperature(), 0.0);
    EXPECT_EQ(mtwMsg->getUnit(), 'C');
}

TEST(MTW, ClonePreservesMtwContent)
{
    MTW source("II", 23.5, 'C');

    std::unique_ptr<nmealib::Message> clonedBase = source.clone();
    ASSERT_NE(clonedBase, nullptr);
    auto cloned = dynamic_cast<MTW*>(clonedBase.get());
    ASSERT_NE(cloned, nullptr);

    EXPECT_TRUE(source.hasEqualContent(*cloned));
    EXPECT_TRUE(source == *cloned);
}

TEST(MTW, FactoryDoesNotPromoteNonMtwSentence)
{
    auto base = Nmea0183Factory::create(NOT_MTW_SENTENCE);
    ASSERT_NE(base, nullptr);
    auto mtw = dynamic_cast<MTW*>(base.get());
    EXPECT_EQ(mtw, nullptr);
}

TEST(MTW, FactoryThrowsOnMalformedMtwSentence)
{
    EXPECT_THROW(Nmea0183Factory::create(MALFORMED_MTW_SENTENCE), NotMTWException);
}

// Additional tests for full coverage
TEST(MTW, GetStringContent)
{
    MTW mtw("II", 23.5, 'C');
    std::string expectedVerbose = "Protocol: NMEA0183\nTalker: II\nSentence Type: MTW\nChecksum: None\nFields:\n\tTemperature: 23.50 C\n";
    std::string expectedOneLiner = "[OK] NMEA0183 II MTW: Temperature=23.50C";
    std::string actualVerbose = mtw.getStringContent(true);
    std::string actualOneLiner = mtw.getStringContent(false);
    EXPECT_EQ(actualVerbose, expectedVerbose);
    EXPECT_EQ(actualOneLiner, expectedOneLiner);
}