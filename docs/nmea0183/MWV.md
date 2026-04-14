# MWV API

Class `nmea0183.MWV` (Python) / `nmealib::nmea0183::MWV` (C++).

## Sentence Overview
Wind speed and angle with reference (relative/true) and status (GPSD: MWV - Wind Speed and Angle).

## Creation
### C++
```cpp
// Direct constructor:
nmealib::nmea0183::MWV msg = nmealib::nmea0183::MWV(std::string talkerId,
        double windAngle,
        char reference,
        double windSpeed,
        char windSpeedUnits,
        char status
    );

// Factory method from raw data:
auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);
```

### Python
```python
# Direct constructor:
msg = nmealib.nmea0183.MWV(/* fields */)

# Factory method from raw data:
msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)
```

## Public Methods

### `get_reference` / `getReference`

```cpp
char getReference() const noexcept;
```

Returns the reference value parsed from this sentence.

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

### `get_wind_angle` / `getWindAngle`

```cpp
double getWindAngle() const noexcept;
```

Returns the wind angle value parsed from this sentence.

### `get_wind_speed` / `getWindSpeed`

```cpp
double getWindSpeed() const noexcept;
```

Returns the wind speed value parsed from this sentence.

### `get_wind_speed_units` / `getWindSpeedUnits`

```cpp
char getWindSpeedUnits() const noexcept;
```

Returns the wind speed units value parsed from this sentence.

## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
- Sentence overview source: https://gpsd.gitlab.io/gpsd/NMEA.html
