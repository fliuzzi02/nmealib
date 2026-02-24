#include <iostream>

#include <nmealib/nmealib.hpp>

void print_usage() {
    std::cout << "Usage: nmealib-cli [options] \"<nmea_sentence>\"\n";
    std::cout << "Options:\n";
    std::cout << "  -h, --help        Show this help message and exit\n";
    std::cout << "  -v, --version     Show version information and exit\n";
    std::cout << "  -V, --verbose     Enable verbose output\n";
    std::cout << "Example:\n";
    std::cout << "  nmealib-cli -V \"$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\"\n";
}

int main(int argc, char** argv) {
    // If no arguments are provided, print usage and exit
    if (argc == 1) {
        print_usage();
        return 0;
    }

    // Check if verbosity is enabled
    bool verbose = false;
    std::string nmea_sentence;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            print_usage();
            return 0;
        } else if (arg == "-v" || arg == "--version") {
            std::cout << "nmealib version 1.0.0\n";
            return 0;
        } else if (arg == "-V" || arg == "--verbose") {
            verbose = true;
        } else if (arg[0] == '-') {
            std::cerr << "Unknown option: " << arg << "\n";
            print_usage();
            return 1;
        } else {
            // Assign nmea sentence without leading/trailing " if present
            if(arg[0] == '"' && arg[arg.size() - 1] == '"')
                nmea_sentence = arg.substr(1, arg.size() - 2);
            else
                nmea_sentence = arg;
        }
    }

    // Create a Message0183 object from the input sentence
    try {
        auto message = nmealib::nmea0183::Message0183::create(nmea_sentence);
        std::cout << message->getStringContent(verbose) << "\n";
    } catch (const nmealib::NmeaException& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}