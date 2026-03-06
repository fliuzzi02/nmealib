# nmealib
Library to parse NMEA0183 and NMEA2000 messages in C++.

## Overview

**nmealib** is a modern, header-inclusive C++20 library for parsing and handling NMEA (National Marine Electronics Association) messages used in marine navigation, weather, and maritime communication systems.

The library provides comprehensive support for two major NMEA standards:

- **NMEA 0183**: Serial sentence-based protocol used for communication between marine equipment. Supports parsing of standard sentence types including GPS fixes (GGA, GLL, RMC), navigation data (VTG, VHW), environmental data (DBT, MWV), and more.

- **NMEA 2000**: Modern CAN (Controller Area Network) based protocol providing higher data rates and multi-node communication. Uses Parameter Group Numbers (PGNs) for message identification and supports both single-frame and multi-frame (fast-packet) transmission.

## Features

- **Robust message parsing** with comprehensive error handling and exception-based error reporting
- **Factory pattern** for easy message creation and automatic message type detection
- **Checksum validation** for NMEA 0183 sentences with built-in validation and generation
- **High-precision latitude/longitude** and other navigational data extraction
- **Typed message classes** for compile-time type safety and IDE autocomplete support
- **Extensible architecture** for easily adding support for new message types
- **C++20 modern features** including move semantics and smart pointers
- **Comprehensive test coverage** with unit tests for all message types

## Quick Start

To quickly get started with **nmealib**, you can use the provided CLI application for parsing NMEA messages without needing to integrate the library into your project. This is ideal for testing and quick parsing tasks.

Download the latest release from [GitHub Releases](../../releases) and run the CLI application to parse NMEA sentences directly from the command line.
```
./nmealib-cli "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47"

cat nmea_sentences.txt | ./nmealib-cli
``` 

To use the library in your project, refer to the [Installation Guide](docs/INSTALLATION.md) for detailed instructions on how to build and integrate **nmealib** into your C++ projects.

You can also find usage examples in the [Examples](docs/EXAMPLES.md) section, or go to the [using-nmealib](https://github.com/fliuzzi02/using-nmealib) repository.

### Prerequisites

To build this library from source, you will need the following tools and dependencies:
- CMake 3.20+
- C++20 compatible compiler (GCC 10+, Clang 12+, MSVC 2019+)
- Build tools (GNU Make, Ninja, or Visual Studio)
- Optional tools for development (Google Test, clang-tidy, cppcheck)

### Installation
To use this library you can either build it from source or use the provided binaries. For detailed installation instructions, please refer to the [Installation Guide](docs/INSTALLATION.md).

### Basic Usage

## Documentation

## Project Structure

## Building from Source

### Requirements

### Build Instructions

### Running Tests

## API Documentation

You can find fully documented API reference in the [API Reference](docs/API.md) section.

## Examples

Once included in your project, you can find usage examples in the [Usage Guide](docs/USAGE.md) section, or go to the [using-nmealib](https://github.com/fliuzzi02/using-nmealib) repository.

## Contributing

To know how to contribute to this project, please refer to the [CONTRIBUTING.md](docs/CONTRIBUTING.md) file.

## License

This project is licensed under the GPL-3.0 License - see the [LICENSE](LICENSE) file for details.

## Support

