# GGA API

Class `nmea0183.GGA` (Python) / `nmealib::nmea0183::GGA` (C++).

## Sentence Overview
Global Positioning System fix data including time, position, fix quality, satellites, and altitude (GPSD: GGA - Global Positioning System Fix Data).

## Creation
### C++
```cpp
// Direct constructor:
nmealib::nmea0183::GGA msg = nmealib::nmea0183::GGA(std::string talkerId,
        double timestamp,
        double latitude,
        char latitudeDirection,
        double longitude,
        char longitudeDirection,
        unsigned int gpsQuality,
        unsigned int satellites,
        double hdop,
        double altitude,
        char altitudeUnits,
        double geoidalSeparation,
        char geoidalSeparationUnits,
        double dgpsAge,
        std::string dgpsReferenceStationId
    );

// Factory method from raw data:
auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);
```

### Python
```python
# Direct constructor:
msg = nmealib.nmea0183.GGA(/* fields */)

# Factory method from raw data:
msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)
```

## Public Methods

### `get_altitude` / `getAltitude`

```cpp
double getAltitude() const noexcept;
```

Returns the altitude value parsed from this sentence.

### `get_altitude_units` / `getAltitudeUnits`

```cpp
char getAltitudeUnits() const noexcept;
```

Returns the altitude units value parsed from this sentence.

### `get_dgps_age` / `getDgpsAge`

```cpp
double getDgpsAge() const noexcept;
```

Returns the dgps age value parsed from this sentence.

### `get_dgps_reference_station_id` / `getDgpsReferenceStationId`

```cpp
std::string getDgpsReferenceStationId() const noexcept;
```

Returns the dgps reference station id value parsed from this sentence.

### `get_geoidal_separation` / `getGeoidalSeparation`

```cpp
double getGeoidalSeparation() const noexcept;
```

Returns the geoidal separation value parsed from this sentence.

### `get_geoidal_separation_units` / `getGeoidalSeparationUnits`

```cpp
char getGeoidalSeparationUnits() const noexcept;
```

Returns the geoidal separation units value parsed from this sentence.

### `get_gps_quality` / `getGpsQuality`

```cpp
unsigned int getGpsQuality() const noexcept;
```

Returns the gps quality value parsed from this sentence.

### `get_hdop` / `getHdop`

```cpp
double getHdop() const noexcept;
```

Returns the hdop value parsed from this sentence.

### `get_latitude` / `getLatitude`

```cpp
double getLatitude() const noexcept;
```

Returns the latitude value parsed from this sentence.

### `get_latitude_direction` / `getLatitudeDirection`

```cpp
char getLatitudeDirection() const noexcept;
```

Returns the latitude direction value parsed from this sentence.

### `get_longitude` / `getLongitude`

```cpp
double getLongitude() const noexcept;
```

Returns the longitude value parsed from this sentence.

### `get_longitude_direction` / `getLongitudeDirection`

```cpp
char getLongitudeDirection() const noexcept;
```

Returns the longitude direction value parsed from this sentence.

### `get_satellites` / `getSatellites`

```cpp
unsigned int getSatellites() const noexcept;
```

Returns the satellites value parsed from this sentence.

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

## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
- Sentence overview source: https://gpsd.gitlab.io/gpsd/NMEA.html
