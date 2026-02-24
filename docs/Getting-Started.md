# Getting Started with nmealib

This guide covers installation, first parse, and typed sentence access.

## Prerequisites

- C++20 compiler (GCC 10+, Clang 12+, MSVC 2019+)
- CMake 3.20+
- Git

## Build

```bash
git clone https://github.com/fliuzzi02/nmealib.git
cd nmealib
cmake --preset gcc-full
cmake --build out/build/gcc-full
```

## Verify with tests

```bash
ctest --test-dir out/build/gcc-full -L nmea0183 --output-on-failure
```

## First parse example

```cpp
#include <nmealib/nmea0183/nmea0183Factory.hpp>
#include <nmealib/nmea0183/gll.hpp>
#include <iostream>

int main() {
    const std::string sentence = "$GNGLL,3150.788156,N,11711.922383,E,062735.00,A,A*76\r\n";

    auto msg = nmealib::nmea0183::Nmea0183Factory::create(sentence);

    std::cout << msg->getTalker() << msg->getSentenceType() << "\n";
    std::cout << "Valid: " << std::boolalpha << msg->validate() << "\n";

    if (auto* gll = dynamic_cast<nmealib::nmea0183::GLL*>(msg.get())) {
        std::cout << "Lat: " << gll->getLatitude() << gll->getLatitudeDirection() << "\n";
        std::cout << "Lon: " << gll->getLongitude() << gll->getLongitudeDirection() << "\n";
    }
}
```

## Constructor-based creation

Construct typed messages from field values and serialize them back to wire format.

```cpp
#include <nmealib/nmea0183/gga.hpp>

nmealib::nmea0183::GGA gga(
    "GN", 62735.0,
    31.8464692667, 'N',
    117.1987063833, 'E',
    1, 12, 2.0,
    90.0, 'M',
    0.0, 'M',
    0.0, ""
);

std::string raw = gga.serialize();
```

## CLI quick check

```bash
./out/build/gcc-full/app/nmealib-cli --nmea0183 $'$GNGGA,062735.00,3150.788156,N,11711.922383,E,1,12,2.0,90.0,M,,M,,*55\r\n'
```

## Error handling

```cpp
try {
    auto msg = nmealib::nmea0183::Nmea0183Factory::create(sentence);
} catch (const nmealib::nmea0183::TooLongSentenceException& e) {
} catch (const nmealib::nmea0183::InvalidStartCharacterException& e) {
} catch (const nmealib::nmea0183::NoChecksumException& e) {
} catch (const nmealib::NmeaException& e) {
}
```

## Next steps

- [Examples](Examples.md)
- [API Reference](API-Reference.md)
- [NMEA 0183 Guide](NMEA-0183-Guide.md)
