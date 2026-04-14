# XTE API

Class `nmea0183.XTE` (Python) / `nmealib::nmea0183::XTE` (C++).

## Sentence Overview
Cross-track error and recommended steering direction (GPSD: XTE - Cross-Track Error, Measured).

## Creation
- Python direct:
```python
msg = nmealib.nmea0183.XTE(<fields...>)
```
- C++ direct:
```cpp
nmealib::nmea0183::XTE msg(/* fields */);
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
