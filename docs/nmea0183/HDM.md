# HDM API

Class `nmea0183.HDM` (Python) / `nmealib::nmea0183::HDM` (C++).

## Sentence Overview
Magnetic heading in degrees relative to magnetic north (GPSD: HDM - Heading - Magnetic).

## Creation
- Python direct:
```python
msg = nmealib.nmea0183.HDM(<fields...>)
```
- C++ direct:
```cpp
nmealib::nmea0183::HDM msg(/* fields */);
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

### `get_heading` / `getHeading`

```cpp
double getHeading() const noexcept;
```

Returns the heading value parsed from this sentence.

### `get_indicator` / `getIndicator`

```cpp
char getIndicator() const noexcept;
```

Returns the indicator value parsed from this sentence.

### `get_string_content` / `getStringContent`

```cpp
std::string getStringContent(bool verbose) const noexcept override;
```

Returns a human-readable representation of the sentence content.

## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
- Sentence overview source: https://gpsd.gitlab.io/gpsd/NMEA.html
