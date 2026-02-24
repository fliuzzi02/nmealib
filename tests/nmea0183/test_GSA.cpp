#include <gtest/gtest.h>

#include <array>
#include <memory>
#include <string>


#include <nmealib/nmealib.hpp>

using namespace nmealib::nmea0183;

static const std::string GSA_SENTENCE = "$GPGSA,A,3,04,05,09,12,24,25,29,31,32,37,43,46,1.80,1.00,1.50*10\r\n";
static const std::string GSA_SENTENCE_WITH_SYSTEM_ID = "$GNGSA,A,3,80,71,73,79,69,,,,,,,,1.83,1.09,1.47,1*37\r\n";
static const std::string GSA_SENTENCE_NO_CHECKSUM = "$GPGSA,A,3,04,05,09,12,24,25,29,31,32,37,43,46,1.80,1.00,1.50\r\n";
static const std::string INCOMPLETE_GSA_SENTENCE = "$GNGSA,M,2,,,,,,,,,,,,,2.40,1.80,1.60\r\n";
static const std::string NOT_GSA_SENTENCE = "$GNGLL,3150.788156,N,11711.922383,E,062735.00,A,A*76\r\n";
static const std::string MALFORMED_GSA_SENTENCE = "$GPGSA,A,3,04,05,09\r\n";

TEST(GSA, CreateFromMessage0183Factory)
{
    auto msg = Nmea0183Factory::create(GSA_SENTENCE);
    ASSERT_NE(msg, nullptr);

    auto gsaMsg = dynamic_cast<GSA*>(msg.get());
    ASSERT_NE(gsaMsg, nullptr);

    EXPECT_EQ(gsaMsg->getTalker(), "GP");
    EXPECT_EQ(gsaMsg->getSentenceType(), "GSA");
    EXPECT_EQ(gsaMsg->getSelectionMode(), 'A');
    EXPECT_EQ(gsaMsg->getMode(), 3u);
    EXPECT_EQ(gsaMsg->getSatelliteId(0), 4u);
    EXPECT_EQ(gsaMsg->getSatelliteId(11), 46u);
    EXPECT_DOUBLE_EQ(gsaMsg->getPdop(), 1.8);
    EXPECT_DOUBLE_EQ(gsaMsg->getHdop(), 1.0);
    EXPECT_DOUBLE_EQ(gsaMsg->getVdop(), 1.5);
    EXPECT_FALSE(gsaMsg->hasSystemId());

    msg = Nmea0183Factory::create(GSA_SENTENCE_NO_CHECKSUM);
    ASSERT_NE(msg, nullptr);
    gsaMsg = dynamic_cast<GSA*>(msg.get());
    ASSERT_NE(gsaMsg, nullptr);

    EXPECT_EQ(gsaMsg->getTalker(), "GP");
    EXPECT_EQ(gsaMsg->getSentenceType(), "GSA");
    EXPECT_EQ(gsaMsg->getSelectionMode(), 'A');
    EXPECT_EQ(gsaMsg->getMode(), 3u);
    EXPECT_DOUBLE_EQ(gsaMsg->getPdop(), 1.8);
    EXPECT_DOUBLE_EQ(gsaMsg->getHdop(), 1.0);
    EXPECT_DOUBLE_EQ(gsaMsg->getVdop(), 1.5);
    EXPECT_FALSE(gsaMsg->hasSystemId());
}

TEST(GSA, CreateFromMessage0183FactoryWithSystemId)
{
    auto msg = Nmea0183Factory::create(GSA_SENTENCE_WITH_SYSTEM_ID);
    ASSERT_NE(msg, nullptr);

    auto gsaMsg = dynamic_cast<GSA*>(msg.get());
    ASSERT_NE(gsaMsg, nullptr);

    EXPECT_EQ(gsaMsg->getTalker(), "GN");
    EXPECT_EQ(gsaMsg->getSentenceType(), "GSA");
    EXPECT_EQ(gsaMsg->getSelectionMode(), 'A');
    EXPECT_EQ(gsaMsg->getMode(), 3u);
    EXPECT_EQ(gsaMsg->getSatelliteId(0), 80u);
    EXPECT_EQ(gsaMsg->getSatelliteId(4), 69u);
    EXPECT_EQ(gsaMsg->getSatelliteId(5), 0u);
    EXPECT_DOUBLE_EQ(gsaMsg->getPdop(), 1.83);
    EXPECT_DOUBLE_EQ(gsaMsg->getHdop(), 1.09);
    EXPECT_DOUBLE_EQ(gsaMsg->getVdop(), 1.47);
    ASSERT_TRUE(gsaMsg->hasSystemId());
    ASSERT_TRUE(gsaMsg->getSystemId().has_value());
    EXPECT_EQ(gsaMsg->getSystemId().value(), 1u);
}

