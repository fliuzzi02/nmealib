# VTG API

Class `nmea0183.VTG` (Python) / `nmealib::nmea0183::VTG` (C++).

## Creation
- Python direct: `msg = nmealib.nmea0183.VTG(<fields...>)`
- C++ direct: `nmealib::nmea0183::VTG msg(/* fields */);`
- Python factory: `msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)`
- C++ factory: `auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);`

## Public Methods
- `get_course_over_ground_magnetic` / `getCourseOverGroundMagnetic`
- `get_course_over_ground_magnetic_type` / `getCourseOverGroundMagneticType`
- `get_course_over_ground_true` / `getCourseOverGroundTrue`
- `get_course_over_ground_true_type` / `getCourseOverGroundTrueType`
- `get_faa_mode_indicator` / `getFaaModeIndicator`
- `get_speed_over_ground_knots` / `getSpeedOverGroundKnots`
- `get_speed_over_ground_knots_type` / `getSpeedOverGroundKnotsType`
- `get_speed_over_ground_kph` / `getSpeedOverGroundKph`
- `get_speed_over_ground_kph_type` / `getSpeedOverGroundKphType`
- `get_string_content` / `getStringContent`
- `has_faa_mode_indicator` / `hasFaaModeIndicator`
- `is_legacy_format` / `isLegacyFormat`
## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
