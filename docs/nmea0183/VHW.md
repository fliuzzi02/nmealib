# VHW API

Class `nmea0183.VHW` (Python) / `nmealib::nmea0183::VHW` (C++).

## Creation
- Python direct: `msg = nmealib.nmea0183.VHW(<fields...>)`
- C++ direct: `nmealib::nmea0183::VHW msg(/* fields */);`
- Python factory: `msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)`
- C++ factory: `auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);`

## Public Methods
- `get_heading_magnetic` / `getHeadingMagnetic`
- `get_heading_magnetic_type` / `getHeadingMagneticType`
- `get_heading_true` / `getHeadingTrue`
- `get_heading_true_type` / `getHeadingTrueType`
- `get_speed_knots` / `getSpeedKnots`
- `get_speed_knots_type` / `getSpeedKnotsType`
- `get_speed_kph` / `getSpeedKph`
- `get_speed_kph_type` / `getSpeedKphType`
- `get_string_content` / `getStringContent`
## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
