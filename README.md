# nmealib

A modern C++20 library for parsing and validating NMEA messages.

## Overview

`nmealib` currently focuses on NMEA 0183 and provides:

- Generic sentence parsing (`Message0183`)
- Typed sentence dispatch via `Nmea0183Factory`
- Sentence-specific classes for `RMC`, `GGA`, and `GLL`
- Checksum validation and serialization
- Exception-based error handling
- CLI utility and test suite

## Requirements

- C++20 compiler (GCC 10+, Clang 12+, MSVC 2019+)
- CMake 3.20+

## Build

```bash
git clone https://github.com/fliuzzi02/nmealib.git
cd nmealib
cmake --preset gcc-full
cmake --build out/build/gcc-full
```

## Testing

```bash
ctest --test-dir out/build/gcc-full --output-on-failure
ctest --test-dir out/build/gcc-full -L nmea0183 --output-on-failure
```

## Quick usage

### Parse with factory

```cpp
#include "nmea0183Factory.hpp"
#include "rmc.hpp"
#include <iostream>

int main() {
    const std::string raw = "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W,A,V*6A\r\n";

    auto msg = nmealib::nmea0183::Nmea0183Factory::create(raw);

    std::cout << msg->getTalker() << msg->getSentenceType() << "\n";
    std::cout << "Valid: " << std::boolalpha << msg->validate() << "\n";

    if (auto* rmc = dynamic_cast<nmealib::nmea0183::RMC*>(msg.get())) {
        std::cout << "SOG: " << rmc->getSpeedOverGround() << " kn\n";
    }
}
```

### Construct typed message from fields

```cpp
#include "gll.hpp"

nmealib::nmea0183::GLL gll(
    "GN",
    31.8464692667, 'N',
    117.1987063833, 'E',
    62735.0,
    'A', 'A'
);

std::string wire = gll.serialize();
```

## NMEA 0183 support

`Nmea0183Factory::create(...)` returns:

- `RMC` for `*RMC`
- `GGA` for `*GGA`
- `GLL` for `*GLL`
- `Message0183` for other sentence types

## CLI

After building:

```bash
./out/build/gcc-full/app/nmealib-cli --nmea0183 $'$GNGLL,3150.788156,N,11711.922383,E,062735.00,A,A*76\r\n'
```

## Error handling

Common exceptions:

- `TooLongSentenceException`
- `InvalidStartCharacterException`
- `NoChecksumException`
- `NotRMCException`
- `NotGGAException`
- `NotGLLException`

All derive from `nmealib::NmeaException`.

## Project structure

```text
nmealib/
├── app/
├── docs/wiki/
├── src/
│   ├── message.hpp
│   ├── nmeaException.hpp
│   └── nmea0183/
│       ├── nmea0183.hpp
│       ├── nmea0183Factory.hpp
│       ├── rmc.hpp
│       ├── gga.hpp
│       └── gll.hpp
├── tests/
└── CMakeLists.txt
```

## Documentation

- [Wiki Home](docs/wiki/Home.md)
- [Getting Started](docs/wiki/Getting-Started.md)
- [API Reference](docs/wiki/API-Reference.md)
- [Examples](docs/wiki/Examples.md)
- [Building and Testing](docs/wiki/Building-and-Testing.md)

## Roadmap

- [x] NMEA 0183 parsing/validation
- [x] Typed sentence support (`RMC`, `GGA`, `GLL`)
- [x] CLI support
- [ ] Additional NMEA 0183 sentence types
- [ ] NMEA 2000 support

## Contributing

See [Contributing Guide](docs/wiki/Contributing.md).

## Support

For issues and feature requests:

- https://github.com/fliuzzi02/nmealib/issues