TEST(GSA, CreateFromFields)
{
    const std::array<unsigned int, 12> satelliteIds{4, 5, 9, 12, 24, 25, 29, 31, 32, 37, 43, 46};
    GSA gsa("GP", 'A', 3u, satelliteIds, 1.8, 1.0, 1.5);

    EXPECT_EQ(gsa.getTalker(), "GP");
    EXPECT_EQ(gsa.getSentenceType(), "GSA");
    EXPECT_EQ(gsa.getSelectionMode(), 'A');
    EXPECT_EQ(gsa.getMode(), 3u);
    EXPECT_EQ(gsa.getSatelliteIds(), satelliteIds);
    EXPECT_DOUBLE_EQ(gsa.getPdop(), 1.8);
    EXPECT_DOUBLE_EQ(gsa.getHdop(), 1.0);
    EXPECT_DOUBLE_EQ(gsa.getVdop(), 1.5);
    EXPECT_FALSE(gsa.hasSystemId());
}

TEST(GSA, SerializeRoundTripFromFields)
{
    const std::array<unsigned int, 12> satelliteIds{80, 71, 73, 79, 69, 0, 0, 0, 0, 0, 0, 0};
    GSA source("GN", 'A', 3u, satelliteIds, 1.83, 1.09, 1.47, 1u);

    auto parsedBase = Nmea0183Factory::create(source.serialize());
    ASSERT_NE(parsedBase, nullptr);
    auto parsed = dynamic_cast<GSA*>(parsedBase.get());
    ASSERT_NE(parsed, nullptr);

    EXPECT_EQ(parsed->getSelectionMode(), 'A');
    EXPECT_EQ(parsed->getMode(), 3u);
    EXPECT_EQ(parsed->getSatelliteId(0), 80u);
    EXPECT_EQ(parsed->getSatelliteId(4), 69u);
    EXPECT_EQ(parsed->getSatelliteId(5), 0u);
    EXPECT_DOUBLE_EQ(parsed->getPdop(), 1.83);
    EXPECT_DOUBLE_EQ(parsed->getHdop(), 1.09);
    EXPECT_DOUBLE_EQ(parsed->getVdop(), 1.47);
    ASSERT_TRUE(parsed->hasSystemId());
    EXPECT_EQ(parsed->getSystemId().value(), 1u);

    const std::string wire = source.serialize();
    EXPECT_NE(wire.find("$GNGSA,A,3,80,71,73,79,69,,,,,,,,1.83,1.09,1.47,1"), std::string::npos);
}

TEST(GSA, CreateFromMessageFactoryWithIncompleteFieldsDefaults)
{
    auto msg = Nmea0183Factory::create(INCOMPLETE_GSA_SENTENCE);
    ASSERT_NE(msg, nullptr);
    auto gsaMsg = dynamic_cast<GSA*>(msg.get());
    ASSERT_NE(gsaMsg, nullptr);

    EXPECT_EQ(gsaMsg->getSelectionMode(), 'M');
    EXPECT_EQ(gsaMsg->getMode(), 2u);
    for (size_t index = 0; index < 12; ++index) {
        EXPECT_EQ(gsaMsg->getSatelliteId(index), 0u);
    }
    EXPECT_DOUBLE_EQ(gsaMsg->getPdop(), 2.4);
    EXPECT_DOUBLE_EQ(gsaMsg->getHdop(), 1.8);
    EXPECT_DOUBLE_EQ(gsaMsg->getVdop(), 1.6);
    EXPECT_FALSE(gsaMsg->hasSystemId());
}

TEST(GSA, ClonePreservesGsaContent)
{
    const std::array<unsigned int, 12> satelliteIds{4, 5, 9, 12, 24, 25, 29, 31, 32, 37, 43, 46};
    GSA source("GP", 'A', 3u, satelliteIds, 1.8, 1.0, 1.5);

    std::unique_ptr<nmealib::Message> clonedBase = source.clone();
    ASSERT_NE(clonedBase, nullptr);
    auto cloned = dynamic_cast<GSA*>(clonedBase.get());
    ASSERT_NE(cloned, nullptr);

    EXPECT_TRUE(source.hasEqualContent(*cloned));
    EXPECT_TRUE(source == *cloned);
}

TEST(GSA, FactoryDoesNotPromoteNonGsaSentence)
{
    auto base = Nmea0183Factory::create(NOT_GSA_SENTENCE);
    ASSERT_NE(base, nullptr);
    auto gsa = dynamic_cast<GSA*>(base.get());
    EXPECT_EQ(gsa, nullptr);
}

TEST(GSA, FactoryThrowsOnMalformedGsaSentence)
{
    EXPECT_THROW(Nmea0183Factory::create(MALFORMED_GSA_SENTENCE), NotGSAException);
}
