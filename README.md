# nmealib

A modern C++20 library for parsing and validating NMEA (National Marine Electronics Association) messages.

## Overview

nmealib is a lightweight, header-friendly C++ library designed to parse, validate, and manipulate NMEA messages. Currently, it supports the NMEA 0183 standard with plans to extend support to NMEA 2000 in the future.

NMEA 0183 is a widely-used standard for communication between marine electronics such as GPS receivers, autopilots, echo sounders, and other navigational equipment.

## Features

- **NMEA 0183 Support**: Full parsing and validation of NMEA 0183 sentences
- **Checksum Validation**: Automatic calculation and verification of message checksums
- **Type-Safe API**: Modern C++20 design with strong type safety
- **Exception-Based Error Handling**: Clear error reporting for invalid messages
- **Timestamp Support**: Automatic timestamping of parsed messages
- **Message Serialization**: Convert parsed messages back to wire format
- **Command-Line Interface**: Included CLI tool for testing and message parsing
- **Comprehensive Testing**: Full test suite using Google Test

## Requirements

- C++20 compatible compiler (GCC 10+, Clang 12+, MSVC 2019+)
- CMake 3.20 or higher
- Google Test (automatically fetched during build if tests are enabled)

## Installation

### Building from Source

1. Clone the repository:
```bash
git clone https://github.com/fliuzzi02/nmealib.git
cd nmealib
```

2. Configure with CMake:
```bash
cmake --preset gcc-full
```

3. Build the library:
```bash
cmake --build out/build/gcc-full
```

4. (Optional) Run tests:
```bash
ctest --test-dir out/build/gcc-full
```

### Using in Your Project

#### As a CMake Subdirectory

Add nmealib as a subdirectory in your CMakeLists.txt:

```cmake
add_subdirectory(nmealib)
target_link_libraries(your_target PRIVATE nmealib)
```

#### Manual Integration

Include the necessary headers and link against the `nmealib_nmea0183` library:

```cmake
target_include_directories(your_target PRIVATE ${NMEALIB_SOURCE_DIR}/src)
target_link_libraries(your_target PRIVATE nmealib_nmea0183)
```

## Usage

### Library Usage

#### Basic Parsing

```cpp
#include "nmea0183/nmea0183.hpp"
#include <iostream>
#include <memory>

int main() {
    std::string sentence = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n";
    
    try {
        auto msg = nmealib::nmea0183::Message0183::create(sentence);
        
        std::cout << "Talker: " << msg->getTalker() << std::endl;
        std::cout << "Sentence Type: " << msg->getSentenceType() << std::endl;
        std::cout << "Payload: " << msg->getPayload() << std::endl;
        std::cout << "Valid: " << (msg->validate() ? "Yes" : "No") << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
```

#### Checksum Validation

```cpp
auto msg = nmealib::nmea0183::Message0183::create(sentence);

// Check if message has a checksum
try {
    std::string checksum = msg->getChecksumStr();
    std::string calculated = msg->getCalculatedChecksumStr();
    
    if (checksum == calculated) {
        std::cout << "Checksum valid!" << std::endl;
    }
} catch (const nmealib::nmea0183::NoChecksumException&) {
    std::cout << "Message has no checksum" << std::endl;
}

// Or simply use validate()
bool isValid = msg->validate();
```

#### Message Cloning

```cpp
auto original = nmealib::nmea0183::Message0183::create(sentence);
auto clone = original->clone();

// Compare messages
if (original->hasEqualContent(*dynamic_cast<nmealib::nmea0183::Message0183*>(clone.get()))) {
    std::cout << "Messages are equal!" << std::endl;
}
```

### Command-Line Interface

The library includes a CLI tool for parsing NMEA sentences:

```bash
# Parse a sentence
./nmealib-cli --nmea0183 '$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n'

# Output:
# Raw:      $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
# Payload:  GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,
# Checksum: 47
# Valid:    true
# Timestamp: 2026-02-14 17:48:28
# Serialized: $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
```

## NMEA 0183 Format

NMEA 0183 sentences follow a specific format:

```
$TALKER_ID,data_field_1,data_field_2,...,data_field_n*CHECKSUM\r\n
```

