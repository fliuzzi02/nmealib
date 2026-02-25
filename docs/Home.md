# nmealib

Modern C++ library for working with NMEA messages.

## Highlights

- NMEA 0183 sentence parsing and validation
- Typed sentence classes (`RMC`, `GGA`, `GLL`, `GSA`, `MWV`, `VTG`, `ZDA`)
- Factory-based dispatch with `Nmea0183Factory`
- NMEA 2000 frame parsing with full CAN ID + payload
- Typed PGN support for `PGN129029` through `Nmea2000Factory`
- CMake-based build with tests and CLI

## Quick start

```cpp
#include <nmealib/nmea0183/nmea0183Factory.hpp>

auto msg = nmealib::nmea0183::Nmea0183Factory::create(
    "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A\r\n");
```

```cpp
#include <nmealib/nmea2000/nmea2000Factory.hpp>

auto msg2000 = nmealib::nmea2000::Nmea2000Factory::create(
    "18F80523 1D 01 23 45 67 FE DC BA 98 00 01 86 A0 12 00 04 E2 0C");
```

Compact raw format is supported too:

```cpp
auto msg2000Compact = nmealib::nmea2000::Nmea2000Factory::create(
    "18F805231D01234567FEDCBA98000186A0120004E20C");
```

## Documentation

- [Getting Started](Getting-Started.md)
- [Building and Testing](Building-and-Testing.md)
- [Examples](Examples.md)
- [API Reference](API-Reference.md)
- [NMEA 0183 Guide](NMEA-0183-Guide.md)
- [Contributing](Contributing.md)

## Status


## Roadmap


## Support


## License


