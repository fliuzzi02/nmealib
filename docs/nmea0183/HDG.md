# HDG API

Class `nmea0183.HDG` (Python) / `nmealib::nmea0183::HDG` (C++).

## Sentence Overview
Magnetic heading with deviation and variation components (GPSD: HDG - Heading - Deviation & Variation).

## Creation
- Python direct:
```python
msg = nmealib.nmea0183.HDG(<fields...>)
```
- C++ direct:
```cpp
nmealib::nmea0183::HDG msg(/* fields */);
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

### `get_deviation_direction` / `getDeviationDirection`

```cpp
char getDeviationDirection() const noexcept;
```

Returns the deviation direction value parsed from this sentence.

### `get_magnetic_deviation` / `getMagneticDeviation`

```cpp
double getMagneticDeviation() const noexcept;
```

Returns the magnetic deviation value parsed from this sentence.

### `get_magnetic_heading` / `getMagneticHeading`

```cpp
double getMagneticHeading() const noexcept;
```

Returns the magnetic heading value parsed from this sentence.

### `get_magnetic_variation` / `getMagneticVariation`

```cpp
double getMagneticVariation() const noexcept;
```

Returns the magnetic variation value parsed from this sentence.

### `get_string_content` / `getStringContent`

```cpp
std::string getStringContent(bool verbose) const noexcept override;
```

Returns a human-readable representation of the sentence content.

### `get_variation_direction` / `getVariationDirection`

```cpp
char getVariationDirection() const noexcept;
```

Returns the variation direction value parsed from this sentence.

## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
- Sentence overview source: https://gpsd.gitlab.io/gpsd/NMEA.html
