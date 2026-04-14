# XDR API

Class `nmea0183.XDR` (Python) / `nmealib::nmea0183::XDR` (C++).

## Sentence Overview
Transducer measurements as typed sensor/value/unit/name groups (GPSD: XDR - Transducer Measurement).

## Creation
### C++
```cpp
// Direct constructor:
nmealib::nmea0183::XDR msg = nmealib::nmea0183::XDR(std::string talkerId, std::vector<TransducerMeasurement> transducers);

// Factory method from raw data:
auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);
```

### Python
```python
# Direct constructor:
msg = nmealib.nmea0183.XDR(/* fields */)

# Factory method from raw data:
msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)
```

## Public Methods

### `get_string_content` / `getStringContent`

```cpp
std::string getStringContent(bool verbose) const noexcept override;
```

Returns a human-readable representation of the sentence content.

### `get_transducer_count` / `getTransducerCount`

```cpp
std::size_t getTransducerCount() const noexcept;
```

Returns the transducer count value parsed from this sentence.

### `get_transducers` / `getTransducers`

```cpp
const std::vector<TransducerMeasurement>& getTransducers() const noexcept;
```

Returns the transducers value parsed from this sentence.

## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
- Sentence overview source: https://gpsd.gitlab.io/gpsd/NMEA.html
