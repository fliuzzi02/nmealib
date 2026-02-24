# Contributing to nmealib

Thanks for contributing.

## Prerequisites

- C++20 compiler
- CMake 3.20+
- Git

## Setup

```bash
git clone https://github.com/fliuzzi02/nmealib.git
cd nmealib
cmake --preset gcc-full
cmake --build out/build/gcc-full
ctest --test-dir out/build/gcc-full --output-on-failure
```

## Branching

Create branches from `main`:

```bash
git checkout main
git pull origin main
git checkout -b feature/short-description
```

## What to include in a change

- Minimal focused implementation
- Tests for new/changed behavior
- Updated docs for user-facing/API changes

## Coding expectations

- Follow existing code style in file/module
- Prefer clear names and small functions
- Use modern C++20 where helpful
- Throw `NmeaException`-derived errors for parse/validation issues

## API usage in docs/tests

Use `Nmea0183Factory::create(...)` for parsing user input.

```cpp
auto msg = nmealib::nmea0183::Nmea0183Factory::create(
    "$GNGLL,3150.788156,N,11711.922383,E,062735.00,A,A*76\r\n");
```

Then downcast only when sentence-specific access is needed.

## Testing checklist

Before opening PR:

```bash
cmake --build out/build/gcc-full
ctest --test-dir out/build/gcc-full --output-on-failure
ctest --test-dir out/build/gcc-full -L nmea0183 --output-on-failure
```

If you touched sentence parsing, add/verify tests for:

- factory parse success
- constructor-from-fields behavior
- serialize/parse round trip
- incomplete/malformed inputs
- clone/equality behavior

## Commit message suggestions

Use concise prefixes:

- `feat:` feature
- `fix:` bug fix
- `test:` tests
- `docs:` documentation
- `refactor:` internal cleanup

Example:

```text
feat: add GLL typed sentence support
```

## Pull request checklist

- [ ] Code builds
- [ ] Tests pass
- [ ] Docs updated
- [ ] Scope is focused and clear

## Links

- [Building and Testing](Building-and-Testing.md)
- [API Reference](API-Reference.md)
- [Examples](Examples.md)
