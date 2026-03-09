#include <gtest/gtest.h>

#include <chrono>
#include <memory>
#include <string>

#include <nmealib.h>

using namespace nmealib::nmea2000;

// ---------------------------------------------------------------------------
// CAN frame constants for PGN 128259 (0x1F503)
// CAN ID = (128259 << 8) = 0x01F50300
//
// Byte layout (8 bytes total):
//   [0]     Sequence ID
//   [1-2]   Speed Water Referenced (uint16_t LE, 0.01 m/s)
//   [3-4]   Speed Ground Referenced (uint16_t LE, 0.01 m/s)
//   [5]     Speed Water Referenced Type
//   [6]     upper nibble = reserved2, lower nibble = Speed Direction
//   [7]     reserved1
// ---------------------------------------------------------------------------

// seqId=1, speedWater=500 raw≈5.00 m/s, speedGround=300 raw≈3.00 m/s,
// waterRefType=0(Paddle), speedDir=0(Forward), reserved1=0, reserved2=0
static const std::string FRAME_STANDARD       = "01F50300:01F4012C01000000";

// Speed direction variants (all else same as STANDARD)
static const std::string FRAME_DIR_REVERSE    = "01F50300:01F4012C01000100"; // byte[6]=0x01
static const std::string FRAME_DIR_RESERVED   = "01F50300:01F4012C01000200"; // byte[6]=0x02

// Water-referenced type variants
static const std::string FRAME_TYPE_PITOT        = "01F50300:01F4012C01010000"; // byte[5]=1
static const std::string FRAME_TYPE_DOPPLER      = "01F50300:01F4012C01020000"; // byte[5]=2
static const std::string FRAME_TYPE_CORRELATION  = "01F50300:01F4012C01030000"; // byte[5]=3
static const std::string FRAME_TYPE_EM           = "01F50300:01F4012C01040000"; // byte[5]=4
static const std::string FRAME_TYPE_UNAVAILABLE  = "01F50300:01F4012C01050000"; // byte[5]=5

// Wrong-length frames (must throw on PGN128259 parsing)
static const std::string FRAME_TOO_SHORT      = "01F50300:01F4012C010000";     // 7 bytes
static const std::string FRAME_TOO_LONG       = "01F50300:01F4012C0100000000"; // 9 bytes

// Helper: construct a PGN128259 with typical values via the public constructor
static PGN128259 makeTypical() {
    return PGN128259(1,
                     Speed::fromValue(5.0f),
                     Speed::fromValue(3.0f),
                     Byte::fromValue(0),
                     HalfByte::fromValue(0),
                     Byte::fromValue(0),
                     HalfByte::fromValue(0));
}

// ============================================================================
// Direct construction via public constructor
// ============================================================================

TEST(PGN128259DirectConstruction, GettersReturnCorrectValues) {
    PGN128259 pgn(1,
                  Speed::fromValue(5.0f),
                  Speed::fromValue(3.0f),
                  Byte::fromValue(2),       // Doppler
                  HalfByte::fromValue(1),   // Reverse
                  Byte::fromValue(42),
                  HalfByte::fromValue(3));

    EXPECT_EQ(pgn.getSequenceId(), 1U);
    EXPECT_EQ(pgn.getSpeedWaterReferenced(), Speed::fromValue(5.0f));
    EXPECT_EQ(pgn.getSpeedGroundReferenced(), Speed::fromValue(3.0f));
    EXPECT_EQ(pgn.getSpeedWaterReferencedType(), Byte::fromValue(2));
    EXPECT_EQ(pgn.getSpeedDirection(),           HalfByte::fromValue(1));
    EXPECT_EQ(pgn.getReserved1(),                Byte::fromValue(42));
    EXPECT_EQ(pgn.getReserved2(),                HalfByte::fromValue(3));
}

TEST(PGN128259DirectConstruction, MessageTypeIsNMEA2000) {
    auto pgn = makeTypical();
    EXPECT_EQ(pgn.getType(), nmealib::Message::Type::NMEA2000);
}

TEST(PGN128259DirectConstruction, PgnIs128259) {
    auto pgn = makeTypical();
    EXPECT_EQ(pgn.getPgn(), 128259U);
}

