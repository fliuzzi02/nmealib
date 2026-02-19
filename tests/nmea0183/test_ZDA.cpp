#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "nmea0183Factory.hpp"
#include "zda.hpp"

using namespace nmealib::nmea0183;

static const std::string ZDA_SENTENCE = "$GPZDA,160012.71,11,03,2004,-1,00*7D\r\n";
static const std::string ZDA_SENTENCE_NO_CHECKSUM = "$GPZDA,160012.71,11,03,2004,-1,00\r\n";
static const std::string INCOMPLETE_ZDA_SENTENCE = "$GPZDA,,11,03,2004,,\r\n";
static const std::string NOT_ZDA_SENTENCE = "$GPVTG,220.86,T,,M,2.550,N,4.724,K,A*34\r\n";
static const std::string MALFORMED_ZDA_SENTENCE = "$GPZDA,160012.71,11,03\r\n";

TEST(ZDA, CreateFromMessage0183Factory)
{
    auto msg = Nmea0183Factory::create(ZDA_SENTENCE);
    ASSERT_NE(msg, nullptr);

    auto zdaMsg = dynamic_cast<ZDA*>(msg.get());
    ASSERT_NE(zdaMsg, nullptr);

    EXPECT_EQ(zdaMsg->getTalker(), "GP");
    EXPECT_EQ(zdaMsg->getSentenceType(), "ZDA");
    EXPECT_DOUBLE_EQ(zdaMsg->getUtcTime(), 160012.71);
    EXPECT_EQ(zdaMsg->getDay(), 11u);
    EXPECT_EQ(zdaMsg->getMonth(), 3u);
    EXPECT_EQ(zdaMsg->getYear(), 2004u);
    EXPECT_EQ(zdaMsg->getLocalZoneHours(), -1);
    EXPECT_EQ(zdaMsg->getLocalZoneMinutes(), 0);

    msg = Nmea0183Factory::create(ZDA_SENTENCE_NO_CHECKSUM);
    ASSERT_NE(msg, nullptr);
    zdaMsg = dynamic_cast<ZDA*>(msg.get());
    ASSERT_NE(zdaMsg, nullptr);

    EXPECT_DOUBLE_EQ(zdaMsg->getUtcTime(), 160012.71);
    EXPECT_EQ(zdaMsg->getDay(), 11u);
    EXPECT_EQ(zdaMsg->getMonth(), 3u);
    EXPECT_EQ(zdaMsg->getYear(), 2004u);
    EXPECT_EQ(zdaMsg->getLocalZoneHours(), -1);
    EXPECT_EQ(zdaMsg->getLocalZoneMinutes(), 0);
}

TEST(ZDA, CreateFromFields)
{
    ZDA zda("GP", 160012.71, 11u, 3u, 2004u, -1, 0);

    EXPECT_EQ(zda.getTalker(), "GP");
    EXPECT_EQ(zda.getSentenceType(), "ZDA");
    EXPECT_DOUBLE_EQ(zda.getUtcTime(), 160012.71);
    EXPECT_EQ(zda.getDay(), 11u);
    EXPECT_EQ(zda.getMonth(), 3u);
    EXPECT_EQ(zda.getYear(), 2004u);
    EXPECT_EQ(zda.getLocalZoneHours(), -1);
    EXPECT_EQ(zda.getLocalZoneMinutes(), 0);
}

TEST(ZDA, SerializeRoundTripFromFields)
{
    ZDA source("GP", 160012.71, 11u, 3u, 2004u, -1, 0);

    auto parsedBase = Nmea0183Factory::create(source.serialize());
    ASSERT_NE(parsedBase, nullptr);
    auto parsed = dynamic_cast<ZDA*>(parsedBase.get());
    ASSERT_NE(parsed, nullptr);

    EXPECT_DOUBLE_EQ(parsed->getUtcTime(), 160012.71);
    EXPECT_EQ(parsed->getDay(), 11u);
    EXPECT_EQ(parsed->getMonth(), 3u);
    EXPECT_EQ(parsed->getYear(), 2004u);
    EXPECT_EQ(parsed->getLocalZoneHours(), -1);
    EXPECT_EQ(parsed->getLocalZoneMinutes(), 0);

    const std::string wire = source.serialize();
    EXPECT_NE(wire.find("$GPZDA,160012.71,11,03,2004,-1,00"), std::string::npos);
}

TEST(ZDA, CreateFromMessageFactoryWithIncompleteFieldsDefaults)
{
    auto msg = Nmea0183Factory::create(INCOMPLETE_ZDA_SENTENCE);
    ASSERT_NE(msg, nullptr);
    auto zdaMsg = dynamic_cast<ZDA*>(msg.get());
    ASSERT_NE(zdaMsg, nullptr);

    EXPECT_DOUBLE_EQ(zdaMsg->getUtcTime(), 0.0);
    EXPECT_EQ(zdaMsg->getDay(), 11u);
    EXPECT_EQ(zdaMsg->getMonth(), 3u);
    EXPECT_EQ(zdaMsg->getYear(), 2004u);
    EXPECT_EQ(zdaMsg->getLocalZoneHours(), 0);
    EXPECT_EQ(zdaMsg->getLocalZoneMinutes(), 0);
}

TEST(ZDA, ClonePreservesZdaContent)
{
    ZDA source("GP", 160012.71, 11u, 3u, 2004u, -1, 0);

    std::unique_ptr<nmealib::Message> clonedBase = source.clone();
    ASSERT_NE(clonedBase, nullptr);
    auto cloned = dynamic_cast<ZDA*>(clonedBase.get());
    ASSERT_NE(cloned, nullptr);

    EXPECT_TRUE(source.hasEqualContent(*cloned));
    EXPECT_TRUE(source == *cloned);
}

TEST(ZDA, FactoryDoesNotPromoteNonZdaSentence)
{
    auto base = Nmea0183Factory::create(NOT_ZDA_SENTENCE);
    ASSERT_NE(base, nullptr);
    auto zda = dynamic_cast<ZDA*>(base.get());
    EXPECT_EQ(zda, nullptr);
}

TEST(ZDA, FactoryThrowsOnMalformedZdaSentence)
{
    EXPECT_THROW(Nmea0183Factory::create(MALFORMED_ZDA_SENTENCE), NotZDAException);
}
