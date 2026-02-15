# nmealib Wiki

Welcome to the nmealib wiki! This documentation provides comprehensive guides, examples, and reference material for using the nmealib library.

## Quick Links

- **[Getting Started](Getting-Started.md)** - Installation and first steps
- **[API Reference](API-Reference.md)** - Detailed API documentation
- **[NMEA 0183 Guide](NMEA-0183-Guide.md)** - Understanding NMEA 0183 format
- **[Examples](Examples.md)** - Code examples and use cases
- **[Building and Testing](Building-and-Testing.md)** - Build instructions and testing guide
- **[Contributing](Contributing.md)** - How to contribute to the project

## What is nmealib?

nmealib is a modern C++20 library for parsing, validating, and manipulating NMEA messages. It provides:

- **Type-safe parsing** of NMEA 0183 sentences
- **Automatic validation** with checksum verification
- **Exception-based error handling** for robust applications
- **Modern C++ design** utilizing C++20 features
- **Comprehensive testing** ensuring reliability

## Supported Standards

### NMEA 0183 ✅
Full support for parsing and validating NMEA 0183 sentences, including:
- Message parsing and validation
- Checksum calculation and verification
- Talker ID and sentence type extraction
- Timestamp management
- Message serialization

### NMEA 2000 🚧
Planned for future releases.

## Use Cases

nmealib is ideal for:

- **Marine navigation applications** - Parse GPS and AIS data
- **Data logging systems** - Record and validate marine sensor data
- **Testing and debugging** - Validate NMEA message streams
- **Protocol bridges** - Convert between different protocols
- **Educational projects** - Learn about marine communication standards

## Getting Help

- Check the [FAQ](FAQ.md) for common questions
- Browse the [Examples](Examples.md) for code samples
- Report issues on [GitHub](https://github.com/fliuzzi02/nmealib/issues)
- Review the API documentation for detailed class and method information

## Version Information

**Current Version:** 0.1.0

See the [CHANGELOG](CHANGELOG.md) for version history and updates.

## License

This project is open source. See the [main repository](https://github.com/fliuzzi02/nmealib) for license details.
