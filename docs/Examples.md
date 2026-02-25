# Examples

Practical usage patterns for nmealib.

## Parse a sentence

```cpp
#include <nmealib/nmea0183/nmea0183Factory.hpp>

auto msg = nmealib::nmea0183::Nmea0183Factory::create(
    "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A\r\n");
```

## Parse a NMEA 2000 raw frame (multiple formats supported)

All of these formats are supported and produce the same result:

**Canonical format (CANID:data):**
```cpp
#include <nmealib/nmea2000/nmea2000Factory.hpp>

auto msg = nmealib::nmea2000::Nmea2000Factory::create(
    "18FD1234:0102030405060708");
```

**Comma-separated with 0x prefix:**
```cpp
auto msg = nmealib::nmea2000::Nmea2000Factory::create(
    "0x18FD1234, 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08");
```

**Space-separated with 0x prefix:**
```cpp
auto msg = nmealib::nmea2000::Nmea2000Factory::create(
    "0x18FD1234 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08");
```

**Space-separated without 0x prefix:**
```cpp
auto msg = nmealib::nmea2000::Nmea2000Factory::create(
    "18FD1234 01 02 03 04 05 06 07 08");
```

## Supports single-frame and fast-packet frames

The library handles both single-frame (0-8 bytes) and fast-packet frames (up to 223 bytes):

```cpp
// Single-frame (8 bytes)
auto single = nmealib::nmea2000::Nmea2000Factory::create(
    "18FD1234:0102030405060708");

// Fast-packet (223 bytes maximum)
auto fastPacket = nmealib::nmea2000::Nmea2000Factory::create(
    "18FD1234:000102030405060708090A0B0C0D0E0F101112131415...");  // up to 223 bytes

// Access message properties
std::cout << "PGN: 0x" << std::hex << msg->getPgn() << std::endl;
std::cout << "Frame size: " << (int)msg->getCanFrameLength() << " bytes" << std::endl;
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
