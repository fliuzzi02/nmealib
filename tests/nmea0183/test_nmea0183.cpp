#include <gtest/gtest.h>
#include <chrono>
#include <memory>
#include <string>
#include <nmealib/nmealib.hpp>

using namespace nmealib::nmea0183;

static const std::string SAMPLE_WITH_CHECKSUM =
    "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n";

static const std::string SAMPLE_NO_CHECKSUM =
    "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,\r\n";

TEST(Message0183, CreateSucceedsAndAccessors)
{
    auto msg = Message0183::create(SAMPLE_WITH_CHECKSUM);
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
    auto msg = Message0183::create(SAMPLE_NO_CHECKSUM);
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
    EXPECT_THROW(Message0183::create(longRaw), TooLongSentenceException);
}

TEST(Message0183, CreateThrowsOnInvalidStartCharacter)
{
    std::string s = SAMPLE_WITH_CHECKSUM;
    s.erase(0,1); // remove leading '$'
    EXPECT_THROW(Message0183::create(s), InvalidStartCharacterException);
}

TEST(Message0183, CreateThrowsOnMissingEndline)
{
    // Remove CRLF from a valid sample
    std::string s = SAMPLE_WITH_CHECKSUM;
    if (s.size() >= 2) s.erase(s.size()-2);
    EXPECT_NO_THROW({
        auto test = Message0183::create(s);
    });
}

TEST(Message0183, CloneProducesEqualObject)
{
    auto msg = Message0183::create(SAMPLE_WITH_CHECKSUM);
    ASSERT_NE(msg, nullptr);

    std::unique_ptr<nmealib::Message> baseClone = msg->clone();
    ASSERT_NE(baseClone, nullptr);

    // Attempt to downcast and compare content
    auto derivedClone = dynamic_cast<Message0183*>(baseClone.get());
    ASSERT_NE(derivedClone, nullptr);
    EXPECT_TRUE(msg->hasEqualContent(*derivedClone));
}

TEST(Message0183, EqualityAndHasEqualContent)
{
    using Clock = std::chrono::system_clock;
    auto ts = Clock::now();

    auto m1 = Message0183::create(SAMPLE_WITH_CHECKSUM, ts);
    auto m2 = Message0183::create(SAMPLE_WITH_CHECKSUM, ts);
    ASSERT_NE(m1, nullptr);
    ASSERT_NE(m2, nullptr);

    // When timestamps are identical, operator== should consider them equal
    EXPECT_TRUE(*m1 == *m2);

    // Different timestamps -> operator== may be false, but hasEqualContent ignores timestamp
    auto m3 = Message0183::create(SAMPLE_WITH_CHECKSUM);
    ASSERT_NE(m3, nullptr);
    EXPECT_TRUE(m1->hasEqualContent(*m3));
}

TEST(Message0183, ValidateBehavior)
{
    // Valid checksum -> validate() == true
    auto good = Message0183::create(SAMPLE_WITH_CHECKSUM);
    ASSERT_NE(good, nullptr);
    EXPECT_TRUE(good->validate());

    // Invalid checksum -> validate() == false
    std::string bad = SAMPLE_WITH_CHECKSUM;
    auto star = bad.find('*');
    if (star != std::string::npos && star + 3 < bad.size()) {
        bad[star+1] = '0';
        bad[star+2] = '0';
    }
    auto invalid = Message0183::create(bad);
    ASSERT_NE(invalid, nullptr);
    EXPECT_FALSE(invalid->validate());

    // No checksum -> validate() == true
    auto no = Message0183::create(SAMPLE_NO_CHECKSUM);
    ASSERT_NE(no, nullptr);
    EXPECT_TRUE(no->validate());
}



