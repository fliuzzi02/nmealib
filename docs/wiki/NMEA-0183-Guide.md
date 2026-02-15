# NMEA 0183 Format Guide

A comprehensive guide to understanding the NMEA 0183 standard and how nmealib handles it.

## What is NMEA 0183?

NMEA 0183 is a combined electrical and data specification for communication between marine electronics such as echo sounders, sonars, anemometers, gyrocompasses, autopilots, GPS receivers, and many other types of instruments.

The standard defines:
- **Electrical interface** - RS-232/RS-422 serial communication
- **Data format** - ASCII text-based sentence structure
- **Sentence types** - Standardized message formats for different data types

## Sentence Structure

### Basic Format

```
$TALKER_ID,data_field_1,data_field_2,...,data_field_n*CHECKSUM\r\n
```

### Components

#### 1. Start Character
- **Symbol:** `$` or `!`
- **Usage:**
  - `$` - Standard field-delimited messages
  - `!` - Messages with special encapsulation
- **Example:** `$GPGGA...`

#### 2. Talker ID (2 characters)
Identifies the source of the data:

| ID | Description |
|----|-------------|
| `GP` | GPS (Global Positioning System) |
| `GL` | GLONASS |
| `GA` | Galileo |
| `GN` | Combined GNSS (multiple systems) |
| `BD` | BeiDou |
| `II` | Integrated Instrumentation |
| `AI` | AIS (Automatic Identification System) |
| `SD` | Depth Sounder |
| `WI` | Weather Instrument |
| `HC` | Heading Compass |

**Example:** `$GPGGA` - GPS sentence, `$GLGGA` - GLONASS sentence

#### 3. Sentence Type (3 characters)
Identifies the type of data:

| Type | Full Name | Description |
|------|-----------|-------------|
| `GGA` | Global Positioning System Fix Data | Position, time, fix quality |
| `RMC` | Recommended Minimum Navigation Information | Essential navigation data |
| `GSA` | GPS DOP and Active Satellites | Dilution of precision, active satellites |
| `GSV` | GPS Satellites in View | Detailed satellite information |
| `VTG` | Track Made Good and Ground Speed | Course and speed information |
| `GLL` | Geographic Position - Latitude/Longitude | Position and time |
| `HDT` | Heading True | True heading |
| `HDG` | Heading Deviation and Variation | Magnetic heading with corrections |
| `DBT` | Depth Below Transducer | Water depth |
| `MWV` | Wind Speed and Angle | Wind information |

#### 4. Data Fields
- **Separator:** Comma `,`
- **Format:** ASCII text, varies by sentence type
- **Empty fields:** Represented by two consecutive commas `,,`
- **No spaces:** Unless part of the data

**Example:**
```
$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
       └─┬──┘ └───┬───┘ │ └────┬────┘ │ │ └┬┘ └──┬──┘ │ └─┬─┘ │ ││
         │        │      │      │       │ │  │    │    │   │   │ ││
      Time    Latitude  N/S  Longitude E/W Fix Sats Alt Units Geoid Empty
```

#### 5. Checksum (optional)
- **Symbol:** `*` followed by two hex digits
- **Calculation:** XOR of all characters between `$` and `*` (exclusive)
- **Format:** Uppercase hexadecimal (00-FF)

**Example:** `*47`

**Calculation Example:**
```
Sentence: $GPGGA,123519*XX
Payload:  GPGGA,123519

XOR calculation:
G ^ P ^ G ^ G ^ A ^ , ^ 1 ^ 2 ^ 3 ^ 5 ^ 1 ^ 9 = checksum
```

#### 6. Terminator
- **Sequence:** `\r\n` (Carriage Return + Line Feed)
- **Required:** Always present at the end
- **Bytes:** 0x0D 0x0A

### Constraints

1. **Maximum Length:** 82 characters (including `$` and `\r\n`)
2. **Character Set:** ASCII 7-bit (printable characters)
3. **Case Sensitivity:** Generally uppercase, but parsers should be case-insensitive for talker/sentence IDs
4. **Timing:** Messages should be sent at regular intervals

## Common Sentence Examples

### GGA - GPS Fix Data

```
$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n
```

**Fields:**
1. `123519` - Time: 12:35:19 UTC
2. `4807.038,N` - Latitude: 48°07.038' N
3. `01131.000,E` - Longitude: 11°31.000' E
4. `1` - Fix quality (0=invalid, 1=GPS, 2=DGPS)
5. `08` - Number of satellites in use
6. `0.9` - Horizontal dilution of precision
7. `545.4,M` - Altitude above mean sea level (meters)
8. `46.9,M` - Height of geoid above WGS84 ellipsoid (meters)
9. Empty - Time since last DGPS update
10. Empty - DGPS reference station ID
11. `*47` - Checksum

### RMC - Recommended Minimum

```
$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A\r\n
```

**Fields:**
1. `123519` - Time: 12:35:19 UTC
2. `A` - Status: A=Active (valid), V=Void (invalid)
3. `4807.038,N` - Latitude: 48°07.038' N
4. `01131.000,E` - Longitude: 11°31.000' E
5. `022.4` - Speed over ground (knots)
6. `084.4` - Track angle (degrees true)
7. `230394` - Date: 23 March 1994
8. `003.1,W` - Magnetic variation: 3.1° West
9. `*6A` - Checksum

### GLL - Geographic Position

```
$GPGLL,4807.038,N,01131.000,E,123519,A,*1D\r\n
```

