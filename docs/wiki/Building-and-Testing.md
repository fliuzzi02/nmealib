# Building and Testing

Comprehensive guide to building, testing, and developing with nmealib.

## Building the Library

### Prerequisites

Ensure you have the required tools installed:

```bash
# Check CMake version (need 3.20+)
cmake --version

# Check compiler (GCC 10+, Clang 12+, or MSVC 2019+)
g++ --version
# or
clang++ --version

# Check Git
git --version
```

### Quick Build

```bash
# Clone the repository
git clone https://github.com/fliuzzi02/nmealib.git
cd nmealib

# Configure using preset
cmake --preset gcc-full

# Build
cmake --build out/build/gcc-full

# Run tests
ctest --test-dir out/build/gcc-full
```

### Manual Configuration

```bash
# Create build directory
mkdir build && cd build

# Configure with tests enabled
cmake .. -DENABLE_TESTS=ON -DCMAKE_BUILD_TYPE=Debug

# Build
cmake --build .

# Run tests
ctest
```

## Build Options

### CMake Variables

| Variable | Default | Description |
|----------|---------|-------------|
| `ENABLE_TESTS` | `OFF` | Enable building and running tests |
| `ENABLE_STATIC_ANALYSIS` | `OFF` | Enable clang-tidy and cppcheck |
| `CMAKE_BUILD_TYPE` | `Debug` | Build type (Debug, Release, RelWithDebInfo, MinSizeRel) |
| `CMAKE_CXX_STANDARD` | `20` | C++ standard version |
| `CMAKE_INSTALL_PREFIX` | system default | Installation directory |

### Example Configurations

#### Debug Build with Tests
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_TESTS=ON
cmake --build build
```

#### Release Build
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

#### Static Analysis Enabled
```bash
cmake -B build -DENABLE_STATIC_ANALYSIS=ON -DENABLE_TESTS=ON
cmake --build build
```

## CMake Presets

The project includes CMake presets for common configurations:

### Available Presets

#### gcc-full
Full-featured build with GCC:
```bash
cmake --preset gcc-full
```

**Configuration:**
- Compiler: GCC
- Build Type: Debug
- Tests: Enabled
- Static Analysis: Disabled
- Output: `out/build/gcc-full/`

### Test Presets

#### test-nmea0183
Run only NMEA 0183 tests:
```bash
ctest --preset test-nmea0183
```

#### test-nmea2000
Run only NMEA 2000 tests (when available):
```bash
ctest --preset test-nmea2000
```

#### test-all
Run all tests:
```bash
ctest --preset test-all
```

## Testing

### Running All Tests

```bash
# Using CTest
ctest --test-dir out/build/gcc-full

# Verbose output
ctest --test-dir out/build/gcc-full --verbose

# Show test output on failure
ctest --test-dir out/build/gcc-full --output-on-failure
```

### Running Specific Tests

```bash
# Run tests matching a pattern
ctest --test-dir out/build/gcc-full -R Message0183

# Run tests with specific label
ctest --test-dir out/build/gcc-full -L nmea0183

# List available tests
ctest --test-dir out/build/gcc-full -N
```

### Test Results

Expected output:
```
Test project /home/user/nmealib/out/build/gcc-full
    Start 1: Message0183.CreateSucceedsAndAccessors
1/8 Test #1: Message0183.CreateSucceedsAndAccessors ..................   Passed    0.00 sec
    Start 2: Message0183.CreateWithoutChecksumAndGetChecksumThrows
2/8 Test #2: Message0183.CreateWithoutChecksumAndGetChecksumThrows ...   Passed    0.00 sec
    [... more tests ...]
    
100% tests passed, 0 tests failed out of 8
```

### Running Tests Directly

```bash
# Run the test executable directly
./out/build/gcc-full/tests/nmea0183/nmea0183_tests

# With Google Test filters
./out/build/gcc-full/tests/nmea0183/nmea0183_tests --gtest_filter=Message0183.*

# List available tests
./out/build/gcc-full/tests/nmea0183/nmea0183_tests --gtest_list_tests
```

## Static Analysis

### Enabling Static Analysis

```bash
cmake -B build -DENABLE_STATIC_ANALYSIS=ON
cmake --build build --target static-analysis
```

### Tools Used

#### clang-tidy
Performs static code analysis and style checking.

**Checks enabled:**
- All standard checks
- Clang analyzer checks
- Excludes: `readability-function-cognitive-complexity`

#### cppcheck
Additional static analysis tool.

**Options:**
- Enable all checks
- Inconclusive results included
- C++20 standard
- Suppresses: `missingIncludeSystem`, `unusedFunction`

### Manual Static Analysis

```bash
# Run clang-tidy manually
clang-tidy src/**/*.cpp -- -std=c++20 -I src/

# Run cppcheck manually
cppcheck --enable=all --inconclusive --std=c++20 \
         --suppress=missingIncludeSystem \
         src/ tests/ app/
```

## Building the CLI Application

The CLI tool is built automatically with the main library.

### Locating the Executable

```bash
# After building
ls out/build/gcc-full/app/nmealib-cli

# Run it
./out/build/gcc-full/app/nmealib-cli -m $'$GPGGA,123519,...*47\r\n'
```

### Installing the CLI

```bash
# Install to system
cmake --install out/build/gcc-full

