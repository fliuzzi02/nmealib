# RMB API

Class `nmea0183.RMB` (Python) / `nmealib::nmea0183::RMB` (C++).

## Sentence Overview
Recommended minimum navigation data toward an active destination waypoint (GPSD: RMB - Recommended Minimum Navigation Information).

## Creation
### C++
```cpp
// Direct constructor:
nmealib::nmea0183::RMB msg = nmealib::nmea0183::RMB(std::string talkerId,
        char status,
        double crossTrackErrorNm,
        char directionToSteer,
        std::string originWaypointId,
        std::string destinationWaypointId,
        double destinationLatitude,
        char destinationLatitudeHemisphere,
        double destinationLongitude,
        char destinationLongitudeHemisphere,
        double rangeToDestinationNm,
        double bearingToDestinationTrue,
        double destinationClosingVelocityKnots,
        char arrivalStatus,
        std::optional<char> faaModeIndicator = std::nullopt
    );

// Factory method from raw data:
auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);
```

### Python
```python
# Direct constructor:
msg = nmealib.nmea0183.RMB(/* fields */)

# Factory method from raw data:
msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)
```

## Public Methods

### `get_arrival_status` / `getArrivalStatus`

```cpp
char getArrivalStatus() const noexcept;
```

Returns the arrival status value parsed from this sentence.

### `get_bearing_to_destination_true` / `getBearingToDestinationTrue`

```cpp
double getBearingToDestinationTrue() const noexcept;
```

Returns the bearing to destination true value parsed from this sentence.

### `get_cross_track_error_nm` / `getCrossTrackErrorNm`

```cpp
double getCrossTrackErrorNm() const noexcept;
```

Returns the cross track error nm value parsed from this sentence.

### `get_destination_closing_velocity_knots` / `getDestinationClosingVelocityKnots`

```cpp
double getDestinationClosingVelocityKnots() const noexcept;
```

Returns the destination closing velocity knots value parsed from this sentence.

### `get_destination_latitude` / `getDestinationLatitude`

```cpp
double getDestinationLatitude() const noexcept;
```

Returns the destination latitude value parsed from this sentence.

### `get_destination_latitude_hemisphere` / `getDestinationLatitudeHemisphere`

```cpp
char getDestinationLatitudeHemisphere() const noexcept;
```

Returns the destination latitude hemisphere value parsed from this sentence.

### `get_destination_longitude` / `getDestinationLongitude`

```cpp
double getDestinationLongitude() const noexcept;
```

Returns the destination longitude value parsed from this sentence.

### `get_destination_longitude_hemisphere` / `getDestinationLongitudeHemisphere`

```cpp
char getDestinationLongitudeHemisphere() const noexcept;
```

Returns the destination longitude hemisphere value parsed from this sentence.

### `get_destination_waypoint_id` / `getDestinationWaypointId`

```cpp
std::string getDestinationWaypointId() const noexcept;
```

Returns the destination waypoint id value parsed from this sentence.

### `get_direction_to_steer` / `getDirectionToSteer`

```cpp
char getDirectionToSteer() const noexcept;
```

Returns the direction to steer value parsed from this sentence.

### `get_faa_mode_indicator` / `getFaaModeIndicator`

```cpp
std::optional<char> getFaaModeIndicator() const noexcept;
```

Returns the faa mode indicator value parsed from this sentence.

### `get_origin_waypoint_id` / `getOriginWaypointId`

```cpp
std::string getOriginWaypointId() const noexcept;
```

Returns the origin waypoint id value parsed from this sentence.

### `get_range_to_destination_nm` / `getRangeToDestinationNm`

```cpp
double getRangeToDestinationNm() const noexcept;
```

Returns the range to destination nm value parsed from this sentence.

### `get_status` / `getStatus`

```cpp
char getStatus() const noexcept;
```

Returns the status value parsed from this sentence.

### `get_string_content` / `getStringContent`

```cpp
std::string getStringContent(bool verbose) const noexcept override;
```

Returns a human-readable representation of the sentence content.

### `has_faa_mode_indicator` / `hasFaaModeIndicator`

```cpp
bool hasFaaModeIndicator() const noexcept;
```

Returns whether the optional field represented by this flag is present.

## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
- Sentence overview source: https://gpsd.gitlab.io/gpsd/NMEA.html
