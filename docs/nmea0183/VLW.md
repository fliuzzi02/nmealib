# VLW API

Class `nmea0183.VLW` (Python) / `nmealib::nmea0183::VLW` (C++).

## Creation
- Python direct: `msg = nmealib.nmea0183.VLW(<fields...>)`
- C++ direct: `nmealib::nmea0183::VLW msg(/* fields */);`
- Python factory: `msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)`
- C++ factory: `auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);`

## Public Methods
- `get_ground_distance_since_reset_nm` / `getGroundDistanceSinceResetNm`
- `get_ground_distance_since_reset_unit` / `getGroundDistanceSinceResetUnit`
- `get_string_content` / `getStringContent`
- `get_total_ground_distance_nm` / `getTotalGroundDistanceNm`
- `get_total_ground_distance_unit` / `getTotalGroundDistanceUnit`
- `get_total_water_distance_nm` / `getTotalWaterDistanceNm`
- `get_total_water_distance_unit` / `getTotalWaterDistanceUnit`
- `get_water_distance_since_reset_nm` / `getWaterDistanceSinceResetNm`
- `get_water_distance_since_reset_unit` / `getWaterDistanceSinceResetUnit`
- `has_ground_distance_data` / `hasGroundDistanceData`
## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
