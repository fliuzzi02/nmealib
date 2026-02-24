# Building and Testing

Build and test instructions for nmealib.

## Prerequisites

- CMake 3.20+
- C++20 compiler
- Git

## Configure and build

```bash
git clone https://github.com/fliuzzi02/nmealib.git
cd nmealib
cmake --preset gcc-full
cmake --build out/build/gcc-full
```

## Run tests

Run all discovered tests:

```bash
ctest --test-dir out/build/gcc-full --output-on-failure
```

Run only NMEA 0183 labeled tests:

```bash
ctest --test-dir out/build/gcc-full -L nmea0183 --output-on-failure
```

List available tests:

```bash
ctest --test-dir out/build/gcc-full -N
```

## Run specific tests

Examples:

```bash
ctest --test-dir out/build/gcc-full -R RMC
ctest --test-dir out/build/gcc-full -R GGA
ctest --test-dir out/build/gcc-full -R GLL
ctest --test-dir out/build/gcc-full -R Message0183
```

## Build options

| Option | Default | Description |
|---|---|---|
| `ENABLE_TESTS` | `OFF` | Builds tests and enables CTest targets |
| `ENABLE_STATIC_ANALYSIS` | `OFF` | Enables clang-tidy/cppcheck tooling |

Manual configure example:

```bash
cmake -B build -DENABLE_TESTS=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
```

## Static analysis

```bash
cmake -B build -DENABLE_TESTS=ON -DENABLE_STATIC_ANALYSIS=ON
cmake --build build --target static-analysis
```

## CLI executable

After build, CLI is available at:

```text
out/build/gcc-full/app/nmealib-cli
```

Example run:

```bash
./out/build/gcc-full/app/nmealib-cli --nmea0183 $'$GNGLL,3150.788156,N,11711.922383,E,062735.00,A,A*76\r\n'
```

## Troubleshooting

### Build fails on language features

Confirm compiler supports C++20 and CMake config is using `cxx_std_20`.

### Parser throws start-character error

Sentence must start with `$` or `!`.

### Parser throws on checksum retrieval

`getChecksumStr()` throws `NoChecksumException` for checksum-free sentences.

## See also

- [Getting Started](Getting-Started.md)
- [Examples](Examples.md)
- [API Reference](API-Reference.md)
