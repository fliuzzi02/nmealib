# GGA API

Class `nmea0183.GGA` (Python) / `nmealib::nmea0183::GGA` (C++).

## Creation
- Python direct: `msg = nmealib.nmea0183.GGA(<fields...>)`
- C++ direct: `nmealib::nmea0183::GGA msg(/* fields */);`
- Python factory: `msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)`
- C++ factory: `auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);`

## Public Methods
- `get_altitude` / `getAltitude`
- `get_altitude_units` / `getAltitudeUnits`
- `get_dgps_age` / `getDgpsAge`
- `get_dgps_reference_station_id` / `getDgpsReferenceStationId`
- `get_geoidal_separation` / `getGeoidalSeparation`
- `get_geoidal_separation_units` / `getGeoidalSeparationUnits`
- `get_gps_quality` / `getGpsQuality`
- `get_hdop` / `getHdop`
- `get_latitude` / `getLatitude`
- `get_latitude_direction` / `getLatitudeDirection`
- `get_longitude` / `getLongitude`
- `get_longitude_direction` / `getLongitudeDirection`
- `get_satellites` / `getSatellites`
- `get_string_content` / `getStringContent`
- `get_utc_time` / `getUtcTime`
## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
