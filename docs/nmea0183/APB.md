# APB API

Class `nmea0183.APB` (Python) / `nmealib::nmea0183::APB` (C++).

## Sentence Overview
Autopilot sentence with cross-track error, steering direction, waypoint arrival status, and bearings for navigation guidance (GPSD: APB - Autopilot Sentence "B").

## Creation
### C++
```cpp
// Direct constructor:
nmealib::nmea0183::APB msg = nmealib::nmea0183::APB(std::string talkerId,
        char status1,
        char status2,
        double crossTrackErrorMagnitude,
        char directionToSteer,
        char crossTrackUnits,
        char arrivalCircleStatus,
        char perpendicularPassedAtWaypoint,
        double bearingOriginToDestination,
        char bearingOriginToDestinationType,
        std::string destinationWaypointId,
        double bearingPresentToDestination,
        char bearingPresentToDestinationType,
        double headingToSteerToDestinationWaypoint,
        char headingToSteerToDestinationWaypointType
    );

// Factory method from raw data:
auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);
```

### Python
```python
# Direct constructor:
msg = nmealib.nmea0183.APB(/* fields */)

# Factory method from raw data:
msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)
```

## Public Methods

### `get_arrival_circle_status` / `getArrivalCircleStatus`

```cpp
char getArrivalCircleStatus() const noexcept;
```

Returns the arrival circle status value parsed from this sentence.

### `get_bearing_origin_to_destination` / `getBearingOriginToDestination`

```cpp
double getBearingOriginToDestination() const noexcept;
```

Returns the bearing origin to destination value parsed from this sentence.

### `get_bearing_origin_to_destination_type` / `getBearingOriginToDestinationType`

```cpp
char getBearingOriginToDestinationType() const noexcept;
```

Returns the bearing origin to destination type value parsed from this sentence.

### `get_bearing_present_to_destination` / `getBearingPresentToDestination`

```cpp
double getBearingPresentToDestination() const noexcept;
```

Returns the bearing present to destination value parsed from this sentence.

### `get_bearing_present_to_destination_type` / `getBearingPresentToDestinationType`

```cpp
char getBearingPresentToDestinationType() const noexcept;
```

Returns the bearing present to destination type value parsed from this sentence.

### `get_cross_track_error_magnitude` / `getCrossTrackErrorMagnitude`

```cpp
double getCrossTrackErrorMagnitude() const noexcept;
```

Returns the cross track error magnitude value parsed from this sentence.

### `get_cross_track_units` / `getCrossTrackUnits`

```cpp
char getCrossTrackUnits() const noexcept;
```

Returns the cross track units value parsed from this sentence.

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

### `get_heading_to_steer_to_destination_waypoint` / `getHeadingToSteerToDestinationWaypoint`

```cpp
double getHeadingToSteerToDestinationWaypoint() const noexcept;
```

Returns the heading to steer to destination waypoint value parsed from this sentence.

### `get_heading_to_steer_to_destination_waypoint_type` / `getHeadingToSteerToDestinationWaypointType`

```cpp
char getHeadingToSteerToDestinationWaypointType() const noexcept;
```

Returns the heading to steer to destination waypoint type value parsed from this sentence.

### `get_perpendicular_passed_at_waypoint` / `getPerpendicularPassedAtWaypoint`

```cpp
char getPerpendicularPassedAtWaypoint() const noexcept;
```

Returns the perpendicular passed at waypoint status value parsed from this sentence.

### `get_status1` / `getStatus1`

```cpp
char getStatus1() const noexcept;
```

Returns the status1 value parsed from this sentence.

### `get_status2` / `getStatus2`

```cpp
char getStatus2() const noexcept;
```

Returns the status2 value parsed from this sentence.

### `get_string_content` / `getStringContent`

```cpp
std::string getStringContent(bool verbose) const noexcept override;
```

Returns a human-readable representation of the sentence content.

## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
- Sentence overview source: https://gpsd.gitlab.io/gpsd/NMEA.html
