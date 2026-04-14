# GSV API

Class `nmea0183.GSV` (Python) / `nmealib::nmea0183::GSV` (C++).

## Sentence Overview
Satellites in view, including per-satellite elevation, azimuth, and SNR (GPSD: GSV - Satellites in view).

## Creation
- Python direct:
```python
msg = nmealib.nmea0183.GSV(<fields...>)
```
- C++ direct:
```cpp
nmealib::nmea0183::GSV msg(/* fields */);
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

### `get_satellite_count` / `getSatelliteCount`

```cpp
std::size_t getSatelliteCount() const noexcept;
```

Returns the satellite count value parsed from this sentence.

### `get_satellites` / `getSatellites`

```cpp
const std::vector<SatelliteInfo>& getSatellites() const noexcept;
```

Returns the satellites value parsed from this sentence.

### `get_satellites_in_view` / `getSatellitesInView`

```cpp
unsigned int getSatellitesInView() const noexcept;
```

Returns the satellites in view value parsed from this sentence.

### `get_sentence_number` / `getSentenceNumber`

```cpp
unsigned int getSentenceNumber() const noexcept;
```

Returns the sentence number value parsed from this sentence.

### `get_signal_id` / `getSignalId`

```cpp
std::optional<unsigned int> getSignalId() const noexcept;
```

Returns the signal id value parsed from this sentence.

### `get_string_content` / `getStringContent`

```cpp
std::string getStringContent(bool verbose) const noexcept override;
```

Returns a human-readable representation of the sentence content.

### `get_total_sentences` / `getTotalSentences`

```cpp
unsigned int getTotalSentences() const noexcept;
```

Returns the total sentences value parsed from this sentence.

### `has_signal_id` / `hasSignalId`

```cpp
bool hasSignalId() const noexcept;
```

Returns whether the optional field represented by this flag is present.

## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
- Sentence overview source: https://gpsd.gitlab.io/gpsd/NMEA.html
