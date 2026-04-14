# HDM API

Class `nmea0183.HDM` (Python) / `nmealib::nmea0183::HDM` (C++).

## Creation
- Python direct: `msg = nmealib.nmea0183.HDM(<fields...>)`
- C++ direct: `nmealib::nmea0183::HDM msg(/* fields */);`
- Python factory: `msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)`
- C++ factory: `auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);`

## Public Methods
- `get_heading` / `getHeading`
- `get_indicator` / `getIndicator`
- `get_string_content` / `getStringContent`
## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
