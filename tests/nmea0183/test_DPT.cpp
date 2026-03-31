#include <gtest/gtest.h>

#include <memory>
#include <string>

#include <nmealib.h>

using namespace nmealib::nmea0183;

static const std::string DPT_SENTENCE = "$INDPT,2.3,0.0,100.0*73\r\n";
static const std::string DPT_SENTENCE_NO_CHECKSUM = "$INDPT,2.3,0.0,100.0\r\n";
static const std::string INCOMPLETE_DPT_SENTENCE = "$INDPT,,,\r\n";
static const std::string NOT_DPT_SENTENCE = "$GPZDA,160012.71,11,03,2004,-1,00*7D\r\n";
static const std::string MALFORMED_DPT_SENTENCE = "$INDPT,2.3,0.0\r\n";

TEST(DPT, CreateFromMessage0183Factory)
{
    auto msg = Nmea0183Factory::create(DPT_SENTENCE);
    ASSERT_NE(msg, nullptr);

    auto dptMsg = dynamic_cast<DPT*>(msg.get());
    ASSERT_NE(dptMsg, nullptr);

    EXPECT_EQ(dptMsg->getTalker(), "IN");
    EXPECT_EQ(dptMsg->getSentenceType(), "DPT");
    EXPECT_DOUBLE_EQ(dptMsg->getWaterDepthMeters(), 2.3);
    EXPECT_DOUBLE_EQ(dptMsg->getTransducerOffset(), 0.0);
    EXPECT_DOUBLE_EQ(dptMsg->getMaxRangeScale(), 100.0);

    msg = Nmea0183Factory::create(DPT_SENTENCE_NO_CHECKSUM);
    ASSERT_NE(msg, nullptr);
    dptMsg = dynamic_cast<DPT*>(msg.get());
    ASSERT_NE(dptMsg, nullptr);

    EXPECT_DOUBLE_EQ(dptMsg->getWaterDepthMeters(), 2.3);
    EXPECT_DOUBLE_EQ(dptMsg->getTransducerOffset(), 0.0);
    EXPECT_DOUBLE_EQ(dptMsg->getMaxRangeScale(), 100.0);
}

TEST(DPT, CreateFromFields)
{
    DPT dpt("IN", 2.3, 0.0, 100.0);

    EXPECT_EQ(dpt.getTalker(), "IN");
    EXPECT_EQ(dpt.getSentenceType(), "DPT");
    EXPECT_DOUBLE_EQ(dpt.getWaterDepthMeters(), 2.3);
    EXPECT_DOUBLE_EQ(dpt.getTransducerOffset(), 0.0);
    EXPECT_DOUBLE_EQ(dpt.getMaxRangeScale(), 100.0);
}

TEST(DPT, SerializeRoundTripFromFields)
{
    DPT source("IN", 2.3, 0.0, 100.0);

    auto parsedBase = Nmea0183Factory::create(source.serialize());
    ASSERT_NE(parsedBase, nullptr);
    auto parsed = dynamic_cast<DPT*>(parsedBase.get());
    ASSERT_NE(parsed, nullptr);

    EXPECT_DOUBLE_EQ(parsed->getWaterDepthMeters(), 2.3);
    EXPECT_DOUBLE_EQ(parsed->getTransducerOffset(), 0.0);
    EXPECT_DOUBLE_EQ(parsed->getMaxRangeScale(), 100.0);

    const std::string wire = source.serialize();
    EXPECT_NE(wire.find("$INDPT,2.3,0.0,100.0"), std::string::npos);
}

TEST(DPT, CreateFromMessageFactoryWithIncompleteFieldsDefaults)
{
    auto msg = Nmea0183Factory::create(INCOMPLETE_DPT_SENTENCE);
    ASSERT_NE(msg, nullptr);
    auto dptMsg = dynamic_cast<DPT*>(msg.get());
    ASSERT_NE(dptMsg, nullptr);

    EXPECT_DOUBLE_EQ(dptMsg->getWaterDepthMeters(), 0.0);
    EXPECT_DOUBLE_EQ(dptMsg->getTransducerOffset(), 0.0);
    EXPECT_DOUBLE_EQ(dptMsg->getMaxRangeScale(), 0.0);
}

TEST(DPT, ClonePreservesDptContent)
{
    DPT source("IN", 2.3, 0.0, 100.0);

    std::unique_ptr<nmealib::Message> clonedBase = source.clone();
    ASSERT_NE(clonedBase, nullptr);
    auto cloned = dynamic_cast<DPT*>(clonedBase.get());
    ASSERT_NE(cloned, nullptr);

    EXPECT_TRUE(source == *cloned);
}

TEST(DPT, FactoryDoesNotPromoteNonDptSentence)
{
    auto base = Nmea0183Factory::create(NOT_DPT_SENTENCE);
    ASSERT_NE(base, nullptr);
    auto dpt = dynamic_cast<DPT*>(base.get());
    EXPECT_EQ(dpt, nullptr);
}

TEST(DPT, FactoryThrowsOnMalformedDptSentence)
{
    EXPECT_THROW(Nmea0183Factory::create(MALFORMED_DPT_SENTENCE), NotDPTException);
}

TEST(DPT, GetStringContent)
{
    DPT dpt("IN", 2.3, 0.0, 100.0);
    std::string expectedVerbose = "--------------------------------\nProtocol: NMEA0183\nTalker: IN\nSentence Type: DPT\nChecksum: None\nFields:\n\tWater depth: 2.30 m\n\tTransducer offset: 0.00 m\n\tMax range scale: 100.00 m\n";
    std::string expectedOneLiner = "[OK] NMEA0183 IN DPT: Depth=2.30m, Offset=0.00m, MaxRange=100.00m";
    std::string actualVerbose = dpt.getStringContent(true);
    std::string actualOneLiner = dpt.getStringContent(false);
    EXPECT_EQ(actualVerbose, expectedVerbose);
    EXPECT_EQ(actualOneLiner, expectedOneLiner);
}
