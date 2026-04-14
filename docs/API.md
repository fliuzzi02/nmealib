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

#### `typeToString` / `type_to_string`

```cpp
static std::string typeToString(Type t);
```

Converts a message type enum value to a readable string.

#### `getType` / `get_type`

```cpp
Type getType() const noexcept;
```

Returns the message type (`Unknown`, `NMEA0183`, or `NMEA2000`).

#### `getRawData` / `get_raw_data`

```cpp
const std::string& getRawData() const noexcept;
```

Returns the original raw message string.

#### `getTimestamp` / `get_timestamp`

```cpp
TimePoint getTimestamp() const noexcept;
```

Returns the timestamp attached to the message.

#### `clone` / `clone`

```cpp
virtual std::unique_ptr<Message> clone() const = 0;
```

Creates a polymorphic deep copy of the message.

#### `serialize` / `serialize`

```cpp
virtual std::string serialize() const = 0;
```

Serializes the message to its wire representation.

#### `validate` / `validate`

```cpp
virtual bool validate() const = 0;
```

Validates the message content according to protocol rules.

### `Message0183` / `nmea0183.Message0183`
Base class for NMEA 0183 ASCII sentences.

#### `getStartChar` / `get_start_char`

```cpp
char getStartChar() const noexcept;
```

Returns the sentence start marker (`$` or `!`).

#### `getTalker` / `get_talker`

```cpp
std::string getTalker() const noexcept;
```

Returns the 2-character talker ID.

#### `getSentenceType` / `get_sentence_type`

```cpp
std::string getSentenceType() const noexcept;
```

Returns the 3-character sentence code.

#### `getPayload` / `get_payload`

```cpp
std::string getPayload() const noexcept;
```

Returns the payload portion of the sentence.

#### `getChecksumStr` / `get_checksum_str`

```cpp
std::string getChecksumStr() const;
```

Returns the checksum included in the raw sentence.

#### `getCalculatedChecksumStr` / `get_calculated_checksum_str`

```cpp
std::string getCalculatedChecksumStr() const noexcept;
```

Returns the checksum computed from payload data.

#### `getStringContent` / `get_string_content`

```cpp
virtual std::string getStringContent(bool verbose) const noexcept;
```

Returns a human-readable representation of sentence data.

#### `convertNmeaCoordinateToDecimalDegrees` / `convert_nmea_coordinate_to_decimal_degrees`

```cpp
static double convertNmeaCoordinateToDecimalDegrees(const std::string& nmeaCoordinate);
```

Converts NMEA coordinate format (`ddmm.mmmm`/`dddmm.mmmm`) to decimal degrees.

### `Message2000` / `nmea2000.Message2000`
Base class for NMEA 2000 CAN messages.

#### `getCanId` / `get_can_id`

```cpp
const std::vector<uint8_t>& getCanId() const noexcept;
```

Returns the 4-byte CAN identifier.

#### `getCanFrame` / `get_can_frame`

```cpp
const std::vector<uint8_t>& getCanFrame() const noexcept;
```

Returns the raw CAN payload bytes.

#### `getCanFrameLength` / `get_can_frame_length`

```cpp
uint8_t getCanFrameLength() const noexcept;
```

Returns the payload length in bytes.

#### `getPriority` / `get_priority`

```cpp
uint8_t getPriority() const noexcept;
```

Returns the message priority bits from the CAN ID.

#### `getDataPage` / `get_data_page`

```cpp
bool getDataPage() const noexcept;
```

Returns the data page bit.

#### `getPDUFormat` / `get_pdu_format`

```cpp
uint8_t getPDUFormat() const noexcept;
```

Returns the PDU Format field.

#### `getPDUSpecific` / `get_pdu_specific`

```cpp
uint8_t getPDUSpecific() const noexcept;
```

Returns the PDU Specific field.

#### `getSourceAddress` / `get_source_address`

```cpp
uint8_t getSourceAddress() const noexcept;
```

Returns the source address.

#### `getDestinationAddress` / `get_destination_address`

```cpp
uint8_t getDestinationAddress() const noexcept;
```

Returns the destination address (or global for broadcast PGNs).

#### `getPgn` / `get_pgn`

```cpp
uint32_t getPgn() const noexcept;
```

Returns the extracted Parameter Group Number (PGN).

#### `getStringContent` / `get_string_content`

```cpp
virtual std::string getStringContent(bool verbose) const noexcept;
```

Returns a human-readable representation of frame content.

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

