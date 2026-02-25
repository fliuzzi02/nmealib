# nmealib

C++ library for working with NMEA messages.

## Highlights

- NMEA 0183 parsing and validation
- Typed sentence classes (`RMC`, `GGA`, `GLL`, `GSA`, `MWV`, `VTG`, `ZDA`)
- Factory-based parsing with `Nmea0183Factory`
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

- `nmealib`: umbrella target (core + all NMEA0183 messages)
- `nmealib_nmea0183_all`: all NMEA0183 message libraries
- `nmealib_nmea0183_<msg>`: single-message libraries (`dbt`, `gga`, `gsa`, `gll`, `mwv`, `rmc`, `vtg`, `zda`)

Example (only GGA + RMC):

```cmake
target_link_libraries(your_app PRIVATE
    nmealib_core
    nmealib_nmea0183_base
    nmealib_nmea0183_gga
    nmealib_nmea0183_rmc
)
```

## Quick start

```cpp
#include <nmealib/nmea0183/nmea0183Factory.hpp>

auto msg = nmealib::nmea0183::Nmea0183Factory::create(
    "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A\r\n");
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


