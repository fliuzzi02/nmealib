# Installation

## Prerequisites

- **Git** (to clone the repository).
- **CMake 3.20 or newer**.
- **C++20-compatible compiler**:
    - GCC 10+
    - Clang 12+
    - MSVC 2019+
- **A supported CMake generator/build backend**:
    - Ninja
    - GNU Make
    - Visual Studio

## Build Requirements

- The project is configured with **C++20** (`CMAKE_CXX_STANDARD 20`).
- Tests are optional and enabled by preset/option (`ENABLE_TESTS=ON`); test dependencies are fetched automatically via CMake `FetchContent`.
- Running tests requires **CTest** (provided by CMake) and first-time network access to download GoogleTest (`v1.14.0`).
- Static analysis is optional and enabled by preset/option (`ENABLE_STATIC_ANALYSIS=ON`):
    - `cppcheck` is used by the `static-analysis` target.
    - `clang-tidy` is used when available (build continues with warning if not installed).
    - `compile_commands.json` must be available (`-DCMAKE_EXPORT_COMPILE_COMMANDS=ON`).

## Installation from Source

### Cloning the Repository

```bash
git clone https://github.com/fliuzzi02/nmealib.git
cd nmealib
```

### Building

Multiple CMake presets are available for different build configurations.

To build the library for use in your project, use the Library Only release preset:

```bash
cmake --preset=gcc-release-lib-only
cmake --build --preset=build-release-lib-only
cmake --install out/build/gcc-release-lib-only --prefix out/install/gcc-release-lib-only
```

If you need also the CLI application, use the full release preset:

```bash
cmake --preset gcc-release
cmake --build --preset build-release
cmake --install out/build/gcc-release --prefix out/install/gcc-release
```

### Testing the Build

Test out the CLI application:

```bash
cd out/install/gcc-release/bin
./nmealib --version
```

## Installation as a Library

Add nmealib to your CMakeLists.txt using FetchContent to download the pre-compiled release:

```cmake
include(FetchContent)

# Fetch the pre-compiled nmealib binary release
set(NMEALIB_VERSION "X.X.X")
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
```

## Installation with PlatformIO

`nmealib` can be consumed as a PlatformIO library dependency.

```ini
[env:esp32]
platform = ...
...
lib_deps =
    ...
    fliuzzi02/nmealib
```

Then include the umbrella header from your firmware source:

```cpp
#include <nmealib.h>
```

