# XTE API

Class `nmea0183.XTE` (Python) / `nmealib::nmea0183::XTE` (C++).

## Sentence Overview
Cross-track error and recommended steering direction (GPSD: XTE - Cross-Track Error, Measured).

## Creation
### C++
```cpp
// Direct constructor:
nmealib::nmea0183::XTE msg = nmealib::nmea0183::XTE(std::string talkerId,
        char status1,
        char status2,
        double crossTrackError,
        char steerDirection,
        char crossTrackUnits,
        std::optional<char> faaModeIndicator = std::nullopt
    );

// Factory method from raw data:
auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);
```

### Python
```python
# Direct constructor:
msg = nmealib.nmea0183.XTE(/* fields */)

# Factory method from raw data:
msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)
```

## Public Methods

### `get_cross_track_error` / `getCrossTrackError`

```cpp
double getCrossTrackError() const noexcept;
```

Returns the cross track error value parsed from this sentence.

### `get_cross_track_units` / `getCrossTrackUnits`

```cpp
char getCrossTrackUnits() const noexcept;
```

Returns the cross track units value parsed from this sentence.

### `get_faa_mode_indicator` / `getFaaModeIndicator`

```cpp
std::optional<char> getFaaModeIndicator() const noexcept;
```

Returns the faa mode indicator value parsed from this sentence.

### `get_status1` / `getStatus1`

```cpp
char getStatus1() const noexcept;
```

Returns the status1 value parsed from this sentence.

### `get_status2` / `getStatus2`

```cpp
char getStatus2() const noexcept;
```

Returns the status2 value parsed from this sentence.

### `get_steer_direction` / `getSteerDirection`

```cpp
char getSteerDirection() const noexcept;
```

Returns the steer direction value parsed from this sentence.

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

## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
- Sentence overview source: https://gpsd.gitlab.io/gpsd/NMEA.html
