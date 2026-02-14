# API Reference

Complete API reference for nmealib classes and functions.

## Namespaces

- `nmealib` - Root namespace for all library components
- `nmealib::nmea0183` - NMEA 0183 specific classes and functions

## Core Classes

### nmealib::Message

**File:** `message.hpp`

Base class for all NMEA message types. Provides common functionality for message handling.

#### Public Types

```cpp
enum class Type { Unknown, NMEA0183, NMEA2000 };
using TimePoint = std::chrono::system_clock::time_point;
```

#### Constructors

```cpp
Message()  // Default constructor
explicit Message(std::string raw, Type type = Type::Unknown, 
                TimePoint ts = std::chrono::system_clock::now()) noexcept
```

#### Public Methods

| Method | Description | Return Type |
|--------|-------------|-------------|
| `getType()` | Get the message type | `Type` |
| `getRawData()` | Get the original raw message string | `const std::string&` |
| `getTimestamp()` | Get the message timestamp | `TimePoint` |
| `clone()` | Create a polymorphic copy of the message | `std::unique_ptr<Message>` |
| `serialize()` | Convert message to wire format | `std::string` |
| `validate()` | Validate message integrity | `bool` |
| `operator==` | Compare messages for equality | `bool` |

**Note:** `clone()`, `serialize()`, and `validate()` are pure virtual and must be implemented by derived classes.

---

### nmealib::nmea0183::Message0183

**File:** `nmea0183/nmea0183.hpp`

Represents an NMEA 0183 sentence with full parsing and validation capabilities.

#### Factory Method

```cpp
static std::unique_ptr<Message0183> create(
    const std::string& raw,
    TimePoint ts = std::chrono::system_clock::now()
);
```

Creates a Message0183 instance from a raw NMEA 0183 sentence string.

**Parameters:**
- `raw` - The raw NMEA 0183 sentence (must include `\r\n` ending)
- `ts` - Optional timestamp (defaults to current time)

**Returns:** `std::unique_ptr<Message0183>` to the parsed message

**Throws:**
- `TooLongSentenceException` - If sentence exceeds 82 characters
- `InvalidStartCharacterException` - If sentence doesn't start with `$` or `!`
- `NoEndlineException` - If sentence doesn't end with `\r\n`

**Example:**
```cpp
auto msg = Message0183::create("$GPGGA,123519,...*47\r\n");
```

#### Constructors

```cpp
Message0183(const Message0183&) = default;           // Copy constructor
Message0183& operator=(const Message0183&) = default; // Copy assignment
Message0183(Message0183&&) noexcept = default;       // Move constructor
Message0183& operator=(Message0183&&) noexcept = default; // Move assignment
~Message0183() override = default;                   // Destructor
```

#### Public Methods

##### getStartChar()
```cpp
char getStartChar() const noexcept;
```
Returns the start character of the sentence (`$` or `!`).

##### getTalker()
```cpp
std::string getTalker() const noexcept;
```
Returns the two-character talker identifier (e.g., "GP", "II", "GL").

**Common Talker IDs:**
- `GP` - GPS
- `GL` - GLONASS
- `GA` - Galileo
- `GN` - Combined GNSS
- `II` - Integrated Instrumentation
- `AI` - AIS

##### getSentenceType()
```cpp
std::string getSentenceType() const noexcept;
```
Returns the three-character sentence type identifier (e.g., "GGA", "RMC", "GSA").

**Common Sentence Types:**
- `GGA` - Global Positioning System Fix Data
- `RMC` - Recommended Minimum Navigation Information
- `GSA` - GPS DOP and Active Satellites
- `GSV` - GPS Satellites in View
- `VTG` - Track Made Good and Ground Speed
- `GLL` - Geographic Position

##### getPayload()
```cpp
std::string getPayload() const noexcept;
```
Returns the full payload string (everything between start character and checksum/endline).

**Example:** For `$GPGGA,123519,...*47\r\n`, returns `"GPGGA,123519,..."`

##### getChecksumStr()
```cpp
std::string getChecksumStr() const;
```
Returns the checksum string from the message (two hex digits).

**Throws:** `NoChecksumException` if the message doesn't contain a checksum.

**Example:**
```cpp
try {
    std::string checksum = msg->getChecksumStr();
    std::cout << "Checksum: " << checksum << std::endl;
} catch (const NoChecksumException&) {
    std::cout << "No checksum present" << std::endl;
}
```

##### getCalculatedChecksumStr()
```cpp
std::string getCalculatedChecksumStr() const noexcept;
```
Returns the calculated checksum for the payload as a two-digit hex string.

Always available, regardless of whether the original message had a checksum.

##### serialize()
```cpp
std::string serialize() const override;
```
Converts the message back to wire format (NMEA 0183 sentence string).

**Returns:** String without `\r\n` terminator

##### validate()
```cpp
bool validate() const override;
```
Validates the message by comparing the provided checksum (if any) with the calculated checksum.

