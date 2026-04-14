# MTW API

Class `nmea0183.MTW` (Python) / `nmealib::nmea0183::MTW` (C++).

## Creation
- Python direct: `msg = nmealib.nmea0183.MTW(<fields...>)`
- C++ direct: `nmealib::nmea0183::MTW msg(/* fields */);`
- Python factory: `msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)`
- C++ factory: `auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);`

## Public Methods
- `get_string_content` / `getStringContent`
- `get_temperature` / `getTemperature`
- `get_unit` / `getUnit`
## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
