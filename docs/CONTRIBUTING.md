# Contributing

## Getting Started

## Setting Up Your Development Environment

## Code Style Guidelines

## Commit Message Convention

## Submitting Changes

### Creating a Branch

### Making Changes

### Creating a Pull Request

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


### Writing Tests

### Code Coverage

## Documentation

## Reporting Issues

## Code Review Process

## License

