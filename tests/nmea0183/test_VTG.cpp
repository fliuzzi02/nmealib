#include <gtest/gtest.h>

#include <memory>
#include <string>

#include <nmealib.h>


using namespace nmealib::nmea0183;

static const std::string VTG_SENTENCE = "$GPVTG,220.86,T,218.90,M,2.550,N,4.724,K*40\r\n";
static const std::string VTG_SENTENCE_WITH_MODE = "$GPVTG,220.86,T,,M,2.550,N,4.724,K,A*34\r\n";
static const std::string VTG_SENTENCE_NO_CHECKSUM = "$GPVTG,220.86,T,,M,2.550,N,4.724,K\r\n";
static const std::string VTG_LEGACY_SENTENCE = "$GPVTG,220.86,218.90,2.550,4.724\r\n";
static const std::string NOT_VTG_SENTENCE = "$GNGLL,3150.788156,N,11711.922383,E,062735.00,A,A*76\r\n";
static const std::string MALFORMED_VTG_SENTENCE = "$GPVTG,220.86,T,218.90\r\n";

TEST(VTG, CreateFromMessage0183Factory)
{
    auto msg = Nmea0183Factory::create(VTG_SENTENCE);
    ASSERT_NE(msg, nullptr);

    auto vtgMsg = dynamic_cast<VTG*>(msg.get());
    ASSERT_NE(vtgMsg, nullptr);

    EXPECT_EQ(vtgMsg->getTalker(), "GP");
    EXPECT_EQ(vtgMsg->getSentenceType(), "VTG");
    EXPECT_DOUBLE_EQ(vtgMsg->getCourseOverGroundTrue(), 220.86);
    EXPECT_EQ(vtgMsg->getCourseOverGroundTrueType(), 'T');
    EXPECT_DOUBLE_EQ(vtgMsg->getCourseOverGroundMagnetic(), 218.90);
    EXPECT_EQ(vtgMsg->getCourseOverGroundMagneticType(), 'M');
    EXPECT_DOUBLE_EQ(vtgMsg->getSpeedOverGroundKnots(), 2.55);
    EXPECT_EQ(vtgMsg->getSpeedOverGroundKnotsType(), 'N');
    EXPECT_DOUBLE_EQ(vtgMsg->getSpeedOverGroundKph(), 4.724);
    EXPECT_EQ(vtgMsg->getSpeedOverGroundKphType(), 'K');
    EXPECT_FALSE(vtgMsg->hasFaaModeIndicator());
    EXPECT_FALSE(vtgMsg->isLegacyFormat());

    msg = Nmea0183Factory::create(VTG_SENTENCE_NO_CHECKSUM);
    ASSERT_NE(msg, nullptr);
    vtgMsg = dynamic_cast<VTG*>(msg.get());
    ASSERT_NE(vtgMsg, nullptr);

    EXPECT_DOUBLE_EQ(vtgMsg->getCourseOverGroundTrue(), 220.86);
    EXPECT_EQ(vtgMsg->getCourseOverGroundTrueType(), 'T');
    EXPECT_DOUBLE_EQ(vtgMsg->getCourseOverGroundMagnetic(), 0.0);
    EXPECT_EQ(vtgMsg->getCourseOverGroundMagneticType(), 'M');
    EXPECT_DOUBLE_EQ(vtgMsg->getSpeedOverGroundKnots(), 2.55);
    EXPECT_DOUBLE_EQ(vtgMsg->getSpeedOverGroundKph(), 4.724);
    EXPECT_FALSE(vtgMsg->hasFaaModeIndicator());
    EXPECT_FALSE(vtgMsg->isLegacyFormat());
}

