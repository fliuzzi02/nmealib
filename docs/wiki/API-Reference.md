# API Reference

Complete API reference for `nmealib` classes and functions.

## Namespaces

- `nmealib` - Root namespace for all library components
- `nmealib::nmea0183` - NMEA 0183 specific classes and functions

## Core Classes

### nmealib::Message

**File:** `src/message.hpp`

Base abstract class for all message types.

#### Public Types

```cpp
enum class Type { Unknown, NMEA0183, NMEA2000 };
using TimePoint = std::chrono::system_clock::time_point;
```

#### Constructors / Assignment

```cpp
Message() = default;
explicit Message(std::string raw,
                 Type type = Type::Unknown,
                 TimePoint ts = std::chrono::system_clock::now()) noexcept;

Message(const Message&) = default;
Message& operator=(const Message&) = default;
Message(Message&&) noexcept = default;
Message& operator=(Message&&) noexcept = default;
virtual ~Message() = default;
```

#### Public Methods

| Method | Return Type | Notes |
|--------|-------------|-------|
| `static std::string typeToString(Type t)` | `std::string` | String representation of message type |
| `getType() const noexcept` | `Type` | Stored message type |
| `getRawData() const noexcept` | `const std::string&` | Raw wire text |
| `getTimestamp() const noexcept` | `TimePoint` | Message timestamp |
| `clone() const` | `std::unique_ptr<Message>` | Pure virtual |
| `serialize() const` | `std::string` | Pure virtual |
| `validate() const` | `bool` | Pure virtual |
| `operator==(const Message&) const noexcept` | `bool` | Compares type, raw data, and timestamp |

---

### nmealib::nmea0183::Message0183

**File:** `src/nmea0183/nmea0183.hpp`

Represents a parsed NMEA 0183 sentence.

#### Creation

`Message0183::create(...)` is an internal/protected factory. Use `Nmea0183Factory::create(...)` for construction.

#### Constructors / Assignment

```cpp
Message0183(const Message0183&) = default;
Message0183& operator=(const Message0183&) = default;
Message0183(Message0183&&) noexcept = default;
Message0183& operator=(Message0183&&) noexcept = default;
~Message0183() override = default;
```

#### Public Methods

| Method | Return Type | Notes |
|--------|-------------|-------|
| `clone() const override` | `std::unique_ptr<nmealib::Message>` | Polymorphic copy |
| `getStartChar() const noexcept` | `char` | `$` or `!` |
| `getTalker() const noexcept` | `std::string` | 2-char talker ID |
| `getSentenceType() const noexcept` | `std::string` | 3-char sentence type |
| `getPayload() const noexcept` | `std::string` | Full payload between start and checksum/end |
| `getChecksumStr() const` | `std::string` | Throws `NoChecksumException` if missing |
| `getCalculatedChecksumStr() const noexcept` | `std::string` | Computed checksum |
| `getStringContent(bool verbose) const noexcept` | `std::string` | Human-readable summary |
| `serialize() const override` | `std::string` | Returns stored raw wire string |
| `validate() const override` | `bool` | True when checksum absent or matching |
| `operator==(const Message0183&) const noexcept` | `bool` | Full equality, includes timestamp via base |
| `hasEqualContent(const Message0183&) const noexcept` | `bool` | Content equality, ignores timestamp |

#### Notes

- Maximum sentence length enforced: 82 chars.
- Start char must be `$` or `!`.
- Checksum is optional; if present, it must be two hex digits.

---

### nmealib::nmea0183::Nmea0183Factory

**File:** `src/nmea0183/nmea0183Factory.hpp`

Public entry point for parsing NMEA 0183 sentences and creating typed sentence objects.

```cpp
class Nmea0183Factory {
public:
    static std::unique_ptr<Message0183> create(
        const std::string& raw,
        Message::TimePoint ts = std::chrono::system_clock::now());
};
```

#### Runtime dispatch

Depending on sentence type, `create(...)` returns:

