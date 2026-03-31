# nmealib

<p align="center">
    <img src="docs/nmealib.png" alt="nmealib logo" width="240"/>
</p>

[![License: GPL-3.0](https://img.shields.io/github/license/fliuzzi02/nmealib)](LICENSE)
[![Release](https://img.shields.io/github/v/release/fliuzzi02/nmealib)](https://github.com/fliuzzi02/nmealib/releases)
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/fliuzzi02/library/nmealib.svg)](https://registry.platformio.org/libraries/fliuzzi02/nmealib)
![C++20](https://img.shields.io/badge/C%2B%2B-20-blue)
[![Coverage Status](https://coveralls.io/repos/github/fliuzzi02/nmealib/badge.svg?branch=main)](https://coveralls.io/github/fliuzzi02/nmealib?branch=main)

Modern C++20 library for parsing **NMEA 0183** sentences and **NMEA 2000** CAN messages, with typed models, validation, and extensible message handling.

---

## Why nmealib

- Unified parsing for **NMEA 0183** and **NMEA 2000**
- Typed message classes for safer integrations
- Built-in validation (including NMEA 0183 checksum handling)
- Extensible architecture for new sentence/PGN support
- CLI tool for quick parsing and debugging
- Unit-tested codebase

---

## Quick Start

### CLI Usage

Download the latest release from [GitHub Releases](https://github.com/fliuzzi02/nmealib/releases), then parse a sentence:

```bash
./nmealib-cli '$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47'
```

Parse from a file/pipe:

```bash
cat nmea_sentences.txt | ./nmealib-cli
```

### Use the library in your C++ project

```cpp
#include <nmealib.h>

int main() {
    std::string sentence = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47";
    try {
        auto msg = nmealib::NMEA0183::GGA::parse(sentence);
        std::cout << "Parsed GGA message: " << msg.toString() << std::endl;
        std::cout << "Latitude: " << msg.getLatitude() << std::endl;
        // Access other fields as needed
    } catch (const nmealib::ParseException& e) {
        std::cerr << "Failed to parse sentence: " << e.what() << std::endl;
    }
    return 0;
}
```

See [using-nmealib](https://github.com/fliuzzi02/using-nmealib) for a full example project demonstrating library usage.

### Use the library in PlatformIO

Add the dependency in your `platformio.ini`:

```ini
lib_deps = fliuzzi02/nmealib
```

The published PlatformIO package is built with exceptions disabled:

- `-fno-exceptions`
- `-DNMEALIB_NO_EXCEPTIONS`

In that configuration, parse failures do not throw. Factory methods return `nullptr`.

```cpp
#include <nmealib.h>

auto message = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);
if (!message) {
    return;
}
```

An embedded reference example is available in `examples/platformio-esp32`.

---

## Build from Source

### Requirements

- CMake 3.20+
- C++20 compiler:
  - GCC 10+
  - Clang 12+
  - MSVC 2019+
- Build system: Ninja, GNU Make, or Visual Studio
- Optional (development): GoogleTest, clang-tidy, cppcheck

### Build

```bash
git clone https://github.com/fliuzzi02/nmealib.git
cd nmealib

cmake --preset gcc-release
cmake --build --preset build-release
cmake --install out/build/gcc-release --prefix out/install/gcc-release
```

Artifacts are installed under:

`out/install/gcc-release`

---

## Documentation

- [Installation Guide](docs/INSTALLATION.md)
- [API Reference](docs/API.md)
- [Examples](docs/EXAMPLES.md)
- [Contributing](docs/CONTRIBUTING.md)

Companion usage repository: [using-nmealib](https://github.com/fliuzzi02/using-nmealib)

---

## Project Structure

```text
nmealib/
├── .github/                  # CI workflow files
├── app/                      # CLI entrypoint
├── docs/                     # Documentation
├── examples/                 # Usage examples (including PlatformIO)
├── include/
│   ├── nmealib.h             # Main umbrella header
│   └── nmealib/
│       ├── nmeaException.h   # Custom exception class
│       ├── message.h         # Base Message class and utilities
│       ├── nmea0183.h        # Base NMEA 0183 class and utilities
│       ├── nmea2000.h        # Base NMEA 2000 class and utilities
│       ├── detail/           # Internal shared parsing/error helpers
│       ├── nmea0183/         # NMEA 0183 message type headers
│       └── nmea2000/         # NMEA 2000 message type headers
├── scripts/                  # CI/dev utility scripts
├── src/                      # Library implementation and parsing logic
├── tests/                    # Unit tests
├── CMakeLists.txt            # Root CMake project configuration
├── CMakePresets.json         # Preset build/test configurations
├── README.md                 # This file
└── library.json              # PlatformIO package manifest
```

---

## Supported Protocols

### NMEA 0183
| Message | Notes |
|---|---|
| [APB](docs/PROTOCOL_SUPPORT.md) | Autopilot Sentence "B" |
| [DBT](docs/PROTOCOL_SUPPORT.md) | Depth Below Transducer |
| [DPT](docs/PROTOCOL_SUPPORT.md) | Depth of Water |
| [GGA](docs/PROTOCOL_SUPPORT.md) | Global Positioning System Fix Data |
| [GLL](docs/PROTOCOL_SUPPORT.md) | Geographic Position |
| [GSA](docs/PROTOCOL_SUPPORT.md) | GPS DOP and Active Satellites |
| [GSV](docs/PROTOCOL_SUPPORT.md) | GPS Satellites in View |
| [HDG](docs/PROTOCOL_SUPPORT.md) | Heading, Deviation and Variation |
| [HDM](docs/PROTOCOL_SUPPORT.md) | Heading, Magnetic |
| [MTW](docs/PROTOCOL_SUPPORT.md) | Mean Temperature of Water |
| [MWV](docs/PROTOCOL_SUPPORT.md) | Wind Speed and Angle |
| [RMA](docs/PROTOCOL_SUPPORT.md) | Recommended Minimum Specific Loran-C Data |
| [RMB](docs/PROTOCOL_SUPPORT.md) | Recommended Minimum Navigation Information |
| [RMC](docs/PROTOCOL_SUPPORT.md) | Recommended Minimum Navigation Data |
| [VHW](docs/PROTOCOL_SUPPORT.md) | Water Speed and Heading |
| [VLW](docs/PROTOCOL_SUPPORT.md) | Distance Traveled Through Water |
| [VTG](docs/PROTOCOL_SUPPORT.md) | Course Over Ground and Ground Speed |
| [VWR](docs/PROTOCOL_SUPPORT.md) | Relative Wind Speed and Angle |
| [XDR](docs/PROTOCOL_SUPPORT.md) | Transducer Measurements |
| [XTE](docs/PROTOCOL_SUPPORT.md) | Cross-Track Error, Measured |
| [ZDA](docs/PROTOCOL_SUPPORT.md) | Time and Date |

### NMEA 2000
| Message / Transport | Notes |
|---|---|
| [Single-frame messages](docs/PROTOCOL_SUPPORT.md) | Standard one-frame CAN payloads |
| [Fast-packet transport](docs/PROTOCOL_SUPPORT.md) | Multi-frame transport handling |
| [PGN 127250](docs/PROTOCOL_SUPPORT.md) | Vessel Heading |
| [PGN 128259](docs/PROTOCOL_SUPPORT.md) | Speed, Water Referenced |
| [PGN 130306](docs/PROTOCOL_SUPPORT.md) | Wind Data |


> Messages/PGNs not listed are currently considered **not implemented**.

---

## Compatibility

| Platform | Compiler | Standard |
|---|---|---|
| Linux | GCC 10+, Clang 12+ | C++20 |
| Windows | MSVC 2019+ | C++20 |
| macOS | Clang 12+ | C++20 |

PlatformIO support is also available through `library.json` with broad platform/framework declaration (`*`).
Target toolchains must support C++20.

---

## License

Licensed under **MIT**. See [LICENSE](LICENSE).

---

## Support

- Open an issue in this repository for bugs/feature requests.
- For usage questions, include sample input data and expected output.
