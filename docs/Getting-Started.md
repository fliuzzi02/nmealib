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

```cmake
add_subdirectory(path/to/nmealib)
target_link_libraries(your_app PRIVATE nmealib)
```

## First parse

```cpp
#include <nmealib/nmea0183/nmea0183Factory.hpp>

auto msg = nmealib::nmea0183::Nmea0183Factory::create(
    "$GNGLL,3150.788156,N,11711.922383,E,062735.00,A,A*76\r\n");
```

## Run the CLI


## Error handling


## Next steps

- [Examples](Examples.md)
- [API Reference](API-Reference.md)
- [NMEA 0183 Guide](NMEA-0183-Guide.md)