TEST(PGN128259DirectConstruction, ValidateReturnsTrue) {
    auto pgn = makeTypical();
    EXPECT_TRUE(pgn.validate());
}

TEST(PGN128259DirectConstruction, CanFrameIs8Bytes) {
    auto pgn = makeTypical();
    EXPECT_EQ(pgn.getCanFrameLength(), 8U);
    EXPECT_EQ(pgn.getCanFrame().size(), 8U);
}

TEST(PGN128259DirectConstruction, ZeroMinimumSpeeds) {
    PGN128259 pgn(0,
                  Speed::fromValue(0.0f),
                  Speed::fromValue(0.0f),
                  Byte::fromValue(0),
                  HalfByte::fromValue(0),
                  Byte::fromValue(0),
                  HalfByte::fromValue(0));

    EXPECT_FLOAT_EQ(pgn.getSpeedWaterReferenced().getValue(),  0.0f);
    EXPECT_FLOAT_EQ(pgn.getSpeedGroundReferenced().getValue(), 0.0f);
    EXPECT_EQ(pgn.getSequenceId(), 0U);
}

TEST(PGN128259DirectConstruction, MaximumSpeeds) {
    Speed maxSpeed = Speed::fromValue(655.0f);
    PGN128259 pgn(255,
                  maxSpeed,
                  maxSpeed,
                  Byte::fromValue(4),
                  HalfByte::fromValue(1),
                  Byte::fromValue(255),
                  HalfByte::fromValue(15));

    EXPECT_FLOAT_EQ(pgn.getSpeedWaterReferenced().getValue(),  655.0f);
    EXPECT_FLOAT_EQ(pgn.getSpeedGroundReferenced().getValue(), 655.0f);
    EXPECT_EQ(pgn.getSequenceId(), 255U);
    EXPECT_EQ(pgn.getReserved1(), Byte::fromValue(255));
    EXPECT_EQ(pgn.getReserved2(), HalfByte::fromValue(15));
}

TEST(PGN128259DirectConstruction, SequenceIdMaxValue) {
    PGN128259 pgn(255,
                  Speed::fromValue(0.0f),
                  Speed::fromValue(0.0f),
                  Byte::fromValue(0),
                  HalfByte::fromValue(0),
                  Byte::fromValue(0),
                  HalfByte::fromValue(0));
    EXPECT_EQ(pgn.getSequenceId(), 255U);
}

// ============================================================================
// Factory-based construction (Nmea2000Factory::create dispatches to PGN128259)
// ============================================================================

TEST(PGN128259Factory, FactoryDispatchesToPGN128259) {
    auto msg = Nmea2000Factory::create(FRAME_STANDARD);
    ASSERT_NE(msg, nullptr);
    auto* pgn = dynamic_cast<PGN128259*>(msg.get());
    EXPECT_NE(pgn, nullptr) << "Expected factory to return a PGN128259 instance";
}

TEST(PGN128259Factory, ParsedValuesMatchExpected) {
    auto msg = Nmea2000Factory::create(FRAME_STANDARD);
    ASSERT_NE(msg, nullptr);
    auto* pgn = dynamic_cast<PGN128259*>(msg.get());
    ASSERT_NE(pgn, nullptr);

    EXPECT_EQ(pgn->getSequenceId(), 1U);
    EXPECT_FLOAT_EQ(pgn->getSpeedWaterReferenced().getValue(),  5.0f);
    EXPECT_FLOAT_EQ(pgn->getSpeedGroundReferenced().getValue(), 3.0f);
    EXPECT_EQ(pgn->getSpeedWaterReferencedType().getValue(), 0U);
    EXPECT_EQ(pgn->getSpeedDirection().getValue(),           0U);
    EXPECT_EQ(pgn->getReserved1().getValue(),                0U);
    EXPECT_EQ(pgn->getReserved2().getValue(),                0U);
}

TEST(PGN128259Factory, ParsedValuesHaveEqualContentToDirectConstruction) {
    auto msg = Nmea2000Factory::create(FRAME_STANDARD);
    ASSERT_NE(msg, nullptr);
    auto* pgn = dynamic_cast<PGN128259*>(msg.get());
    ASSERT_NE(pgn, nullptr);

    PGN128259 direct = makeTypical();
    EXPECT_TRUE(pgn->hasEqualContent(direct));
}

