#include <iostream>
#include <string>
#include <algorithm>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include <nmealib.h>

void print_usage() {
    std::cout << "Usage: nmealib-cli [options] [-m \"<nmea_sentence>\" | -m -]\n";
    std::cout << "Options:\n";
    std::cout << "  -h, --help                      Show this help message and exit\n";
    std::cout << "  -v, --version                   Show version information and exit\n";
    std::cout << "  -V, --verbose                   Enable verbose output\n";
    std::cout << "  -t=<type>, --type=<type>        Protocol type: N0183 (default) or N2K\n";
    std::cout << "  -m, --message                   Parse a single NMEA sentence (use '-' for stdin)\n";
    std::cout << "  -c, --candump                   Enable candump mode for NMEA 2000 input (raw CAN frames)\n";
    std::cout << "Example:\n";
    std::cout << "  nmealib-cli -V -t=N0183 -m \"$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\"\n";
    std::cout << "  nmealib-cli -t=N2K -m \"18F80523 1D 01 23 45 67 FE DC BA 98 00 01 86 A0 12 00 04 E2 0C\"\n";
    std::cout << "  cat nmea.txt | nmealib-cli\n";
}

namespace {
enum class ProtocolType {
    N0183,
    N2K
};

bool parse_protocol(const std::string& arg, ProtocolType& protocol) {
    std::string value = arg;
    std::transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
    if (value == "N0183") {
        protocol = ProtocolType::N0183;
        return true;
    }
    if (value == "N2K") {
        protocol = ProtocolType::N2K;
        return true;
    }
    return false;
}

std::size_t process_stream_from_stdin(bool verbose, ProtocolType protocol, bool candump_mode = false) {
    std::string line;
    std::size_t error_count = 0;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }

        // If in candump mode convert line from
        // "(timestamp) canX CANID#data" or "can0  09F8027F   [8]  00 FC FF FF 00 00 FF FF"
        // to "CANID:data"
        if(candump_mode) {
            // In candump mode, we expect lines in the format: "(timestamp) canX CANID#data"
            // or "can0  09F8027F   [8]  00 FC FF FF 00 00 FF FF"
            // We need to extract the CANID and data parts to create a valid NMEA 2000 message string
            
            std::string canid, data;
            size_t hash_pos = line.find('#');
            
            if (hash_pos != std::string::npos) {
                // Format: "(timestamp) canX CANID#data"
                size_t space_before_canid = line.rfind(' ', hash_pos);
                if (space_before_canid != std::string::npos) {
                    canid = line.substr(space_before_canid + 1, hash_pos - space_before_canid - 1);
                    data = line.substr(hash_pos + 1);
                }
            } else {
                // Format: "can0  09F8027F   [8]  00 FC FF FF 00 00 FF FF"
                std::istringstream iss(line);
                std::string token;
                int token_count = 0;
                
                while (iss >> token) {
                    if (token_count == 1) {
                        canid = token;
                    } else if (token_count >= 3 && token != "[8]" && token.find('[') == std::string::npos) {
                        if (!data.empty()) data += " ";
                        data += token;
                    }
                    token_count++;
                }
            }
            
            if (!canid.empty() && !data.empty()) {
                line = canid + ":" + data;
            }
        }

        try {
            if (protocol == ProtocolType::N0183) {
                auto message = nmealib::nmea0183::Nmea0183Factory::create(line);
                std::cout << message->getStringContent(verbose) << "\n";
            } else {
                auto message = nmealib::nmea2000::Nmea2000Factory::create(line);
                std::cout << message->getStringContent(verbose) << "\n";
            }
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
    bool candump_mode = false;
    ProtocolType protocol = ProtocolType::N0183;

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
        } else if (arg.rfind("-t=", 0) == 0 || arg.rfind("--type=", 0) == 0) {
            std::string protocolValue = arg.rfind("-t=", 0) == 0 ? arg.substr(3) : arg.substr(7);
            if (!parse_protocol(protocolValue, protocol)) {
                std::cerr << "Invalid protocol type: " << protocolValue << ". Use N0183 or N2K\n";
                print_usage();
                return 1;
            }
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
        } else if (arg == "-c" || arg == "--candump") {
            candump_mode = true;
            protocol = ProtocolType::N2K;
            use_stdin_stream = true;
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
        const auto error_count = process_stream_from_stdin(verbose, protocol, candump_mode);
        return error_count == 0 ? 0 : 1;
    }

    if (nmea_sentence.empty()) {
        std::cerr << "No NMEA sentence provided.\n";
        print_usage();
        return 1;
    }

    // Create a Message0183 object from the input sentence
    try {
        if (protocol == ProtocolType::N0183) {
            auto message = nmealib::nmea0183::Nmea0183Factory::create(nmea_sentence);
            std::cout << message->getStringContent(verbose) << "\n";
        } else {
            auto message = nmealib::nmea2000::Nmea2000Factory::create(nmea_sentence);
            std::cout << message->getStringContent(verbose) << "\n";
        }
    } catch (const nmealib::NmeaException& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}