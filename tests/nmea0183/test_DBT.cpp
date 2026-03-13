#include <gtest/gtest.h>

#include <memory>
#include <string>

#include <nmealib.h>

using namespace nmealib::nmea0183;

static const std::string DBT_SENTENCE = "$SDDBT,15.5,f,4.7,M,2.6,F*2C\r\n";
static const std::string DBT_SENTENCE_NO_CHECKSUM = "$SDDBT,15.5,f,4.7,M,2.6,F\r\n";
static const std::string INCOMPLETE_DBT_SENTENCE = "$SDDBT,,f,,M,,F\r\n";
static const std::string NOT_DBT_SENTENCE = "$GPZDA,160012.71,11,03,2004,-1,00*7D\r\n";
static const std::string MALFORMED_DBT_SENTENCE = "$SDDBT,15.5,f,4.7\r\n";

TEST(DBT, CreateFromMessage0183Factory)
{
    auto msg = Nmea0183Factory::create(DBT_SENTENCE);
    ASSERT_NE(msg, nullptr);

    auto dbtMsg = dynamic_cast<DBT*>(msg.get());
    ASSERT_NE(dbtMsg, nullptr);

    EXPECT_EQ(dbtMsg->getTalker(), "SD");
    EXPECT_EQ(dbtMsg->getSentenceType(), "DBT");
    EXPECT_DOUBLE_EQ(dbtMsg->getDepthFeet(), 15.5);
    EXPECT_EQ(dbtMsg->getFeetUnit(), 'f');
    EXPECT_DOUBLE_EQ(dbtMsg->getDepthMeters(), 4.7);
    EXPECT_EQ(dbtMsg->getMetersUnit(), 'M');
    EXPECT_DOUBLE_EQ(dbtMsg->getDepthFathoms(), 2.6);
    EXPECT_EQ(dbtMsg->getFathomsUnit(), 'F');

    msg = Nmea0183Factory::create(DBT_SENTENCE_NO_CHECKSUM);
    ASSERT_NE(msg, nullptr);
    dbtMsg = dynamic_cast<DBT*>(msg.get());
    ASSERT_NE(dbtMsg, nullptr);

    EXPECT_DOUBLE_EQ(dbtMsg->getDepthFeet(), 15.5);
    EXPECT_EQ(dbtMsg->getFeetUnit(), 'f');
    EXPECT_DOUBLE_EQ(dbtMsg->getDepthMeters(), 4.7);
    EXPECT_EQ(dbtMsg->getMetersUnit(), 'M');
    EXPECT_DOUBLE_EQ(dbtMsg->getDepthFathoms(), 2.6);
    EXPECT_EQ(dbtMsg->getFathomsUnit(), 'F');
}

TEST(DBT, CreateFromFields)
{
    DBT dbt("SD", 15.5, 'f', 4.7, 'M', 2.6, 'F');

    EXPECT_EQ(dbt.getTalker(), "SD");
    EXPECT_EQ(dbt.getSentenceType(), "DBT");
    EXPECT_DOUBLE_EQ(dbt.getDepthFeet(), 15.5);
    EXPECT_EQ(dbt.getFeetUnit(), 'f');
    EXPECT_DOUBLE_EQ(dbt.getDepthMeters(), 4.7);
    EXPECT_EQ(dbt.getMetersUnit(), 'M');
    EXPECT_DOUBLE_EQ(dbt.getDepthFathoms(), 2.6);
    EXPECT_EQ(dbt.getFathomsUnit(), 'F');
}

TEST(DBT, SerializeRoundTripFromFields)
{
    DBT source("SD", 15.5, 'f', 4.7, 'M', 2.6, 'F');

    auto parsedBase = Nmea0183Factory::create(source.serialize());
    ASSERT_NE(parsedBase, nullptr);
    auto parsed = dynamic_cast<DBT*>(parsedBase.get());
    ASSERT_NE(parsed, nullptr);

    EXPECT_DOUBLE_EQ(parsed->getDepthFeet(), 15.5);
    EXPECT_EQ(parsed->getFeetUnit(), 'f');
    EXPECT_DOUBLE_EQ(parsed->getDepthMeters(), 4.7);
    EXPECT_EQ(parsed->getMetersUnit(), 'M');
    EXPECT_DOUBLE_EQ(parsed->getDepthFathoms(), 2.6);
    EXPECT_EQ(parsed->getFathomsUnit(), 'F');

    const std::string wire = source.serialize();
    EXPECT_NE(wire.find("$SDDBT,15.5,f,4.7,M,2.6,F"), std::string::npos);
}

TEST(DBT, CreateFromMessageFactoryWithIncompleteFieldsDefaults)
{
    auto msg = Nmea0183Factory::create(INCOMPLETE_DBT_SENTENCE);
    ASSERT_NE(msg, nullptr);
    auto dbtMsg = dynamic_cast<DBT*>(msg.get());
    ASSERT_NE(dbtMsg, nullptr);

    EXPECT_DOUBLE_EQ(dbtMsg->getDepthFeet(), 0.0);
    EXPECT_EQ(dbtMsg->getFeetUnit(), 'f');
    EXPECT_DOUBLE_EQ(dbtMsg->getDepthMeters(), 0.0);
    EXPECT_EQ(dbtMsg->getMetersUnit(), 'M');
    EXPECT_DOUBLE_EQ(dbtMsg->getDepthFathoms(), 0.0);
    EXPECT_EQ(dbtMsg->getFathomsUnit(), 'F');
}

TEST(DBT, ClonePreservesDbtContent)
{
    DBT source("SD", 15.5, 'f', 4.7, 'M', 2.6, 'F');

    std::unique_ptr<nmealib::Message> clonedBase = source.clone();
    ASSERT_NE(clonedBase, nullptr);
    auto cloned = dynamic_cast<DBT*>(clonedBase.get());
    ASSERT_NE(cloned, nullptr);

    EXPECT_TRUE(source.hasEqualContent(*cloned));
    EXPECT_TRUE(source == *cloned);
}

TEST(DBT, FactoryDoesNotPromoteNonDbtSentence)
{
    auto base = Nmea0183Factory::create(NOT_DBT_SENTENCE);
    ASSERT_NE(base, nullptr);
    auto dbt = dynamic_cast<DBT*>(base.get());
    EXPECT_EQ(dbt, nullptr);
}

TEST(DBT, FactoryThrowsOnMalformedDbtSentence)
{
    EXPECT_THROW(Nmea0183Factory::create(MALFORMED_DBT_SENTENCE), NotDBTException);
}

// Additional tests for full coverage
TEST(DBT, GetStringContent)
{
    DBT dbt("SD", 15.5, 'f', 4.7, 'M', 2.6, 'F');
    std::string expectedVerbose = "--------------------------------\nProtocol: NMEA0183\nTalker: SD\nSentence Type: DBT\nChecksum: None\nFields:\n\tDepth: 15.50 f\n\tDepth (meters): 4.70 M\n\tDepth (fathoms): 2.60 F\n";
    std::string expectedOneLiner = "[OK] NMEA0183 SD DBT: Depth=15.50f, 4.70M, 2.60F";
    std::string actualVerbose = dbt.getStringContent(true);
    std::string actualOneLiner = dbt.getStringContent(false);
    EXPECT_EQ(actualVerbose, expectedVerbose);
    EXPECT_EQ(actualOneLiner, expectedOneLiner);
}
