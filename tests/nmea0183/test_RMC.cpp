#include <gtest/gtest.h>
#include <chrono>
#include <memory>
#include <string>
#include "nmea0183Factory.hpp"
#include "rmc.hpp"

using namespace nmealib::nmea0183;

static const std::string RMC_SENTENCE = "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A\r\n";
static const std::string NOT_RMC_SENTENCE = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n";

// Test RMC creation through parent class factory method
TEST(RMC, CreateFromMessage0183Factory)
{
    auto msg = Nmea0183Factory::create(RMC_SENTENCE);
    ASSERT_NE(msg, nullptr);
    // Attempt to downcast to RMC
    auto rmcMsg = dynamic_cast<RMC*>(msg.get());
    ASSERT_NE(rmcMsg, nullptr);
    EXPECT_EQ(rmcMsg->getTalker(), "GP");
    EXPECT_EQ(rmcMsg->getSentenceType(), "RMC");
    EXPECT_EQ(rmcMsg->getUtcFix(), 123519u);
    EXPECT_EQ(rmcMsg->getStatus(), 'A');
    EXPECT_DOUBLE_EQ(rmcMsg->getLatitude(), 48.1173);
    EXPECT_DOUBLE_EQ(rmcMsg->getLongitude(), 11.516666666666667);
    EXPECT_DOUBLE_EQ(rmcMsg->getSpeedOverGround(), 22.4);
    EXPECT_DOUBLE_EQ(rmcMsg->getCourseOverGround(), 84.4);
    EXPECT_EQ(rmcMsg->getDate(), 230394u);
    EXPECT_DOUBLE_EQ(rmcMsg->getMagneticVariation(), -3.1);
}