# DPT API

Class `nmea0183.DPT` (Python) / `nmealib::nmea0183::DPT` (C++).

## Creation
- Python direct: `msg = nmealib.nmea0183.DPT(<fields...>)`
- C++ direct: `nmealib::nmea0183::DPT msg(/* fields */);`
- Python factory: `msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)`
- C++ factory: `auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);`

## Public Methods
- `get_max_range_scale` / `getMaxRangeScale`
- `get_string_content` / `getStringContent`
- `get_transducer_offset` / `getTransducerOffset`
- `get_water_depth_meters` / `getWaterDepthMeters`
## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