# Or specify custom prefix
cmake --install out/build/gcc-full --prefix /usr/local
```

## Development Workflow

### Setting Up Development Environment

```bash
# Clone repository
git clone https://github.com/fliuzzi02/nmealib.git
cd nmealib

# Configure for development
cmake --preset gcc-full

# Build
cmake --build out/build/gcc-full

# Run tests continuously during development
watch -n 1 'cmake --build out/build/gcc-full && ctest --test-dir out/build/gcc-full --output-on-failure'
```

### Adding New Tests

1. Create test file in `tests/` directory:

```cpp
// tests/nmea0183/test_new_feature.cpp
#include <gtest/gtest.h>
#include "nmea0183/nmea0183.hpp"

TEST(NewFeature, BasicTest) {
    // Your test code
    EXPECT_TRUE(true);
}
```

2. Update `tests/CMakeLists.txt` (if needed):

```cmake
# Tests are automatically discovered by Google Test
```

3. Rebuild and run tests:

```bash
cmake --build out/build/gcc-full
ctest --test-dir out/build/gcc-full
```

### Adding New Source Files

1. Create source files in `src/` directory
2. Update `src/CMakeLists.txt`:

```cmake
add_library(nmealib_nmea0183 STATIC
    nmea0183.cpp
    new_file.cpp  # Add your new file
)
```

3. Rebuild:

```bash
cmake --build out/build/gcc-full
```

## Cross-Platform Building

### Linux

```bash
cmake --preset gcc-full
cmake --build out/build/gcc-full
```

### macOS

```bash
# Use Clang (default on macOS)
cmake -B build -DCMAKE_CXX_COMPILER=clang++
cmake --build build
```

### Windows (Visual Studio)

```bash
# Configure for Visual Studio
cmake -B build -G "Visual Studio 17 2022"

# Build
cmake --build build --config Debug

# Run tests
ctest --test-dir build -C Debug
```

### Windows (MinGW)

```bash
# Configure for MinGW
cmake -B build -G "MinGW Makefiles"

# Build
cmake --build build

# Run tests
ctest --test-dir build
```

## Troubleshooting

### CMake Configuration Issues

**Problem:** "CMake version is too old"
```bash
# Solution: Upgrade CMake
sudo apt-get update
sudo apt-get install cmake
```

**Problem:** "Could not find C++20 compiler"
```bash
# Solution: Install modern compiler
sudo apt-get install g++-10
cmake -DCMAKE_CXX_COMPILER=g++-10 -B build
```

### Build Issues

**Problem:** Linker errors
```bash
# Solution: Clean and rebuild
rm -rf build
cmake --preset gcc-full
cmake --build out/build/gcc-full
```

**Problem:** Missing headers
```bash
# Solution: Ensure include paths are correct
cmake -B build -DCMAKE_VERBOSE_MAKEFILE=ON
cmake --build build
```

### Test Failures

**Problem:** Tests fail to run
```bash
# Solution: Check test output
ctest --test-dir out/build/gcc-full --verbose --output-on-failure
```

**Problem:** Google Test not found
```bash
# Solution: Enable network access for FetchContent
# Google Test is automatically downloaded during configuration
# Ensure you have internet access
```

## Performance Optimization

### Release Build

For production use, build in Release mode:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

**Benefits:**
- Full optimization (-O3)
- No debug symbols
- Smaller binary size
- Better runtime performance

### Link-Time Optimization

Enable LTO for additional optimization:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON
cmake --build build
```

## Installation

### System-Wide Installation

```bash
# Build in Release mode
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build

# Install (may require sudo)
sudo cmake --install build

# Default locations:
# - Library: /usr/local/lib/
# - Headers: /usr/local/include/nmealib/
# - Executable: /usr/local/bin/
```

### Custom Installation Path

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX=$HOME/.local

cmake --build build
cmake --install build
```

### Using in Your Project

After installation:

```cmake
# In your CMakeLists.txt
find_package(nmealib REQUIRED)
target_link_libraries(your_target PRIVATE nmealib::nmealib)
```

## Continuous Integration

### GitHub Actions Example

```yaml
name: Build and Test

on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    
    steps:
    - uses: actions/checkout@v2
    
    - name: Install dependencies
      run: |
        sudo apt-get update
        sudo apt-get install -y cmake g++
    
    - name: Configure
      run: cmake --preset gcc-full
    
    - name: Build
      run: cmake --build out/build/gcc-full
    
    - name: Test
      run: ctest --test-dir out/build/gcc-full --output-on-failure
```

## Benchmarking

### Simple Performance Test

```cpp
#include "nmea0183/nmea0183.hpp"
#include <chrono>
#include <iostream>

int main() {
    const std::string sentence = 
        "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n";
    
    const int iterations = 100000;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; ++i) {
        auto msg = nmealib::nmea0183::Message0183::create(sentence);
        msg->validate();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "Parsed " << iterations << " messages in " 
              << duration.count() << "ms" << std::endl;
    std::cout << "Average: " << (duration.count() * 1000.0 / iterations) 
              << " µs per message" << std::endl;
    
    return 0;
}
```

## See Also

- [Getting Started](Getting-Started.md)
- [API Reference](API-Reference.md)
- [Examples](Examples.md)
- [Contributing](Contributing.md)
