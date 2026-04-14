# GSA API

Class `nmea0183.GSA` (Python) / `nmealib::nmea0183::GSA` (C++).

## Sentence Overview
GNSS DOP and active satellites used for the current fix solution (GPSD: GSA - GPS DOP and active satellites).

## Creation
### C++
```cpp
// Direct constructor:
nmealib::nmea0183::GSA msg = nmealib::nmea0183::GSA(std::string talkerId,
        char selectionMode,
        unsigned int mode,
        std::array<unsigned int, 12> satelliteIds,
        double pdop,
        double hdop,
        double vdop,
        std::optional<unsigned int> systemId = std::nullopt
    );

// Factory method from raw data:
auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);
```

### Python
```python
# Direct constructor:
msg = nmealib.nmea0183.GSA(/* fields */)

# Factory method from raw data:
msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)
```

## Public Methods

### `get_hdop` / `getHdop`

```cpp
double getHdop() const noexcept;
```

Returns the hdop value parsed from this sentence.

### `get_mode` / `getMode`

```cpp
unsigned int getMode() const noexcept;
```

Returns the mode value parsed from this sentence.

### `get_pdop` / `getPdop`

```cpp
double getPdop() const noexcept;
```

Returns the pdop value parsed from this sentence.

### `get_satellite_id` / `getSatelliteId`

```cpp
unsigned int getSatelliteId(size_t index) const noexcept;
```

Returns the satellite id value parsed from this sentence.

### `get_satellite_ids` / `getSatelliteIds`

```cpp
std::array<unsigned int, 12> getSatelliteIds() const noexcept;
```

Returns the satellite ids value parsed from this sentence.

### `get_selection_mode` / `getSelectionMode`

```cpp
char getSelectionMode() const noexcept;
```

Returns the selection mode value parsed from this sentence.

### `get_string_content` / `getStringContent`

```cpp
std::string getStringContent(bool verbose) const noexcept override;
```

Returns a human-readable representation of the sentence content.

### `get_system_id` / `getSystemId`

```cpp
std::optional<unsigned int> getSystemId() const noexcept;
```

Returns the system id value parsed from this sentence.

### `get_vdop` / `getVdop`

```cpp
double getVdop() const noexcept;
```

Returns the vdop value parsed from this sentence.

### `has_system_id` / `hasSystemId`

```cpp
bool hasSystemId() const noexcept;
```

Returns whether the optional field represented by this flag is present.

## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
- Sentence overview source: https://gpsd.gitlab.io/gpsd/NMEA.html
