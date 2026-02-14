// ...existing code...
#include <iostream>
#include <chrono>
#include <string>
#include <iomanip>
#include <ctime>

#include "nmea0183.hpp"

static void print_usage(const char* prog) {
    std::cerr << "Usage: " << prog << " --nmea0183 \"<NMEA_SENTENCE>\"\n";
}

int main(int argc, char** argv) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    std::string sentence;
    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        const std::string prefix = "--nmea0183=";
        if (a == "--nmea0183") {
            if (i + 1 < argc) {
                sentence = argv[i + 1];
                break;
            } else {
                print_usage(argv[0]);
                return 1;
            }
        } else if (a.rfind(prefix, 0) == 0) {
            sentence = a.substr(prefix.size());
            break;
        }
    }

    if (sentence.empty()) {
        print_usage(argv[0]);
        return 1;
    }

    auto now = std::chrono::system_clock::now();
    std::unique_ptr<nmealib::nmea0183::Message0183> msg;
    try {
        msg = nmealib::nmea0183::Message0183::create(sentence, now);
    } catch (const std::exception& ex) {
        std::cerr << "Failed to parse NMEA sentence: " << ex.what() << '\n';
        return 2;
    }

    std::cout << "Raw:      " << msg->getRawData() << '\n';
    std::cout << "Payload:  " << msg->getPayload() << '\n';
    try {
        std::cout << "Checksum: " << msg->getChecksumStr() << '\n';
    } catch (const nmealib::nmea0183::NoChecksumException&) {
        std::cout << "Checksum: (none)" << '\n';
    }
    std::cout << "Valid:    " << std::boolalpha << msg->validate() << '\n';

    auto ts = msg->getTimestamp();
    std::time_t tt = std::chrono::system_clock::to_time_t(ts);
    std::tm tm = *std::localtime(&tt);
    std::cout << "Timestamp:" << std::put_time(&tm, " %Y-%m-%d %H:%M:%S") << '\n';

    std::cout << "Serialized: " << msg->serialize() << '\n';

    return 0;
}