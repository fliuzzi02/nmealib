# Copilot Coding Agent Instructions for `nmealib`

## Repo summary
- `nmealib` is a C++20 CMake project that provides:
  - static library `nmealib` for NMEA 0183/NMEA 2000 parsing,
  - CLI `nmealib-cli` for quick parse/debug,
  - GoogleTest unit tests.
- Languages/tools: C++, CMake, YAML workflows, small Python helper script.
- Validated toolchain in this container: `cmake/ctest 3.28.3`, `g++ 13.3.0`, `cppcheck 2.13.0`.

## Fast layout map
- Build config: `CMakeLists.txt`, `CMakePresets.json`.
- Public API: `include/nmealib.h`, `include/nmealib/**`.
- Core sources: `src/message.cpp`, `src/nmeaException.cpp`, `src/nmea0183.cpp`, `src/nmea2000.cpp`.
- Protocol implementations:
  - NMEA 0183: `src/nmea0183/**` (factories, registry, sentence handlers).
  - NMEA 2000: `src/nmea2000/**` (factory, registry).
- CLI entrypoint: `app/main.cpp` (arg parsing, stdin support, protocol dispatch).
- Tests: `tests/nmea0183/test_*.cpp`, `tests/nmea2000/test_nmea2000.cpp`.
- CI: `.github/workflows/{CI,StaticAnalysis,Test,Build,Release}.yml`.

## Command order (validated; always use this order)
Run from repo root.

1) Clean (recommended before reproducing CI):
```bash
rm -rf out/build out/install build
```

2) Configure + build for tests:
```bash
cmake --preset gcc-release-tests
cmake --build --preset build-release-tests
```

3) Run tests (strict, CI-like):
```bash
ctest --test-dir out/build/gcc-release-tests --output-on-failure --no-tests=error
```

4) Configure + run static analysis:
```bash
cmake --preset gcc-release-static -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build --preset build-release-static --target static-analysis
```

5) Release build + install (for packaging/consumer checks):
```bash
cmake --preset gcc-release
cmake --build --preset build-release
cmake --install out/build/gcc-release --prefix out/install/gcc-release
```

6) Run CLI example:
```bash
./out/build/gcc-release-tests/app/nmealib-cli -t=N0183 -m '$GPGLL,4916.45,N,12311.12,W,225444,A,*1D'
```

## Failures/misbehavior you should expect
- `ctest --preset test-all` from a clean tree can print `No tests were found!!!` without failing.
  - Always use `--no-tests=error` and explicit `--test-dir`.
- `static-analysis` can fail with `compile_commands.json not found`.
  - Reconfigure with `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON` (command above).
- `clang-tidy` may be absent; warning is non-fatal (`cppcheck` still runs).
- Long builds may be interrupted if timeout budgets are too low (observed interruption around ~2m).
  - Use higher timeouts for full test builds.
- `/usr/bin/time` is missing in this dev container; use shell builtin `time`.

## CI and pre-PR replication
- CI gate order in `CI.yml`: `StaticAnalysis` -> gate -> `Test` -> conditional `Release`.
- `Test.yml` uses coverage flags + strict ctest behavior + lcov.
- Before opening PR, run at least:
  1. test configure/build (`gcc-release-tests`),
  2. strict ctest command above,
  3. static analysis command above.

## Non-obvious dependencies and gotchas
- Tests fetch GoogleTest `v1.14.0` via `FetchContent` (network needed on first configure).
- Installing from test build tree (`out/build/gcc-release-tests`) also installs GoogleTest/GMock artifacts.
  - Use `gcc-release` install path for clean distributable layout.
- Root build emits many component libraries (`nmealib0183_*`) plus monolithic `nmealib`; prefer editing/testing monolithic paths unless task explicitly targets component libs.

## Root inventory + README essentials
- Root files: `CMakeLists.txt`, `CMakePresets.json`, `README.md`, `VERSION`, `LICENSE`.
- Root dirs: `.github/`, `app/`, `docs/`, `include/`, `src/`, `tests/`, `scripts/`.
- README says: CMake 3.20+, C++20, preset-based build/test flow, CLI usage examples, install under `out/install/...`.

## Python bindings tests (required when touching bindings or PGN/message constructors)
Run from repo root.

1) Create and activate a virtual environment (required in this container due PEP 668):
```bash
python3 -m venv .venv
source .venv/bin/activate
```

2) Install Python test/build prerequisites:
```bash
python -m pip install --upgrade pip
python -m pip install pybind11 pytest
```

3) Configure a build with Python bindings enabled (explicit `pybind11_DIR` is required for CMake discovery):
```bash
cmake -S . -B out/build/python-tests -DCMAKE_BUILD_TYPE=Release -DBUILD_PYTHON_BINDINGS=ON -Dpybind11_DIR="$(python -m pybind11 --cmakedir)"
```

4) Build:
```bash
cmake --build out/build/python-tests -j
```

5) Run Python tests:
```bash
PYTHONPATH="$PWD/python:$PWD/out/build/python-tests/python" pytest -q tests/python
```

If `ModuleNotFoundError: nmealib` appears, `PYTHONPATH` is not set correctly or `_core` was not built.

If `find_package(pybind11)` fails during configure, ensure the venv is active and pass `-Dpybind11_DIR="$(python -m pybind11 --cmakedir)"`.

Current known issue (as of 2026-04-16): Python tests may fail at import with `ImportError: initialization failed` / `UnicodeDecodeError` even after successful configure/build. Treat this as a bindings runtime issue, not a setup-command issue.

## Mandatory checklist for new PGN or NMEA0183 messages
Whenever a new PGN or NMEA0183 message is added, all of the following are required:

1. Wire the new message in CMake (library sources/tests CMake files as appropriate).
2. Add unit tests matching the style/coverage of existing implemented messages.
3. Add/update Python bindings for the message in `python/src/bindings.cpp`.
4. Document support details in `docs/PROTOCOL_SUPPORT.md`.

## Search policy
Trust this file first. Only run broad repository searches if this file is incomplete, outdated, or your task touches an area not listed here.
