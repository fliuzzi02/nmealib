#include <gtest/gtest.h>

#include <chrono>
#include <memory>
#include <string>

#include <nmealib.h>

using namespace nmealib::nmea2000;

// Valid CAN frames for testing
// Format: CANID:data (CANID in hex, data in hex pairs)
// Note: PGN is extracted from CAN ID bits as: (CANID >> 8) & 0x3FFFF
static const std::string VALID_FRAME_1 = "18FD1234:0102030405060708";
static const std::string VALID_FRAME_2 = "18FD25B6:AABBCCDD";
static const std::string VALID_FRAME_EMPTY = "18FD0000:";
static const std::string VALID_FRAME_SINGLE_BYTE = "18FD5A12:FF";

// Multi-frame test data (9-223 bytes of payload)
static const std::string VALID_FRAME_9_BYTES = "18FD1234:0102030405060708FF";
static const std::string VALID_FRAME_16_BYTES = "18FD1234:000102030405060708090A0B0C0D0E0F";
static const std::string VALID_FRAME_MAX_223_BYTES = "18FD1234:" + std::string(446, 'A');  // 223 bytes in hex

// Test basic creation and accessors
TEST(Message2000, CreateSucceedsWithValidPduFrame)
{
    auto msg = Nmea2000Factory::create(VALID_FRAME_1);
    ASSERT_NE(msg, nullptr);
    
    // PGN should be extracted correctly from CAN ID 0x18FD1234
    // PGN is bits 24-8 of the CAN ID: (0x18FD1234 >> 8) & 0x3FFFF = 0xFD12
    EXPECT_EQ(msg->getPgn(), 0xFD12U);
    EXPECT_EQ(msg->getCanFrameLength(), 8U);
    
    const auto& frame = msg->getCanFrame();
    EXPECT_EQ(frame.size(), 8U);
    EXPECT_EQ(frame[0], 0x01);
    EXPECT_EQ(frame[1], 0x02);
    EXPECT_EQ(frame[7], 0x08);
}

TEST(Message2000, CreateSucceedsWithShorterFrame)
{
    auto msg = Nmea2000Factory::create(VALID_FRAME_2);
    ASSERT_NE(msg, nullptr);
    
    EXPECT_EQ(msg->getPgn(), 0xFD25U);
    EXPECT_EQ(msg->getCanFrameLength(), 4U);
    
    const auto& frame = msg->getCanFrame();
    EXPECT_EQ(frame.size(), 4U);
    EXPECT_EQ(frame[0], 0xAA);
    EXPECT_EQ(frame[3], 0xDD);
}

TEST(Message2000, CreateSucceedsWithEmptyFrame)
{
    auto msg = Nmea2000Factory::create(VALID_FRAME_EMPTY);
    ASSERT_NE(msg, nullptr);
    
    EXPECT_EQ(msg->getPgn(), 0xFD00U);
    EXPECT_EQ(msg->getCanFrameLength(), 0U);
    EXPECT_EQ(msg->getCanFrame().size(), 0U);
}

TEST(Message2000, CreateSucceedsWithSingleByteFrame)
{
    auto msg = Nmea2000Factory::create(VALID_FRAME_SINGLE_BYTE);
    ASSERT_NE(msg, nullptr);
    
    EXPECT_EQ(msg->getCanFrameLength(), 1U);
    EXPECT_EQ(msg->getCanFrame()[0], 0xFF);
}

// Test invalid CAN frame formats
TEST(Message2000, CreateThrowsOnMissingColon)
{
    std::string noColon = "18FF12340102030405060708";
    EXPECT_THROW(Nmea2000Factory::create(noColon), InvalidCanFrameException);
}

TEST(Message2000, CreateThrowsOnInvalidHexCanId)
{
    std::string invalidHex = "ZZZZ1234:0102030405060708";
    EXPECT_THROW(Nmea2000Factory::create(invalidHex), InvalidCanFrameException);
}

TEST(Message2000, CreateThrowsOnInvalidHexData)
{
    std::string invalidDataHex = "18FF1234:0102030ZZ060708";
    EXPECT_THROW(Nmea2000Factory::create(invalidDataHex), InvalidCanFrameException);
}

TEST(Message2000, CreateThrowsOnOddLengthHexData)
{
    std::string oddLength = "18FF1234:010203040506070";
    EXPECT_THROW(Nmea2000Factory::create(oddLength), InvalidCanFrameException);
}

