# RMA API

Class `nmea0183.RMA` (Python) / `nmealib::nmea0183::RMA` (C++).

## Sentence Overview
Recommended minimum navigation information in legacy format (GPSD: RMA - Recommended Minimum Navigation Information).

## Creation
- Python direct:
```python
msg = nmealib.nmea0183.RMA(<fields...>)
```
- C++ direct:
```cpp
nmealib::nmea0183::RMA msg(/* fields */);
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

### `get_magnetic_variation` / `getMagneticVariation`

```cpp
double getMagneticVariation() const noexcept;
```

Returns the magnetic variation value parsed from this sentence.

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

### `get_time_difference_a` / `getTimeDifferenceA`

```cpp
double getTimeDifferenceA() const noexcept;
```

Returns the time difference a value parsed from this sentence.

### `get_time_difference_b` / `getTimeDifferenceB`

```cpp
double getTimeDifferenceB() const noexcept;
```

Returns the time difference b value parsed from this sentence.

### `get_track_made_good` / `getTrackMadeGood`

```cpp
double getTrackMadeGood() const noexcept;
```

Returns the track made good value parsed from this sentence.

### `get_variation_direction` / `getVariationDirection`

```cpp
char getVariationDirection() const noexcept;
```

Returns the variation direction value parsed from this sentence.

## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
- Sentence overview source: https://gpsd.gitlab.io/gpsd/NMEA.html
