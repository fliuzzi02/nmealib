# DPT API

Class `nmea0183.DPT` (Python) / `nmealib::nmea0183::DPT` (C++).

## Sentence Overview
Depth of water relative to the transducer with transducer offset and optional max range scale (GPSD: DPT - Depth of Water).

## Creation
- Python direct:
```python
msg = nmealib.nmea0183.DPT(<fields...>)
```
- C++ direct:
```cpp
nmealib::nmea0183::DPT msg(/* fields */);
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

### `get_max_range_scale` / `getMaxRangeScale`

```cpp
double getMaxRangeScale() const noexcept;
```

Returns the max range scale value parsed from this sentence.

### `get_string_content` / `getStringContent`

```cpp
std::string getStringContent(bool verbose) const noexcept override;
```

Returns a human-readable representation of the sentence content.

### `get_transducer_offset` / `getTransducerOffset`

```cpp
double getTransducerOffset() const noexcept;
```

Returns the transducer offset value parsed from this sentence.

### `get_water_depth_meters` / `getWaterDepthMeters`

```cpp
double getWaterDepthMeters() const noexcept;
```

Returns the water depth meters value parsed from this sentence.

## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
- Sentence overview source: https://gpsd.gitlab.io/gpsd/NMEA.html
