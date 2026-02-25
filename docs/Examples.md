# Examples

Practical usage patterns for nmealib.

## Parse a sentence

```cpp
#include <nmealib/nmea0183/nmea0183Factory.hpp>

auto msg = nmealib::nmea0183::Nmea0183Factory::create(
    "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A\r\n");
```

## Parse a NMEA 2000 raw frame

```cpp
#include <nmealib/nmea2000/nmea2000Factory.hpp>

auto msg = nmealib::nmea2000::Nmea2000Factory::create(
    "18F80523 1D 01 23 45 67 FE DC BA 98 00 01 86 A0 12 00 04 E2 0C");
```

Compact (no spaces):

```cpp
auto msgCompact = nmealib::nmea2000::Nmea2000Factory::create(
    "18F805231D01234567FEDCBA98000186A0120004E20C");
```

## Typed PGN access (129029)

```cpp
auto base = nmealib::nmea2000::Nmea2000Factory::create(
    "18F80523 1D 01 23 45 67 FE DC BA 98 00 01 86 A0 12 00 04 E2 0C");

auto pgn = dynamic_cast<nmealib::nmea2000::PGN129029*>(base.get());
if (pgn != nullptr) {
    const auto lat = pgn->getLatitude();
    const auto lon = pgn->getLongitude();
    const auto alt = pgn->getAltitude();
}
```

## Typed sentence access


## Serialize a sentence


## Clone and compare


## Batch parsing


## See also

- [Getting Started](Getting-Started.md)
- [API Reference](API-Reference.md)
- [NMEA 0183 Guide](NMEA-0183-Guide.md)
