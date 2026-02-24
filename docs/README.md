# Wiki Documentation

This folder contains source markdown for the project wiki.

## Pages

- [Home](Home.md)
- [Getting Started](Getting-Started.md)
- [API Reference](API-Reference.md)
- [NMEA 0183 Guide](NMEA-0183-Guide.md)
- [Examples](Examples.md)
- [Building and Testing](Building-and-Testing.md)
- [Contributing](Contributing.md)

## Maintenance rules

- Keep examples aligned with current code.
- Use `Nmea0183Factory::create(...)` for parsing examples.
- Update docs in the same PR as API/behavior changes.
- Keep page links relative (`Page-Name.md`).

## Validation suggestions

- Build project after docs-linked code changes.
- Run `ctest --test-dir out/build/gcc-full -L nmea0183`.
- Spot-check snippets in [Examples](Examples.md).

## Upstream links

- Main repository: https://github.com/fliuzzi02/nmealib
- Published wiki: https://github.com/fliuzzi02/nmealib/wiki
