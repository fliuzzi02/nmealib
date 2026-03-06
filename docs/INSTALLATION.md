# Installation

## Prerequisites

## Build Requirements

## Installation from Source

### Cloning the Repository

### Building

### Testing the Build

## Installation as a Library

Add nmealib to your CMakeLists.txt using FetchContent to download the pre-compiled release:

```cmake
include(FetchContent)

# Fetch the pre-compiled nmealib binary release
set(NMEALIB_VERSION "0.3.1")
FetchContent_Declare(
    nmealib
    URL "https://github.com/fliuzzi02/nmealib/releases/download/v${NMEALIB_VERSION}/nmealib-${NMEALIB_VERSION}-linux64.tar.gz"
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(nmealib)

# Add the nmealib cmake directory to the prefix path so find_package can locate it
list(APPEND CMAKE_PREFIX_PATH "${nmealib_SOURCE_DIR}/lib/cmake")

find_package(nmealib REQUIRED)
target_link_libraries(your_target PRIVATE nmealib::nmealib)
```

Then include and use the library:

```cpp
#include <nmealib.h>

auto msg = nmealib::nmea0183::Nmea0183Factory::create("$GPGGA,...*47\r\n");
```s

## Verifying Installation

## Troubleshooting