TEST(VTG, CreateFromMessage0183FactoryWithMode)
{
    auto msg = Nmea0183Factory::create(VTG_SENTENCE_WITH_MODE);
    ASSERT_NE(msg, nullptr);

    auto vtgMsg = dynamic_cast<VTG*>(msg.get());
    ASSERT_NE(vtgMsg, nullptr);

    EXPECT_EQ(vtgMsg->getTalker(), "GP");
    EXPECT_EQ(vtgMsg->getSentenceType(), "VTG");
    EXPECT_DOUBLE_EQ(vtgMsg->getCourseOverGroundTrue(), 220.86);
    EXPECT_EQ(vtgMsg->getCourseOverGroundTrueType(), 'T');
    EXPECT_DOUBLE_EQ(vtgMsg->getCourseOverGroundMagnetic(), 0.0);
    EXPECT_EQ(vtgMsg->getCourseOverGroundMagneticType(), 'M');
    EXPECT_DOUBLE_EQ(vtgMsg->getSpeedOverGroundKnots(), 2.55);
    EXPECT_EQ(vtgMsg->getSpeedOverGroundKnotsType(), 'N');
    EXPECT_DOUBLE_EQ(vtgMsg->getSpeedOverGroundKph(), 4.724);
    EXPECT_EQ(vtgMsg->getSpeedOverGroundKphType(), 'K');
    ASSERT_TRUE(vtgMsg->hasFaaModeIndicator());
    ASSERT_TRUE(vtgMsg->getFaaModeIndicator().has_value());
    EXPECT_EQ(vtgMsg->getFaaModeIndicator().value(), 'A');
    EXPECT_FALSE(vtgMsg->isLegacyFormat());
}

TEST(VTG, CreateFromMessage0183FactoryLegacy)
{
    auto msg = Nmea0183Factory::create(VTG_LEGACY_SENTENCE);
    ASSERT_NE(msg, nullptr);

    auto vtgMsg = dynamic_cast<VTG*>(msg.get());
    ASSERT_NE(vtgMsg, nullptr);

    EXPECT_DOUBLE_EQ(vtgMsg->getCourseOverGroundTrue(), 220.86);
    EXPECT_EQ(vtgMsg->getCourseOverGroundTrueType(), '\0');
    EXPECT_DOUBLE_EQ(vtgMsg->getCourseOverGroundMagnetic(), 218.90);
    EXPECT_EQ(vtgMsg->getCourseOverGroundMagneticType(), '\0');
    EXPECT_DOUBLE_EQ(vtgMsg->getSpeedOverGroundKnots(), 2.55);
    EXPECT_EQ(vtgMsg->getSpeedOverGroundKnotsType(), '\0');
    EXPECT_DOUBLE_EQ(vtgMsg->getSpeedOverGroundKph(), 4.724);
    EXPECT_EQ(vtgMsg->getSpeedOverGroundKphType(), '\0');
    EXPECT_FALSE(vtgMsg->hasFaaModeIndicator());
    EXPECT_TRUE(vtgMsg->isLegacyFormat());
}

TEST(VTG, CreateFromFields)
{
    VTG vtg("GP", 220.86, 218.90, 2.55, 4.724, 'A', false);

    EXPECT_EQ(vtg.getTalker(), "GP");
    EXPECT_EQ(vtg.getSentenceType(), "VTG");
    EXPECT_DOUBLE_EQ(vtg.getCourseOverGroundTrue(), 220.86);
    EXPECT_EQ(vtg.getCourseOverGroundTrueType(), 'T');
    EXPECT_DOUBLE_EQ(vtg.getCourseOverGroundMagnetic(), 218.90);
    EXPECT_EQ(vtg.getCourseOverGroundMagneticType(), 'M');
    EXPECT_DOUBLE_EQ(vtg.getSpeedOverGroundKnots(), 2.55);
    EXPECT_EQ(vtg.getSpeedOverGroundKnotsType(), 'N');
    EXPECT_DOUBLE_EQ(vtg.getSpeedOverGroundKph(), 4.724);
    EXPECT_EQ(vtg.getSpeedOverGroundKphType(), 'K');
    ASSERT_TRUE(vtg.hasFaaModeIndicator());
    EXPECT_EQ(vtg.getFaaModeIndicator().value(), 'A');
    EXPECT_FALSE(vtg.isLegacyFormat());
}

