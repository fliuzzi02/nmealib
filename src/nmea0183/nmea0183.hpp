#pragma once

#include "../message.hpp"
#include "../nmeaException.hpp"

#include <string>

namespace nmealib {
namespace nmea0183 {

class TooLongSentenceException : public NmeaException {
public:
    explicit TooLongSentenceException(const std::string& context, const std::string& details = "") : 
    NmeaException(context, "NMEA 0183 sentence exceeds maximum length of 82 characters", details) {}
};

class InvalidStartCharacterException : public NmeaException {
public:
    explicit InvalidStartCharacterException(const std::string& context, const std::string& details = "") : 
    NmeaException(context, "NMEA 0183 sentence must start with '$' or '!'", details) {}
};

class NoChecksumException : public NmeaException {
public:
    explicit NoChecksumException(const std::string& context, const std::string& details = "") : 
    NmeaException(context, "This sentence does not contain a checksum", details) {}
};

class NoEndlineException : public NmeaException {
public:
    explicit NoEndlineException(const std::string& context, const std::string& details = "") : 
    NmeaException(context, "NMEA 0183 sentence must end with <CR><LF>", details) {}
};

/**
 * @brief Represents an NMEA 0183 sentence
 * @attention NMEA 0183 sentences are ASCII text strings that follow a specific format:
 *      - Messages have a maximum length of 82 characters, including the $ or ! starting character and the ending <LF>
 *      - The start character for each message can be either a $ (For conventional field delimited messages) or ! (for messages that have special encapsulation in them)
 *      - The next five characters identify the talker (two characters) and the type of message (three characters).
 *      - All data fields that follow are comma-delimited.
 *      - Where data is unavailable, the corresponding field remains blank
 *      - The first character that immediately follows the last data field character is an asterisk, but it is only included if a checksum is supplied.
 *      - The asterisk is immediately followed by a checksum represented as a two-digit hexadecimal number.
 *      - <CR><LF> ends the message.
 */
class Message0183 : public nmealib::Message {
public:
    // Factory method
    /**
     * @brief Creates a Message0183 instance from a raw NMEA 0183 sentence string. 
     * This method performs validation on the input string, ensuring it adheres to the NMEA 0183 format and constraints. 
     * If the input is valid, it returns a unique pointer to a Message0183 object; otherwise, 
     * it throws an appropriate exception indicating the reason for failure.
     * 
     * @param raw The raw NMEA 0183 sentence string to parse and validate.
     * @return std::unique_ptr<Message0183> A unique pointer to the created Message0183 instance if the input is valid.
     * @throws TooLongSentenceException If the input string exceeds the maximum allowed length of 82 characters.
     * @throws InvalidStartCharacterException If the input string does not start with either '$' or '!'.
     * @throws NoEndlineException If the input string does not end with the required <CR><LF> sequence.
     */
    static std::unique_ptr<Message0183> create(const std::string& raw, 
                                               TimePoint ts = std::chrono::system_clock::now());

    // Accessory constructors
    Message0183(const Message0183&) = default;
    Message0183& operator=(const Message0183&) = default;
    Message0183(Message0183&&) noexcept = default;
    Message0183& operator=(Message0183&&) noexcept = default;

    // Destructor
    ~Message0183() override = default;

    // Polymorphic copy
    std::unique_ptr<nmealib::Message> clone() const override;
    
    char getStartChar() const noexcept;
    std::string getTalker() const noexcept;
    std::string getSentenceType() const noexcept;
    std::string getPayload() const noexcept;
    /**
     * @brief Get the Checksum Str object
     * 
     * @return std::string The checksum string extracted from the raw NMEA 0183 sentence, if present.
     * @throws NoChecksumException If the sentence does not contain a checksum (i.e., no '*' character followed by two hex digits).
     */
    std::string getChecksumStr() const;
    /**
     * @brief Get the Calculated Checksum Str object
     * 
     * @return std::string The checksum string calculated from the payload of the NMEA 0183 sentence, represented as a two-digit hexadecimal string.
     */
    std::string getCalculatedChecksumStr() const noexcept;
    
    /**
     * @brief Returns a human-readable string representation of the message content.
     * 
     * @param verbose Selects wheter to print an one-liner or a more detailed multi-line string with field names and values.
     * @return std::string The string representation of the message content 
     */
    virtual std::string getStringContent(bool verbose) const noexcept;

    /**
     * @brief Returns the wire-format representation of the NMEA 0183 sentence, 
     * that is, the raw information that was passed when the message was created.
     * 
     * @return std::string A string with thw wire-format
     */
    std::string serialize() const override;

    // Override of the == operator to compare Message0183 objects based on their content rather than their memory addresses.
    bool operator==(const Message0183& other) const noexcept {
        return startChar_ == other.startChar_ &&
               talker_ == other.talker_ &&
               sentenceType_ == other.sentenceType_ &&
               payload_ == other.payload_ &&
               checksumStr_ == other.checksumStr_ &&
               calculatedChecksumStr_ == other.calculatedChecksumStr_ &&
               Message::operator==(other);
    }

    /**
     * @brief Compares the content of the message only, ignoring the timestamp
     * 
     * @param other The other Message0183 object to compare with
     * @return true if the content of both messages is equal (start char, talker, sentence type, payload, checksum string, and calculated checksum string), regardless of their timestamps;
     * @return false otherwise
     */
    bool hasEqualContent(const Message0183& other) const noexcept {
        return startChar_ == other.startChar_ &&
               talker_ == other.talker_ &&
               sentenceType_ == other.sentenceType_ &&
               payload_ == other.payload_ &&
               checksumStr_ == other.checksumStr_ &&
               calculatedChecksumStr_ == other.calculatedChecksumStr_;
    }

    /**
     * @brief Returns wheter the message is valid or not
     * 
     * @return true If there is no checksum or if the checksum matches the calculated checksum for the payload
     * @return false if there is a checksum and it does not match the calculated checksum for the payload
     */
    bool validate() const override;

protected:
    char startChar_; // '$' or '!'
    std::string talker_; // e.g. "GP", "II", etc. (first two chars of payload)
    std::string sentenceType_; // e.g. "GGA", "RMC", etc. (chars 3-5 of payload)
    std::string payload_; // the part between startChar and * (or between startChar and CRLF if no checksum)
    std::string checksumStr_; // the two hex digits after '*', if present
    std::string calculatedChecksumStr_; // the checksum computed from the payload, as a two-digit hex string

private:
    explicit Message0183(std::string raw,
                        TimePoint ts,
                        char startChar,
                        std::string talker,
                        std::string sentenceType,
                        std::string payload) noexcept;
    explicit Message0183(std::string raw,
                        TimePoint ts,
                        char startChar,
                        std::string talker,
                        std::string sentenceType,
                        std::string payload,
                        std::string checksumStr) noexcept;
    
    static std::string computeChecksum(const std::string& payload) noexcept;
    static bool isHexByte(const std::string& s) noexcept;
};

} // namespace nmea0183
} // namespace nmealib