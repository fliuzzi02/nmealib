# VWR API

Class `nmea0183.VWR` (Python) / `nmealib::nmea0183::VWR` (C++).

## Sentence Overview
Relative wind speed and angle with multiple speed units (GPSD: VWR - Relative Wind Speed and Angle).

## Creation
- Python direct:
```python
msg = nmealib.nmea0183.VWR(<fields...>)
```
- C++ direct:
```cpp
nmealib::nmea0183::VWR msg(/* fields */);
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

### `get_speed_knots` / `getSpeedKnots`

```cpp
double getSpeedKnots() const noexcept;
```

Returns the speed knots value parsed from this sentence.

### `get_speed_knots_unit` / `getSpeedKnotsUnit`

```cpp
char getSpeedKnotsUnit() const noexcept;
```

Returns the speed knots unit value parsed from this sentence.

### `get_speed_kph` / `getSpeedKph`

```cpp
double getSpeedKph() const noexcept;
```

Returns the speed kph value parsed from this sentence.

### `get_speed_kph_unit` / `getSpeedKphUnit`

```cpp
char getSpeedKphUnit() const noexcept;
```

Returns the speed kph unit value parsed from this sentence.

### `get_speed_mps` / `getSpeedMps`

```cpp
double getSpeedMps() const noexcept;
```

Returns the speed mps value parsed from this sentence.

### `get_speed_mps_unit` / `getSpeedMpsUnit`

```cpp
char getSpeedMpsUnit() const noexcept;
```

Returns the speed mps unit value parsed from this sentence.

### `get_string_content` / `getStringContent`

```cpp
std::string getStringContent(bool verbose) const noexcept override;
```

Returns a human-readable representation of the sentence content.

### `get_wind_angle` / `getWindAngle`

```cpp
double getWindAngle() const noexcept;
```

Returns the wind angle value parsed from this sentence.

### `get_wind_side` / `getWindSide`

```cpp
char getWindSide() const noexcept;
```

Returns the wind side value parsed from this sentence.

## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
- Sentence overview source: https://gpsd.gitlab.io/gpsd/NMEA.html