TEST(PGN128259Factory, ThrowsOnFrameTooShort) {
    EXPECT_THROW(Nmea2000Factory::create(FRAME_TOO_SHORT), InvalidCanFrameException);
}

TEST(PGN128259Factory, ThrowsOnFrameTooLong) {
    EXPECT_THROW(Nmea2000Factory::create(FRAME_TOO_LONG), InvalidCanFrameException);
}

TEST(PGN128259Factory, SpeedDirectionForward) {
    auto msg = Nmea2000Factory::create(FRAME_STANDARD);
    auto* pgn = dynamic_cast<PGN128259*>(msg.get());
    ASSERT_NE(pgn, nullptr);
    EXPECT_EQ(pgn->getSpeedDirection().getValue(), 0U);
}

TEST(PGN128259Factory, SpeedDirectionReverse) {
    auto msg = Nmea2000Factory::create(FRAME_DIR_REVERSE);
    auto* pgn = dynamic_cast<PGN128259*>(msg.get());
    ASSERT_NE(pgn, nullptr);
    EXPECT_EQ(pgn->getSpeedDirection().getValue(), 1U);
}

TEST(PGN128259Factory, SpeedDirectionReserved) {
    auto msg = Nmea2000Factory::create(FRAME_DIR_RESERVED);
    auto* pgn = dynamic_cast<PGN128259*>(msg.get());
    ASSERT_NE(pgn, nullptr);
    EXPECT_EQ(pgn->getSpeedDirection().getValue(), 2U);
}

TEST(PGN128259Factory, WaterRefTypePitot) {
    auto msg = Nmea2000Factory::create(FRAME_TYPE_PITOT);
    auto* pgn = dynamic_cast<PGN128259*>(msg.get());
    ASSERT_NE(pgn, nullptr);
    EXPECT_EQ(pgn->getSpeedWaterReferencedType().getValue(), 1U);
}

TEST(PGN128259Factory, WaterRefTypeDoppler) {
    auto msg = Nmea2000Factory::create(FRAME_TYPE_DOPPLER);
    auto* pgn = dynamic_cast<PGN128259*>(msg.get());
    ASSERT_NE(pgn, nullptr);
    EXPECT_EQ(pgn->getSpeedWaterReferencedType().getValue(), 2U);
}

TEST(PGN128259Factory, WaterRefTypeCorrelation) {
    auto msg = Nmea2000Factory::create(FRAME_TYPE_CORRELATION);
    auto* pgn = dynamic_cast<PGN128259*>(msg.get());
    ASSERT_NE(pgn, nullptr);
    EXPECT_EQ(pgn->getSpeedWaterReferencedType().getValue(), 3U);
}

TEST(PGN128259Factory, WaterRefTypeEM) {
    auto msg = Nmea2000Factory::create(FRAME_TYPE_EM);
    auto* pgn = dynamic_cast<PGN128259*>(msg.get());
    ASSERT_NE(pgn, nullptr);
    EXPECT_EQ(pgn->getSpeedWaterReferencedType().getValue(), 4U);
}

TEST(PGN128259Factory, WaterRefTypeUnavailable) {
    auto msg = Nmea2000Factory::create(FRAME_TYPE_UNAVAILABLE);
    auto* pgn = dynamic_cast<PGN128259*>(msg.get());
    ASSERT_NE(pgn, nullptr);
    EXPECT_EQ(pgn->getSpeedWaterReferencedType().getValue(), 5U);
}

// Verify that the reserved2 nibble in byte[6] upper nibble is parsed correctly
// and does not corrupt speedDirection in the lower nibble.
TEST(PGN128259Factory, Reserved2NibbleDoesNotCorruptSpeedDirection) {
    // byte[6] = 0xF1 -> reserved2 = 0xF (upper nibble), speedDir = 0x1 (Reverse)
    std::string frame = "01F50300:01F4012C0100F100";
    auto msg = Nmea2000Factory::create(frame);
    auto* pgn = dynamic_cast<PGN128259*>(msg.get());
    ASSERT_NE(pgn, nullptr);
    EXPECT_EQ(pgn->getSpeedDirection().getValue(), 1U);
    EXPECT_EQ(pgn->getReserved2().getValue(),      15U);
}

