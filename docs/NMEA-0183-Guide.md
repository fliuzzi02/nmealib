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
- `GSA` — DOP values and active satellites
- `MWV` — wind speed and angle
- `VTG` — track made good and ground speed
- `ZDA` — UTC time/date and local zone offset

`GSA` parsing supports both classic format and NMEA 4.1+ format with optional trailing `System ID`.
`MWV` parsing supports wind angle/reference, speed, units, and validity status.
`VTG` parsing supports both modern format and the older compact 5-field format.
`ZDA` parsing supports UTC time with date and local time zone fields.

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
#include <nmealib/nmea0183/nmea0183Factory.hpp>

auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);
```

Runtime dispatch:

- sentence type `RMC` → `RMC`
- sentence type `GGA` → `GGA`
- sentence type `GLL` → `GLL`
- sentence type `GSA` → `GSA`
- sentence type `MWV` → `MWV`
- sentence type `VTG` → `VTG`
- sentence type `ZDA` → `ZDA`
- otherwise → `Message0183`

## Example sentences

```text
$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W,A,V*6A\r\n
$GNGGA,062735.00,3150.788156,N,11711.922383,E,1,12,2.0,90.0,M,,M,,*55\r\n
$GNGLL,3150.788156,N,11711.922383,E,062735.00,A,A*76\r\n
$GNGSA,A,3,80,71,73,79,69,,,,,,,,1.83,1.09,1.47,1*37\r\n
$IIMWV,45.0,R,10.5,N,A*26\r\n
$GPVTG,220.86,T,,M,2.550,N,4.724,K,A*34\r\n
$GPZDA,160012.71,11,03,2004,-1,00*7D\r\n
```

## Exceptions you may encounter

- `TooLongSentenceException`
- `InvalidStartCharacterException`
- `NoChecksumException`
- sentence-specific parse exceptions: `NotRMCException`, `NotGGAException`, `NotGLLException`, `NotGSAException`, `NotMWVException`, `NotVTGException`, `NotZDAException`

## See also

- [API Reference](API-Reference.md)
- [Examples](Examples.md)
- [Building and Testing](Building-and-Testing.md)
