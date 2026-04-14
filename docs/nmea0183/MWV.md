# MWV API

Class `nmea0183.MWV` (Python) / `nmealib::nmea0183::MWV` (C++).

## Creation
- Python direct: `msg = nmealib.nmea0183.MWV(<fields...>)`
- C++ direct: `nmealib::nmea0183::MWV msg(/* fields */);`
- Python factory: `msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)`
- C++ factory: `auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);`

## Public Methods
- `get_reference` / `getReference`
- `get_status` / `getStatus`
- `get_string_content` / `getStringContent`
- `get_wind_angle` / `getWindAngle`
- `get_wind_speed` / `getWindSpeed`
- `get_wind_speed_units` / `getWindSpeedUnits`
## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
