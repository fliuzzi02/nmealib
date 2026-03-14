#pragma once

#include "message.h"
#include "nmeaException.h"

#include <string>

namespace nmealib {
namespace nmea0183 {

/**
 * @brief Exception thrown when an NMEA 0183 sentence exceeds the maximum allowed length of 82 characters.
 */
class TooLongSentenceException : public NmeaException {
public:
    /**
     * @brief Constructs a TooLongSentenceException.
     *
     * @param context A string identifying where the error occurred.
     * @param details Optional extra information (e.g., actual sentence length).
     */
    explicit TooLongSentenceException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "NMEA 0183 sentence exceeds maximum length of 82 characters", details) {}
};

/**
 * @brief Exception thrown when an NMEA 0183 sentence does not start with '$' or '!'.
 */
class InvalidStartCharacterException : public NmeaException {
public:
    /**
     * @brief Constructs an InvalidStartCharacterException.
     *
     * @param context A string identifying where the error occurred.
     * @param details Optional extra information.
     */
    explicit InvalidStartCharacterException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "NMEA 0183 sentence must start with '$' or '!'", details) {}
};

/**
 * @brief Exception thrown when an operation requires a checksum but none is present in the sentence.
 */
class NoChecksumException : public NmeaException {
public:
    /**
     * @brief Constructs a NoChecksumException.
     *
     * @param context A string identifying where the error occurred.
     * @param details Optional extra information.
     */
    explicit NoChecksumException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "This sentence does not contain a checksum", details) {}
};

/**
 * @brief Exception thrown when an NMEA 0183 sentence is missing the required <CR><LF> terminator.
 */
class NoEndlineException : public NmeaException {
public:
    /**
     * @brief Constructs a NoEndlineException.
     *
     * @param context A string identifying where the error occurred.
     * @param details Optional extra information.
     */
    explicit NoEndlineException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "NMEA 0183 sentence must end with <CR><LF>", details) {}
};

/**
 * @brief Exception thrown when an NMEA 0183 sentence does not contain enough comma-separated fields.
 */
class NotEnoughFieldsException : public NmeaException {
public:
    /**
     * @brief Constructs a NotEnoughFieldsException.
     *
     * @param context A string identifying where the error occurred.
     * @param details Optional extra information (e.g., expected vs. actual field count).
     */
    explicit NotEnoughFieldsException(const std::string& context, const std::string& details = "") :
    NmeaException(context, "NMEA 0183 sentence does not contain enough fields", details) {}
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
    /**
     * @brief Copy constructor.
     */
    Message0183(const Message0183&) = default;

    /**
     * @brief Copy assignment operator.
     */
    Message0183& operator=(const Message0183&) = default;

    /**
     * @brief Move constructor.
     */
    Message0183(Message0183&&) noexcept = default;

    /**
     * @brief Move assignment operator.
     */
    Message0183& operator=(Message0183&&) noexcept = default;

    /**
     * @brief Destructor.
     */
    ~Message0183() override = default;

    /**
     * @brief Creates a polymorphic deep copy of this Message0183.
     *
     * @return std::unique_ptr<nmealib::Message> A unique pointer to the cloned message.
     */
    std::unique_ptr<nmealib::Message> clone() const override;

    /**
     * @brief Returns the start character of the NMEA 0183 sentence.
     *
     * @return char Either '$' (field-delimited) or '!' (encapsulated) sentence start character.
     */
    char getStartChar() const noexcept;

    /**
     * @brief Returns the talker identifier extracted from the sentence.
     *
     * @return std::string A two-character talker ID (e.g., "GP" for GPS, "II" for integrated instrumentation).
     */
    std::string getTalker() const noexcept;

    /**
     * @brief Returns the sentence type identifier extracted from the sentence.
     *
     * @return std::string A three-character sentence type code (e.g., "GGA", "RMC").
     */
    std::string getSentenceType() const noexcept;

    /**
     * @brief Returns the payload of the NMEA 0183 sentence.
     *
     * The payload is the portion of the sentence between the start character and the
     * checksum delimiter '*' (or end of data if no checksum is present).
     *
     * @return std::string The sentence payload string.
     */
    std::string getPayload() const noexcept;

    /**
     * @brief Get the checksum string extracted from the raw sentence.
     *
     * @return std::string The checksum string extracted from the raw NMEA 0183 sentence, if present.
     * @throws NoChecksumException If the sentence does not contain a checksum (i.e., no '*' character followed by two hex digits).
     */
    std::string getChecksumStr() const;

    /**
     * @brief Get the calculated checksum string for the sentence payload.
     *
     * @return std::string The checksum string calculated from the payload of the NMEA 0183 sentence, represented as a two-digit hexadecimal string.
     */
    std::string getCalculatedChecksumStr() const noexcept;

    /**
     * @brief Returns a human-readable string representation of the message content.
     *
     * @param verbose Selects whether to print a one-liner or a more detailed multi-line string with field names and values.
     * @return std::string The string representation of the message content
     */
    virtual std::string getStringContent(bool verbose) const noexcept;

    /**
     * @brief Returns the wire-format representation of the NMEA 0183 sentence,
     * that is, the raw information that was passed when the message was created.
     *
     * @return std::string A string with the wire-format
     */
    std::string serialize() const override;

    /**
     * @brief Compares two Message0183 objects for equality based on their content and timestamp.
     *
     * @param other The other Message0183 object to compare with.
     * @return true  If all fields and the base Message data (including timestamp) are equal.
     * @return false Otherwise.
     */
    bool operator==(const Message0183& other) const noexcept;

    /**
     * @brief Returns whether the message is valid or not.
     *
     * @return true If there is no checksum or if the checksum matches the calculated checksum for the payload
     * @return false If there is a checksum and it does not match the calculated checksum for the payload
     */
    bool validate() const noexcept override;

    /**
     * @brief Converts an NMEA coordinate in ddmm.mmmm / dddmm.mmmm format to decimal degrees.
     *
     * @param nmeaCoordinate Coordinate field as received from the sentence payload.
     * @return double Decimal degrees representation.
     */
    static double convertNmeaCoordinateToDecimalDegrees(const std::string& nmeaCoordinate);

protected:
    char startChar_;
    std::string talker_;
    std::string sentenceType_;
    std::string payload_;
    std::string checksumStr_;
    std::string calculatedChecksumStr_;

    /**
     * @brief Protected factory method to create a Message0183 from a raw sentence string.
     *
     * Validates format and parses the raw sentence. Intended to be called from
     * Nmea0183Factory or derived-class factories.
     *
     * @param raw The raw NMEA 0183 sentence string to parse and validate.
     * @param ts  Optional timestamp; defaults to the current system time.
     * @return std::unique_ptr<Message0183> A unique pointer to the created Message0183 instance.
     * @throws TooLongSentenceException If the input string exceeds the maximum allowed length of 82 characters.
     * @throws InvalidStartCharacterException If the input string does not start with either '$' or '!'.
     */
    static std::unique_ptr<Message0183> create(const std::string& raw, TimePoint ts = std::chrono::system_clock::now());

    /**
     * @brief Provides the common string representation for every NMEA 0183 message, which can be used by derived classes to implement their own getStringContent method.
     * 
     * @param verbose Selects wether to print a one-liner or a more detailed multi-line string with field names and values.
     * @return std::string the head of the stringified message
     */
    std::string toString(bool verbose) const noexcept;

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
    static bool validateFormat(const std::string& context, const std::string& raw);

    friend class Nmea0183Factory;
};

} // namespace nmea0183
} // namespace nmealib
