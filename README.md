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

## Library usage

### NMEA 0183

```cpp
#include <nmealib/nmea0183/nmea0183Factory.hpp>

auto msg = nmealib::nmea0183::Nmea0183Factory::create(
    "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A\r\n");
```

### NMEA 2000

Raw frame with CAN ID + payload bytes:

```cpp
#include <nmealib/nmea2000/nmea2000Factory.hpp>

auto msg2000 = nmealib::nmea2000::Nmea2000Factory::create(
    "18F80523 1D 01 23 45 67 FE DC BA 98 00 01 86 A0 12 00 04 E2 0C");
```

Compact raw format (no spaces) is also supported:

```cpp
auto msg2000Compact = nmealib::nmea2000::Nmea2000Factory::create(
    "18F805231D01234567FEDCBA98000186A0120004E20C");
```

## CLI usage

### Build CLI

```bash
cmake --preset gcc-debug-tests
cmake --build out/build/gcc-debug-tests --target nmealib-cli
```

### Protocol selection

- `-t=<type>` or `--type=<type>`
- Supported values:
    - `N0183`
    - `N2K`
- If omitted, default protocol is `N0183`.

### Parse one message

NMEA 0183:

```bash
./nmealib-cli -t=N0183 -m '$GNGGA,062735.00,3150.788156,N,11711.922383,E,1,12,2.0,90.0,M,,M,,*55\r\n'
```

NMEA 2000 (spaced raw frame):

```bash
./nmealib-cli -t=N2K -m '18F80523 1D 01 23 45 67 FE DC BA 98 00 01 86 A0 12 00 04 E2 0C'
```

NMEA 2000 (compact raw frame):

```bash
./nmealib-cli --type=N2K -m '18F805231D01234567FEDCBA98000186A0120004E20C'
```

### Parse stream from stdin

```bash
cat messages.txt | ./nmealib-cli -t=N0183
cat n2k_frames.txt | ./nmealib-cli -t=N2K
```

## NMEA 2000 disclaimer

NMEA 2000 support in this project is based on reverse engineering and publicly available field observations.
For full official and compliant NMEA 2000 support, you must obtain the appropriate license and documentation from the NMEA Association.

## Documentation

- [Home](docs/Home.md)
- [Getting Started](docs/Getting-Started.md)
- [Building and Testing](docs/Building-and-Testing.md)
- [Examples](docs/Examples.md)
- [API Reference](docs/API-Reference.md)
- [NMEA 0183 Guide](docs/NMEA-0183-Guide.md)
- [Contributing](docs/Contributing.md)

## Roadmap

- [x] NMEA0183 Support:
    - [x] Depth transducers sentencies
    - [x] Autopilot sentencies
    - [x] Water speed sentencies
    - [x] Wind sentencies
    - [x] Cross track error sentencies
- [x] NMEA2000 Support:
    - [x] TBD, possibly all sentencies that can be translated to/from supporten N0183 ones
- [x] Documentation
- [x] PlatformIO Release
- [x] Windows support
- [x] MAcOS support
- [x] Full linux support across distros


## Contributing

See [Contributing](docs/Contributing.md).

## License

This project is licensed under the GNU GPLv3 (or later). See [LICENSE](LICENSE).


## Support


