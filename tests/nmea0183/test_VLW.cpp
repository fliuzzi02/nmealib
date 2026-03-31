#include <gtest/gtest.h>

#include <memory>
#include <string>

#include <nmealib.h>

using namespace nmealib::nmea0183;

static const std::string VLW_SENTENCE_WITH_GROUND = "$IIVLW,123.4,N,23.4,N,234.5,N,34.5,N*4E\r\n";
static const std::string VLW_SENTENCE_WITHOUT_GROUND = "$IIVLW,123.4,N,23.4,N*7C\r\n";
static const std::string VLW_SENTENCE_NO_CHECKSUM = "$IIVLW,123.4,N,23.4,N,234.5,N,34.5,N\r\n";
static const std::string INCOMPLETE_VLW_SENTENCE_WITH_GROUND = "$IIVLW,,N,,N,,N,,N*4D\r\n";
static const std::string INCOMPLETE_VLW_SENTENCE_WITHOUT_GROUND = "$IIVLW,,N,,N\r\n";
static const std::string NOT_VLW_SENTENCE = "$GPZDA,160012.71,11,03,2004,-1,00*7D\r\n";
static const std::string MALFORMED_VLW_SENTENCE = "$IIVLW,123.4,N,23.4\r\n";

TEST(VLW, CreateFromMessage0183Factory)
{
    auto msg = Nmea0183Factory::create(VLW_SENTENCE_WITH_GROUND);
    ASSERT_NE(msg, nullptr);

    auto vlwMsg = dynamic_cast<VLW*>(msg.get());
    ASSERT_NE(vlwMsg, nullptr);

    EXPECT_EQ(vlwMsg->getTalker(), "II");
    EXPECT_EQ(vlwMsg->getSentenceType(), "VLW");
    EXPECT_DOUBLE_EQ(vlwMsg->getTotalWaterDistanceNm(), 123.4);
    EXPECT_EQ(vlwMsg->getTotalWaterDistanceUnit(), 'N');
    EXPECT_DOUBLE_EQ(vlwMsg->getWaterDistanceSinceResetNm(), 23.4);
    EXPECT_EQ(vlwMsg->getWaterDistanceSinceResetUnit(), 'N');
    EXPECT_TRUE(vlwMsg->hasGroundDistanceData());
    EXPECT_DOUBLE_EQ(vlwMsg->getTotalGroundDistanceNm(), 234.5);
    EXPECT_EQ(vlwMsg->getTotalGroundDistanceUnit(), 'N');
    EXPECT_DOUBLE_EQ(vlwMsg->getGroundDistanceSinceResetNm(), 34.5);
    EXPECT_EQ(vlwMsg->getGroundDistanceSinceResetUnit(), 'N');

    msg = Nmea0183Factory::create(VLW_SENTENCE_WITHOUT_GROUND);
    ASSERT_NE(msg, nullptr);
    vlwMsg = dynamic_cast<VLW*>(msg.get());
    ASSERT_NE(vlwMsg, nullptr);

    EXPECT_DOUBLE_EQ(vlwMsg->getTotalWaterDistanceNm(), 123.4);
    EXPECT_DOUBLE_EQ(vlwMsg->getWaterDistanceSinceResetNm(), 23.4);
    EXPECT_FALSE(vlwMsg->hasGroundDistanceData());
}

TEST(VLW, CreateFromFields)
{
    VLW vlw("II", 123.4, 23.4, 234.5, 34.5);

    EXPECT_EQ(vlw.getTalker(), "II");
    EXPECT_EQ(vlw.getSentenceType(), "VLW");
    EXPECT_DOUBLE_EQ(vlw.getTotalWaterDistanceNm(), 123.4);
    EXPECT_EQ(vlw.getTotalWaterDistanceUnit(), 'N');
    EXPECT_DOUBLE_EQ(vlw.getWaterDistanceSinceResetNm(), 23.4);
    EXPECT_EQ(vlw.getWaterDistanceSinceResetUnit(), 'N');
    EXPECT_TRUE(vlw.hasGroundDistanceData());
    EXPECT_DOUBLE_EQ(vlw.getTotalGroundDistanceNm(), 234.5);
    EXPECT_EQ(vlw.getTotalGroundDistanceUnit(), 'N');
    EXPECT_DOUBLE_EQ(vlw.getGroundDistanceSinceResetNm(), 34.5);
    EXPECT_EQ(vlw.getGroundDistanceSinceResetUnit(), 'N');
}