Where:
- `$` or `!` - Start character ($ for standard, ! for encapsulated)
- `TALKER_ID` - Two-character talker identifier (e.g., GP for GPS)
- `SENTENCE_TYPE` - Three-character sentence type (e.g., GGA, RMC)
- Data fields - Comma-separated values
- `*CHECKSUM` - Optional: asterisk followed by two-digit hex checksum
- `\r\n` - Carriage return and line feed

**Constraints:**
- Maximum length: 82 characters (including start and end characters)
- Checksum: XOR of all characters between $ and * (exclusive)

## API Reference

### Core Classes

#### `nmealib::Message`
Base class for all NMEA message types. Provides common functionality:
- `getType()` - Get message type (NMEA0183, NMEA2000, etc.)
- `getRawData()` - Get original raw message string
- `getTimestamp()` - Get message timestamp
- `clone()` - Create a polymorphic copy
- `serialize()` - Convert to wire format
- `validate()` - Validate message integrity

#### `nmealib::nmea0183::Message0183`
NMEA 0183 specific message implementation. Key methods:
- `static create(raw, timestamp)` - Factory method to create from raw string
- `getStartChar()` - Get start character ($ or !)
- `getTalker()` - Get two-character talker ID
- `getSentenceType()` - Get three-character sentence type
- `getPayload()` - Get message payload (data between start and checksum)
- `getChecksumStr()` - Get checksum from message (throws if none)
- `getCalculatedChecksumStr()` - Get calculated checksum
- `validate()` - Validate checksum and format

### Exception Types

All exceptions inherit from `nmealib::NmeaException`:

- `TooLongSentenceException` - Sentence exceeds 82 characters
- `InvalidStartCharacterException` - Invalid start character (not $ or !)
- `NoEndlineException` - Missing \r\n terminator
- `NoChecksumException` - No checksum when one was expected

## Building and Testing

### Build Options

CMake options:
- `ENABLE_TESTS` - Enable building tests (default: OFF)
- `ENABLE_STATIC_ANALYSIS` - Enable clang-tidy and cppcheck (default: OFF)

### Running Tests

```bash
# Configure with tests enabled
cmake --preset gcc-full

# Build
cmake --build out/build/gcc-full

# Run all tests
ctest --test-dir out/build/gcc-full

# Run specific test suite
ctest --test-dir out/build/gcc-full -L nmea0183
```

### Static Analysis

```bash
# Configure with static analysis enabled
cmake -B build -DENABLE_STATIC_ANALYSIS=ON

# Run analysis
cmake --build build --target static-analysis
```

## Project Structure

```
nmealib/
├── src/               # Library source code
│   ├── message.hpp    # Base message class
│   ├── nmeaException.hpp  # Exception definitions
│   └── nmea0183/      # NMEA 0183 implementation
│       ├── nmea0183.hpp
│       └── nmea0183.cpp
├── app/               # Command-line application
│   └── main.cpp
├── tests/             # Test suite
│   └── nmea0183/
│       └── test_nmea0183.cpp
├── CMakeLists.txt     # Root CMake configuration
└── README.md          # This file
```

## Roadmap

- [x] NMEA 0183 basic parsing and validation
- [x] Command-line interface
- [x] Comprehensive test suite
- [ ] Sentence-specific parsers (GGA, RMC, etc.)
- [ ] NMEA 2000 support
- [ ] Doxygen documentation generation
- [ ] Python bindings
- [ ] Additional examples and tutorials

## Contributing

Contributions are welcome! Please:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Make your changes with appropriate tests
4. Ensure all tests pass (`ctest`)
5. Run static analysis if possible
6. Commit your changes (`git commit -m 'Add amazing feature'`)
7. Push to the branch (`git push origin feature/amazing-feature`)
8. Open a Pull Request

### Coding Standards

- Follow C++20 best practices
- Use meaningful variable and function names
- Add tests for new functionality
- Document public APIs with Doxygen-style comments
- Ensure code passes clang-tidy and cppcheck

## License

This project is open source. Please see the LICENSE file for details.

## Acknowledgments

- NMEA 0183 Standard by National Marine Electronics Association
- Google Test framework for testing infrastructure

## Support

For issues, questions, or contributions, please use the GitHub issue tracker:
https://github.com/fliuzzi02/nmealib/issues

## See Also

- [Wiki Documentation](https://github.com/fliuzzi02/nmealib/wiki) - Detailed guides and tutorials
- [NMEA 0183 Standard Reference](https://www.nmea.org/)
