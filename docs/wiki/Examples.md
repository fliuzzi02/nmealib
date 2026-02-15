# Examples

Practical examples demonstrating various use cases of nmealib.

## Table of Contents

- [Basic Parsing](#basic-parsing)
- [Batch Processing](#batch-processing)
- [Stream Processing](#stream-processing)
- [Error Handling](#error-handling)
- [Message Validation](#message-validation)
- [Custom Message Handler](#custom-message-handler)
- [Data Extraction](#data-extraction)
- [Real-Time GPS Logger](#real-time-gps-logger)

## Basic Parsing

### Simple Sentence Parsing

```cpp
#include "nmea0183/nmea0183.hpp"
#include <iostream>

int main() {
    std::string sentence = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n";
    
    try {
        auto msg = nmealib::nmea0183::Message0183::create(sentence);
        
        std::cout << "Successfully parsed: " << msg->getTalker() 
                  << msg->getSentenceType() << std::endl;
        std::cout << "Payload: " << msg->getPayload() << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Parse error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
```

### Extract Components

```cpp
#include "nmea0183/nmea0183.hpp"
#include <iostream>

void printMessageDetails(const nmealib::nmea0183::Message0183& msg) {
    std::cout << "─────────────────────────────────────" << std::endl;
    std::cout << "Start Character: " << msg.getStartChar() << std::endl;
    std::cout << "Talker ID:       " << msg.getTalker() << std::endl;
    std::cout << "Sentence Type:   " << msg.getSentenceType() << std::endl;
    std::cout << "Payload:         " << msg.getPayload() << std::endl;
    
    try {
        std::cout << "Checksum:        " << msg.getChecksumStr() << std::endl;
        std::cout << "Calculated:      " << msg.getCalculatedChecksumStr() << std::endl;
    } catch (const nmealib::nmea0183::NoChecksumException&) {
        std::cout << "Checksum:        (none)" << std::endl;
    }
    
    std::cout << "Valid:           " << (msg.validate() ? "Yes" : "No") << std::endl;
    std::cout << "Raw Data:        " << msg.getRawData() << std::endl;
    std::cout << "─────────────────────────────────────" << std::endl;
}

int main() {
    std::string sentence = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n";
    auto msg = nmealib::nmea0183::Message0183::create(sentence);
    printMessageDetails(*msg);
    return 0;
}
```

## Batch Processing

### Process Multiple Sentences

```cpp
#include "nmea0183/nmea0183.hpp"
#include <iostream>
#include <vector>
#include <string>

int main() {
    std::vector<std::string> sentences = {
        "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n",
        "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A\r\n",
        "$GPGLL,4807.038,N,01131.000,E,123519,A,*1D\r\n"
    };
    
    std::vector<std::unique_ptr<nmealib::nmea0183::Message0183>> messages;
    
    for (const auto& sentence : sentences) {
        try {
            auto msg = nmealib::nmea0183::Message0183::create(sentence);
            messages.push_back(std::move(msg));
        } catch (const std::exception& e) {
            std::cerr << "Failed to parse: " << sentence << std::endl;
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
    
    std::cout << "Successfully parsed " << messages.size() 
              << " out of " << sentences.size() << " sentences" << std::endl;
    
    // Process parsed messages
    for (const auto& msg : messages) {
        std::cout << msg->getTalker() << msg->getSentenceType() 
                  << " - Valid: " << msg->validate() << std::endl;
    }
    
    return 0;
}
```

### Filter by Sentence Type

```cpp
#include "nmea0183/nmea0183.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

std::vector<std::unique_ptr<nmealib::nmea0183::Message0183>>
filterBySentenceType(
    const std::vector<std::unique_ptr<nmealib::nmea0183::Message0183>>& messages,
    const std::string& sentenceType)
{
    std::vector<std::unique_ptr<nmealib::nmea0183::Message0183>> filtered;
    
    for (const auto& msg : messages) {
        if (msg->getSentenceType() == sentenceType) {
            filtered.push_back(std::unique_ptr<nmealib::nmea0183::Message0183>(
                dynamic_cast<nmealib::nmea0183::Message0183*>(msg->clone().release())
            ));
        }
    }
    
    return filtered;
}

int main() {
    // Parse multiple messages...
    std::vector<std::unique_ptr<nmealib::nmea0183::Message0183>> allMessages;
    // ... (populate allMessages)
    
    // Get only GGA messages
    auto ggaMessages = filterBySentenceType(allMessages, "GGA");
    
    std::cout << "Found " << ggaMessages.size() << " GGA messages" << std::endl;
    
    return 0;
}
```

## Stream Processing

### Read from File

```cpp
#include "nmea0183/nmea0183.hpp"
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <nmea_file>" << std::endl;
        return 1;
    }
    
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << argv[1] << std::endl;
        return 1;
    }
    
    std::string line;
    int lineNumber = 0;
    int validCount = 0;
    int invalidCount = 0;
    
    while (std::getline(file, line)) {
        lineNumber++;
        
        // NMEA sentences need \r\n terminator
        if (!line.empty() && line.back() == '\r') {
            line += '\n';
        } else {
            line += "\r\n";
        }
        
        try {
            auto msg = nmealib::nmea0183::Message0183::create(line);
            
            if (msg->validate()) {
                validCount++;
                std::cout << "Line " << lineNumber << ": " 
                          << msg->getTalker() << msg->getSentenceType() 
                          << " [VALID]" << std::endl;
            } else {
                invalidCount++;
                std::cout << "Line " << lineNumber << ": "
                          << msg->getTalker() << msg->getSentenceType() 
                          << " [INVALID CHECKSUM]" << std::endl;
            }
            
        } catch (const std::exception& e) {
            invalidCount++;
            std::cerr << "Line " << lineNumber << ": Parse error - " 
                      << e.what() << std::endl;
        }
    }
    
    std::cout << "\nSummary:" << std::endl;
    std::cout << "  Total lines: " << lineNumber << std::endl;
    std::cout << "  Valid:       " << validCount << std::endl;
    std::cout << "  Invalid:     " << invalidCount << std::endl;
    
    return 0;
}
```

### Read from stdin

```cpp
#include "nmea0183/nmea0183.hpp"
#include <iostream>
#include <string>

int main() {
    std::cout << "Enter NMEA sentences (Ctrl+D to end):" << std::endl;
    
    std::string line;
    while (std::getline(std::cin, line)) {
        // Add CRLF if not present
        if (line.find("\r\n") == std::string::npos) {
            line += "\r\n";
        }
        
        try {
            auto msg = nmealib::nmea0183::Message0183::create(line);
            
            std::cout << "✓ " << msg->getTalker() << msg->getSentenceType();
            if (msg->validate()) {
                std::cout << " [VALID]" << std::endl;
            } else {
                std::cout << " [CHECKSUM MISMATCH]" << std::endl;
            }
            
        } catch (const std::exception& e) {
            std::cout << "✗ Parse error: " << e.what() << std::endl;
        }
    }
    
    return 0;
}
```

## Error Handling

### Comprehensive Error Handling

```cpp
#include "nmea0183/nmea0183.hpp"
#include <iostream>
#include <string>

enum class ParseResult {
    Success,
    TooLong,
    InvalidStart,
    NoEndline,
    OtherError
};

ParseResult tryParse(const std::string& sentence, 
                     std::unique_ptr<nmealib::nmea0183::Message0183>& outMsg) {
    try {
        outMsg = nmealib::nmea0183::Message0183::create(sentence);
        return ParseResult::Success;
        
    } catch (const nmealib::nmea0183::TooLongSentenceException& e) {
        std::cerr << "Error: Sentence too long - " << e.what() << std::endl;
        return ParseResult::TooLong;
        
    } catch (const nmealib::nmea0183::InvalidStartCharacterException& e) {
        std::cerr << "Error: Invalid start character - " << e.what() << std::endl;
        return ParseResult::InvalidStart;
        
    } catch (const nmealib::nmea0183::NoEndlineException& e) {
        std::cerr << "Error: Missing endline - " << e.what() << std::endl;
        return ParseResult::NoEndline;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return ParseResult::OtherError;
    }
}

int main() {
    std::vector<std::string> testSentences = {
        "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n",  // Valid
        "GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n",   // Missing $
        "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47",      // Missing \r\n
        std::string(100, 'A') + "\r\n"                                              // Too long
    };
    
    for (size_t i = 0; i < testSentences.size(); ++i) {
        std::cout << "\nTest " << (i + 1) << ": ";
        
        std::unique_ptr<nmealib::nmea0183::Message0183> msg;
        ParseResult result = tryParse(testSentences[i], msg);
        
        if (result == ParseResult::Success) {
            std::cout << "SUCCESS - " << msg->getTalker() 
                      << msg->getSentenceType() << std::endl;
        }
    }
    
    return 0;
}
```

## Message Validation

### Validate with Custom Logic

```cpp
#include "nmea0183/nmea0183.hpp"
#include <iostream>
#include <string>

struct ValidationResult {
    bool formatValid;
    bool checksumValid;
    bool contentValid;
    std::string message;
};

ValidationResult validateMessage(const std::string& sentence) {
    ValidationResult result{false, false, false, ""};
    
    try {
        auto msg = nmealib::nmea0183::Message0183::create(sentence);
        result.formatValid = true;
        
        // Check checksum
        result.checksumValid = msg->validate();
        
        // Custom content validation
        std::string talker = msg->getTalker();
        std::string type = msg->getSentenceType();
        
        // Example: Only accept GPS GGA and RMC messages
        if (talker == "GP" && (type == "GGA" || type == "RMC")) {
            result.contentValid = true;
            result.message = "Valid GPS fix data";
        } else {
            result.message = "Unsupported sentence type: " + talker + type;
        }
        
    } catch (const std::exception& e) {
        result.message = std::string("Parse error: ") + e.what();
    }
    
    return result;
}

int main() {
    std::string sentence = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n";
    
    auto result = validateMessage(sentence);
    
    std::cout << "Format valid:   " << (result.formatValid ? "Yes" : "No") << std::endl;
    std::cout << "Checksum valid: " << (result.checksumValid ? "Yes" : "No") << std::endl;
    std::cout << "Content valid:  " << (result.contentValid ? "Yes" : "No") << std::endl;
    std::cout << "Message:        " << result.message << std::endl;
    
    return 0;
}
```

## Custom Message Handler

### Message Handler Class

```cpp
#include "nmea0183/nmea0183.hpp"
#include <iostream>
#include <string>
#include <map>
#include <functional>

class NmeaMessageHandler {
public:
    using HandlerFunc = std::function<void(const nmealib::nmea0183::Message0183&)>;
    
    void registerHandler(const std::string& sentenceType, HandlerFunc handler) {
        handlers_[sentenceType] = handler;
    }
    
    void process(const std::string& sentence) {
        try {
            auto msg = nmealib::nmea0183::Message0183::create(sentence);
            
            if (!msg->validate()) {
                std::cerr << "Warning: Invalid checksum for " 
                          << msg->getTalker() << msg->getSentenceType() << std::endl;
            }
            
            auto it = handlers_.find(msg->getSentenceType());
            if (it != handlers_.end()) {
                it->second(*msg);
            } else {
                defaultHandler_(*msg);
            }
            
        } catch (const std::exception& e) {
            std::cerr << "Failed to process: " << e.what() << std::endl;
        }
    }
    
    void setDefaultHandler(HandlerFunc handler) {
        defaultHandler_ = handler;
    }
    
private:
    std::map<std::string, HandlerFunc> handlers_;
    HandlerFunc defaultHandler_ = [](const nmealib::nmea0183::Message0183& msg) {
        std::cout << "Unhandled: " << msg.getTalker() << msg.getSentenceType() << std::endl;
    };
};

void handleGGA(const nmealib::nmea0183::Message0183& msg) {
    std::cout << "Processing GGA (GPS Fix): " << msg.getPayload() << std::endl;
}

void handleRMC(const nmealib::nmea0183::Message0183& msg) {
    std::cout << "Processing RMC (Recommended Minimum): " << msg.getPayload() << std::endl;
}

int main() {
    NmeaMessageHandler handler;
    
    // Register specific handlers
    handler.registerHandler("GGA", handleGGA);
    handler.registerHandler("RMC", handleRMC);
    
    // Process messages
    handler.process("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n");
    handler.process("$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A\r\n");
    handler.process("$GPGLL,4807.038,N,01131.000,E,123519,A,*1D\r\n");
    
    return 0;
}
```

## Data Extraction

### Parse Coordinate Data

```cpp
#include "nmea0183/nmea0183.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

struct Coordinates {
    double latitude;
    double longitude;
    bool valid;
};

std::vector<std::string> splitPayload(const std::string& payload) {
    std::vector<std::string> fields;
    std::stringstream ss(payload);
    std::string field;
    
    while (std::getline(ss, field, ',')) {
        fields.push_back(field);
    }
    
    return fields;
}

double convertToDecimalDegrees(const std::string& coord, bool isLongitude) {
    if (coord.empty()) return 0.0;
    
    size_t dotPos = coord.find('.');
    if (dotPos == std::string::npos) return 0.0;
    
    int degreeDigits = isLongitude ? 3 : 2;
    double degrees = std::stod(coord.substr(0, degreeDigits));
    double minutes = std::stod(coord.substr(degreeDigits));
    
    return degrees + (minutes / 60.0);
}

Coordinates extractCoordinatesFromGGA(const nmealib::nmea0183::Message0183& msg) {
    Coordinates coords{0.0, 0.0, false};
    
    if (msg.getSentenceType() != "GGA") {
        return coords;
    }
    
    auto fields = splitPayload(msg.getPayload());
    
    // GGA format: $GPGGA,time,lat,N/S,lon,E/W,quality,...
    if (fields.size() < 7) {
        return coords;
    }
    
    try {
        coords.latitude = convertToDecimalDegrees(fields[2], false);
        if (fields[3] == "S") coords.latitude = -coords.latitude;
        
        coords.longitude = convertToDecimalDegrees(fields[4], true);
        if (fields[5] == "W") coords.longitude = -coords.longitude;
        
        coords.valid = true;
    } catch (...) {
        coords.valid = false;
    }
    
    return coords;
}

int main() {
    std::string sentence = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n";
    auto msg = nmealib::nmea0183::Message0183::create(sentence);
    
    auto coords = extractCoordinatesFromGGA(*msg);
    
    if (coords.valid) {
        std::cout << "Latitude:  " << coords.latitude << "°" << std::endl;
        std::cout << "Longitude: " << coords.longitude << "°" << std::endl;
    } else {
        std::cout << "Failed to extract coordinates" << std::endl;
    }
    
    return 0;
}
```

## Real-Time GPS Logger

### Simple GPS Data Logger

```cpp
#include "nmea0183/nmea0183.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <ctime>

class GpsLogger {
public:
    explicit GpsLogger(const std::string& logFile) 
        : logFile_(logFile), file_(logFile, std::ios::app) {
        
        if (!file_.is_open()) {
            throw std::runtime_error("Failed to open log file: " + logFile);
        }
        
        logMessage("GPS Logger started");
    }
    
    ~GpsLogger() {
        logMessage("GPS Logger stopped");
    }
    
    void processNmeaSentence(const std::string& sentence) {
        try {
            auto msg = nmealib::nmea0183::Message0183::create(sentence);
            
            // Log only if valid
            if (msg->validate()) {
                logNmeaMessage(*msg);
                sentenceCount_++;
            } else {
                invalidCount_++;
            }
            
        } catch (const std::exception& e) {
            errorCount_++;
            logError(sentence, e.what());
        }
    }
    
    void printStatistics() const {
        std::cout << "\nStatistics:" << std::endl;
        std::cout << "  Valid sentences:   " << sentenceCount_ << std::endl;
        std::cout << "  Invalid checksums: " << invalidCount_ << std::endl;
        std::cout << "  Parse errors:      " << errorCount_ << std::endl;
    }
    
private:
    void logMessage(const std::string& message) {
        file_ << getCurrentTimestamp() << " [INFO] " << message << std::endl;
    }
    
    void logNmeaMessage(const nmealib::nmea0183::Message0183& msg) {
        file_ << getCurrentTimestamp() << " [NMEA] " 
              << msg.getTalker() << msg.getSentenceType() << " " 
              << msg.getPayload() << std::endl;
    }
    
    void logError(const std::string& sentence, const std::string& error) {
        file_ << getCurrentTimestamp() << " [ERROR] " 
              << error << " - " << sentence << std::endl;
    }
    
    std::string getCurrentTimestamp() const {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto tm = *std::localtime(&time_t);
        
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
    
    std::string logFile_;
    std::ofstream file_;
    size_t sentenceCount_ = 0;
    size_t invalidCount_ = 0;
    size_t errorCount_ = 0;
};

int main() {
    try {
        GpsLogger logger("gps_log.txt");
        
        // Simulate receiving NMEA sentences
        std::vector<std::string> sentences = {
            "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n",
            "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A\r\n",
            "$GPGLL,4807.038,N,01131.000,E,123519,A,*1D\r\n"
        };
        
        for (const auto& sentence : sentences) {
            logger.processNmeaSentence(sentence);
        }
        
        logger.printStatistics();
        
    } catch (const std::exception& e) {
        std::cerr << "Logger error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
```

## See Also

- [Getting Started](Getting-Started.md)
- [API Reference](API-Reference.md)
- [NMEA 0183 Guide](NMEA-0183-Guide.md)
- [Building and Testing](Building-and-Testing.md)
