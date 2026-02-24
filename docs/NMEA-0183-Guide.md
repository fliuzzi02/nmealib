# NMEA 0183 Guide

## Overview

NMEA 0183 is an ASCII sentence protocol used by marine instruments. nmealib provides a parser and typed sentence classes for a subset of common sentence types.

## Sentence structure

```text
$TTXXX,field1,field2,...*CS\r\n
```

- `$` or `!` start character
- `TT` talker ID
- `XXX` sentence type
- `CS` optional 2-hex-digit checksum
- `\r\n` terminator

## Parsing with nmealib

```cpp
#include <nmealib/nmea0183/nmea0183Factory.hpp>

auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);
```

## Supported typed sentences

- `RMC`
- `GGA`
- `GLL`
- `GSA`
- `MWV`
- `VTG`
- `ZDA`

## Validation behavior


## Coordinate conversion

NMEA stores coordinates as degrees and minutes:

- Latitude: `DDMM.MMMMMM`
- Longitude: `DDDMM.MMMMMM`

Conversion to decimal degrees:

$$
\operatorname{degrees} = \lfloor x/100 \rfloor + \frac{\operatorname{fmod}(x,100)}{60}
$$

## Example sentences


## See also

- [API Reference](API-Reference.md)
- [Examples](Examples.md)
- [Building and Testing](Building-and-Testing.md)
