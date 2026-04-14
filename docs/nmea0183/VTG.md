# VTG API

Class `nmea0183.VTG` (Python) / `nmealib::nmea0183::VTG` (C++).

## Sentence Overview
Track made good and ground speed, including true/magnetic course and speed units (GPSD: VTG - Track made good and Ground speed).

## Creation
### C++
```cpp
// Direct constructor:
nmealib::nmea0183::VTG msg = nmealib::nmea0183::VTG(std::string talkerId,
        double courseOverGroundTrue,
        double courseOverGroundMagnetic,
        double speedOverGroundKnots,
        double speedOverGroundKph,
        std::optional<char> faaModeIndicator = std::nullopt,
        bool legacyFormat = false
    );

// Factory method from raw data:
auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);
```

### Python
```python
# Direct constructor:
msg = nmealib.nmea0183.VTG(/* fields */)

# Factory method from raw data:
msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)
```

## Public Methods

### `get_course_over_ground_magnetic` / `getCourseOverGroundMagnetic`

```cpp
double getCourseOverGroundMagnetic() const noexcept;
```

Returns the course over ground magnetic value parsed from this sentence.

### `get_course_over_ground_magnetic_type` / `getCourseOverGroundMagneticType`

```cpp
char getCourseOverGroundMagneticType() const noexcept;
```

Returns the course over ground magnetic type value parsed from this sentence.

### `get_course_over_ground_true` / `getCourseOverGroundTrue`

```cpp
double getCourseOverGroundTrue() const noexcept;
```

Returns the course over ground true value parsed from this sentence.

### `get_course_over_ground_true_type` / `getCourseOverGroundTrueType`

```cpp
char getCourseOverGroundTrueType() const noexcept;
```

Returns the course over ground true type value parsed from this sentence.

### `get_faa_mode_indicator` / `getFaaModeIndicator`

```cpp
std::optional<char> getFaaModeIndicator() const noexcept;
```

Returns the faa mode indicator value parsed from this sentence.

### `get_speed_over_ground_knots` / `getSpeedOverGroundKnots`

```cpp
double getSpeedOverGroundKnots() const noexcept;
```

Returns the speed over ground knots value parsed from this sentence.

### `get_speed_over_ground_knots_type` / `getSpeedOverGroundKnotsType`

```cpp
char getSpeedOverGroundKnotsType() const noexcept;
```

Returns the speed over ground knots type value parsed from this sentence.

### `get_speed_over_ground_kph` / `getSpeedOverGroundKph`

```cpp
double getSpeedOverGroundKph() const noexcept;
```

Returns the speed over ground kph value parsed from this sentence.

### `get_speed_over_ground_kph_type` / `getSpeedOverGroundKphType`

```cpp
char getSpeedOverGroundKphType() const noexcept;
```

Returns the speed over ground kph type value parsed from this sentence.

### `get_string_content` / `getStringContent`

```cpp
std::string getStringContent(bool verbose) const noexcept override;
```

Returns a human-readable representation of the sentence content.

### `has_faa_mode_indicator` / `hasFaaModeIndicator`

```cpp
bool hasFaaModeIndicator() const noexcept;
```

Returns whether the optional field represented by this flag is present.

### `is_legacy_format` / `isLegacyFormat`

```cpp
// C++ method: auto value = msg.isLegacyFormat(/* args if any */);
```

Returns the value provided by this API method.

## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
- Sentence overview source: https://gpsd.gitlab.io/gpsd/NMEA.html
