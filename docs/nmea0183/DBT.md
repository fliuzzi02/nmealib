# DBT API

Class `nmea0183.DBT` (Python) / `nmealib::nmea0183::DBT` (C++).

## Creation
- Python direct: `msg = nmealib.nmea0183.DBT(<fields...>)`
- C++ direct: `nmealib::nmea0183::DBT msg(/* fields */);`
- Python factory: `msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)`
- C++ factory: `auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);`

## Public Methods
- `get_depth_fathoms` / `getDepthFathoms`
- `get_depth_feet` / `getDepthFeet`
- `get_depth_meters` / `getDepthMeters`
- `get_fathoms_unit` / `getFathomsUnit`
- `get_feet_unit` / `getFeetUnit`
- `get_meters_unit` / `getMetersUnit`
- `get_string_content` / `getStringContent`
## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
