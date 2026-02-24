# nmealib

Modern C++ library for working with NMEA messages.

## Highlights

- NMEA 0183 sentence parsing and validation
- Typed sentence classes (`RMC`, `GGA`, `GLL`, `GSA`, `MWV`, `VTG`, `ZDA`)
- Factory-based dispatch with `Nmea0183Factory`
- CMake-based build with tests and CLI

## Quick start

```cpp
#include <nmealib/nmea0183/nmea0183Factory.hpp>

auto msg = nmealib::nmea0183::Nmea0183Factory::create(
    "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A\r\n");
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