**Fields:**
1. `4807.038,N` - Latitude: 48°07.038' N
2. `01131.000,E` - Longitude: 11°31.000' E
3. `123519` - Time: 12:35:19 UTC
4. `A` - Status: A=Active, V=Void
5. `*1D` - Checksum

### GSA - Satellite Data

```
$GPGSA,A,3,04,05,,09,12,,,24,,,,,2.5,1.3,2.1*39\r\n
```

**Fields:**
1. `A` - Mode: A=Automatic, M=Manual
2. `3` - Fix type: 1=No fix, 2=2D, 3=3D
3. `04,05,,09,12,,,24` - PRNs of satellites used (up to 12)
4. `2.5` - PDOP (Position dilution of precision)
5. `1.3` - HDOP (Horizontal DOP)
6. `2.1` - VDOP (Vertical DOP)
7. `*39` - Checksum

## Coordinate Formats

### Latitude
- **Format:** `DDMM.MMM` (Degrees + Minutes with decimals)
- **Direction:** `N` (North) or `S` (South)
- **Example:** `4807.038,N` = 48° 07.038' North

**Conversion to Decimal Degrees:**
```
Degrees = DD + (MM.MMM / 60)
4807.038 → 48 + (07.038 / 60) = 48.1173°
```

### Longitude
- **Format:** `DDDMM.MMM` (Degrees + Minutes with decimals)
- **Direction:** `E` (East) or `W` (West)
- **Example:** `01131.000,E` = 11° 31.000' East

**Conversion to Decimal Degrees:**
```
Degrees = DDD + (MM.MMM / 60)
01131.000 → 11 + (31.000 / 60) = 11.5167°
```

## Checksum Calculation

The checksum is the XOR of all characters between `$` and `*` (exclusive).

### Algorithm

```cpp
uint8_t checksum = 0;
for (char c : payload) {  // payload = everything after $ and before *
    checksum ^= static_cast<uint8_t>(c);
}
// Convert to 2-digit hex string
```

### Example

For sentence: `$GPGGA,123519*XX`

```
Payload: GPGGA,123519

G (0x47) ^ P (0x50) = 0x17
0x17 ^ G (0x47) = 0x50
0x50 ^ G (0x47) = 0x17
0x17 ^ A (0x41) = 0x56
0x56 ^ , (0x2C) = 0x7A
0x7A ^ 1 (0x31) = 0x4B
0x4B ^ 2 (0x32) = 0x79
0x79 ^ 3 (0x33) = 0x4A
0x4A ^ 5 (0x35) = 0x7F
0x7F ^ 1 (0x31) = 0x4E
0x4E ^ 9 (0x39) = 0x77

Result: 0x77 → "77"
```

## Using nmealib with NMEA 0183

### Parsing

```cpp
#include "nmea0183/nmea0183.hpp"

// Parse a sentence
std::string sentence = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n";
auto msg = nmealib::nmea0183::Message0183::create(sentence);
```

### Validation

```cpp
// Check format validity
bool valid = msg->validate();  // Validates checksum if present

// Get individual components
char start = msg->getStartChar();        // '$'
std::string talker = msg->getTalker();   // "GP"
std::string type = msg->getSentenceType(); // "GGA"
```

### Checksum Verification

```cpp
try {
    std::string provided = msg->getChecksumStr();    // From message
    std::string calculated = msg->getCalculatedChecksumStr(); // Computed
    
    if (provided == calculated) {
        std::cout << "Checksum OK" << std::endl;
    }
} catch (const nmealib::nmea0183::NoChecksumException&) {
    // Message has no checksum
}
```

### Error Handling

nmealib throws specific exceptions for format errors:

```cpp
try {
    auto msg = Message0183::create(input);
} catch (const TooLongSentenceException&) {
    // Sentence > 82 characters
} catch (const InvalidStartCharacterException&) {
    // Doesn't start with $ or !
} catch (const NoEndlineException&) {
    // Missing \r\n terminator
}
```

## Best Practices

1. **Always validate input** before processing
2. **Handle missing checksums** gracefully
3. **Check fix quality** in GGA sentences before using position
4. **Verify status** in RMC sentences (A=valid, V=invalid)
5. **Use multiple sentence types** for redundancy
6. **Time synchronization** - UTC time in sentences
7. **Empty fields** - Don't assume all fields are populated

## Common Issues

### 1. Missing Terminator
**Problem:** Parser fails with "must end with <CR><LF>"
**Solution:** Ensure `\r\n` is present at the end

```cpp
// Correct
std::string sentence = "$GPGGA,...*47\r\n";

// In bash, use $'...'
./app --nmea0183 $'$GPGGA,...*47\r\n'
```

### 2. Checksum Mismatch
**Problem:** `validate()` returns false
**Solution:** Recalculate checksum or check for data corruption

```cpp
if (!msg->validate()) {
    std::cout << "Expected: " << msg->getCalculatedChecksumStr() << std::endl;
    std::cout << "Got: " << msg->getChecksumStr() << std::endl;
}
```

### 3. Sentence Too Long
**Problem:** TooLongSentenceException thrown
**Solution:** NMEA 0183 has 82-character limit, truncate or split data

## References

- [NMEA 0183 Standard](https://www.nmea.org/content/STANDARDS/NMEA_0183_Standard)
- [GPS NMEA Sentence Information](http://www.gpsinformation.org/dale/nmea.htm)
- [nmealib API Reference](API-Reference.md)

## See Also

- [Getting Started](Getting-Started.md)
- [API Reference](API-Reference.md)
- [Examples](Examples.md)
