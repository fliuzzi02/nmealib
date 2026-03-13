# nmealib

<p align="center">
    <img src="docs/nmealib.png" alt="nmealib logo" width="240"/>
</p>

[![License: GPL-3.0](https://img.shields.io/github/license/fliuzzi02/nmealib)](LICENSE)
[![Release](https://img.shields.io/github/v/release/fliuzzi02/nmealib)](https://github.com/fliuzzi02/nmealib/releases)
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

## Supported Protocols

### NMEA 0183
| Message | Implemented | Tested| Notes |
|---|---|---|---|
| [DBT](docs/PROTOCOL_SUPPORT.md#dbt--depth-below-transducer) | ✅ Yes | ✅ Yes | Depth Below Transducer |
| [GGA](docs/PROTOCOL_SUPPORT.md#gga--global-positioning-system-fix-data) | ✅ Yes | ✅ Yes | Global Positioning System Fix Data |
| [GLL](docs/PROTOCOL_SUPPORT.md#gll--geographic-position---latitudelongitude) | ✅ Yes | ✅ Yes | Geographic Position |
| [GSA](docs/PROTOCOL_SUPPORT.md#gsa--gps-dop-and-active-satellites) | ✅ Yes | ✅ Yes | GPS DOP and Active Satellites |
| [MTW](docs/PROTOCOL_SUPPORT.md#mtw--mean-temperature-of-water) | ✅ Yes | ✅ Yes | Mean Temperature of Water |
| [MWV](docs/PROTOCOL_SUPPORT.md#mwv--wind-speed-and-angle) | ✅ Yes | ✅ Yes | Wind Speed and Angle |
| [RMC](docs/PROTOCOL_SUPPORT.md#rmc--recommended-minimum-navigation-information) | ✅ Yes | ✅ Yes | Recommended Minimum Navigation Data |
| [VHW](docs/PROTOCOL_SUPPORT.md#vhw--water-speed-and-heading) | ✅ Yes | ✅ Yes | Water Speed and Heading |
| [VTG](docs/PROTOCOL_SUPPORT.md#vtg--track-made-good-and-ground-speed) | ✅ Yes | ✅ Yes | Course Over Ground and Ground Speed |
| [VWR](docs/PROTOCOL_SUPPORT.md#vwr--relative-wind-speed-and-angle) | ✅ Yes | ✅ Yes | Relative Wind Speed and Angle |
| [ZDA](docs/PROTOCOL_SUPPORT.md#zda--time--date-utc-day-month-year-local-time-zone) | ✅ Yes | ✅ Yes | Time and Date |

### NMEA 2000
| Message / Transport | Implemented | Tested | Notes |
|---|---|---|---|
| [Single-frame messages](docs/PROTOCOL_SUPPORT.md#nmea-2000-single-frame-messages) | ✅ Yes | ❌ No | Standard one-frame CAN payloads |
| [Fast-packet transport](docs/PROTOCOL_SUPPORT.md#nmea-2000-fast-packet-transport) | ✅ Yes | ❌ No | Multi-frame transport handling ||

> Messages/PGNs not listed are currently considered **not implemented**.

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
    } catch (const nmealib::ParseException& e) {
        std::cerr << "Failed to parse sentence: " << e.what() << std::endl;
    }
    return 0;
}
```

See [using-nmealib](https://github.com/fliuzzi02/using-nmealib) for a full example project demonstrating library usage.

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

### Run Tests

```bash
cmake --preset gcc-release-tests
cmake --build --preset build-release-tests
ctest --preset build-release-tests
```

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
├── include/
│   ├── nmealib.h             # Main umbrella header
│   └── nmealib/
│       ├── nmeaExceptions.h  # Custom exception classes
│       ├── message.h         # Base Message class and utilities
│       ├── nmea0183.h        # Base NMEA 0183 class and utilities
│       ├── nmea2000.h        # Base NMEA 2000 class and utilities
│       ├── nmea0183/         # NMEA 0183 message type headers
│       └── nmea2000/         # NMEA 2000 message type headers
├── src/                      # Library implementation and parsing logic
├── tests/                    # Unit tests
├── scripts/                  # CI/dev utility scripts
└── docs/                     # Documentation
```

---

## Compatibility

| Platform | Compiler | Standard |
|---|---|---|
| Linux | GCC 10+, Clang 12+ | C++20 |
| Windows | MSVC 2019+ | C++20 |
| macOS | Clang 12+ | C++20 |

---

## License

Licensed under **GPL-3.0**. See [LICENSE](LICENSE).

---

## Support

- Open an issue in this repository for bugs/feature requests.
- For usage questions, include sample input data and expected output.
