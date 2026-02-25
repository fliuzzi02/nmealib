# nmealib

C++ library for working with NMEA messages.

## Highlights

- NMEA 0183 parsing and validation
- Typed sentence classes (`RMC`, `GGA`, `GLL`, `GSA`, `MWV`, `VTG`, `ZDA`)
- Factory-based parsing with `Nmea0183Factory`
- NMEA 2000 parsing with full CAN ID frame input
- Typed PGN class (`PGN129029`) via `Nmea2000Factory`
- CMake build, tests, and CLI

## Requirements

- C++20 compiler
- CMake 3.20+

## Build

```bash
cmake --preset gcc-full
cmake --build out/build/gcc-full
```

## Link options (CMake)

- `nmealib`: umbrella target (core + all NMEA0183 + NMEA2000 messages)
- `nmealib_nmea0183_all`: all NMEA0183 message libraries
- `nmealib_nmea0183_<msg>`: single-message libraries (`dbt`, `gga`, `gsa`, `gll`, `mwv`, `rmc`, `vtg`, `zda`)
- `nmealib_nmea2000_all`: all NMEA2000 message libraries
- `nmealib_nmea2000_pgn129029`: single PGN library

Example (only GGA + RMC):

```cmake
target_link_libraries(your_app PRIVATE
    nmealib_core
    nmealib_nmea0183_base
    nmealib_nmea0183_gga
    nmealib_nmea0183_rmc
)
```

Example (only PGN129029):

```cmake
target_link_libraries(your_app PRIVATE
    nmealib_core
    nmealib_nmea2000_base
    nmealib_nmea2000_pgn129029
)
```

## Quick start

```cpp
#include <nmealib/nmea0183/nmea0183Factory.hpp>

auto msg = nmealib::nmea0183::Nmea0183Factory::create(
    "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A\r\n");
```

NMEA 2000 (raw frame with CAN ID + payload bytes):

```cpp
#include <nmealib/nmea2000/nmea2000Factory.hpp>

auto msg2000 = nmealib::nmea2000::Nmea2000Factory::create(
    "18F80523 1D 01 23 45 67 FE DC BA 98 00 01 86 A0 12 00 04 E2 0C");
```

Compact (no spaces) format is also supported:

```cpp
auto msg2000Compact = nmealib::nmea2000::Nmea2000Factory::create(
    "18F805231D01234567FEDCBA98000186A0120004E20C");
```

## Documentation

- [Home](docs/Home.md)
- [Getting Started](docs/Getting-Started.md)
- [Building and Testing](docs/Building-and-Testing.md)
- [Examples](docs/Examples.md)
- [API Reference](docs/API-Reference.md)
- [NMEA 0183 Guide](docs/NMEA-0183-Guide.md)
- [Contributing](docs/Contributing.md)

## Roadmap


## Contributing

See [Contributing](docs/Contributing.md).

## License


## Support


