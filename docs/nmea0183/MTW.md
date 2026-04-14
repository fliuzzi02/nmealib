# MTW API

Class `nmea0183.MTW` (Python) / `nmealib::nmea0183::MTW` (C++).

## Sentence Overview
Water temperature sentence in Celsius (GPSD: MTW - Mean Temperature of Water).

## Creation
### C++
```cpp
// Direct constructor:
nmealib::nmea0183::MTW msg = nmealib::nmea0183::MTW(std::string talkerId,
        double temperature,
        char unit
    );

// Factory method from raw data:
auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);
```

### Python
```python
# Direct constructor:
msg = nmealib.nmea0183.MTW(/* fields */)

# Factory method from raw data:
msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)
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
