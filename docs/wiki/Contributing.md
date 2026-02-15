# Contributing to nmealib

Thank you for your interest in contributing to nmealib! This document provides guidelines and information for contributors.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Process](#development-process)
- [Coding Standards](#coding-standards)
- [Testing Guidelines](#testing-guidelines)
- [Documentation](#documentation)
- [Submitting Changes](#submitting-changes)
- [Review Process](#review-process)

## Code of Conduct

### Our Pledge

We are committed to providing a welcoming and inclusive environment for all contributors. We expect:

- **Respectful communication** in all interactions
- **Constructive feedback** when reviewing code
- **Patience** with newcomers and those learning
- **Collaboration** over competition

### Unacceptable Behavior

- Harassment, trolling, or personal attacks
- Publishing others' private information
- Any conduct that could be considered inappropriate in a professional setting

## Getting Started

### Prerequisites

Before contributing, ensure you have:

- C++20 compatible compiler (GCC 10+, Clang 12+, MSVC 2019+)
- CMake 3.20 or higher
- Git for version control
- Familiarity with NMEA standards (or willingness to learn)

### Setting Up Development Environment

1. **Fork the repository** on GitHub

2. **Clone your fork:**
```bash
git clone https://github.com/YOUR_USERNAME/nmealib.git
cd nmealib
```

3. **Add upstream remote:**
```bash
git remote add upstream https://github.com/fliuzzi02/nmealib.git
```

4. **Configure and build:**
```bash
cmake --preset gcc-full
cmake --build out/build/gcc-full
```

5. **Run tests to verify setup:**
```bash
ctest --test-dir out/build/gcc-full
```

### Finding Issues to Work On

- Look for issues labeled `good first issue` for newcomers
- Check issues labeled `help wanted` for high-priority items
- Review the project roadmap for planned features
- Propose your own improvements via GitHub issues

## Development Process

### Branching Strategy

1. **Create a feature branch from main:**
```bash
git checkout main
git pull upstream main
git checkout -b feature/your-feature-name
```

2. **Use descriptive branch names:**
   - `feature/add-nmea2000-support`
   - `fix/checksum-validation-bug`
   - `docs/update-api-reference`
   - `test/add-gga-parser-tests`

### Making Changes

1. **Write code** following our coding standards
2. **Add tests** for new functionality
3. **Update documentation** as needed
4. **Run tests** frequently during development
5. **Commit changes** with clear messages

### Commit Message Guidelines

Use clear, descriptive commit messages:

**Format:**
```
<type>: <subject>

<body>

<footer>
```

**Types:**
- `feat:` New feature
- `fix:` Bug fix
- `docs:` Documentation changes
- `test:` Adding or updating tests
- `refactor:` Code refactoring
- `style:` Formatting changes
- `chore:` Maintenance tasks

**Example:**
```
feat: Add NMEA 2000 message parsing support

- Implement basic PGN parser
- Add Message2000 class
- Include validation logic

Closes #123
```

## Coding Standards

### C++ Style Guide

#### General Principles

- Write clear, readable code
- Use meaningful variable and function names
- Keep functions focused and concise
- Prefer const correctness
- Use modern C++20 features appropriately

#### Naming Conventions

```cpp
// Classes: PascalCase
class Message0183;
class NmeaException;

// Functions and methods: camelCase
void parseMessage();
std::string getPayload();

// Private member variables: camelCase with trailing underscore
private:
    std::string payload_;
    char startChar_;

// Constants: SCREAMING_SNAKE_CASE or kPascalCase
const int MAX_SENTENCE_LENGTH = 82;
const std::string kDefaultTalker = "GP";

// Namespaces: lowercase
namespace nmealib {
namespace nmea0183 {
```

#### Code Formatting

```cpp
// Indentation: 4 spaces (no tabs)
void exampleFunction() {
    if (condition) {
        // code here
    }
}

// Braces: Opening brace on same line
class Example {
public:
    void method() {
        // code
    }
};

// Line length: Prefer 100 characters, max 120
// Use line breaks for long statements
auto result = someFunction(
    parameter1,
    parameter2,
    parameter3
);
```

#### Header Guards

Use `#pragma once`:

```cpp
#pragma once

#include <string>
// ... rest of header
```

#### Include Order

```cpp
// 1. Corresponding header (for .cpp files)
#include "nmea0183.hpp"

// 2. C++ standard library headers
#include <string>
#include <memory>
#include <chrono>

// 3. Third-party library headers
#include <some_library.hpp>

// 4. Project headers
#include "message.hpp"
#include "nmeaException.hpp"
```

#### Documentation Comments

Use Doxygen-style comments for public APIs:

```cpp
/**
 * @brief Parses an NMEA 0183 sentence
 * 
 * This function takes a raw NMEA sentence string and creates
 * a Message0183 object with validated data.
 * 
 * @param raw The raw NMEA sentence (must include \\r\\n)
 * @param ts Optional timestamp (defaults to current time)
 * @return std::unique_ptr<Message0183> Parsed message object
 * @throws TooLongSentenceException If sentence exceeds 82 chars
 * @throws InvalidStartCharacterException If missing $ or !
 * @throws NoEndlineException If missing \\r\\n terminator
 * 
 * @code
 * auto msg = Message0183::create("$GPGGA,...*47\\r\\n");
 * @endcode
 */
static std::unique_ptr<Message0183> create(
    const std::string& raw,
    TimePoint ts = std::chrono::system_clock::now()
);
```

### Error Handling

- Use exceptions for error conditions
- Create specific exception types for different errors
- Provide meaningful error messages
- Document exceptions in function comments

```cpp
// Good: Specific exception with context
throw InvalidStartCharacterException(
    "Message0183::create",
    "Expected $ or !"
);

// Bad: Generic exception with no context
throw std::runtime_error("Invalid");
```

### Modern C++ Features

**Use:**
- `auto` for type deduction when type is obvious
- Smart pointers (`std::unique_ptr`, `std::shared_ptr`)
- Range-based for loops
- `constexpr` for compile-time constants
- `noexcept` for functions that don't throw
- Structured bindings
- `std::string_view` for non-owning string references

**Example:**
```cpp
// Modern style
auto msg = Message0183::create(sentence);
for (const auto& field : fields) {
    processField(field);
}

// Use noexcept appropriately
std::string getTalker() const noexcept;
```

## Testing Guidelines

### Writing Tests

Use Google Test framework:

```cpp
#include <gtest/gtest.h>
#include "nmea0183/nmea0183.hpp"

TEST(Message0183, ParsesValidSentence) {
    std::string sentence = "$GPGGA,123519,...*47\r\n";
    
    auto msg = nmealib::nmea0183::Message0183::create(sentence);
    
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getTalker(), "GP");
    EXPECT_EQ(msg->getSentenceType(), "GGA");
    EXPECT_TRUE(msg->validate());
}

TEST(Message0183, ThrowsOnInvalidInput) {
    std::string invalid = "INVALID SENTENCE";
    
    EXPECT_THROW(
        nmealib::nmea0183::Message0183::create(invalid),
        nmealib::nmea0183::InvalidStartCharacterException
    );
}
```

### Test Coverage

- Write tests for all new functionality
- Include edge cases and error conditions
- Test boundary conditions
- Aim for high code coverage (>80%)

### Test Organization

```cpp
// Group related tests
TEST(ClassName, MethodName_Condition_ExpectedBehavior) {
    // Arrange
    // Act
    // Assert
}

// Examples:
TEST(Message0183, Create_ValidSentence_ReturnsMessage)
TEST(Message0183, Create_TooLong_ThrowsException)
TEST(Message0183, Validate_NoChecksum_ReturnsTrue)
```

### Running Tests

```bash
# Run all tests
ctest --test-dir out/build/gcc-full

# Run with verbose output
ctest --test-dir out/build/gcc-full --verbose

# Run specific test
ctest --test-dir out/build/gcc-full -R Message0183
```

## Documentation

### Code Documentation

- Document all public APIs with Doxygen comments
- Include usage examples in documentation
- Explain complex algorithms with inline comments
- Update documentation when changing code

### README and Wiki

- Update README.md for user-facing changes
- Add examples to the Examples wiki page
- Update API Reference for new classes/methods
- Keep Getting Started guide current

### Example Documentation

```cpp
/**
 * @brief Validates the NMEA message checksum
 * 
 * Compares the provided checksum (if present) with the calculated
 * checksum for the message payload. If no checksum is provided,
 * the message is considered valid.
 * 
 * @return true if checksum is valid or absent
 * @return false if checksum is present but incorrect
 * 
 * @note This method does not throw exceptions
 * 
 * @see getChecksumStr()
 * @see getCalculatedChecksumStr()
 */
bool validate() const override;
```

## Submitting Changes

### Before Submitting

1. **Run all tests:**
```bash
cmake --build out/build/gcc-full
ctest --test-dir out/build/gcc-full
```

2. **Run static analysis (if enabled):**
```bash
cmake --build out/build/gcc-full --target static-analysis
```

3. **Update documentation:**
   - Update relevant wiki pages
   - Add/update code comments
   - Update README if needed

4. **Clean commit history:**
```bash
# Squash commits if needed
git rebase -i HEAD~n

# Ensure commits have good messages
git log --oneline
```

### Creating Pull Request

1. **Push to your fork:**
```bash
git push origin feature/your-feature-name
```

2. **Create PR on GitHub:**
   - Use a descriptive title
   - Reference related issues
   - Describe what changed and why
   - Include screenshots for UI changes
   - List any breaking changes

3. **PR Template:**
```markdown
## Description
Brief description of changes

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Breaking change
- [ ] Documentation update

## Testing
- [ ] All tests pass
- [ ] Added new tests for new functionality
- [ ] Manual testing completed

## Checklist
- [ ] Code follows project style guidelines
- [ ] Self-review completed
- [ ] Comments added for complex code
- [ ] Documentation updated
- [ ] No new warnings generated

## Related Issues
Closes #123
```

## Review Process

### What to Expect

- Reviews typically happen within 1-3 days
- Reviewers may request changes
- Be responsive to feedback
- Multiple iterations are normal

### As a Reviewer

- Be constructive and respectful
- Explain reasoning for requested changes
- Approve when satisfied
- Thank contributors for their work

### Addressing Feedback

```bash
# Make requested changes
git add .
git commit -m "Address review feedback"
git push origin feature/your-feature-name
```

## Community

### Communication Channels

- **GitHub Issues** - Bug reports, feature requests
- **GitHub Discussions** - Questions, ideas
- **Pull Requests** - Code review, discussions

### Getting Help

- Search existing issues first
- Provide minimal reproducible examples
- Include relevant system information
- Be patient and respectful

## Recognition

Contributors are recognized in:
- CONTRIBUTORS.md file
- Release notes
- Project documentation

Thank you for contributing to nmealib!

## See Also

- [Getting Started](Getting-Started.md)
- [Building and Testing](Building-and-Testing.md)
- [API Reference](API-Reference.md)
- [Examples](Examples.md)
