# MTW API

Class `nmea0183.MTW` (Python) / `nmealib::nmea0183::MTW` (C++).

## Sentence Overview
Water temperature sentence in Celsius (GPSD: MTW - Mean Temperature of Water).

## Creation
- Python direct:
```python
msg = nmealib.nmea0183.MTW(<fields...>)
```
- C++ direct:
```cpp
nmealib::nmea0183::MTW msg(/* fields */);
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

### `get_string_content` / `getStringContent`

```cpp
std::string getStringContent(bool verbose) const noexcept override;
```

Returns a human-readable representation of the sentence content.

### `get_temperature` / `getTemperature`

```cpp
double getTemperature() const noexcept;
```

Returns the temperature value parsed from this sentence.

### `get_unit` / `getUnit`

```cpp
char getUnit() const noexcept;
```

Returns the unit value parsed from this sentence.

## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
- Sentence overview source: https://gpsd.gitlab.io/gpsd/NMEA.html