TEST(VTG, SerializeRoundTripFromFields)
{
    VTG source("GP", 220.86, 0.0, 2.55, 4.724, 'A', false);

    auto parsedBase = Nmea0183Factory::create(source.serialize());
    ASSERT_NE(parsedBase, nullptr);
    auto parsed = dynamic_cast<VTG*>(parsedBase.get());
    ASSERT_NE(parsed, nullptr);

    EXPECT_DOUBLE_EQ(parsed->getCourseOverGroundTrue(), 220.86);
    EXPECT_EQ(parsed->getCourseOverGroundTrueType(), 'T');
    EXPECT_DOUBLE_EQ(parsed->getCourseOverGroundMagnetic(), 0.0);
    EXPECT_EQ(parsed->getCourseOverGroundMagneticType(), 'M');
    EXPECT_DOUBLE_EQ(parsed->getSpeedOverGroundKnots(), 2.55);
    EXPECT_EQ(parsed->getSpeedOverGroundKnotsType(), 'N');
    EXPECT_DOUBLE_EQ(parsed->getSpeedOverGroundKph(), 4.724);
    EXPECT_EQ(parsed->getSpeedOverGroundKphType(), 'K');
    ASSERT_TRUE(parsed->hasFaaModeIndicator());
    EXPECT_EQ(parsed->getFaaModeIndicator().value(), 'A');
    EXPECT_FALSE(parsed->isLegacyFormat());

    const std::string wire = source.serialize();
    EXPECT_NE(wire.find("$GPVTG,220.86,T,0.00,M,2.550,N,4.724,K,A"), std::string::npos);
}

TEST(VTG, SerializeRoundTripLegacyFromFields)
{
    VTG source("GP", 220.86, 218.90, 2.55, 4.724, std::nullopt, true);

    auto parsedBase = Nmea0183Factory::create(source.serialize());
    ASSERT_NE(parsedBase, nullptr);
    auto parsed = dynamic_cast<VTG*>(parsedBase.get());
    ASSERT_NE(parsed, nullptr);

    EXPECT_TRUE(parsed->isLegacyFormat());
    EXPECT_DOUBLE_EQ(parsed->getCourseOverGroundTrue(), 220.86);
    EXPECT_DOUBLE_EQ(parsed->getCourseOverGroundMagnetic(), 218.9);
    EXPECT_DOUBLE_EQ(parsed->getSpeedOverGroundKnots(), 2.55);
    EXPECT_DOUBLE_EQ(parsed->getSpeedOverGroundKph(), 4.724);
    EXPECT_FALSE(parsed->hasFaaModeIndicator());

    const std::string wire = source.serialize();
    EXPECT_NE(wire.find("$GPVTG,220.86,218.90,2.550,4.724"), std::string::npos);
}

TEST(VTG, ClonePreservesVtgContent)
{
    VTG source("GP", 220.86, 218.90, 2.55, 4.724, 'A', false);

    std::unique_ptr<nmealib::Message> clonedBase = source.clone();
    ASSERT_NE(clonedBase, nullptr);
    auto cloned = dynamic_cast<VTG*>(clonedBase.get());
    ASSERT_NE(cloned, nullptr);

    EXPECT_TRUE(source.hasEqualContent(*cloned));
    EXPECT_TRUE(source == *cloned);
}

TEST(VTG, FactoryDoesNotPromoteNonVtgSentence)
{
    auto base = Nmea0183Factory::create(NOT_VTG_SENTENCE);
    ASSERT_NE(base, nullptr);
    auto vtg = dynamic_cast<VTG*>(base.get());
    EXPECT_EQ(vtg, nullptr);
}

TEST(VTG, FactoryThrowsOnMalformedVtgSentence)
{
    EXPECT_THROW(Nmea0183Factory::create(MALFORMED_VTG_SENTENCE), NotVTGException);
}

// Additional tests for full coverage
TEST(VTG, GetStringContent)
{
    VTG vtg("GP", 220.86, 218.90, 2.55, 4.724, 'A', false);
    std::string expectedVerbose = "--------------------------------\nProtocol: NMEA0183\nTalker: GP\nSentence Type: VTG\nChecksum: None\nFields:\n\tLegacy Format: No\n\tCourse Over Ground: 220.86T\n\tCourse Over Ground: 218.90M\n\tSpeed Over Ground: 2.55kts\n\tSpeed Over Ground: 4.72kph\n\tFAA Mode Indicator: A\n";
    std::string expectedOneLiner = "[OK] NMEA0183 GP VTG: True=220.86, Magnetic=218.90, Knots=2.55, KPH=4.72, FAA=A, Legacy=N";
    std::string actualVerbose = vtg.getStringContent(true);
    std::string actualOneLiner = vtg.getStringContent(false);
    EXPECT_EQ(actualVerbose, expectedVerbose);
    EXPECT_EQ(actualOneLiner, expectedOneLiner);
}