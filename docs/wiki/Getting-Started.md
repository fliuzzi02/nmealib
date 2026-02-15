# Getting Started with nmealib

This guide will help you get started with nmealib, from installation to your first NMEA message parsing.

## Prerequisites

Before you begin, ensure you have:

- **C++ Compiler** with C++20 support:
  - GCC 10 or later
  - Clang 12 or later
  - MSVC 2019 or later
- **CMake** 3.20 or higher
- **Git** for cloning the repository

## Installation

### Step 1: Clone the Repository

```bash
git clone https://github.com/fliuzzi02/nmealib.git
cd nmealib
```

### Step 2: Configure the Build

Use CMake presets for an easy configuration:

```bash
cmake --preset gcc-full
```

Or configure manually:

```bash
cmake -B build -DENABLE_TESTS=ON
```

### Step 3: Build the Library

```bash
cmake --build out/build/gcc-full
```

Or for manual configuration:

```bash
cmake --build build
```

### Step 4: Verify Installation

Run the test suite to ensure everything is working:

```bash
ctest --test-dir out/build/gcc-full
```

You should see output indicating all tests passed:
```
100% tests passed, 0 tests failed out of 8
```

## Your First Program

Let's create a simple program that parses an NMEA sentence.

### Create a New File

Create a file called `my_first_parser.cpp`:

```cpp
#include "nmea0183/nmea0183.hpp"
#include <iostream>
#include <string>

int main() {
    // A sample NMEA 0183 GGA sentence (GPS Fix Data)
    std::string sentence = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n";
    
    try {
        // Parse the sentence
        auto msg = nmealib::nmea0183::Message0183::create(sentence);
        
        // Extract information
        std::cout << "Successfully parsed NMEA sentence!" << std::endl;
        std::cout << "  Talker ID: " << msg->getTalker() << std::endl;
        std::cout << "  Sentence Type: " << msg->getSentenceType() << std::endl;
        std::cout << "  Valid: " << (msg->validate() ? "Yes" : "No") << std::endl;
        
        // Get the checksum
        try {
            std::cout << "  Checksum: " << msg->getChecksumStr() << std::endl;
            std::cout << "  Calculated: " << msg->getCalculatedChecksumStr() << std::endl;
        } catch (const nmealib::nmea0183::NoChecksumException&) {
            std::cout << "  No checksum present" << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
```

### Compile and Run

If using the nmealib project structure:

```bash
# Add your file to app/CMakeLists.txt or create a new target
# Then rebuild
cmake --build out/build/gcc-full
```

Or compile manually:

```bash
g++ -std=c++20 my_first_parser.cpp \
    -I/path/to/nmealib/src \
    -L/path/to/nmealib/out/build/gcc-full/src/nmea0183 \
    -lnmealib_nmea0183 \
    -o my_first_parser

./my_first_parser
```

### Expected Output

```
Successfully parsed NMEA sentence!
  Talker ID: GP
  Sentence Type: GGA
  Valid: Yes
  Checksum: 47
  Calculated: 47
```

## Using the Command-Line Tool

nmealib includes a CLI tool for quick testing:

```bash
# Navigate to the build directory
cd out/build/gcc-full/app

# Parse a sentence (note: use $'...' for proper escape sequences in bash)
./nmealib-cli --nmea0183 $'$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n'
```

Output:
```
Raw:      $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
Payload:  GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,
Checksum: 47
Valid:    true
Timestamp: 2026-02-14 17:48:28
Serialized: $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
```

## Integrating into Your Project

### As a CMake Subdirectory

In your project's `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.20)
project(MyProject)

# Add nmealib
add_subdirectory(nmealib)

# Create your executable
add_executable(my_app main.cpp)

# Link against nmealib
target_link_libraries(my_app PRIVATE nmealib)

# Ensure C++20
target_compile_features(my_app PRIVATE cxx_std_20)
```

### Manual Integration

1. Copy the `src/` directory to your project
2. Add include paths in your build system
3. Compile and link the library files

## Common NMEA Sentences

Here are some common NMEA 0183 sentence types you might encounter:

- **GGA** - Global Positioning System Fix Data
- **RMC** - Recommended Minimum Navigation Information
- **GSA** - GPS DOP and Active Satellites
- **GSV** - GPS Satellites in View
- **VTG** - Track Made Good and Ground Speed
- **GLL** - Geographic Position - Latitude/Longitude

Example sentences you can test with:

```cpp
// GPS Fix Data
"$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n"

// Recommended Minimum
"$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A\r\n"

// Geographic Position
"$GPGLL,4807.038,N,01131.000,E,123519,A,*1D\r\n"
```

## Error Handling

nmealib uses exceptions for error handling. Always wrap parsing code in try-catch blocks:

```cpp
try {
    auto msg = nmealib::nmea0183::Message0183::create(sentence);
    // Process message...
} catch (const nmealib::nmea0183::TooLongSentenceException& e) {
    std::cerr << "Sentence too long: " << e.what() << std::endl;
} catch (const nmealib::nmea0183::InvalidStartCharacterException& e) {
    std::cerr << "Invalid start character: " << e.what() << std::endl;
} catch (const nmealib::nmea0183::NoEndlineException& e) {
    std::cerr << "Missing endline: " << e.what() << std::endl;
} catch (const nmealib::NmeaException& e) {
    std::cerr << "NMEA error: " << e.what() << std::endl;
} catch (const std::exception& e) {
    std::cerr << "Unexpected error: " << e.what() << std::endl;
}
```

## Next Steps

Now that you have nmealib installed and working:

1. Explore the [API Reference](API-Reference.md) for detailed class documentation
2. Check out more [Examples](Examples.md) for advanced usage
3. Learn about [NMEA 0183 Format](NMEA-0183-Guide.md) in detail
4. Read about [Building and Testing](Building-and-Testing.md) for development

## Troubleshooting

### Compiler Errors

**Problem:** "error: 'std::chrono' is not a namespace"
- **Solution:** Ensure you're using C++20 standard: `target_compile_features(target PRIVATE cxx_std_20)`

**Problem:** Linker errors about undefined references
- **Solution:** Make sure you're linking against `nmealib_nmea0183`: `target_link_libraries(target PRIVATE nmealib_nmea0183)`

### Runtime Errors

**Problem:** "sentence must end with <CR><LF>"
- **Solution:** Ensure your sentence string includes `\r\n` at the end. Use `$'...\r\n'` syntax in bash or proper escape sequences in your code.

**Problem:** "Invalid start character"
- **Solution:** NMEA sentences must start with `$` or `!` character.

## Getting Help

If you run into issues:

1. Check this guide and the [FAQ](FAQ.md)
2. Review the [Examples](Examples.md) for similar use cases
3. Search [existing issues](https://github.com/fliuzzi02/nmealib/issues) on GitHub
4. Create a new issue with:
   - Your compiler version
   - CMake version
   - Code snippet demonstrating the problem
   - Full error message
