# ZDA API

Class `nmea0183.ZDA` (Python) / `nmealib::nmea0183::ZDA` (C++).

## Sentence Overview
UTC time and date plus local zone offset fields (GPSD: ZDA - Time & Date - UTC, day, month, year and local time zone).

## Creation
- Python direct:
```python
msg = nmealib.nmea0183.ZDA(<fields...>)
```
- C++ direct:
```cpp
nmealib::nmea0183::ZDA msg(/* fields */);
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

### `get_day` / `getDay`

```cpp
unsigned int getDay() const noexcept;
```

Returns the day value parsed from this sentence.

### `get_local_zone_hours` / `getLocalZoneHours`

```cpp
int getLocalZoneHours() const noexcept;
```

Returns the local zone hours value parsed from this sentence.

### `get_local_zone_minutes` / `getLocalZoneMinutes`

```cpp
int getLocalZoneMinutes() const noexcept;
```

Returns the local zone minutes value parsed from this sentence.

### `get_month` / `getMonth`

```cpp
unsigned int getMonth() const noexcept;
```

Returns the month value parsed from this sentence.

### `get_string_content` / `getStringContent`

```cpp
std::string getStringContent(bool verbose) const noexcept override;
```

Returns a human-readable representation of the sentence content.

### `get_utc_time` / `getUtcTime`

```cpp
double getUtcTime() const noexcept;
```

Returns the utc time value parsed from this sentence.

### `get_year` / `getYear`

```cpp
unsigned int getYear() const noexcept;
```

Returns the year value parsed from this sentence.

## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
- Sentence overview source: https://gpsd.gitlab.io/gpsd/NMEA.html
