# nmealib

<p align="center">
    <img src="docs/nmealib.png" alt="nmealib logo" width="240"/>
</p>

[![License: GPL-3.0](https://img.shields.io/github/license/fliuzzi02/nmealib)](https://github.com/fliuzzi02/nmealib/blob/main/LICENSE)
[![Release](https://img.shields.io/github/v/release/fliuzzi02/nmealib)](https://github.com/fliuzzi02/nmealib/releases)
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/fliuzzi02/library/nmealib.svg)](https://registry.platformio.org/libraries/fliuzzi02/nmealib)
[![PyPI version](https://badge.fury.io/py/nmealib.svg)](https://badge.fury.io/py/nmealib)
![C++20](https://img.shields.io/badge/C%2B%2B-20-blue)
[![Coverage Status](https://coveralls.io/repos/github/fliuzzi02/nmealib/badge.svg?branch=main)](https://coveralls.io/github/fliuzzi02/nmealib?branch=main)

Modern C++20 library for parsing **NMEA 0183** sentences and **NMEA 2000** CAN messages, with typed models, validation, and extensible message handling.

---

## Why nmealib

- Unified parsing for **NMEA 0183** and **NMEA 2000**
- Python and C++ libraries with consistent APIs
- Typed message classes for safer integrations
- Built-in validation (including NMEA 0183 checksum handling)
- Extensible architecture for new sentence/PGN support
- CLI tool for quick parsing and debugging
- Unit-tested codebase

---

## Quick Start

### CLI

Download the latest release from [GitHub Releases](https://github.com/fliuzzi02/nmealib/releases), then parse a sentence:

```bash
./nmealib-cli '$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47'
```

Parse from a file/pipe:

```bash
cat nmea_sentences.txt | ./nmealib-cli
```

### C++ Library

Add the library to your CMake project (see [Installation Guide](docs/INSTALLATION.md#installation-as-a-library)), then include and use it:

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

### Python Library (PyPI)

Install from PyPI:

```bash
python -m pip install nmealib
```

Quick usage example:

```python
import nmealib

raw_0183 = "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W,A,V*6A\r\n"
raw_2000 = "09F11260:342C71FF7FFF7FFD"

try:
    msg_0183 = nmealib.nmea0183.Nmea0183Factory.create(raw_0183)
    print(type(msg_0183).__name__)  # RMC
    print(msg_0183.get_latitude(), msg_0183.get_longitude())

    msg_2000 = nmealib.nmea2000.Nmea2000Factory.create(raw_2000)
    print(type(msg_2000).__name__)  # PGN127250
except nmealib.NmeaException as exc:
    print(f"Parse error: {exc}")
```


### Use in PlatformIO

The library is published on the PlatformIO registry:

https://registry.platformio.org/libraries/fliuzzi02/nmealib

Add the dependency in your `platformio.ini`:

```ini
lib_deps =
    fliuzzi02/nmealib
```

Or install it with the PlatformIO CLI:

```bash
pio pkg install --library "fliuzzi02/nmealib"
```

An embedded reference example is available in `examples/platformio-esp32`.

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

All messages/PGNs listed in the [Protocol Support](docs/PROTOCOL_SUPPORT.md) are currently implemented and supported.


> Messages/PGNs not listed are currently considered **not implemented**.

---

## License

Licensed under **MIT**. See [LICENSE](https://github.com/fliuzzi02/nmealib/blob/main/LICENSE).

---

## Support

- Open an issue in this repository for bugs/feature requests.
- For usage questions, include sample input data and expected output.
