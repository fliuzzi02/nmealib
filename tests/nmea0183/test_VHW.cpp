#include <gtest/gtest.h>

#include <memory>
#include <string>

#include <nmealib/nmealib.hpp>

using namespace nmealib::nmea0183;

static const std::string VHW_SENTENCE = "$IIVHW,220.86,T,218.90,M,2.550,N,4.724,K*5A\r\n";
static const std::string VHW_SENTENCE_NO_CHECKSUM = "$IIVHW,220.86,T,,M,2.550,N,4.724,K\r\n";
static const std::string INCOMPLETE_VHW_SENTENCE = "$IIVHW,,T,,M,,N,,K\r\n";
static const std::string NOT_VHW_SENTENCE = "$GPZDA,160012.71,11,03,2004,-1,00*7D\r\n";
static const std::string MALFORMED_VHW_SENTENCE = "$IIVHW,220.86,T,218.90\r\n";

TEST(VHW, CreateFromMessage0183Factory)
{
    auto msg = Nmea0183Factory::create(VHW_SENTENCE);
    ASSERT_NE(msg, nullptr);

    auto vhwMsg = dynamic_cast<VHW*>(msg.get());
    ASSERT_NE(vhwMsg, nullptr);

    EXPECT_EQ(vhwMsg->getTalker(), "II");
    EXPECT_EQ(vhwMsg->getSentenceType(), "VHW");
    EXPECT_DOUBLE_EQ(vhwMsg->getHeadingTrue(), 220.86);
    EXPECT_EQ(vhwMsg->getHeadingTrueType(), 'T');
    EXPECT_DOUBLE_EQ(vhwMsg->getHeadingMagnetic(), 218.9);
    EXPECT_EQ(vhwMsg->getHeadingMagneticType(), 'M');
    EXPECT_DOUBLE_EQ(vhwMsg->getSpeedKnots(), 2.55);
    EXPECT_EQ(vhwMsg->getSpeedKnotsType(), 'N');
    EXPECT_DOUBLE_EQ(vhwMsg->getSpeedKph(), 4.724);
    EXPECT_EQ(vhwMsg->getSpeedKphType(), 'K');

    msg = Nmea0183Factory::create(VHW_SENTENCE_NO_CHECKSUM);
    ASSERT_NE(msg, nullptr);
    vhwMsg = dynamic_cast<VHW*>(msg.get());
    ASSERT_NE(vhwMsg, nullptr);

    EXPECT_DOUBLE_EQ(vhwMsg->getHeadingTrue(), 220.86);
    EXPECT_EQ(vhwMsg->getHeadingTrueType(), 'T');
    EXPECT_DOUBLE_EQ(vhwMsg->getHeadingMagnetic(), 0.0);
    EXPECT_EQ(vhwMsg->getHeadingMagneticType(), 'M');
    EXPECT_DOUBLE_EQ(vhwMsg->getSpeedKnots(), 2.55);
    EXPECT_EQ(vhwMsg->getSpeedKnotsType(), 'N');
    EXPECT_DOUBLE_EQ(vhwMsg->getSpeedKph(), 4.724);
    EXPECT_EQ(vhwMsg->getSpeedKphType(), 'K');
}

TEST(VHW, CreateFromFields)
{
    VHW vhw("II", 220.86, 218.90, 2.55, 4.724);

    EXPECT_EQ(vhw.getTalker(), "II");
    EXPECT_EQ(vhw.getSentenceType(), "VHW");
    EXPECT_DOUBLE_EQ(vhw.getHeadingTrue(), 220.86);
    EXPECT_EQ(vhw.getHeadingTrueType(), 'T');
    EXPECT_DOUBLE_EQ(vhw.getHeadingMagnetic(), 218.9);
    EXPECT_EQ(vhw.getHeadingMagneticType(), 'M');
    EXPECT_DOUBLE_EQ(vhw.getSpeedKnots(), 2.55);
    EXPECT_EQ(vhw.getSpeedKnotsType(), 'N');
    EXPECT_DOUBLE_EQ(vhw.getSpeedKph(), 4.724);
    EXPECT_EQ(vhw.getSpeedKphType(), 'K');
}

TEST(VHW, SerializeRoundTripFromFields)
{
    VHW source("II", 220.86, 218.90, 2.55, 4.724);

    auto parsedBase = Nmea0183Factory::create(source.serialize());
    ASSERT_NE(parsedBase, nullptr);
    auto parsed = dynamic_cast<VHW*>(parsedBase.get());
    ASSERT_NE(parsed, nullptr);

    EXPECT_DOUBLE_EQ(parsed->getHeadingTrue(), 220.86);
    EXPECT_EQ(parsed->getHeadingTrueType(), 'T');
    EXPECT_DOUBLE_EQ(parsed->getHeadingMagnetic(), 218.9);
    EXPECT_EQ(parsed->getHeadingMagneticType(), 'M');
    EXPECT_DOUBLE_EQ(parsed->getSpeedKnots(), 2.55);
    EXPECT_EQ(parsed->getSpeedKnotsType(), 'N');
    EXPECT_DOUBLE_EQ(parsed->getSpeedKph(), 4.724);
    EXPECT_EQ(parsed->getSpeedKphType(), 'K');

    const std::string wire = source.serialize();
    EXPECT_NE(wire.find("$IIVHW,220.86,T,218.90,M,2.550,N,4.724,K"), std::string::npos);
}

TEST(VHW, CreateFromMessageFactoryWithIncompleteFieldsDefaults)
{
    auto msg = Nmea0183Factory::create(INCOMPLETE_VHW_SENTENCE);
    ASSERT_NE(msg, nullptr);
    auto vhwMsg = dynamic_cast<VHW*>(msg.get());
    ASSERT_NE(vhwMsg, nullptr);

    EXPECT_DOUBLE_EQ(vhwMsg->getHeadingTrue(), 0.0);
    EXPECT_EQ(vhwMsg->getHeadingTrueType(), 'T');
    EXPECT_DOUBLE_EQ(vhwMsg->getHeadingMagnetic(), 0.0);
    EXPECT_EQ(vhwMsg->getHeadingMagneticType(), 'M');
    EXPECT_DOUBLE_EQ(vhwMsg->getSpeedKnots(), 0.0);
    EXPECT_EQ(vhwMsg->getSpeedKnotsType(), 'N');
    EXPECT_DOUBLE_EQ(vhwMsg->getSpeedKph(), 0.0);
    EXPECT_EQ(vhwMsg->getSpeedKphType(), 'K');
}

TEST(VHW, ClonePreservesVhwContent)
{
    VHW source("II", 220.86, 218.90, 2.55, 4.724);

    std::unique_ptr<nmealib::Message> clonedBase = source.clone();
    ASSERT_NE(clonedBase, nullptr);
    auto cloned = dynamic_cast<VHW*>(clonedBase.get());
    ASSERT_NE(cloned, nullptr);

    EXPECT_TRUE(source.hasEqualContent(*cloned));
    EXPECT_TRUE(source == *cloned);
}

TEST(VHW, FactoryDoesNotPromoteNonVhwSentence)
{
    auto base = Nmea0183Factory::create(NOT_VHW_SENTENCE);
    ASSERT_NE(base, nullptr);
    auto vhw = dynamic_cast<VHW*>(base.get());
    EXPECT_EQ(vhw, nullptr);
}

TEST(VHW, FactoryThrowsOnMalformedVhwSentence)
{
    EXPECT_THROW(Nmea0183Factory::create(MALFORMED_VHW_SENTENCE), NotVHWException);
}
