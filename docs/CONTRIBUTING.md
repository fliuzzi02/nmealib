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
TODO: These steps should be automated by a CD pipeline, but here are the steps to upload the library to the registries.

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
