# RMC API

Class `nmea0183.RMC` (Python) / `nmealib::nmea0183::RMC` (C++).

## Creation
- Python direct: `msg = nmealib.nmea0183.RMC(<fields...>)`
- C++ direct: `nmealib::nmea0183::RMC msg(/* fields */);`
- Python factory: `msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)`
- C++ factory: `auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);`

## Public Methods
- `get_course_over_ground` / `getCourseOverGround`
- `get_date` / `getDate`
- `get_latitude` / `getLatitude`
- `get_latitude_direction` / `getLatitudeDirection`
- `get_longitude` / `getLongitude`
- `get_longitude_direction` / `getLongitudeDirection`
- `get_magnetic_variation` / `getMagneticVariation`
- `get_magnetic_variation_direction` / `getMagneticVariationDirection`
- `get_mode_indicator` / `getModeIndicator`
- `get_navigation_status` / `getNavigationStatus`
- `get_speed_over_ground` / `getSpeedOverGround`
- `get_status` / `getStatus`
- `get_string_content` / `getStringContent`
- `get_utc_fix` / `getUtcFix`
## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
