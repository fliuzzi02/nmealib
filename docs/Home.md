# nmealib

Modern C++ library for working with NMEA messages.

## Highlights

- NMEA 0183 sentence parsing and validation
- Typed sentence classes (`RMC`, `GGA`, `GLL`, `GSA`, `MWV`, `VTG`, `ZDA`)
- Factory-based dispatch with `Nmea0183Factory`
- NMEA 2000 single-frame and fast-packet message support (up to 223 bytes)
- Flexible multi-format input parsing for NMEA 2000 frames
- Typed PGN support through `Nmea2000Factory`
- CMake-based build with tests and CLI

## Quick start

```cpp
#include <nmealib/nmea0183/nmea0183Factory.hpp>

auto msg = nmealib::nmea0183::Nmea0183Factory::create(
    "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A\r\n");
```

```cpp
#include <nmealib/nmea2000/nmea2000Factory.hpp>

// Multiple NMEA 2000 formats supported:
auto msg2000 = nmealib::nmea2000::Nmea2000Factory::create(
    "0x18FD1234, 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08");
```

All formats are supported:

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