TEST(VLW, CreateFromFieldsWithoutGroundDistance)
{
    VLW vlw("II", 123.4, 23.4);

    EXPECT_EQ(vlw.getTalker(), "II");
    EXPECT_EQ(vlw.getSentenceType(), "VLW");
    EXPECT_DOUBLE_EQ(vlw.getTotalWaterDistanceNm(), 123.4);
    EXPECT_EQ(vlw.getTotalWaterDistanceUnit(), 'N');
    EXPECT_DOUBLE_EQ(vlw.getWaterDistanceSinceResetNm(), 23.4);
    EXPECT_EQ(vlw.getWaterDistanceSinceResetUnit(), 'N');
    EXPECT_FALSE(vlw.hasGroundDistanceData());
}

TEST(VLW, SerializeRoundTripFromFieldsWithGroundDistance)
{
    VLW source("II", 123.4, 23.4, 234.5, 34.5);

    auto parsedBase = Nmea0183Factory::create(source.serialize());
    ASSERT_NE(parsedBase, nullptr);
    auto parsed = dynamic_cast<VLW*>(parsedBase.get());
    ASSERT_NE(parsed, nullptr);

    EXPECT_DOUBLE_EQ(parsed->getTotalWaterDistanceNm(), 123.4);
    EXPECT_EQ(parsed->getTotalWaterDistanceUnit(), 'N');
    EXPECT_DOUBLE_EQ(parsed->getWaterDistanceSinceResetNm(), 23.4);
    EXPECT_EQ(parsed->getWaterDistanceSinceResetUnit(), 'N');
    EXPECT_TRUE(parsed->hasGroundDistanceData());
    EXPECT_DOUBLE_EQ(parsed->getTotalGroundDistanceNm(), 234.5);
    EXPECT_EQ(parsed->getTotalGroundDistanceUnit(), 'N');
    EXPECT_DOUBLE_EQ(parsed->getGroundDistanceSinceResetNm(), 34.5);
    EXPECT_EQ(parsed->getGroundDistanceSinceResetUnit(), 'N');

    const std::string wire = source.serialize();
    EXPECT_NE(wire.find("$IIVLW,123.4,N,23.4,N,234.5,N,34.5,N"), std::string::npos);
}

TEST(VLW, SerializeRoundTripFromFieldsWithoutGroundDistance)
{
    VLW source("II", 123.4, 23.4);

    auto parsedBase = Nmea0183Factory::create(source.serialize());
    ASSERT_NE(parsedBase, nullptr);
    auto parsed = dynamic_cast<VLW*>(parsedBase.get());
    ASSERT_NE(parsed, nullptr);

    EXPECT_DOUBLE_EQ(parsed->getTotalWaterDistanceNm(), 123.4);
    EXPECT_EQ(parsed->getTotalWaterDistanceUnit(), 'N');
    EXPECT_DOUBLE_EQ(parsed->getWaterDistanceSinceResetNm(), 23.4);
    EXPECT_EQ(parsed->getWaterDistanceSinceResetUnit(), 'N');
    EXPECT_FALSE(parsed->hasGroundDistanceData());

    const std::string wire = source.serialize();
    EXPECT_NE(wire.find("$IIVLW,123.4,N,23.4,N"), std::string::npos);
}

TEST(VLW, CreateFromMessageFactoryWithIncompleteFieldsDefaults)
{
    auto msg = Nmea0183Factory::create(INCOMPLETE_VLW_SENTENCE_WITH_GROUND);
    ASSERT_NE(msg, nullptr);
    auto vlwMsg = dynamic_cast<VLW*>(msg.get());
    ASSERT_NE(vlwMsg, nullptr);

    EXPECT_DOUBLE_EQ(vlwMsg->getTotalWaterDistanceNm(), 0.0);
    EXPECT_EQ(vlwMsg->getTotalWaterDistanceUnit(), 'N');
    EXPECT_DOUBLE_EQ(vlwMsg->getWaterDistanceSinceResetNm(), 0.0);
    EXPECT_EQ(vlwMsg->getWaterDistanceSinceResetUnit(), 'N');
    EXPECT_TRUE(vlwMsg->hasGroundDistanceData());
    EXPECT_DOUBLE_EQ(vlwMsg->getTotalGroundDistanceNm(), 0.0);
    EXPECT_EQ(vlwMsg->getTotalGroundDistanceUnit(), 'N');
    EXPECT_DOUBLE_EQ(vlwMsg->getGroundDistanceSinceResetNm(), 0.0);
    EXPECT_EQ(vlwMsg->getGroundDistanceSinceResetUnit(), 'N');

    msg = Nmea0183Factory::create(INCOMPLETE_VLW_SENTENCE_WITHOUT_GROUND);
    ASSERT_NE(msg, nullptr);
    vlwMsg = dynamic_cast<VLW*>(msg.get());
    ASSERT_NE(vlwMsg, nullptr);

    EXPECT_DOUBLE_EQ(vlwMsg->getTotalWaterDistanceNm(), 0.0);
    EXPECT_EQ(vlwMsg->getTotalWaterDistanceUnit(), 'N');
    EXPECT_DOUBLE_EQ(vlwMsg->getWaterDistanceSinceResetNm(), 0.0);
    EXPECT_EQ(vlwMsg->getWaterDistanceSinceResetUnit(), 'N');
    EXPECT_FALSE(vlwMsg->hasGroundDistanceData());
}

