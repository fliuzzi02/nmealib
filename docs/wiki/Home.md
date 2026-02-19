# nmealib Wiki

Welcome to the `nmealib` wiki. This documentation covers parsing, validation, typed sentence handling, and development workflows.

## Quick Links

- [Getting Started](Getting-Started.md)
- [API Reference](API-Reference.md)
- [NMEA 0183 Guide](NMEA-0183-Guide.md)
- [Examples](Examples.md)
- [Building and Testing](Building-and-Testing.md)
- [Contributing](Contributing.md)

## What is nmealib?

`nmealib` is a modern C++20 library for NMEA processing with:

- Type-safe parsing via `Nmea0183Factory`
- Checksum-aware validation
- Sentence-specific classes for common sentence types
- Exception-based error handling
- Unit-tested behavior

## Current NMEA 0183 Support

### Core parser

- Generic `Message0183` parsing and validation
- Talker/sentence extraction
- Checksum calculation and verification

### Typed sentence support

- `RMC` (Recommended Minimum Navigation Information)
- `GGA` (Fix data)
- `GLL` (Geographic position)
- `GSA` (DOP and active satellites)

`Nmea0183Factory::create(...)` automatically returns typed objects (`RMC`, `GGA`, `GLL`, `GSA`) when recognized.

## Suggested reading order

1. [Getting Started](Getting-Started.md)
2. [Examples](Examples.md)
3. [API Reference](API-Reference.md)
4. [NMEA 0183 Guide](NMEA-0183-Guide.md)

## Version

Current project version: `0.1.0`.

## Repository

- Main repository: https://github.com/fliuzzi02/nmealib
- Issues: https://github.com/fliuzzi02/nmealib/issues
