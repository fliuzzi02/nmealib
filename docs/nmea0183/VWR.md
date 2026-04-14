# VWR API

Class `nmea0183.VWR` (Python) / `nmealib::nmea0183::VWR` (C++).

## Sentence Overview
Relative wind speed and angle with multiple speed units (GPSD: VWR - Relative Wind Speed and Angle).

## Creation
### C++
```cpp
// Direct constructor:
nmealib::nmea0183::VWR msg = nmealib::nmea0183::VWR(std::string talkerId,
        double windAngle,
        char windSide,
        double speedKnots,
        double speedMps,
        double speedKph
    );

// Factory method from raw data:
auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);
```

### Python
```python
# Direct constructor:
msg = nmealib.nmea0183.VWR(/* fields */)

# Factory method from raw data:
msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)
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
