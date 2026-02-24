# nmealib

A modern C++20 library for parsing and validating NMEA messages.

## Overview

`nmealib` currently focuses on NMEA 0183 and provides:

- Generic sentence parsing (`Message0183`)
- Typed sentence dispatch via `Nmea0183Factory`
- Sentence-specific classes for `RMC`, `GGA`, `GLL`, `GSA`, `MWV`, `VTG`, and `ZDA`
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
#include <nmealib/nmea0183/nmea0183Factory.hpp>
#include <nmealib/nmea0183/rmc.hpp>
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
#include <nmealib/nmea0183/gll.hpp>

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
- `GSA` for `*GSA`
- `MWV` for `*MWV`
- `VTG` for `*VTG`
- `ZDA` for `*ZDA`
- `Message0183` for other sentence types

`MWV` provides wind angle/reference, wind speed, units, and data validity status.

`GSA` supports both standard and NMEA 4.1+ forms:

- Standard: selection mode, fix mode, up to 12 satellite IDs, PDOP/HDOP/VDOP
- NMEA 4.1+: optional trailing `System ID` field before checksum

`VTG` supports both newer and older forms:

- Newer form: includes `T/M/N/K` unit markers and optional FAA mode indicator (NMEA 2.3+)
- Older form: compact 5-field payload without explicit unit markers

`ZDA` provides UTC time/date plus local timezone offset fields.

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
- `NotGSAException`
- `NotMWVException`
- `NotVTGException`
- `NotZDAException`

All derive from `nmealib::NmeaException`.

## Project structure

```text
nmealib/
├── app/
├── docs/wiki/
├── include/
│   └── nmealib/
│       ├── message.hpp
│       ├── nmeaException.hpp
│       ├── nmea0183.hpp
│       └── nmea0183/
│           ├── nmea0183Factory.hpp
│           ├── rmc.hpp
│           ├── gga.hpp
│           ├── gll.hpp
│           ├── gsa.hpp
│           ├── mwv.hpp
│           ├── vtg.hpp
│           └── zda.hpp
├── src/
│   ├── message.cpp
│   └── nmea0183/
│       ├── nmea0183.cpp
│       ├── nmea0183Factory.cpp
│       ├── messageRegistry.cpp
│       ├── rmc.cpp
│       ├── gga.cpp
│       ├── gll.cpp
│       ├── gsa.cpp
│       ├── mwv.cpp
│       ├── vtg.cpp
│       └── zda.cpp
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
- [x] Typed sentence support (`RMC`, `GGA`, `GLL`, `GSA`, `MWV`, `VTG`, `ZDA`)
- [x] CLI support
- [ ] Additional NMEA 0183 sentence types
- [ ] NMEA 2000 support

## Contributing

See [Contributing Guide](docs/wiki/Contributing.md).

## Support

For issues and feature requests:

- https://github.com/fliuzzi02/nmealib/issues
