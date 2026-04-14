# XDR API

Class `nmea0183.XDR` (Python) / `nmealib::nmea0183::XDR` (C++).

## Creation
- Python direct: `msg = nmealib.nmea0183.XDR(<fields...>)`
- C++ direct: `nmealib::nmea0183::XDR msg(/* fields */);`
- Python factory: `msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)`
- C++ factory: `auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);`

## Public Methods
- `get_string_content` / `getStringContent`
- `get_transducer_count` / `getTransducerCount`
- `get_transducers` / `getTransducers`
## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
