# RMC API

Class `nmea0183.RMC` (Python) / `nmealib::nmea0183::RMC` (C++).

## Sentence Overview
Recommended minimum GNSS data with time, status, position, speed, course, and date (GPSD: RMC - Recommended Minimum Navigation Information).

## Creation
- Python direct:
```python
msg = nmealib.nmea0183.RMC(<fields...>)
```
- C++ direct:
```cpp
nmealib::nmea0183::RMC msg(/* fields */);
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

### `get_course_over_ground` / `getCourseOverGround`

```cpp
double getCourseOverGround() const noexcept;
```

Returns the course over ground value parsed from this sentence.

### `get_date` / `getDate`

```cpp
unsigned int getDate() const noexcept;
```

Returns the date value parsed from this sentence.

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

### `get_magnetic_variation` / `getMagneticVariation`

```cpp
double getMagneticVariation() const noexcept;
```

Returns the magnetic variation value parsed from this sentence.

### `get_magnetic_variation_direction` / `getMagneticVariationDirection`

```cpp
char getMagneticVariationDirection() const noexcept;
```

Returns the magnetic variation direction value parsed from this sentence.

### `get_mode_indicator` / `getModeIndicator`

```cpp
char getModeIndicator() const noexcept;
```

Returns the mode indicator value parsed from this sentence.

### `get_navigation_status` / `getNavigationStatus`

```cpp
char getNavigationStatus() const noexcept;
```

Returns the navigation status value parsed from this sentence.

### `get_speed_over_ground` / `getSpeedOverGround`

```cpp
double getSpeedOverGround() const noexcept;
```

Returns the speed over ground value parsed from this sentence.

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

### `get_utc_fix` / `getUtcFix`

```cpp
unsigned int getUtcFix() const noexcept;
```

Returns the utc fix value parsed from this sentence.

## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
- Sentence overview source: https://gpsd.gitlab.io/gpsd/NMEA.html