// Test frame length validation
TEST(Message2000, CreateThrowsOnFrameTooLong)
{
    // 224 bytes exceeds the 223-byte limit for fast-packet frames
    std::string tooLong = "18FD1234:" + std::string(448, 'A');  // 224 bytes in hex
    EXPECT_THROW(Nmea2000Factory::create(tooLong), FrameTooLongException);
}

// Test serialization
TEST(Message2000, SerializeProducesHexString)
{
    auto msg = Nmea2000Factory::create(VALID_FRAME_1);
    ASSERT_NE(msg, nullptr);
    
    std::string serialized = msg->serialize();
    // Should contain PGN and frame data in hex format
    EXPECT_NE(serialized.find("fd12"), std::string::npos);  // PGN in lowercase hex
}

// Test clone functionality
TEST(Message2000, CloneCreatesIndependentCopy)
{
    auto original = Nmea2000Factory::create(VALID_FRAME_1);
    ASSERT_NE(original, nullptr);
    
    auto cloned = original->clone();
    ASSERT_NE(cloned, nullptr);
    
    // Both should be equal
    EXPECT_EQ(*original, *cloned);
    
    // But they should be different objects
    EXPECT_NE(original.get(), cloned.get());
}

// Test equality operators
TEST(Message2000, EqualityComparison)
{
    using Clock = std::chrono::system_clock;
    auto ts = Clock::now();

    auto msg1 = Nmea2000Factory::create(VALID_FRAME_1, ts);
    auto msg2 = Nmea2000Factory::create(VALID_FRAME_1, ts);
    auto msg3 = Nmea2000Factory::create(VALID_FRAME_2, ts);
    
    ASSERT_NE(msg1, nullptr);
    ASSERT_NE(msg2, nullptr);
    ASSERT_NE(msg3, nullptr);
    
    // Same content and timestamp messages should be equal
    EXPECT_EQ(*msg1, *msg2);
    
    // Different content messages should not be equal
    EXPECT_NE(*msg1, *msg3);
}

// Test validation
TEST(Message2000, ValidateReturnsTrueForValidMessage)
{
    auto msg = Nmea2000Factory::create(VALID_FRAME_1);
    ASSERT_NE(msg, nullptr);
    EXPECT_TRUE(msg->validate());
}

TEST(Message2000, GetStringContentVerbose)
{
    auto msg = Nmea2000Factory::create(VALID_FRAME_1);
    ASSERT_NE(msg, nullptr);
    
    std::string verbose = msg->getStringContent(true);
    EXPECT_NE(verbose.find("PGN"), std::string::npos);
    EXPECT_NE(verbose.find("Frame Length"), std::string::npos);
    EXPECT_NE(verbose.find("Frame Data"), std::string::npos);
}

TEST(Message2000, GetStringContentConcise)
{
    auto msg = Nmea2000Factory::create(VALID_FRAME_1);
    ASSERT_NE(msg, nullptr);
    
    std::string concise = msg->getStringContent(false);
    EXPECT_NE(concise.find("PGN"), std::string::npos);
    EXPECT_NE(concise.find("Len"), std::string::npos);
}

// Test message type
TEST(Message2000, MessageTypeIsNMEA2000)
{
    auto msg = Nmea2000Factory::create(VALID_FRAME_1);
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getType(), nmealib::Message::Type::NMEA2000);
}

// Test raw data preservation
TEST(Message2000, RawDataIsPreserved)
{
    auto msg = Nmea2000Factory::create(VALID_FRAME_1);
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getRawData(), VALID_FRAME_1);
}

// Test multi-frame support (9-223 bytes)
TEST(Message2000, CreateSucceedsWithMultiFrameNineBytes)
{
    auto msg = Nmea2000Factory::create(VALID_FRAME_9_BYTES);
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getCanFrameLength(), 9U);
    EXPECT_TRUE(msg->validate());
}

TEST(Message2000, CreateSucceedsWithMultiFrame16Bytes)
{
    auto msg = Nmea2000Factory::create(VALID_FRAME_16_BYTES);
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getCanFrameLength(), 16U);
    EXPECT_TRUE(msg->validate());
}

TEST(Message2000, CreateSucceedsWithMaximumSize223Bytes)
{
    auto msg = Nmea2000Factory::create(VALID_FRAME_MAX_223_BYTES);
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getCanFrameLength(), 223U);
    EXPECT_TRUE(msg->validate());
}

