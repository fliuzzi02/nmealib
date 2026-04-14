# HDG API

Class `nmea0183.HDG` (Python) / `nmealib::nmea0183::HDG` (C++).

## Creation
- Python direct: `msg = nmealib.nmea0183.HDG(<fields...>)`
- C++ direct: `nmealib::nmea0183::HDG msg(/* fields */);`
- Python factory: `msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)`
- C++ factory: `auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);`

## Public Methods
- `get_deviation_direction` / `getDeviationDirection`
- `get_magnetic_deviation` / `getMagneticDeviation`
- `get_magnetic_heading` / `getMagneticHeading`
- `get_magnetic_variation` / `getMagneticVariation`
- `get_string_content` / `getStringContent`
- `get_variation_direction` / `getVariationDirection`
## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