- `RMC` for `*RMC`
- `GGA` for `*GGA`
- `GLL` for `*GLL`
- `GSA` for `*GSA`
- base `Message0183` for other types

---

## Sentence Classes

### nmealib::nmea0183::RMC

**File:** `src/nmea0183/rmc.hpp`

Recommended Minimum Navigation Information.

#### Public Constructor

```cpp
RMC(std::string talkerId,
    unsigned int utcFix,
    char status,
    double latitude,
    char latitudeDirection,
    double longitude,
    char longitudeDirection,
    double speedOverGround,
    double courseOverGround,
    unsigned int date,
    double magneticVariation,
    char magneticVariationDirection,
    char modeIndicator,
    char navigationStatus);
```

#### Public Methods

| Method | Return Type |
|--------|-------------|
| `clone() const override` | `std::unique_ptr<nmealib::Message>` |
| `getUtcFix() const noexcept` | `unsigned int` |
| `getStatus() const noexcept` | `char` |
| `getLatitude() const noexcept` | `double` |
| `getLatitudeDirection() const noexcept` | `char` |
| `getLongitude() const noexcept` | `double` |
| `getLongitudeDirection() const noexcept` | `char` |
| `getSpeedOverGround() const noexcept` | `double` |
| `getCourseOverGround() const noexcept` | `double` |
| `getDate() const noexcept` | `unsigned int` |
| `getMagneticVariation() const noexcept` | `double` |
| `getMagneticVariationDirection() const noexcept` | `char` |
| `getModeIndicator() const noexcept` | `char` |
| `getNavigationStatus() const noexcept` | `char` |
| `getStringContent(bool verbose) const noexcept override` | `std::string` |
| `operator==(const RMC&) const noexcept` | `bool` |
| `hasEqualContent(const RMC&) const noexcept` | `bool` |

---

### nmealib::nmea0183::GGA

**File:** `src/nmea0183/gga.hpp`

Global Positioning System Fix Data.

#### Public Constructor

```cpp
GGA(std::string talkerId,
    double timestamp,
    double latitude,
    char latitudeDirection,
    double longitude,
    char longitudeDirection,
    unsigned int gpsQuality,
    unsigned int satellites,
    double hdop,
    double altitude,
    char altitudeUnits,
    double geoidalSeparation,
    char geoidalSeparationUnits,
    double dgpsAge,
    std::string dgpsReferenceStationId);
```

#### Public Methods

| Method | Return Type |
|--------|-------------|
| `clone() const override` | `std::unique_ptr<nmealib::Message>` |
| `getTimestamp() const noexcept` | `double` |
| `getLatitude() const noexcept` | `double` |
| `getLatitudeDirection() const noexcept` | `char` |
| `getLongitude() const noexcept` | `double` |
| `getLongitudeDirection() const noexcept` | `char` |
| `getGpsQuality() const noexcept` | `unsigned int` |
| `getSatellites() const noexcept` | `unsigned int` |
| `getHdop() const noexcept` | `double` |
| `getAltitude() const noexcept` | `double` |
| `getAltitudeUnits() const noexcept` | `char` |
| `getGeoidalSeparation() const noexcept` | `double` |
| `getGeoidalSeparationUnits() const noexcept` | `char` |
| `getDgpsAge() const noexcept` | `double` |
| `getDgpsReferenceStationId() const noexcept` | `std::string` |
| `getStringContent(bool verbose) const noexcept override` | `std::string` |
| `operator==(const GGA&) const noexcept` | `bool` |
| `hasEqualContent(const GGA&) const noexcept` | `bool` |

---

### nmealib::nmea0183::GLL

**File:** `src/nmea0183/gll.hpp`

Geographic position (latitude/longitude), timestamp, status, and mode.

#### Public Constructor

```cpp
GLL(std::string talkerId,
    double latitude,
    char latitudeDirection,
    double longitude,
    char longitudeDirection,
    double timestamp,
    char status,
    char modeIndicator);
```

