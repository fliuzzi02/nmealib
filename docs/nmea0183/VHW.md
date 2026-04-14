# VHW API

Class `nmea0183.VHW` (Python) / `nmealib::nmea0183::VHW` (C++).

## Sentence Overview
Water-referenced vessel heading and speed in knots and km/h (GPSD: VHW - Water speed and heading).

## Creation
### C++
```cpp
// Direct constructor:
nmealib::nmea0183::VHW msg = nmealib::nmea0183::VHW(std::string talkerId,
        double headingTrue,
        double headingMagnetic,
        double speedKnots,
        double speedKph
    );

// Factory method from raw data:
auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);
```

### Python
```python
# Direct constructor:
msg = nmealib.nmea0183.VHW(/* fields */)

# Factory method from raw data:
msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)
```

## Public Methods

### `get_heading_magnetic` / `getHeadingMagnetic`

```cpp
double getHeadingMagnetic() const noexcept;
```

Returns the heading magnetic value parsed from this sentence.

### `get_heading_magnetic_type` / `getHeadingMagneticType`

```cpp
char getHeadingMagneticType() const noexcept;
```

Returns the heading magnetic type value parsed from this sentence.

### `get_heading_true` / `getHeadingTrue`

```cpp
double getHeadingTrue() const noexcept;
```

Returns the heading true value parsed from this sentence.

### `get_heading_true_type` / `getHeadingTrueType`

```cpp
char getHeadingTrueType() const noexcept;
```

Returns the heading true type value parsed from this sentence.

### `get_speed_knots` / `getSpeedKnots`

```cpp
double getSpeedKnots() const noexcept;
```

Returns the speed knots value parsed from this sentence.

### `get_speed_knots_type` / `getSpeedKnotsType`

```cpp
char getSpeedKnotsType() const noexcept;
```

Returns the speed knots type value parsed from this sentence.

### `get_speed_kph` / `getSpeedKph`

```cpp
double getSpeedKph() const noexcept;
```

Returns the speed kph value parsed from this sentence.

### `get_speed_kph_type` / `getSpeedKphType`

```cpp
char getSpeedKphType() const noexcept;
```

Returns the speed kph type value parsed from this sentence.

### `get_string_content` / `getStringContent`

```cpp
std::string getStringContent(bool verbose) const noexcept override;
```

Returns a human-readable representation of the sentence content.

## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
- Sentence overview source: https://gpsd.gitlab.io/gpsd/NMEA.html
