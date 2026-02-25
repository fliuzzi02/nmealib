# Building and Testing

## Configure and build (preset)

```bash
cmake --preset gcc-full
cmake --build out/build/gcc-full
```

## Configure and build (manual)

```bash
cmake -B build -DENABLE_TESTS=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

## Run tests

```bash
ctest --test-dir out/build/gcc-full --output-on-failure
```

## Run labeled tests

```bash
ctest --test-dir out/build/gcc-full -L nmea0183 --output-on-failure
ctest --test-dir out/build/gcc-full -L nmea2000 --output-on-failure
```

## Test presets

```bash
ctest --preset test-nmea0183
ctest --preset test-nmea2000
ctest --preset test-all
```

## Build options

| Option | Default | Description |
|---|---|---|
| `ENABLE_TESTS` | `OFF` | Build tests and enable CTest targets |
| `ENABLE_STATIC_ANALYSIS` | `OFF` | Enable clang-tidy and cppcheck integration |

## Static analysis

```bash
cmake -B build -DENABLE_TESTS=ON -DENABLE_STATIC_ANALYSIS=ON
cmake --build build --target static-analysis
```

## CLI


## Troubleshooting


## See also

- [Getting Started](Getting-Started.md)
- [Examples](Examples.md)
- [API Reference](API-Reference.md)