#### Public Methods

| Method | Return Type |
|--------|-------------|
| `clone() const override` | `std::unique_ptr<nmealib::Message>` |
| `getLatitude() const noexcept` | `double` |
| `getLatitudeDirection() const noexcept` | `char` |
| `getLongitude() const noexcept` | `double` |
| `getLongitudeDirection() const noexcept` | `char` |
| `getTimestamp() const noexcept` | `double` |
| `getStatus() const noexcept` | `char` |
| `getModeIndicator() const noexcept` | `char` |
| `getStringContent(bool verbose) const noexcept override` | `std::string` |
| `operator==(const GLL&) const noexcept` | `bool` |
| `hasEqualContent(const GLL&) const noexcept` | `bool` |

---

### nmealib::nmea0183::GSA

**File:** `src/nmea0183/gsa.hpp`

GPS DOP and active satellites.

Supports both classic GSA payloads and NMEA 4.1+ payloads with optional trailing `System ID`.

#### Public Constructor

```cpp
GSA(std::string talkerId,
    char selectionMode,
    unsigned int mode,
    std::array<unsigned int, 12> satelliteIds,
    double pdop,
    double hdop,
    double vdop,
    std::optional<unsigned int> systemId = std::nullopt);
```

#### Public Methods

| Method | Return Type |
|--------|-------------|
| `clone() const override` | `std::unique_ptr<nmealib::Message>` |
| `getSelectionMode() const noexcept` | `char` |
| `getMode() const noexcept` | `unsigned int` |
| `getSatelliteIds() const noexcept` | `std::array<unsigned int, 12>` |
| `getSatelliteId(size_t index) const noexcept` | `unsigned int` |
| `getPdop() const noexcept` | `double` |
| `getHdop() const noexcept` | `double` |
| `getVdop() const noexcept` | `double` |
| `hasSystemId() const noexcept` | `bool` |
| `getSystemId() const noexcept` | `std::optional<unsigned int>` |
| `getStringContent(bool verbose) const noexcept override` | `std::string` |
| `operator==(const GSA&) const noexcept` | `bool` |
| `hasEqualContent(const GSA&) const noexcept` | `bool` |

---

## Exceptions

### nmealib::NmeaException

**File:** `src/nmeaException.hpp`

```cpp
class NmeaException : public std::runtime_error {
public:
    explicit NmeaException(const std::string& context,
                           const std::string& message,
                           const std::string& details = "");
};
```

### NMEA 0183 generic exceptions

- `TooLongSentenceException`
- `InvalidStartCharacterException`
- `NoChecksumException`
- `NoEndlineException`
- `NotEnoughFieldsException`

### Sentence-specific exceptions

- `NotRMCException`
- `NotGGAException`
- `NotGLLException`
- `NotGSAException`

All inherit from `NmeaException`.

---

## Minimal usage examples

### Parse a sentence

```cpp
#include "nmea0183Factory.hpp"

auto msg = nmealib::nmea0183::Nmea0183Factory::create(
    "$GNGLL,3150.788156,N,11711.922383,E,062735.00,A,A*76\r\n");
```

### Type-dispatch after parse

```cpp
if (auto* rmc = dynamic_cast<nmealib::nmea0183::RMC*>(msg.get())) {
    // use rmc-specific getters
} else if (auto* gga = dynamic_cast<nmealib::nmea0183::GGA*>(msg.get())) {
    // use gga-specific getters
} else if (auto* gll = dynamic_cast<nmealib::nmea0183::GLL*>(msg.get())) {
    // use gll-specific getters
} else if (auto* gsa = dynamic_cast<nmealib::nmea0183::GSA*>(msg.get())) {
    // use gsa-specific getters
}
```

---

## See also

- [Getting Started](Getting-Started.md)
- [NMEA 0183 Guide](NMEA-0183-Guide.md)
- [Examples](Examples.md)
- [Building and Testing](Building-and-Testing.md)
