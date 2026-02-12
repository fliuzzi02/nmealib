#include <gtest/gtest.h>
#include <forward_list>
#include "nmea0183.hpp"

using namespace nmealib::nmea0183;

// Messages to test the class with
std::forward_list<std::string> validMessages = {
    "$PFUGDP,GN,033615.00,3953.88002,N,10506.75324,W,13,9,FF,0.1,0.1,149,0.1*13\r\n",
    "$GPGGA,210230,3855.4487,N,09446.0071,W,1,07,1.1,370.5,M,-29.5,M,,*7A\r\n"
};
std::forward_list<std::string> invalidMessages = {
    "GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*45", // Missing leading '$'
    "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*ZZ", // Invalid checksum
    "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*27\r\nEXTRA" // Extra data after CRLF
};

// Just a basic test to ensure the class hierarchy compiles and can be used polymorphically.
TEST(Message0183, PolymorphicClone)
{
    Message0183 msg("!AIVDM,1,1,,A,15MuqP0000G?tO`K@E>4?wN0<00,0*5C");
    std::unique_ptr<nmealib::Message> basePtr = std::make_unique<Message0183>(msg);
    auto clonedPtr = basePtr->clone();
    EXPECT_TRUE(clonedPtr != nullptr);
    EXPECT_EQ(clonedPtr->serialize(), msg.serialize());
}

// Test checksum validation
TEST(Message0183, Validate) {
    for (const auto& raw : validMessages) {
        Message0183 msg(raw);
        EXPECT_TRUE(msg.validate()) << "Valid message failed validation: " << raw;
    }
    for (const auto& raw : invalidMessages) {
        Message0183 msg(raw);
        EXPECT_FALSE(msg.validate()) << "Invalid message passed validation: " << raw;
    }
}
