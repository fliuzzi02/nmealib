# Examples

Practical examples for parsing and working with NMEA 0183 data using `nmealib`.

## Parse a generic sentence

```cpp
#include "nmea0183Factory.hpp"
#include <iostream>

int main() {
    const std::string raw = "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W,A,V*6A\r\n";

    auto msg = nmealib::nmea0183::Nmea0183Factory::create(raw);

    std::cout << "Talker: " << msg->getTalker() << "\n";
    std::cout << "Type: " << msg->getSentenceType() << "\n";
    std::cout << "Valid: " << std::boolalpha << msg->validate() << "\n";
}
```

## Typed dispatch (`RMC`, `GGA`, `GLL`, `GSA`, `VTG`, `ZDA`)

```cpp
#include "nmea0183Factory.hpp"
#include "rmc.hpp"
#include "gga.hpp"
#include "gll.hpp"
#include "gsa.hpp"
#include "vtg.hpp"
#include "zda.hpp"

void handle(const std::string& sentence) {
    auto msg = nmealib::nmea0183::Nmea0183Factory::create(sentence);

    if (auto* rmc = dynamic_cast<nmealib::nmea0183::RMC*>(msg.get())) {
        // RMC fields
        (void)rmc->getUtcFix();
        (void)rmc->getSpeedOverGround();
    } else if (auto* gga = dynamic_cast<nmealib::nmea0183::GGA*>(msg.get())) {
        // GGA fields
        (void)gga->getGpsQuality();
        (void)gga->getSatellites();
    } else if (auto* gll = dynamic_cast<nmealib::nmea0183::GLL*>(msg.get())) {
        // GLL fields
        (void)gll->getStatus();
        (void)gll->getModeIndicator();
    } else if (auto* gsa = dynamic_cast<nmealib::nmea0183::GSA*>(msg.get())) {
        // GSA fields
        (void)gsa->getMode();
        (void)gsa->getPdop();
    } else if (auto* vtg = dynamic_cast<nmealib::nmea0183::VTG*>(msg.get())) {
        // VTG fields
        (void)vtg->getCourseOverGroundTrue();
        (void)vtg->getSpeedOverGroundKnots();
    } else if (auto* zda = dynamic_cast<nmealib::nmea0183::ZDA*>(msg.get())) {
        // ZDA fields
        (void)zda->getUtcTime();
        (void)zda->getYear();
    }
}
```

## Construct from fields and serialize

### RMC

```cpp
#include "rmc.hpp"

nmealib::nmea0183::RMC rmc(
    "GP", 123519, 'A',
    48.1173, 'N',
    11.5166, 'E',
    22.4, 84.4,
    230394,
    3.1, 'W',
    'A', 'V'
);

std::string raw = rmc.serialize();
```

### GGA

```cpp
#include "gga.hpp"

nmealib::nmea0183::GGA gga(
    "GN", 62735.0,
    31.8464692667, 'N',
    117.1987063833, 'E',
    1, 12, 2.0,
    90.0, 'M',
    0.0, 'M',
    0.0, ""
);

std::string raw = gga.serialize();
```

### GLL

```cpp
#include "gll.hpp"

nmealib::nmea0183::GLL gll(
    "GN",
    31.8464692667, 'N',
    117.1987063833, 'E',
    62735.0,
    'A', 'A'
);

std::string raw = gll.serialize();
```

### GSA

```cpp
#include "gsa.hpp"

nmealib::nmea0183::GSA gsa(
    "GN",
    'A',
    3,
    std::array<unsigned int, 12>{80, 71, 73, 79, 69, 0, 0, 0, 0, 0, 0, 0},
    1.83,
    1.09,
    1.47,
    1
);

std::string raw = gsa.serialize();
```

### VTG

```cpp
#include "vtg.hpp"

nmealib::nmea0183::VTG vtg(
    "GP",
    220.86,
    0.0,
    2.550,
    4.724,
    'A'
);

std::string raw = vtg.serialize();
```

### ZDA

```cpp
#include "zda.hpp"

nmealib::nmea0183::ZDA zda(
    "GP",
    160012.71,
    11,
    3,
    2004,
    -1,
    0
);

std::string raw = zda.serialize();
```

## Clone and content comparison

```cpp
auto msg = nmealib::nmea0183::Nmea0183Factory::create(
    "$GNGLL,3150.788156,N,11711.922383,E,062735.00,A,A*76\r\n");

auto cloned = msg->clone();
auto* original0183 = dynamic_cast<nmealib::nmea0183::Message0183*>(msg.get());
auto* clone0183 = dynamic_cast<nmealib::nmea0183::Message0183*>(cloned.get());

bool sameContent = original0183 && clone0183 && original0183->hasEqualContent(*clone0183);
```

## Checksum handling

```cpp
auto msg = nmealib::nmea0183::Nmea0183Factory::create(
    "$GNGGA,062735.00,3150.788156,N,11711.922383,E,1,12,2.0,90.0,M,,M,,*55\r\n");

if (msg->validate()) {
    try {
        auto provided = msg->getChecksumStr();
        auto calculated = msg->getCalculatedChecksumStr();
        (void)provided;
        (void)calculated;
    } catch (const nmealib::nmea0183::NoChecksumException&) {
        // sentence had no checksum
    }
}
```

## Batch parsing with error handling

```cpp
#include <vector>

std::vector<std::string> input = {
    "$GNGLL,3150.788156,N,11711.922383,E,062735.00,A,A*76\r\n",
    "$GNGGA,062735.00,3150.788156,N,11711.922383,E,1,12,2.0,90.0,M,,M,,*55\r\n",
    "INVALID\r\n"
};

for (const auto& line : input) {
    try {
        auto msg = nmealib::nmea0183::Nmea0183Factory::create(line);
        std::cout << msg->getTalker() << msg->getSentenceType() << "\n";
    } catch (const nmealib::NmeaException& e) {
        std::cerr << "nmea error: " << e.what() << "\n";
    }
}
```

## See also

- [Getting Started](Getting-Started.md)
- [API Reference](API-Reference.md)
- [NMEA 0183 Guide](NMEA-0183-Guide.md)
