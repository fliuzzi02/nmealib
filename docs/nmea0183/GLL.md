# GLL API

Class `nmea0183.GLL` (Python) / `nmealib::nmea0183::GLL` (C++).

## Sentence Overview
Geographic latitude/longitude position with UTC time and validity/mode flags (GPSD: GLL - Geographic Position - Latitude/Longitude).

## Creation
- Python direct:
```python
msg = nmealib.nmea0183.GLL(<fields...>)
```
- C++ direct:
```cpp
nmealib::nmea0183::GLL msg(/* fields */);
```
- Python factory:
```python
msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)
```
- C++ factory:
```cpp
auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);
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
