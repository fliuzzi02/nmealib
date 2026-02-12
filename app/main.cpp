#include <iostream>

#include "nmea0183.hpp"

int main() {
    std::cout << "Hello, World!" << std::endl;

    // Current time
    auto now = std::chrono::system_clock::now();
    auto raw = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47";
    nmealib::nmea0183::Message0183 msg(raw, now);

    std::cout << "Raw data: " << msg.getRawData() << std::endl;
    std::cout << "Payload: " << msg.getPayload() << std::endl;
    std::cout << "Checksum: " << msg.getChecksumStr() << std::endl;
    std::cout << "Valid: " << std::boolalpha << msg.validate() << std::endl;

    return 0;
}