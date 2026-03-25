#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <vector>

#include <nmealib.h>

using namespace nmealib::nmea0183;

static const std::string XDR_SENTENCE = "$HCXDR,A,171,D,PITCH,A,-37,D,ROLL,G,367,,MAGX,G,2420,,MAGY,G,-8984,,MAGZ*41\r\n";
static const std::string XDR_SENTENCE_NO_CHECKSUM = "$HCXDR,A,171,D,PITCH,A,-37,D,ROLL,G,367,,MAGX,G,2420,,MAGY,G,-8984,,MAGZ\r\n";
static const std::string INCOMPLETE_XDR_SENTENCE = "$HCXDR,A,,D,PITCH\r\n";
static const std::string NOT_XDR_SENTENCE = "$GPGLL,4916.45,N,12311.12,W,225444,A,*1D\r\n";
static const std::string MALFORMED_XDR_SENTENCE = "$HCXDR,A,171,D\r\n";

TEST(XDR, CreateFromMessage0183Factory)
{
    auto msg = Nmea0183Factory::create(XDR_SENTENCE);
    ASSERT_NE(msg, nullptr);

    auto xdrMsg = dynamic_cast<XDR*>(msg.get());
    ASSERT_NE(xdrMsg, nullptr);

    EXPECT_EQ(xdrMsg->getTalker(), "HC");
    EXPECT_EQ(xdrMsg->getSentenceType(), "XDR");
    ASSERT_EQ(xdrMsg->getTransducerCount(), 5u);

    const auto* pitch = xdrMsg->getTransducer(0);
    ASSERT_NE(pitch, nullptr);
    EXPECT_EQ(pitch->transducerType, 'A');
    EXPECT_DOUBLE_EQ(pitch->measurement, 171.0);
    EXPECT_EQ(pitch->units, 'D');
    EXPECT_EQ(pitch->name, "PITCH");

    const auto* roll = xdrMsg->getTransducer(1);
    ASSERT_NE(roll, nullptr);
    EXPECT_EQ(roll->transducerType, 'A');
    EXPECT_DOUBLE_EQ(roll->measurement, -37.0);
    EXPECT_EQ(roll->units, 'D');
    EXPECT_EQ(roll->name, "ROLL");

    const auto* magx = xdrMsg->getTransducer(2);
    ASSERT_NE(magx, nullptr);
    EXPECT_EQ(magx->transducerType, 'G');
    EXPECT_DOUBLE_EQ(magx->measurement, 367.0);
    EXPECT_EQ(magx->units, '\0');
    EXPECT_EQ(magx->name, "MAGX");

    msg = Nmea0183Factory::create(XDR_SENTENCE_NO_CHECKSUM);
    ASSERT_NE(msg, nullptr);
    xdrMsg = dynamic_cast<XDR*>(msg.get());
    ASSERT_NE(xdrMsg, nullptr);
    ASSERT_EQ(xdrMsg->getTransducerCount(), 5u);
}

TEST(XDR, CreateFromFields)
{
    std::vector<XDR::TransducerMeasurement> transducers = {
        {'A', 171.0, 'D', "PITCH"},
        {'A', -37.0, 'D', "ROLL"},
        {'G', 367.0, '\0', "MAGX"}
    };

    XDR xdr("HC", transducers);

    EXPECT_EQ(xdr.getTalker(), "HC");
    EXPECT_EQ(xdr.getSentenceType(), "XDR");
    ASSERT_EQ(xdr.getTransducerCount(), 3u);

    const auto* transducer = xdr.getTransducer(2);
    ASSERT_NE(transducer, nullptr);
    EXPECT_EQ(transducer->transducerType, 'G');
    EXPECT_DOUBLE_EQ(transducer->measurement, 367.0);
    EXPECT_EQ(transducer->units, '\0');
    EXPECT_EQ(transducer->name, "MAGX");
}

TEST(XDR, SerializeRoundTripFromFields)
{
    std::vector<XDR::TransducerMeasurement> transducers = {
        {'A', 171.0, 'D', "PITCH"},
        {'A', -37.0, 'D', "ROLL"},
        {'G', -8984.0, '\0', "MAGZ"}
    };

    XDR source("HC", transducers);

    auto parsedBase = Nmea0183Factory::create(source.serialize());
    ASSERT_NE(parsedBase, nullptr);
    auto parsed = dynamic_cast<XDR*>(parsedBase.get());
    ASSERT_NE(parsed, nullptr);

    ASSERT_EQ(parsed->getTransducerCount(), 3u);
    const auto* magz = parsed->getTransducer(2);
    ASSERT_NE(magz, nullptr);
    EXPECT_EQ(magz->transducerType, 'G');
    EXPECT_DOUBLE_EQ(magz->measurement, -8984.0);
    EXPECT_EQ(magz->units, '\0');
    EXPECT_EQ(magz->name, "MAGZ");

    const std::string wire = source.serialize();
    EXPECT_NE(wire.find("$HCXDR,A,171,D,PITCH,A,-37,D,ROLL,G,-8984,,MAGZ"), std::string::npos);
}

TEST(XDR, CreateFromMessageFactoryWithIncompleteFieldsDefaults)
{
    auto msg = Nmea0183Factory::create(INCOMPLETE_XDR_SENTENCE);
    ASSERT_NE(msg, nullptr);
    auto xdrMsg = dynamic_cast<XDR*>(msg.get());
    ASSERT_NE(xdrMsg, nullptr);

    ASSERT_EQ(xdrMsg->getTransducerCount(), 1u);
    const auto* transducer = xdrMsg->getTransducer(0);
    ASSERT_NE(transducer, nullptr);
    EXPECT_EQ(transducer->transducerType, 'A');
    EXPECT_DOUBLE_EQ(transducer->measurement, 0.0);
    EXPECT_EQ(transducer->units, 'D');
    EXPECT_EQ(transducer->name, "PITCH");
}

TEST(XDR, ClonePreservesXdrContent)
{
    std::vector<XDR::TransducerMeasurement> transducers = {
        {'A', 171.0, 'D', "PITCH"}
    };
    XDR source("HC", transducers);

    std::unique_ptr<nmealib::Message> clonedBase = source.clone();
    ASSERT_NE(clonedBase, nullptr);
    auto cloned = dynamic_cast<XDR*>(clonedBase.get());
    ASSERT_NE(cloned, nullptr);

    EXPECT_TRUE(source == *cloned);
}

TEST(XDR, FactoryDoesNotPromoteNonXdrSentence)
{
    auto base = Nmea0183Factory::create(NOT_XDR_SENTENCE);
    ASSERT_NE(base, nullptr);
    auto xdr = dynamic_cast<XDR*>(base.get());
    EXPECT_EQ(xdr, nullptr);
}

TEST(XDR, FactoryThrowsOnMalformedXdrSentence)
{
    EXPECT_THROW(Nmea0183Factory::create(MALFORMED_XDR_SENTENCE), NotXDRException);
}
