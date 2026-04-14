# GLL API

Class `nmea0183.GLL` (Python) / `nmealib::nmea0183::GLL` (C++).

## Sentence Overview
Geographic latitude/longitude position with UTC time and validity/mode flags (GPSD: GLL - Geographic Position - Latitude/Longitude).

## Creation
### C++
```cpp
// Direct constructor:
nmealib::nmea0183::GLL msg = nmealib::nmea0183::GLL(std::string talkerId,
        double latitude,
        char latitudeDirection,
        double longitude,
        char longitudeDirection,
        double timestamp,
        char status,
        char modeIndicator
    );

// Factory method from raw data:
auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);
```

### Python
```python
# Direct constructor:
msg = nmealib.nmea0183.GLL(/* fields */)

# Factory method from raw data:
msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)
```

## Public Methods

### `get_latitude` / `getLatitude`

```cpp
double getLatitude() const noexcept;
```

Returns the latitude value parsed from this sentence.

### `get_latitude_direction` / `getLatitudeDirection`

```cpp
char getLatitudeDirection() const noexcept;
```

Returns the latitude direction value parsed from this sentence.

### `get_longitude` / `getLongitude`

```cpp
double getLongitude() const noexcept;
```

Returns the longitude value parsed from this sentence.

### `get_longitude_direction` / `getLongitudeDirection`

```cpp
char getLongitudeDirection() const noexcept;
```

Returns the longitude direction value parsed from this sentence.

### `get_mode_indicator` / `getModeIndicator`

```cpp
char getModeIndicator() const noexcept;
```

Returns the mode indicator value parsed from this sentence.

### `get_status` / `getStatus`

```cpp
char getStatus() const noexcept;
```

Returns the status value parsed from this sentence.

### `get_string_content` / `getStringContent`

```cpp
std::string getStringContent(bool verbose) const noexcept override;
```

Returns a human-readable representation of the sentence content.

### `get_utc_time` / `getUtcTime`

```cpp
double getUtcTime() const noexcept;
```

Returns the utc time value parsed from this sentence.

## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
- Sentence overview source: https://gpsd.gitlab.io/gpsd/NMEA.html
