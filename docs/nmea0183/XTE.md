# XTE API

Class `nmea0183.XTE` (Python) / `nmealib::nmea0183::XTE` (C++).

## Creation
- Python direct: `msg = nmealib.nmea0183.XTE(<fields...>)`
- C++ direct: `nmealib::nmea0183::XTE msg(/* fields */);`
- Python factory: `msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)`
- C++ factory: `auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);`

## Public Methods
- `get_cross_track_error` / `getCrossTrackError`
- `get_cross_track_units` / `getCrossTrackUnits`
- `get_faa_mode_indicator` / `getFaaModeIndicator`
- `get_status1` / `getStatus1`
- `get_status2` / `getStatus2`
- `get_steer_direction` / `getSteerDirection`
- `get_string_content` / `getStringContent`
- `has_faa_mode_indicator` / `hasFaaModeIndicator`
## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
