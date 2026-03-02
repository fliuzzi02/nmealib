# Getting Started

## Requirements

- C++20 compiler
- CMake 3.20+
- Git

## Install from source

```bash
git clone https://github.com/fliuzzi02/nmealib.git
cd nmealib
cmake --preset gcc-full
cmake --build out/build/gcc-full
```

## Link in your project (CMake)

### Full library (recommended)

```cmake
add_subdirectory(path/to/nmealib)
target_link_libraries(your_app PRIVATE nmealib)
```

### Specific messages only (GGA + RMC)

```cmake
add_subdirectory(path/to/nmealib)
target_link_libraries(your_app PRIVATE
    nmealib0183_gga
    nmealib0183_rmc
)
```

### All NMEA0183 messages

```cmake
add_subdirectory(path/to/nmealib)
target_link_libraries(your_app PRIVATE nmealib0183)
```

### NMEA2000 only

```cmake
add_subdirectory(path/to/nmealib)
target_link_libraries(your_app PRIVATE nmealib2000)
```

## First parse

```cpp
#include <nmealib/nmea0183/nmea0183Factory.h>

auto msg = nmealib::nmea0183::Nmea0183Factory::create(
    "$GNGLL,3150.788156,N,11711.922383,E,062735.00,A,A*76\r\n");
```

### NMEA 2000 (multiple formats supported)

All of these formats are equivalent:

**Canonical format:**
```cpp
#include <nmealib/nmea2000/nmea2000Factory.h>

auto msg2000 = nmealib::nmea2000::Nmea2000Factory::create(
    "18FD1234:0102030405060708");
```

**Comma-separated with 0x prefix:**
```cpp
auto msg2000 = nmealib::nmea2000::Nmea2000Factory::create(
    "0x18FD1234, 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08");
```

**Space-separated with 0x prefix:**
```cpp
auto msg2000 = nmealib::nmea2000::Nmea2000Factory::create(
    "0x18FD1234 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08");
```

**Space-separated without 0x prefix:**
```cpp
auto msg2000 = nmealib::nmea2000::Nmea2000Factory::create(
    "18FD1234 01 02 03 04 05 06 07 08");
```

## Run the CLI


## Error handling


## Next steps

- [Examples](Examples.md)
- [API Reference](API-Reference.md)
- [NMEA 0183 Guide](NMEA-0183-Guide.md)
