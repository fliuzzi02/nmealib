# VLW API

Class `nmea0183.VLW` (Python) / `nmealib::nmea0183::VLW` (C++).

## Sentence Overview
Distance traveled through water and, in newer variants, over ground (GPSD: VLW - Distance Traveled through Water).

## Creation
### C++
```cpp
// Direct constructor:
nmealib::nmea0183::VLW msg = nmealib::nmea0183::VLW(std::string talkerId,
        double totalWaterDistanceNm,
        double waterDistanceSinceResetNm,
        std::optional<double> totalGroundDistanceNm = std::nullopt,
        std::optional<double> groundDistanceSinceResetNm = std::nullopt
    );

// Factory method from raw data:
auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);
```

### Python
```python
# Direct constructor:
msg = nmealib.nmea0183.VLW(/* fields */)

# Factory method from raw data:
msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)
```

## Public Methods

### `get_ground_distance_since_reset_nm` / `getGroundDistanceSinceResetNm`

```cpp
double getGroundDistanceSinceResetNm() const noexcept;
```

Returns the ground distance since reset nm value parsed from this sentence.

### `get_ground_distance_since_reset_unit` / `getGroundDistanceSinceResetUnit`

```cpp
char getGroundDistanceSinceResetUnit() const noexcept;
```

Returns the ground distance since reset unit value parsed from this sentence.

### `get_string_content` / `getStringContent`

```cpp
std::string getStringContent(bool verbose) const noexcept override;
```

Returns a human-readable representation of the sentence content.

### `get_total_ground_distance_nm` / `getTotalGroundDistanceNm`

```cpp
double getTotalGroundDistanceNm() const noexcept;
```

Returns the total ground distance nm value parsed from this sentence.

### `get_total_ground_distance_unit` / `getTotalGroundDistanceUnit`

```cpp
char getTotalGroundDistanceUnit() const noexcept;
```

Returns the total ground distance unit value parsed from this sentence.

### `get_total_water_distance_nm` / `getTotalWaterDistanceNm`

```cpp
double getTotalWaterDistanceNm() const noexcept;
```

Returns the total water distance nm value parsed from this sentence.

### `get_total_water_distance_unit` / `getTotalWaterDistanceUnit`

```cpp
char getTotalWaterDistanceUnit() const noexcept;
```

Returns the total water distance unit value parsed from this sentence.

### `get_water_distance_since_reset_nm` / `getWaterDistanceSinceResetNm`

```cpp
double getWaterDistanceSinceResetNm() const noexcept;
```

Returns the water distance since reset nm value parsed from this sentence.

### `get_water_distance_since_reset_unit` / `getWaterDistanceSinceResetUnit`

```cpp
char getWaterDistanceSinceResetUnit() const noexcept;
```

Returns the water distance since reset unit value parsed from this sentence.

### `has_ground_distance_data` / `hasGroundDistanceData`

```cpp
bool hasGroundDistanceData() const noexcept;
```

Returns whether the optional field represented by this flag is present.

## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
- Sentence overview source: https://gpsd.gitlab.io/gpsd/NMEA.html
