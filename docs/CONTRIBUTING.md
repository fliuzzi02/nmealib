# Contributing

## Getting Started

## Setting Up Your Development Environment

## Code Style Guidelines

## Commit Message Convention

## Submitting Changes

## Testing

The project includes a suite of unit tests. To run the tests, you can use the CMake presets provided in the repository.

### Running Tests

If you want to run the tests or the static analysis, you can use the test preset:

```bash
cmake --preset gcc-release-tests
cmake --build --preset build-release-tests
ctest --test-dir out/build/gcc-release-tests --output-on-failure --no-tests=error
```

or

```bash
cmake --preset gcc-release-static -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build --preset build-release-static --target static-analysis
```

To evaluate code coverage, use the coverage preset:

```bash
TODO: add coverage instructions
```

## Uploading the library

The project now includes a release-wired CI/CD skeleton for publishing artifacts and packages.

Current topology:

1. `prepare`
2. Linux release build (existing tarball artifacts)
3. Linux publishing matrix skeleton (manylinux x86_64 + aarch64)
4. GitHub Release publish (existing)
5. TestPyPI skeleton stage
6. PyPI skeleton stage
7. PlatformIO skeleton stages (validate metadata, publish placeholder, verify placeholder)

This is intentionally a skeleton-only phase. The publish jobs are wired and gated, but their implementation commands are still placeholders.

### CI/CD skeleton files

- `.github/workflows/BuildLinuxMatrix.yml`
- `.github/workflows/PublishPython.yml`
- `.github/workflows/PublishPlatformIO.yml`

Release orchestration wiring is in `.github/workflows/Release.yml`.

### Publish gating policy

- Publishing stages run only in release context (`workflow_dispatch` or `publish=true` in reusable release calls).
- Python publish skeleton keeps a staged order: TestPyPI before PyPI.
- PlatformIO publish skeleton validates `library.json` version against release version before publish placeholder stage.

### PlatformIO registry

After releasing a new version throught the CI pipeline (it will be tagged and have a proper version number), you can upload the library to the PlatformIO registry by running the following command:

```bash
# Install platformio if you haven't already
sudo apt-get update
sudo apt install pipx -y
pipx ensurepath
pipx install platformio

# Login to your PlatformIO account
pio account login

# Upload the library to the PlatformIO registry
pio package publish
```

### PyPI registry

Publishing is now wired through CI skeleton stages; implementation commands will be added in the next pass.
