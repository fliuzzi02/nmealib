#include <gtest/gtest.h>

#include <memory>
#include <string>


#include <nmealib.h>

using namespace nmealib::nmea0183;

static const std::string MWV_SENTENCE = "$IIMWV,45.0,R,10.5,N,A*26\r\n";
static const std::string MWV_SENTENCE_NO_CHECKSUM = "$IIMWV,45.0,R,10.5,N,A\r\n";
static const std::string INCOMPLETE_MWV_SENTENCE = "$IIMWV,,T,,M,V\r\n";
static const std::string NOT_MWV_SENTENCE = "$GPZDA,160012.71,11,03,2004,-1,00*7D\r\n";
static const std::string MALFORMED_MWV_SENTENCE = "$IIMWV,45.0,R,10.5\r\n";

TEST(MWV, CreateFromMessage0183Factory)
{
    auto msg = Nmea0183Factory::create(MWV_SENTENCE);
    ASSERT_NE(msg, nullptr);

    auto mwvMsg = dynamic_cast<MWV*>(msg.get());
    ASSERT_NE(mwvMsg, nullptr);

    EXPECT_EQ(mwvMsg->getTalker(), "II");
    EXPECT_EQ(mwvMsg->getSentenceType(), "MWV");
    EXPECT_DOUBLE_EQ(mwvMsg->getWindAngle(), 45.0);
    EXPECT_EQ(mwvMsg->getReference(), 'R');
    EXPECT_DOUBLE_EQ(mwvMsg->getWindSpeed(), 10.5);
    EXPECT_EQ(mwvMsg->getWindSpeedUnits(), 'N');
    EXPECT_EQ(mwvMsg->getStatus(), 'A');

    msg = Nmea0183Factory::create(MWV_SENTENCE_NO_CHECKSUM);
    ASSERT_NE(msg, nullptr);
    mwvMsg = dynamic_cast<MWV*>(msg.get());
    ASSERT_NE(mwvMsg, nullptr);

    EXPECT_DOUBLE_EQ(mwvMsg->getWindAngle(), 45.0);
    EXPECT_EQ(mwvMsg->getReference(), 'R');
    EXPECT_DOUBLE_EQ(mwvMsg->getWindSpeed(), 10.5);
    EXPECT_EQ(mwvMsg->getWindSpeedUnits(), 'N');
    EXPECT_EQ(mwvMsg->getStatus(), 'A');
}

TEST(MWV, CreateFromFields)
{
    MWV mwv("II", 45.0, 'R', 10.5, 'N', 'A');

    EXPECT_EQ(mwv.getTalker(), "II");
    EXPECT_EQ(mwv.getSentenceType(), "MWV");
    EXPECT_DOUBLE_EQ(mwv.getWindAngle(), 45.0);
    EXPECT_EQ(mwv.getReference(), 'R');
    EXPECT_DOUBLE_EQ(mwv.getWindSpeed(), 10.5);
    EXPECT_EQ(mwv.getWindSpeedUnits(), 'N');
    EXPECT_EQ(mwv.getStatus(), 'A');
}

TEST(MWV, SerializeRoundTripFromFields)
{
    MWV source("II", 45.0, 'R', 10.5, 'N', 'A');

    auto parsedBase = Nmea0183Factory::create(source.serialize());
    ASSERT_NE(parsedBase, nullptr);
    auto parsed = dynamic_cast<MWV*>(parsedBase.get());
    ASSERT_NE(parsed, nullptr);

    EXPECT_DOUBLE_EQ(parsed->getWindAngle(), 45.0);
    EXPECT_EQ(parsed->getReference(), 'R');
    EXPECT_DOUBLE_EQ(parsed->getWindSpeed(), 10.5);
    EXPECT_EQ(parsed->getWindSpeedUnits(), 'N');
    EXPECT_EQ(parsed->getStatus(), 'A');

    const std::string wire = source.serialize();
    EXPECT_NE(wire.find("$IIMWV,45.0,R,10.5,N,A"), std::string::npos);
}

TEST(MWV, CreateFromMessageFactoryWithIncompleteFieldsDefaults)
{
    auto msg = Nmea0183Factory::create(INCOMPLETE_MWV_SENTENCE);
    ASSERT_NE(msg, nullptr);
    auto mwvMsg = dynamic_cast<MWV*>(msg.get());
    ASSERT_NE(mwvMsg, nullptr);

    EXPECT_DOUBLE_EQ(mwvMsg->getWindAngle(), 0.0);
    EXPECT_EQ(mwvMsg->getReference(), 'T');
    EXPECT_DOUBLE_EQ(mwvMsg->getWindSpeed(), 0.0);
    EXPECT_EQ(mwvMsg->getWindSpeedUnits(), 'M');
    EXPECT_EQ(mwvMsg->getStatus(), 'V');
}

TEST(MWV, ClonePreservesMwvContent)
{
    MWV source("II", 45.0, 'R', 10.5, 'N', 'A');

    std::unique_ptr<nmealib::Message> clonedBase = source.clone();
    ASSERT_NE(clonedBase, nullptr);
    auto cloned = dynamic_cast<MWV*>(clonedBase.get());
    ASSERT_NE(cloned, nullptr);

    EXPECT_TRUE(source.hasEqualContent(*cloned));
    EXPECT_TRUE(source == *cloned);
}

TEST(MWV, FactoryDoesNotPromoteNonMwvSentence)
{
    auto base = Nmea0183Factory::create(NOT_MWV_SENTENCE);
    ASSERT_NE(base, nullptr);
    auto mwv = dynamic_cast<MWV*>(base.get());
    EXPECT_EQ(mwv, nullptr);
}

TEST(MWV, FactoryThrowsOnMalformedMwvSentence)
{
    EXPECT_THROW(Nmea0183Factory::create(MALFORMED_MWV_SENTENCE), NotMWVException);
}

// Additional tests for full coverage
TEST(MWV, GetStringContent)
{
    MWV mwv("II", 45.0, 'R', 10.5, 'N', 'A');
    std::string expectedVerbose = "Protocol: NMEA0183\nTalker: II\nSentence Type: MWV\nChecksum: None\nFields:\n\tWind Angle: 45.00 R\n\tWind Speed: 10.50 N\n\tStatus: A";
    std::string expectedOneLiner = "[OK] NMEA0183 II MWV: Angle=45.00R, Speed=10.50N, Status=A";
    std::string actualVerbose = mwv.getStringContent(true);
    std::string actualOneLiner = mwv.getStringContent(false);
    EXPECT_EQ(actualVerbose, expectedVerbose);
    EXPECT_EQ(actualOneLiner, expectedOneLiner);
}