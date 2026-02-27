# API Reference

This page summarizes the public API surface. It is organized for GitHub Pages navigation and links to the relevant headers.

## Public headers

- `nmealib.h`
- `nmealib/nmealib.hpp`
- `nmealib/message.hpp`
- `nmealib/nmeaException.hpp`
- `nmealib/nmea0183.hpp`
- `nmealib/nmea2000.hpp`
- `nmealib/nmea0183/nmea0183Factory.hpp`
- `nmealib/nmea2000/nmea2000Factory.hpp`
- `nmealib/nmea2000/pgn129029.hpp`
- `nmealib/nmea0183/rmc.hpp`
- `nmealib/nmea0183/gga.hpp`
- `nmealib/nmea0183/gll.hpp`
- `nmealib/nmea0183/gsa.hpp`
- `nmealib/nmea0183/mwv.hpp`
- `nmealib/nmea0183/vhw.hpp`
- `nmealib/nmea0183/vtg.hpp`
- `nmealib/nmea0183/zda.hpp`

## Namespaces

- `nmealib`
- `nmealib::nmea0183`
- `nmealib::nmea2000`

## Core types

### `nmealib::Message`

- Base abstract message type
- Provides `clone()`, `serialize()`, and `validate()` virtual methods

### `nmealib::NmeaException`

- Base exception for library errors

## NMEA 0183

### `nmealib::nmea0183::Message0183`

- Generic NMEA 0183 sentence representation
- Provides talker ID, sentence type, payload, and checksum helpers

### `nmealib::nmea0183::Nmea0183Factory`

- Entry point for parsing raw NMEA 0183 sentences
- Returns a typed sentence when the sentence type is supported

### Sentence classes

- `RMC`
- `GGA`
- `GLL`
- `GSA`
- `MWV`
- `VHW`
- `VTG`
- `ZDA`

## NMEA 2000

### `nmealib::nmea2000::Message2000`

- Generic NMEA 2000 message representation
- Supports both single-frame (0-8 bytes) and fast-packet frames (up to 223 bytes)
- Accepts any valid 18-bit PGN (0x000000-0x3FFFF)
- Flexible input format parsing:
  - Canonical format: `"CANID:data"` (e.g., `"18FD1234:0102030405060708"`)
  - Comma-separated: `"0xCANID, 0xBB 0xCC ..."` (e.g., `"0x18FD1234, 0x01 0x02 0x03 ..."`)
  - Space-separated with 0x: `"0xCANID 0xBB 0xCC ..."` (e.g., `"0x18FD1234 0x01 0x02 0x03 ..."`)
  - Space-separated: `"CANID BB CC ..."` (e.g., `"18FD1234 01 02 03 ..."`)

### `nmealib::nmea2000::Nmea2000Factory`

- Entry point for parsing raw NMEA 2000 frames in multiple formats
- Automatically detects and normalizes input format
- Returns a typed PGN object when the PGN is supported

### PGN classes

- `PGN129029`

## Exceptions

- `TooLongSentenceException`
- `InvalidStartCharacterException`
- `NoChecksumException`
- `NoEndlineException`
- `NotEnoughFieldsException`
- `NotRMCException`
- `NotGGAException`
- `NotGLLException`
- `NotGSAException`
- `NotMWVException`
- `NotVHWException`
- `NotVTGException`
- `NotZDAException`
- `InvalidPgnException`
- `NotPGN129029Exception`

## Notes

## See also

- [Getting Started](Getting-Started.md)
- [Examples](Examples.md)
- [NMEA 0183 Guide](NMEA-0183-Guide.md)
- [Building and Testing](Building-and-Testing.md)
