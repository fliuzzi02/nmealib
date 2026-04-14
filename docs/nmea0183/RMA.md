# RMA API

Class `nmea0183.RMA` (Python) / `nmealib::nmea0183::RMA` (C++).

## Creation
- Python direct: `msg = nmealib.nmea0183.RMA(<fields...>)`
- C++ direct: `nmealib::nmea0183::RMA msg(/* fields */);`
- Python factory: `msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)`
- C++ factory: `auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);`

## Public Methods
- `get_latitude` / `getLatitude`
- `get_latitude_direction` / `getLatitudeDirection`
- `get_longitude` / `getLongitude`
- `get_longitude_direction` / `getLongitudeDirection`
- `get_magnetic_variation` / `getMagneticVariation`
- `get_speed_over_ground` / `getSpeedOverGround`
- `get_status` / `getStatus`
- `get_string_content` / `getStringContent`
- `get_time_difference_a` / `getTimeDifferenceA`
- `get_time_difference_b` / `getTimeDifferenceB`
- `get_track_made_good` / `getTrackMadeGood`
- `get_variation_direction` / `getVariationDirection`
## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
