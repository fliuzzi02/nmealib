# DBT API

Class `nmea0183.DBT` (Python) / `nmealib::nmea0183::DBT` (C++).

## Sentence Overview
Depth below transducer, typically reported in feet, meters, and fathoms (GPSD: DBT - Depth below transducer).

## Creation
### C++
```cpp
// Direct constructor:
nmealib::nmea0183::DBT msg = nmealib::nmea0183::DBT(std::string talkerId,
        double depthFeet,
        char feetUnit,
        double depthMeters,
        char metersUnit,
        double depthFathoms,
        char fathomsUnit
    );

// Factory method from raw data:
auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);
```

### Python
```python
# Direct constructor:
msg = nmealib.nmea0183.DBT(/* fields */)

# Factory method from raw data:
msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)
```

## Public Methods

### `get_depth_fathoms` / `getDepthFathoms`

```cpp
double getDepthFathoms() const noexcept;
```

Returns the depth fathoms value parsed from this sentence.

### `get_depth_feet` / `getDepthFeet`

```cpp
double getDepthFeet() const noexcept;
```

Returns the depth feet value parsed from this sentence.

### `get_depth_meters` / `getDepthMeters`

```cpp
double getDepthMeters() const noexcept;
```

Returns the depth meters value parsed from this sentence.

### `get_fathoms_unit` / `getFathomsUnit`

```cpp
char getFathomsUnit() const noexcept;
```

Returns the fathoms unit value parsed from this sentence.

### `get_feet_unit` / `getFeetUnit`

```cpp
char getFeetUnit() const noexcept;
```

Returns the feet unit value parsed from this sentence.

### `get_meters_unit` / `getMetersUnit`

```cpp
char getMetersUnit() const noexcept;
```

Returns the meters unit value parsed from this sentence.

### `get_string_content` / `getStringContent`

```cpp
std::string getStringContent(bool verbose) const noexcept override;
```

Returns a human-readable representation of the sentence content.

## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
- Sentence overview source: https://gpsd.gitlab.io/gpsd/NMEA.html