TEST(VLW, ClonePreservesVlwContent)
{
    VLW source("II", 123.4, 23.4, 234.5, 34.5);

    std::unique_ptr<nmealib::Message> clonedBase = source.clone();
    ASSERT_NE(clonedBase, nullptr);
    auto cloned = dynamic_cast<VLW*>(clonedBase.get());
    ASSERT_NE(cloned, nullptr);

    EXPECT_TRUE(source == *cloned);
}

TEST(VLW, FactoryDoesNotPromoteNonVlwSentence)
{
    auto base = Nmea0183Factory::create(NOT_VLW_SENTENCE);
    ASSERT_NE(base, nullptr);
    auto vlw = dynamic_cast<VLW*>(base.get());
    EXPECT_EQ(vlw, nullptr);
}

TEST(VLW, FactoryThrowsOnMalformedVlwSentence)
{
    EXPECT_THROW(Nmea0183Factory::create(MALFORMED_VLW_SENTENCE), NotVLWException);
}

TEST(VLW, GetStringContent)
{
    VLW vlwWithGround("II", 123.4, 23.4, 234.5, 34.5);
    std::string expectedVerboseWithGround = "--------------------------------\nProtocol: NMEA0183\nTalker: II\nSentence Type: VLW\nChecksum: None\nFields:\n\tTotal Water Distance: 123.40 N\n\tWater Distance Since Reset: 23.40 N\n\tTotal Ground Distance: 234.50 N\n\tGround Distance Since Reset: 34.50 N\n";
    std::string expectedOneLinerWithGround = "[OK] NMEA0183 II VLW: WaterTotal=123.40, WaterReset=23.40, GroundTotal=234.50, GroundReset=34.50";
    std::string actualVerboseWithGround = vlwWithGround.getStringContent(true);
    std::string actualOneLinerWithGround = vlwWithGround.getStringContent(false);
    EXPECT_EQ(actualVerboseWithGround, expectedVerboseWithGround);
    EXPECT_EQ(actualOneLinerWithGround, expectedOneLinerWithGround);

    VLW vlwWithoutGround("II", 123.4, 23.4);
    std::string expectedVerboseWithoutGround = "--------------------------------\nProtocol: NMEA0183\nTalker: II\nSentence Type: VLW\nChecksum: None\nFields:\n\tTotal Water Distance: 123.40 N\n\tWater Distance Since Reset: 23.40 N\n";
    std::string expectedOneLinerWithoutGround = "[OK] NMEA0183 II VLW: WaterTotal=123.40, WaterReset=23.40";
    std::string actualVerboseWithoutGround = vlwWithoutGround.getStringContent(true);
    std::string actualOneLinerWithoutGround = vlwWithoutGround.getStringContent(false);
    EXPECT_EQ(actualVerboseWithoutGround, expectedVerboseWithoutGround);
    EXPECT_EQ(actualOneLinerWithoutGround, expectedOneLinerWithoutGround);
}

TEST(VLW, CreateFromMessage0183FactoryWithoutChecksum)
{
    auto msg = Nmea0183Factory::create(VLW_SENTENCE_NO_CHECKSUM);
    ASSERT_NE(msg, nullptr);

    auto vlwMsg = dynamic_cast<VLW*>(msg.get());
    ASSERT_NE(vlwMsg, nullptr);
    EXPECT_TRUE(vlwMsg->hasGroundDistanceData());
    EXPECT_DOUBLE_EQ(vlwMsg->getTotalWaterDistanceNm(), 123.4);
    EXPECT_DOUBLE_EQ(vlwMsg->getWaterDistanceSinceResetNm(), 23.4);
    EXPECT_DOUBLE_EQ(vlwMsg->getTotalGroundDistanceNm(), 234.5);
    EXPECT_DOUBLE_EQ(vlwMsg->getGroundDistanceSinceResetNm(), 34.5);
}
