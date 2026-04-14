#include <gtest/gtest.h>
#include <chrono>
#include <memory>
#include <string>

#include <nmealib.h>

using namespace nmealib::nmea0183;

static const std::string SAMPLE_WITH_CHECKSUM =
    "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n";

static const std::string SAMPLE_NO_CHECKSUM =
    "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,\r\n";

TEST(Message0183, CreateSucceedsAndAccessors)
{
    auto msg = Nmea0183Factory::create(SAMPLE_WITH_CHECKSUM);
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getStartChar(), '$');
    EXPECT_EQ(msg->getTalker(), "GP");
    EXPECT_EQ(msg->getSentenceType(), "GGA");
    EXPECT_EQ(msg->getPayload().substr(0,5), "GPGGA");

    // Checksum string should match the provided two hex digits
    EXPECT_NO_THROW({
        auto cs = msg->getChecksumStr();
        EXPECT_EQ(cs, "47");
    });

    // Calculated checksum should match the provided checksum for a valid sentence
    EXPECT_EQ(msg->getChecksumStr(), msg->getCalculatedChecksumStr());
}

TEST(Message0183, CreateWithoutChecksumAndGetChecksumThrows)
{
    auto msg = Nmea0183Factory::create(SAMPLE_NO_CHECKSUM);
    ASSERT_NE(msg, nullptr);
    EXPECT_THROW(msg->getChecksumStr(), NoChecksumException);
    // But calculated checksum is still available and noexcept
    EXPECT_NO_THROW({
        auto calc = msg->getCalculatedChecksumStr();
        EXPECT_EQ(calc.size(), 2u);
    });
}

TEST(Message0183, CreateThrowsOnTooLongSentence)
{
    // Construct a string longer than 82 characters
    std::string longRaw(90, 'A');
    longRaw[0] = '$';
    EXPECT_THROW(Nmea0183Factory::create(longRaw), TooLongSentenceException);
}

TEST(Message0183, CreateThrowsOnInvalidStartCharacter)
{
    std::string s = SAMPLE_WITH_CHECKSUM;
    s.erase(0,1); // remove leading '$'
    EXPECT_THROW(Nmea0183Factory::create(s), InvalidStartCharacterException);
}

TEST(Message0183, CreateThrowsOnMissingEndline)
{
    // Remove CRLF from a valid sample
    std::string s = SAMPLE_WITH_CHECKSUM;
    if (s.size() >= 2) s.erase(s.size()-2);
    EXPECT_NO_THROW({
        auto test = Nmea0183Factory::create(s);
    });
}

TEST(Message0183, CloneProducesEqualObject)
{
    auto msg = Nmea0183Factory::create(SAMPLE_WITH_CHECKSUM);
    ASSERT_NE(msg, nullptr);

    std::unique_ptr<nmealib::Message> baseClone = msg->clone();
    ASSERT_NE(baseClone, nullptr);

    // Attempt to downcast and compare content
    auto derivedClone = dynamic_cast<Message0183*>(baseClone.get());
    ASSERT_NE(derivedClone, nullptr);
    EXPECT_TRUE(*msg == *derivedClone);
}

TEST(Message0183, ValidateBehavior)
{
    // Valid checksum -> validate() == true
    auto good = Nmea0183Factory::create(SAMPLE_WITH_CHECKSUM);
    ASSERT_NE(good, nullptr);
    EXPECT_TRUE(good->validate());

    // Invalid checksum -> validate() == false
    std::string bad = SAMPLE_WITH_CHECKSUM;
    auto star = bad.find('*');
    if (star != std::string::npos && star + 3 < bad.size()) {
        bad[star+1] = '0';
        bad[star+2] = '0';
    }
    auto invalid = Nmea0183Factory::create(bad);
    ASSERT_NE(invalid, nullptr);
    EXPECT_FALSE(invalid->validate());

    // No checksum -> validate() == true
    auto no = Nmea0183Factory::create(SAMPLE_NO_CHECKSUM);
    ASSERT_NE(no, nullptr);
    EXPECT_TRUE(no->validate());
}

TEST(Message0183, CloneBehavior) {
    auto original = Nmea0183Factory::create(SAMPLE_WITH_CHECKSUM);
    ASSERT_NE(original, nullptr);

    auto clone = original->clone();
    ASSERT_NE(clone, nullptr);

    // The clone should be equal to the original
    EXPECT_TRUE(*original == *clone);

    // But they should not be the same object (different addresses)
    EXPECT_NE(original.get(), clone.get());
}

TEST(Message0183, GetStringContent) {
    std::string unimplemented = "$XXABC,123*00\r\n";
    auto msg = Nmea0183Factory::create(unimplemented);
    ASSERT_NE(msg, nullptr);

    // Verbose should include all fields
    std::string verbose = msg->getStringContent(true);
    EXPECT_NE(verbose.find("Talker: XX"), std::string::npos);
    EXPECT_NE(verbose.find("Sentence Type: ABC"), std::string::npos);
    EXPECT_NE(verbose.find("Unimplemented sentence"), std::string::npos);

    // Non-verbose should be a single line summary
    std::string nonVerbose = msg->getStringContent(false);
    EXPECT_NE(nonVerbose.find("XX ABC"), std::string::npos);
    EXPECT_NE(nonVerbose.find("Unimplemented sentence"), std::string::npos);
}
