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

Add nmealib to your CMakeLists.txt using FetchContent to download the pre-compiled release:

```cmake
include(FetchContent)

# Fetch the pre-compiled nmealib binary release
set(NMEALIB_VERSION "0.3.1")
FetchContent_Declare(
    nmealib
    URL "https://github.com/fliuzzi02/nmealib/releases/download/v${NMEALIB_VERSION}/nmealib-${NMEALIB_VERSION}-linux64.tar.gz"
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(nmealib)

# Add the nmealib cmake directory to the prefix path so find_package can locate it
list(APPEND CMAKE_PREFIX_PATH "${nmealib_SOURCE_DIR}/lib/cmake")

find_package(nmealib REQUIRED)
target_link_libraries(your_target PRIVATE nmealib::nmealib)
```

Then include and use the library:

```cpp
#include <nmealib.h>

auto msg = nmealib::nmea0183::Nmea0183Factory::create("$GPGGA,...*47\r\n");
```

For detailed integration examples, see the [using-nmealib](https://github.com/fliuzzi02/using-nmealib) repository.

Alternatively, a CLI application binary is provided for quick message parsing without integration into your project.

Releases are available on [GitHub Releases](../../releases).

### Prerequisites

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

## Examples

## Contributing

To know how to contribute to this project, please refer to the [CONTRIBUTING.md](docs/CONTRIBUTING.md) file.

## License

This project is licensed under the GPL-3.0 License - see the [LICENSE](LICENSE) file for details.

## Support

