# RMB API

Class `nmea0183.RMB` (Python) / `nmealib::nmea0183::RMB` (C++).

## Creation
- Python direct: `msg = nmealib.nmea0183.RMB(<fields...>)`
- C++ direct: `nmealib::nmea0183::RMB msg(/* fields */);`
- Python factory: `msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)`
- C++ factory: `auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);`

## Public Methods
- `get_arrival_status` / `getArrivalStatus`
- `get_bearing_to_destination_true` / `getBearingToDestinationTrue`
- `get_cross_track_error_nm` / `getCrossTrackErrorNm`
- `get_destination_closing_velocity_knots` / `getDestinationClosingVelocityKnots`
- `get_destination_latitude` / `getDestinationLatitude`
- `get_destination_latitude_hemisphere` / `getDestinationLatitudeHemisphere`
- `get_destination_longitude` / `getDestinationLongitude`
- `get_destination_longitude_hemisphere` / `getDestinationLongitudeHemisphere`
- `get_destination_waypoint_id` / `getDestinationWaypointId`
- `get_direction_to_steer` / `getDirectionToSteer`
- `get_faa_mode_indicator` / `getFaaModeIndicator`
- `get_origin_waypoint_id` / `getOriginWaypointId`
- `get_range_to_destination_nm` / `getRangeToDestinationNm`
- `get_status` / `getStatus`
- `get_string_content` / `getStringContent`
- `has_faa_mode_indicator` / `hasFaaModeIndicator`
## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
