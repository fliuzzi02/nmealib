# ZDA API

Class `nmea0183.ZDA` (Python) / `nmealib::nmea0183::ZDA` (C++).

## Creation
- Python direct: `msg = nmealib.nmea0183.ZDA(<fields...>)`
- C++ direct: `nmealib::nmea0183::ZDA msg(/* fields */);`
- Python factory: `msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)`
- C++ factory: `auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);`

## Public Methods
- `get_day` / `getDay`
- `get_local_zone_hours` / `getLocalZoneHours`
- `get_local_zone_minutes` / `getLocalZoneMinutes`
- `get_month` / `getMonth`
- `get_string_content` / `getStringContent`
- `get_utc_time` / `getUtcTime`
- `get_year` / `getYear`
## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
