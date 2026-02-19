# NMEA 0183 Guide

This guide explains the NMEA 0183 sentence format and how `nmealib` handles it.

## Sentence format

Canonical structure:

```text
$TTXXX,field1,field2,...*CS\r\n
```

Where:

- `$` or `!` is the start character
- `TT` is the 2-char talker ID
- `XXX` is the 3-char sentence type
- `CS` is optional 2-digit hexadecimal checksum
- `\r\n` is sentence terminator

## Common talker IDs

- `GP` GPS
- `GL` GLONASS
- `GA` Galileo
- `GB` BeiDou
- `GN` multi-constellation

## Supported typed sentences in nmealib

- `RMC` — recommended minimum navigation data
- `GGA` — fix data
- `GLL` — latitude/longitude + time/status

Other sentence types are parsed as generic `Message0183` objects.

## Validation rules used by nmealib

- Maximum sentence length: 82 chars
- Start char must be `$` or `!`
- If checksum exists, it must be valid hex
- `validate()` returns:
  - `true` when checksum is absent
  - `true` when checksum is present and matches
  - `false` when checksum is present and mismatched

## Coordinate conversion

NMEA stores coordinates as degrees+minutes:

- Latitude: `DDMM.MMMMMM`
- Longitude: `DDDMM.MMMMMM`

nmealib typed classes (`RMC`, `GGA`, `GLL`) expose latitude/longitude in decimal degrees.

Conversion from NMEA to decimal degrees:

$$
\text{degrees} = \lfloor x/100 \rfloor + \frac{\text{fmod}(x,100)}{60}
$$

## Parse flow in code

```cpp
#include "nmea0183Factory.hpp"

auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);
```

Runtime dispatch:

- sentence type `RMC` → `RMC`
- sentence type `GGA` → `GGA`
- sentence type `GLL` → `GLL`
- otherwise → `Message0183`

## Example sentences

```text
$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W,A,V*6A\r\n
$GNGGA,062735.00,3150.788156,N,11711.922383,E,1,12,2.0,90.0,M,,M,,*55\r\n
$GNGLL,3150.788156,N,11711.922383,E,062735.00,A,A*76\r\n
```

## Exceptions you may encounter

- `TooLongSentenceException`
- `InvalidStartCharacterException`
- `NoChecksumException`
- sentence-specific parse exceptions: `NotRMCException`, `NotGGAException`, `NotGLLException`

## See also

- [API Reference](API-Reference.md)
- [Examples](Examples.md)
- [Building and Testing](Building-and-Testing.md)