// Verify that reserved1 (byte[7]) is parsed independently of byte[6]
TEST(PGN128259Factory, Reserved1InByte7IsParsedCorrectly) {
    // byte[6]=0x00, byte[7]=0xAB -> reserved1=0xAB=171
    std::string frame = "01F50300:01F4012C010000AB";
    auto msg = Nmea2000Factory::create(frame);
    auto* pgn = dynamic_cast<PGN128259*>(msg.get());
    ASSERT_NE(pgn, nullptr);
    EXPECT_EQ(pgn->getReserved1().getValue(), 171U);
}

// ============================================================================
// Clone
// ============================================================================

TEST(PGN128259Clone, CloneReturnsIndependentNonNullCopy) {
    auto original = makeTypical();
    auto cloned = original.clone();
    ASSERT_NE(cloned, nullptr);
    EXPECT_NE(cloned.get(), &original);
}

TEST(PGN128259Clone, ClonedInstanceHasEqualContent) {
    auto original = makeTypical();
    auto cloned = original.clone();
    ASSERT_NE(cloned, nullptr);

    auto* clonedPgn = dynamic_cast<PGN128259*>(cloned.get());
    ASSERT_NE(clonedPgn, nullptr);
    EXPECT_TRUE(original.hasEqualContent(*clonedPgn));
}

TEST(PGN128259Clone, CloneViaFactoryReturnsCorrectType) {
    auto msg = Nmea2000Factory::create(FRAME_STANDARD);
    ASSERT_NE(msg, nullptr);
    auto cloned = msg->clone();
    ASSERT_NE(cloned, nullptr);

    auto* clonedPgn = dynamic_cast<PGN128259*>(cloned.get());
    EXPECT_NE(clonedPgn, nullptr);
}

// ============================================================================
// Equality operators
// ============================================================================

TEST(PGN128259Equality, OperatorEqual_SameTimestampAndContent_ReturnsTrue) {
    using Clock = std::chrono::system_clock;
    auto ts = Clock::now();

    auto msg1 = Nmea2000Factory::create(FRAME_STANDARD, ts);
    auto msg2 = Nmea2000Factory::create(FRAME_STANDARD, ts);
    ASSERT_NE(msg1, nullptr);
    ASSERT_NE(msg2, nullptr);

    auto* p1 = dynamic_cast<PGN128259*>(msg1.get());
    auto* p2 = dynamic_cast<PGN128259*>(msg2.get());
    ASSERT_NE(p1, nullptr);
    ASSERT_NE(p2, nullptr);

    EXPECT_TRUE(*p1 == *p2);
}

TEST(PGN128259Equality, OperatorEqual_DifferentContent_ReturnsFalse) {
    using Clock = std::chrono::system_clock;
    auto ts = Clock::now();

    auto msg1 = Nmea2000Factory::create(FRAME_STANDARD,     ts);
    auto msg2 = Nmea2000Factory::create(FRAME_TYPE_DOPPLER, ts);
    ASSERT_NE(msg1, nullptr);
    ASSERT_NE(msg2, nullptr);

    auto* p1 = dynamic_cast<PGN128259*>(msg1.get());
    auto* p2 = dynamic_cast<PGN128259*>(msg2.get());
    ASSERT_NE(p1, nullptr);
    ASSERT_NE(p2, nullptr);

    EXPECT_FALSE(*p1 == *p2);
}

TEST(PGN128259Equality, HasEqualContent_SameValues_DifferentTimestamp_ReturnsTrue) {
    using Clock = std::chrono::system_clock;
    auto ts1 = Clock::now();
    auto ts2 = ts1 + std::chrono::seconds(1);

    auto msg1 = Nmea2000Factory::create(FRAME_STANDARD, ts1);
    auto msg2 = Nmea2000Factory::create(FRAME_STANDARD, ts2);

    auto* p1 = dynamic_cast<PGN128259*>(msg1.get());
    auto* p2 = dynamic_cast<PGN128259*>(msg2.get());
    ASSERT_NE(p1, nullptr);
    ASSERT_NE(p2, nullptr);

    EXPECT_TRUE(p1->hasEqualContent(*p2));
    // Timestamps differ, so operator== must return false
    EXPECT_FALSE(*p1 == *p2);
}

