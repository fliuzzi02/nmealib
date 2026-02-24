#include <iostream>
#include <string>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include <nmealib/nmealib.hpp>

void print_usage() {
    std::cout << "Usage: nmealib-cli [options] [-m \"<nmea_sentence>\" | -m -]\n";
    std::cout << "Options:\n";
    std::cout << "  -h, --help        Show this help message and exit\n";
    std::cout << "  -v, --version     Show version information and exit\n";
    std::cout << "  -V, --verbose     Enable verbose output\n";
    std::cout << "  -m, --message     Parse a single NMEA sentence (use '-' for stdin)\n";
    std::cout << "Example:\n";
    std::cout << "  nmealib-cli -V -m \"$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\"\n";
    std::cout << "  cat nmea.txt | nmealib-cli\n";
}

namespace {
std::size_t process_stream_from_stdin(bool verbose) {
    std::string line;
    std::size_t error_count = 0;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }

        try {
            auto message = nmealib::nmea0183::Nmea0183Factory::create(line);
            std::cout << message->getStringContent(verbose) << "\n";
        } catch (const nmealib::NmeaException& e) {
            std::cerr << e.what() << "\n";
            ++error_count;
        }
    }

    return error_count;
}

bool stdin_is_tty() {
#ifdef _WIN32
    return ::_isatty(::_fileno(stdin)) == 1;
#else
    return ::isatty(STDIN_FILENO) == 1;
#endif
}
}

int main(int argc, char** argv) {
    // If no arguments are provided and stdin is interactive, print usage and exit
    if (argc == 1 && stdin_is_tty()) {
        print_usage();
        return 0;
    }

    // Check if verbosity is enabled
    bool verbose = false;
    std::string nmea_sentence;
    bool expect_message = false;
    bool use_stdin_stream = false;

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
        } else if (arg == "-m" || arg == "--message") {
            if (i + 1 >= argc) {
                std::cerr << "Missing argument for " << arg << "\n";
                print_usage();
                return 1;
            }
            expect_message = true;
            continue;
        } else if (expect_message) {
            expect_message = false;
            if (arg == "-") {
                use_stdin_stream = true;
            } else {
                // Assign nmea sentence without leading/trailing " if present
                if (arg[0] == '"' && arg[arg.size() - 1] == '"')
                    nmea_sentence = arg.substr(1, arg.size() - 2);
                else
                    nmea_sentence = arg;
            }
        } else if (arg[0] == '-') {
            std::cerr << "Unknown option: " << arg << "\n";
            print_usage();
            return 1;
        } else {
            std::cerr << "Unexpected argument: " << arg << "\n";
            print_usage();
            return 1;
        }
    }

    if (expect_message) {
        std::cerr << "Missing argument for -m/--message\n";
        print_usage();
        return 1;
    }

    if (!use_stdin_stream && nmea_sentence.empty() && !stdin_is_tty()) {
        use_stdin_stream = true;
    }

    if (use_stdin_stream) {
        const auto error_count = process_stream_from_stdin(verbose);
        return error_count == 0 ? 0 : 1;
    }

    if (nmea_sentence.empty()) {
        std::cerr << "No NMEA sentence provided.\n";
        print_usage();
        return 1;
    }

    // Create a Message0183 object from the input sentence
    try {
        auto message = nmealib::nmea0183::Nmea0183Factory::create(nmea_sentence);
        std::cout << message->getStringContent(verbose) << "\n";
    } catch (const nmealib::NmeaException& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}