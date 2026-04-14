# GSV API

Class `nmea0183.GSV` (Python) / `nmealib::nmea0183::GSV` (C++).

## Creation
- Python direct: `msg = nmealib.nmea0183.GSV(<fields...>)`
- C++ direct: `nmealib::nmea0183::GSV msg(/* fields */);`
- Python factory: `msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)`
- C++ factory: `auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);`

## Public Methods
- `get_satellite_count` / `getSatelliteCount`
- `get_satellites` / `getSatellites`
- `get_satellites_in_view` / `getSatellitesInView`
- `get_sentence_number` / `getSentenceNumber`
- `get_signal_id` / `getSignalId`
- `get_string_content` / `getStringContent`
- `get_total_sentences` / `getTotalSentences`
- `has_signal_id` / `hasSignalId`
## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
