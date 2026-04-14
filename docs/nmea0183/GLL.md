# GLL API

Class `nmea0183.GLL` (Python) / `nmealib::nmea0183::GLL` (C++).

## Creation
- Python direct: `msg = nmealib.nmea0183.GLL(<fields...>)`
- C++ direct: `nmealib::nmea0183::GLL msg(/* fields */);`
- Python factory: `msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)`
- C++ factory: `auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);`

## Public Methods
- `get_latitude` / `getLatitude`
- `get_latitude_direction` / `getLatitudeDirection`
- `get_longitude` / `getLongitude`
- `get_longitude_direction` / `getLongitudeDirection`
- `get_mode_indicator` / `getModeIndicator`
- `get_status` / `getStatus`
- `get_string_content` / `getStringContent`
- `get_utc_time` / `getUtcTime`
## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
