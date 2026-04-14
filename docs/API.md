# API Reference

This page documents the high-level API for both C++ and Python.

Naming convention:
- C++ uses `camelCase` (for example: `getType()`).
- Python uses `snake_case` (for example: `get_type()`).

## Core Module

### C++ entrypoint
- Include everything with: `#include <nmealib.h>`
- Example: `auto msg = nmealib::nmea0183::Nmea0183Factory::create("$GPGLL,4916.45,N,12311.12,W,225444,A,*1D\r\n");`

### Python entrypoint
- Import with: `import nmealib`
- Example: `msg = nmealib.nmea0183.Nmea0183Factory.create("$GPGLL,4916.45,N,12311.12,W,225444,A,*1D\r\n")`

## Core Classes

### `Message` / `Message`
Base class for every parsed message.

Common methods:
- `typeToString()` / `type_to_string()` - Convert message type enum to a string.
- `getType()` / `get_type()` - Return message type (`Unknown`, `NMEA0183`, `NMEA2000`).
- `getRawData()` / `get_raw_data()` - Return original raw payload.
- `getTimestamp()` / `get_timestamp()` - Return message timestamp.
- `clone()` / `clone()` - Deep-copy the message polymorphically.
- `serialize()` / `serialize()` - Return wire-format string.
- `validate()` / `validate()` - Validate frame/checksum/length rules.
### `Message0183` / `nmea0183.Message0183`
Base class for NMEA 0183 ASCII sentences.

Methods:
- `getStartChar()` / `get_start_char()` - Return start marker (`$` or `!`).
- `getTalker()` / `get_talker()` - Return 2-char talker ID.
- `getSentenceType()` / `get_sentence_type()` - Return 3-char sentence code.
- `getPayload()` / `get_payload()` - Return sentence payload.
- `getChecksumStr()` / `get_checksum_str()` - Return checksum from sentence (if present).
- `getCalculatedChecksumStr()` / `get_calculated_checksum_str()` - Return computed checksum.
- `getStringContent(verbose)` / `get_string_content(verbose)` - Return human-readable content.
- `convertNmeaCoordinateToDecimalDegrees()` / `convert_nmea_coordinate_to_decimal_degrees()` - Convert `ddmm.mmmm`/`dddmm.mmmm` to decimal degrees.
### `Message2000` / `nmea2000.Message2000`
Base class for NMEA 2000 CAN messages.

Methods:
- `getCanId()` / `get_can_id()` - Return 4-byte CAN identifier.
- `getCanFrame()` / `get_can_frame()` - Return payload bytes.
- `getCanFrameLength()` / `get_can_frame_length()` - Return payload length.
- `getPriority()` / `get_priority()` - Return CAN priority bits.
- `getDataPage()` / `get_data_page()` - Return DP bit.
- `getPDUFormat()` / `get_pdu_format()` - Return PDU format byte.
- `getPDUSpecific()` / `get_pdu_specific()` - Return PDU specific byte.
- `getSourceAddress()` / `get_source_address()` - Return source address.
- `getDestinationAddress()` / `get_destination_address()` - Return destination address.
- `getPgn()` / `get_pgn()` - Return extracted PGN.
- `getStringContent(verbose)` / `get_string_content(verbose)` - Return human-readable content.
## Factories

### `Nmea0183Factory` / `nmea0183.Nmea0183Factory`
Create typed NMEA 0183 message objects from raw strings.
- `create(raw, timestamp)` - Parse sentence and return concrete message type (`RMC`, `GGA`, ...). Example: `msg = nmealib.nmea0183.Nmea0183Factory.create(raw)`

### `Nmea2000Factory` / `nmea2000.Nmea2000Factory`
Create typed NMEA 2000 PGN objects from raw CAN strings.
- `create(raw, timestamp)` - Parse frame and return concrete PGN class (`PGN127250`, ...). Example: `msg = nmealib.nmea2000.Nmea2000Factory.create(raw)`

## Exceptions

### Common
- `NmeaException` - Base class for all library exceptions.

### NMEA 0183
- `TooLongSentenceException`
- `InvalidStartCharacterException`
- `NoChecksumException`
- `NoEndlineException`
- `NotEnoughFieldsException`
- `Not*Exception` sentence-specific errors (for example `NotRMCException`, `NotGGAException`)

### NMEA 2000
- `InvalidPgnException`
- `InvalidCanFrameException`
- `FrameTooLongException`

## Protocol-Specific API

- NMEA 0183 message classes are documented in [docs/nmea0183/README.md](nmea0183/README.md).
- NMEA 2000 PGN classes are documented in [docs/nmea2000/README.md](nmea2000/README.md).