TEST(PGN128259Equality, HasEqualContent_DifferentContent_ReturnsFalse) {
    auto msg1 = Nmea2000Factory::create(FRAME_STANDARD);
    auto msg2 = Nmea2000Factory::create(FRAME_DIR_REVERSE);

    auto* p1 = dynamic_cast<PGN128259*>(msg1.get());
    auto* p2 = dynamic_cast<PGN128259*>(msg2.get());
    ASSERT_NE(p1, nullptr);
    ASSERT_NE(p2, nullptr);

    EXPECT_FALSE(p1->hasEqualContent(*p2));
}

// ============================================================================
// getStringContent — non-verbose (one-liner)
// ============================================================================

TEST(PGN128259StringContent, ConciseContainsPgnAndSpeeds) {
    auto pgn = makeTypical();
    std::string s = pgn.getStringContent(false);
    EXPECT_NE(s.find("128259"), std::string::npos);
    EXPECT_NE(s.find("5.00"),   std::string::npos);
    EXPECT_NE(s.find("3.00"),   std::string::npos);
}

TEST(PGN128259StringContent, ConciseSpeedDirForward) {
    PGN128259 pgn(1,
                  Speed::fromValue(1.0f),
                  Speed::fromValue(1.0f),
                  Byte::fromValue(0),
                  HalfByte::fromValue(0), // Forward
                  Byte::fromValue(0),
                  HalfByte::fromValue(0));
    std::string s = pgn.getStringContent(false);
    EXPECT_NE(s.find("SpeedDir=F"), std::string::npos);
}

TEST(PGN128259StringContent, ConciseSpeedDirReverse) {
    PGN128259 pgn(1,
                  Speed::fromValue(1.0f),
                  Speed::fromValue(1.0f),
                  Byte::fromValue(0),
                  HalfByte::fromValue(1), // Reverse
                  Byte::fromValue(0),
                  HalfByte::fromValue(0));
    std::string s = pgn.getStringContent(false);
    EXPECT_NE(s.find("SpeedDir=R"), std::string::npos);
}

TEST(PGN128259StringContent, ConciseSpeedDirReserved) {
    PGN128259 pgn(1,
                  Speed::fromValue(1.0f),
                  Speed::fromValue(1.0f),
                  Byte::fromValue(0),
                  HalfByte::fromValue(5), // Reserved/unknown
                  Byte::fromValue(0),
                  HalfByte::fromValue(0));
    std::string s = pgn.getStringContent(false);
    EXPECT_NE(s.find("SpeedDir=X"), std::string::npos);
}

// ============================================================================
// getStringContent — verbose (multi-line)
// ============================================================================

TEST(PGN128259StringContent, VerboseContainsProtocolAndPgn) {
    auto pgn = makeTypical();
    std::string s = pgn.getStringContent(true);
    EXPECT_NE(s.find("NMEA2000"), std::string::npos);
    EXPECT_NE(s.find("128259"),   std::string::npos);
}

TEST(PGN128259StringContent, VerboseContainsAllFieldLabels) {
    auto pgn = makeTypical();
    std::string s = pgn.getStringContent(true);
    EXPECT_NE(s.find("Sequence ID"),             std::string::npos);
    EXPECT_NE(s.find("Speed Water Referenced"),  std::string::npos);
    EXPECT_NE(s.find("Speed Ground Referenced"), std::string::npos);
    EXPECT_NE(s.find("Speed Water Referenced Type"), std::string::npos);
    EXPECT_NE(s.find("Speed Direction"),         std::string::npos);
}

TEST(PGN128259StringContent, VerboseWaterRefTypePaddle) {
    PGN128259 pgn(1, Speed::fromValue(1.0f), Speed::fromValue(1.0f),
                  Byte::fromValue(0), HalfByte::fromValue(0),
                  Byte::fromValue(0), HalfByte::fromValue(0));
    EXPECT_NE(pgn.getStringContent(true).find("Paddle"), std::string::npos);
}

TEST(PGN128259StringContent, VerboseWaterRefTypePitot) {
    PGN128259 pgn(1, Speed::fromValue(1.0f), Speed::fromValue(1.0f),
                  Byte::fromValue(1), HalfByte::fromValue(0),
                  Byte::fromValue(0), HalfByte::fromValue(0));
    EXPECT_NE(pgn.getStringContent(true).find("Pitot"), std::string::npos);
}