**Returns:**
- `true` - If no checksum present OR checksum matches calculated value
- `false` - If checksum present but doesn't match calculated value

##### clone()
```cpp
std::unique_ptr<nmealib::Message> clone() const override;
```
Creates a polymorphic copy of the message.

**Returns:** Unique pointer to base Message class containing a copy

**Example:**
```cpp
auto original = Message0183::create(sentence);
auto copy = original->clone();
auto* derived = dynamic_cast<Message0183*>(copy.get());
```

##### operator==
```cpp
bool operator==(const Message0183& other) const noexcept;
```
Compares two messages for complete equality including timestamps.

**Returns:** `true` if all fields match (including timestamp)

##### hasEqualContent()
```cpp
bool hasEqualContent(const Message0183& other) const noexcept;
```
Compares message content only, ignoring timestamps.

**Returns:** `true` if message content is identical (start char, talker, type, payload, checksums)

**Example:**
```cpp
auto msg1 = Message0183::create(sentence, time1);
auto msg2 = Message0183::create(sentence, time2);
msg1 == msg2;  // false (different timestamps)
msg1.hasEqualContent(msg2);  // true (same content)
```

#### Protected Members

```cpp
char startChar_;                    // '$' or '!'
std::string talker_;                // e.g. "GP", "II"
std::string sentenceType_;          // e.g. "GGA", "RMC"
std::string payload_;               // Full payload string
std::string checksumStr_;           // Provided checksum (if any)
std::string calculatedChecksumStr_; // Calculated checksum
```

---

## Exception Classes

All exception classes inherit from `nmealib::NmeaException`.

### nmealib::NmeaException

**File:** `nmeaException.hpp`

Base exception class for all nmealib exceptions.

```cpp
class NmeaException : public std::runtime_error {
public:
    NmeaException(const std::string& context,
                  const std::string& message,
                  const std::string& details = "");
                  
    const std::string& getContext() const noexcept;
    const std::string& getDetails() const noexcept;
};
```

### NMEA 0183 Exceptions

#### TooLongSentenceException
```cpp
class TooLongSentenceException : public NmeaException;
```
Thrown when a sentence exceeds the maximum length of 82 characters.

#### InvalidStartCharacterException
```cpp
class InvalidStartCharacterException : public NmeaException;
```
Thrown when a sentence doesn't start with `$` or `!`.

#### NoEndlineException
```cpp
class NoEndlineException : public NmeaException;
```
Thrown when a sentence doesn't end with `\r\n`.

#### NoChecksumException
```cpp
class NoChecksumException : public NmeaException;
```
Thrown when attempting to retrieve a checksum from a message that doesn't have one.

---

## Usage Examples

### Basic Parsing

```cpp
#include "nmea0183/nmea0183.hpp"

std::string sentence = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n";
auto msg = nmealib::nmea0183::Message0183::create(sentence);

std::cout << "Talker: " << msg->getTalker() << std::endl;
std::cout << "Type: " << msg->getSentenceType() << std::endl;
std::cout << "Valid: " << msg->validate() << std::endl;
```

### Error Handling

```cpp
try {
    auto msg = Message0183::create(input);
    if (msg->validate()) {
        // Process valid message
    }
} catch (const TooLongSentenceException& e) {
    // Handle length error
} catch (const InvalidStartCharacterException& e) {
    // Handle start character error
} catch (const NoEndlineException& e) {
    // Handle endline error
}
```

### Checksum Operations

```cpp
auto msg = Message0183::create(sentence);

// Check if checksum exists
try {
    std::string provided = msg->getChecksumStr();
    std::string calculated = msg->getCalculatedChecksumStr();
    
    if (provided == calculated) {
        std::cout << "Checksum valid!" << std::endl;
    }
} catch (const NoChecksumException&) {
    // Message has no checksum, use validate() instead
    bool isValid = msg->validate();  // Returns true for no checksum
}
```

### Message Cloning

```cpp
auto original = Message0183::create(sentence);
auto clone = original->clone();

// Downcast for NMEA0183-specific operations
if (auto* msg0183 = dynamic_cast<Message0183*>(clone.get())) {
    std::cout << "Talker: " << msg0183->getTalker() << std::endl;
}
```

### Polymorphic Usage

```cpp
std::vector<std::unique_ptr<nmealib::Message>> messages;

messages.push_back(Message0183::create(sentence1));
messages.push_back(Message0183::create(sentence2));

for (const auto& msg : messages) {
    std::cout << "Type: " << static_cast<int>(msg->getType()) << std::endl;
    std::cout << "Valid: " << msg->validate() << std::endl;
    std::cout << "Serialized: " << msg->serialize() << std::endl;
}
```

---

## See Also

- [Getting Started Guide](Getting-Started.md)
- [NMEA 0183 Format Guide](NMEA-0183-Guide.md)
- [Examples](Examples.md)
- [Building and Testing](Building-and-Testing.md)
