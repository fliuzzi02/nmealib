# GSA API

Class `nmea0183.GSA` (Python) / `nmealib::nmea0183::GSA` (C++).

## Creation
- Python direct: `msg = nmealib.nmea0183.GSA(<fields...>)`
- C++ direct: `nmealib::nmea0183::GSA msg(/* fields */);`
- Python factory: `msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)`
- C++ factory: `auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);`

## Public Methods
- `get_hdop` / `getHdop`
- `get_mode` / `getMode`
- `get_pdop` / `getPdop`
- `get_satellite_id` / `getSatelliteId`
- `get_satellite_ids` / `getSatelliteIds`
- `get_selection_mode` / `getSelectionMode`
- `get_string_content` / `getStringContent`
- `get_system_id` / `getSystemId`
- `get_vdop` / `getVdop`
- `has_system_id` / `hasSystemId`
## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