TEST(PGN128259StringContent, VerboseWaterRefTypeDoppler) {
    PGN128259 pgn(1, Speed::fromValue(1.0f), Speed::fromValue(1.0f),
                  Byte::fromValue(2), HalfByte::fromValue(0),
                  Byte::fromValue(0), HalfByte::fromValue(0));
    EXPECT_NE(pgn.getStringContent(true).find("Doppler"), std::string::npos);
}

TEST(PGN128259StringContent, VerboseWaterRefTypeCorrelation) {
    PGN128259 pgn(1, Speed::fromValue(1.0f), Speed::fromValue(1.0f),
                  Byte::fromValue(3), HalfByte::fromValue(0),
                  Byte::fromValue(0), HalfByte::fromValue(0));
    EXPECT_NE(pgn.getStringContent(true).find("Correlation"), std::string::npos);
}

TEST(PGN128259StringContent, VerboseWaterRefTypeEM) {
    PGN128259 pgn(1, Speed::fromValue(1.0f), Speed::fromValue(1.0f),
                  Byte::fromValue(4), HalfByte::fromValue(0),
                  Byte::fromValue(0), HalfByte::fromValue(0));
    EXPECT_NE(pgn.getStringContent(true).find("EM"), std::string::npos);
}

TEST(PGN128259StringContent, VerboseWaterRefTypeUnavailable) {
    PGN128259 pgn(1, Speed::fromValue(1.0f), Speed::fromValue(1.0f),
                  Byte::fromValue(5), HalfByte::fromValue(0),
                  Byte::fromValue(0), HalfByte::fromValue(0));
    EXPECT_NE(pgn.getStringContent(true).find("Unavailable"), std::string::npos);
}

TEST(PGN128259StringContent, VerboseSpeedDirForward) {
    PGN128259 pgn(1, Speed::fromValue(1.0f), Speed::fromValue(1.0f),
                  Byte::fromValue(0), HalfByte::fromValue(0),
                  Byte::fromValue(0), HalfByte::fromValue(0));
    EXPECT_NE(pgn.getStringContent(true).find("Forward"), std::string::npos);
}

TEST(PGN128259StringContent, VerboseSpeedDirReverse) {
    PGN128259 pgn(1, Speed::fromValue(1.0f), Speed::fromValue(1.0f),
                  Byte::fromValue(0), HalfByte::fromValue(1),
                  Byte::fromValue(0), HalfByte::fromValue(0));
    EXPECT_NE(pgn.getStringContent(true).find("Reverse"), std::string::npos);
}

TEST(PGN128259StringContent, VerboseSpeedDirReserved) {
    PGN128259 pgn(1, Speed::fromValue(1.0f), Speed::fromValue(1.0f),
                  Byte::fromValue(0), HalfByte::fromValue(7),
                  Byte::fromValue(0), HalfByte::fromValue(0));
    EXPECT_NE(pgn.getStringContent(true).find("Unavailable"), std::string::npos);
}

// ============================================================================
// DataType range validation
// ============================================================================

TEST(PGN128259DataTypes, SpeedAboveMaxThrows) {
    EXPECT_THROW(Speed::fromValue(655.33f), OutOfRangeException);
}

TEST(PGN128259DataTypes, SpeedBelowMinThrows) {
    EXPECT_THROW(Speed::fromValue(-0.01f), OutOfRangeException);
}

TEST(PGN128259DataTypes, HalfByteAboveMaxThrows) {
    EXPECT_THROW(HalfByte::fromValue(16U), OutOfRangeException);
}

TEST(PGN128259DataTypes, SpeedToStringRoundTrip) {
    Speed s = Speed::fromValue(5.0f);
    EXPECT_EQ(s.toString(), "5.00");
}

TEST(PGN128259DataTypes, SpeedRawRoundTrip) {
    Speed s = Speed::fromValue(12.34f);
    uint16_t raw = s.getRaw();
    Speed reconstructed = Speed::fromValue(static_cast<float>(raw) * 0.01f);
    EXPECT_EQ(s, reconstructed);
}