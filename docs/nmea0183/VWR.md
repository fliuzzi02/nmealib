# VWR API

Class `nmea0183.VWR` (Python) / `nmealib::nmea0183::VWR` (C++).

## Creation
- Python direct: `msg = nmealib.nmea0183.VWR(<fields...>)`
- C++ direct: `nmealib::nmea0183::VWR msg(/* fields */);`
- Python factory: `msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)`
- C++ factory: `auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);`

## Public Methods
- `get_speed_knots` / `getSpeedKnots`
- `get_speed_knots_unit` / `getSpeedKnotsUnit`
- `get_speed_kph` / `getSpeedKph`
- `get_speed_kph_unit` / `getSpeedKphUnit`
- `get_speed_mps` / `getSpeedMps`
- `get_speed_mps_unit` / `getSpeedMpsUnit`
- `get_string_content` / `getStringContent`
- `get_wind_angle` / `getWindAngle`
- `get_wind_side` / `getWindSide`
## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