// Test edge case PGNs
TEST(Message2000, AcceptsZeroPgn)
{
    // CAN ID with PGN = 0x000000
    std::string frameZeroPgn = "18000012:0102030405060708";
    auto msg = Nmea2000Factory::create(frameZeroPgn);
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getPgn(), 0x0000U);
}

TEST(Message2000, AcceptsMaximumPgn)
{
    // CAN ID with PGN = 0x3FFFF (maximum 18-bit value)
    // CAN ID = (0x3FFFF << 8) | 0x12 = 0x3FFFF12
    std::string frameMaxPgn = "3FFFF12:0102030405060708";
    auto msg = Nmea2000Factory::create(frameMaxPgn);
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getPgn(), 0x3FFFFU);
}

TEST(Message2000, AcceptsArbitraryPgnsInRange)
{
    // Test various PGNs that were previously rejected
    std::string frame1 = "18F00012:0102";
    std::string frame2 = "18100012:AABB";
    
    auto msg1 = Nmea2000Factory::create(frame1);
    auto msg2 = Nmea2000Factory::create(frame2);
    
    ASSERT_NE(msg1, nullptr);
    ASSERT_NE(msg2, nullptr);
    EXPECT_TRUE(msg1->validate());
    EXPECT_TRUE(msg2->validate());
}

// Test alternative format support (space-separated and comma-separated)
TEST(Message2000, ParsesCommaSeparatedFormatWithHexPrefix)
{
    // Format: 0xCANID, 0xBB 0xCC ...
    std::string frame = "0x18FD1234, 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08";
    auto msg = Nmea2000Factory::create(frame);
    
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getPgn(), 0xFD12U);
    EXPECT_EQ(msg->getCanFrameLength(), 8U);
    
    const auto& frameData = msg->getCanFrame();
    EXPECT_EQ(frameData[0], 0x01);
    EXPECT_EQ(frameData[7], 0x08);
}

TEST(Message2000, ParsesSpaceSeparatedFormatWithHexPrefix)
{
    // Format: 0xCANID 0xBB 0xCC ...
    std::string frame = "0x18FD1234 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08";
    auto msg = Nmea2000Factory::create(frame);
    
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getPgn(), 0xFD12U);
    EXPECT_EQ(msg->getCanFrameLength(), 8U);
    
    const auto& frameData = msg->getCanFrame();
    EXPECT_EQ(frameData[0], 0x01);
    EXPECT_EQ(frameData[7], 0x08);
}

TEST(Message2000, ParsesSpaceSeparatedFormatWithoutHexPrefix)
{
    // Format: CANID BB CC ...
    std::string frame = "18FD1234 01 02 03 04 05 06 07 08";
    auto msg = Nmea2000Factory::create(frame);
    
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getPgn(), 0xFD12U);
    EXPECT_EQ(msg->getCanFrameLength(), 8U);
    
    const auto& frameData = msg->getCanFrame();
    EXPECT_EQ(frameData[0], 0x01);
    EXPECT_EQ(frameData[7], 0x08);
}

TEST(Message2000, ParsesExampleFormats)
{
    // From the user examples
    std::string format1 = "0x1CFF63CC, 0x3B 0x9F 0x00 0x01 0x00 0x00 0x02 0xFF";
    std::string format2 = "0x1CFF63CC 0x3B 0x9F 0x00 0x01 0x00 0x00 0x02 0xFF";
    std::string format3 = "1CFF63CC 3B 9F 00 01 00 00 02 FF";
    
    auto msg1 = Nmea2000Factory::create(format1);
    auto msg2 = Nmea2000Factory::create(format2);
    auto msg3 = Nmea2000Factory::create(format3);
    
    // All should parse successfully to the same message
    ASSERT_NE(msg1, nullptr);
    ASSERT_NE(msg2, nullptr);
    ASSERT_NE(msg3, nullptr);
    
    // All should have the same PGN and data
    EXPECT_EQ(msg1->getPgn(), msg2->getPgn());
    EXPECT_EQ(msg2->getPgn(), msg3->getPgn());
    EXPECT_EQ(msg1->getCanFrameLength(), 8U);
    
    // Verify data bytes match
    const auto& frame1 = msg1->getCanFrame();
    const auto& frame3 = msg3->getCanFrame();
    EXPECT_EQ(frame1[0], 0x3B);
    EXPECT_EQ(frame1[1], 0x9F);
    EXPECT_EQ(frame3[7], 0xFF);
}