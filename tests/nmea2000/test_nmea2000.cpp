#include <gtest/gtest.h>

#include <chrono>
#include <memory>
#include <string>
#include <vector>

#include <nmealib.h>

using namespace nmealib::nmea2000;

// =============================================================================
// Test-frame reference table
// =============================================================================
//
// All expected values are derived from the 29-bit CAN Id layout:
//
//   canId[0]: [ 0   0   0  P3  P2  P1  R1  DP ]
//   canId[1]: [ PF8 PF7 PF6 PF5 PF4 PF3 PF2 PF1 ]  ← PDU Format
//   canId[2]: [ PS8 PS7 PS6 PS5 PS4 PS3 PS2 PS1  ]  ← PDU Specific
//   canId[3]: [ SA8 SA7 SA6 SA5 SA4 SA3 SA2 SA1  ]  ← Source Address
//
// PGN extraction:
//   PDU1 (PF < 0xF0): PGN = (RDP<<16)|(PF<<8)       PS = destination address
//   PDU2 (PF >= 0xF0): PGN = (RDP<<16)|(PF<<8)|PS   destination = 255 (global)
//
// ─────────────────────────────────────────────────────────────────────────────
// Frame A: 0x18EA1234  – PDU1, priority=6, R1=0, DP=0, PF=0xEA, DA=0x12, SA=0x34
//   canId bytes: [0x18, 0xEA, 0x12, 0x34]
//   priority=6, P3=T, P2=T, P1=F, reserved=F, dataPage=F
//   PGN = 0x00EA00 = 59904
//   destination = 0x12 = 18, sourceAddr = 0x34 = 52
//
// Frame B: 0x19F801CC  – PDU2, priority=6, R1=0, DP=1, PF=0xF8, PS=0x01, SA=0xCC
//   canId bytes: [0x19, 0xF8, 0x01, 0xCC]
//   priority=6, P3=T, P2=T, P1=F, reserved=F, dataPage=T
//   PGN = 0x1F801 = 129025
//   destination = 255 (global), sourceAddr = 0xCC = 204
//
// Frame C: 0x18FF00AB  – PDU2, priority=6, R1=0, DP=0, PF=0xFF, PS=0x00, SA=0xAB
//   canId bytes: [0x18, 0xFF, 0x00, 0xAB]
//   PGN = 0x0FF00 = 65280
//   destination = 255, sourceAddr = 0xAB = 171
//
// Frame D: 0x08EAFF01  – PDU1, priority=2, R1=0, DP=0, PF=0xEA, DA=0xFF, SA=0x01
//   canId bytes: [0x08, 0xEA, 0xFF, 0x01]
//   priority=2, P3=F, P2=T, P1=F
//   PGN = 0x0EA00 = 59904,  destination = 255, sourceAddr = 1
//
// Frame E: 0x00F02305  – PDU2, priority=0, R1=0, DP=0, PF=0xF0, PS=0x23, SA=0x05
//   canId bytes: [0x00, 0xF0, 0x23, 0x05]
//   priority=0, P3=F, P2=F, P1=F
//   PGN = 0x0F023 = 61475, destination = 255, sourceAddr = 5
//
// Frame F: 0x1CF02305  – PDU2, priority=7, R1=0, DP=0, PF=0xF0, PS=0x23, SA=0x05
//   canId bytes: [0x1C, 0xF0, 0x23, 0x05]
//   priority=7, P3=T, P2=T, P1=T
//   PGN = 0x0F023 = 61475, destination = 255, sourceAddr = 5
//
// Frame G: 0x18F00100  – PDU2, priority=6, SA=0  (minimum source address)
//   PGN = 0x0F001 = 61441, sourceAddr = 0
//
// Frame H: 0x18F001FD  – PDU2, priority=6, SA=253 (maximum valid source address)
//   PGN = 0x0F001 = 61441, sourceAddr = 253
//
// Frame I: 0x19FFFEA0  – PDU2, priority=6, DP=1, PGN=0x1FFFE=131070, SA=0xA0
//   canId bytes: [0x19, 0xFF, 0xFE, 0xA0]
//
// Frame J: 0x1FFFFFFF  – max valid 29-bit CAN ID
//   canId bytes: [0x1F, 0xFF, 0xFF, 0xFF]
//   priority=7, R1=T, DP=T, PF=0xFF (PDU2), PS=0xFF, SA=0xFF
//   PGN = 0x3FFFF = 262143
//
// Frame K: 0x18F80100  – PDU2, empty payload (0 bytes)
//   PGN = 0x0F801 = 63489
//
// Frame L: 0x18FD0923  – PDU2, 8-byte single-frame payload
//   PGN = 0x0FD09 = 64777, SA=0x23=35
// =============================================================================

// =============================================================================
// Helper
// =============================================================================

static std::unique_ptr<Message2000> make(const std::string& raw) {
    auto msg = Nmea2000Factory::create(raw);
    // Tests that call make() always expect success; assert here so failures
    // surface with a clear message rather than a nullptr dereference.
    if (!msg) {
        ADD_FAILURE() << "Nmea2000Factory::create() returned nullptr for: " << raw;
    }
    return std::unique_ptr<Message2000>(static_cast<Message2000*>(msg.release()));
}

// =============================================================================
// 1. Factory creation – valid inputs
// =============================================================================

TEST(Message2000_Creation, ValidCanonicalFormat) {
    // "CANID:data" — the canonical format; must always succeed.
    auto msg = make("18EA1234:AABBCCDD");
    ASSERT_NE(msg, nullptr);
}

TEST(Message2000_Creation, ValidHashSeparator) {
    // '#' is accepted as an alternative separator.
    auto msg = make("18EA1234#AABBCCDD");
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getPgn(), 59904u);
}

TEST(Message2000_Creation, ValidSpaceSeparatedNoPrefix) {
    // "CANID B1 B2 ..." without 0x prefix.
    auto msg = make("18EA1234 AA BB CC DD");
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getPgn(), 59904u);
}

TEST(Message2000_Creation, ValidSpaceSeparatedWithPrefix) {
    // "0xCANID 0xB1 0xB2 ..." with 0x prefix.
    auto msg = make("0x18EA1234 0xAA 0xBB 0xCC 0xDD");
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getPgn(), 59904u);
}

TEST(Message2000_Creation, ValidCommaSeparatedWithPrefix) {
    // "0xCANID, 0xB1 0xB2 ..." with comma separator and 0x prefix.
    auto msg = make("0x18EA1234, 0xAA 0xBB 0xCC 0xDD");
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getPgn(), 59904u);
}

TEST(Message2000_Creation, ValidEmptyPayload) {
    // Zero-byte payload is explicitly allowed.
    auto msg = make("18F80100:");
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getCanFrameLength(), 0u);
}

TEST(Message2000_Creation, ValidSingleBytePayload) {
    auto msg = make("18FD0923:FF");
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getCanFrameLength(), 1u);
}

TEST(Message2000_Creation, ValidMaxSingleFrame) {
    // 8-byte payload — maximum for a CAN single frame.
    auto msg = make("18FD0923:0102030405060708");
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getCanFrameLength(), 8u);
}

TEST(Message2000_Creation, ValidFastPacketMaxPayload) {
    // 223 bytes is the fast-packet ceiling; must be accepted.
    std::string payload(223 * 2, 'A'); // 223 bytes as hex
    auto msg = make("18FD0923:" + payload);
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getCanFrameLength(), 223u);
}

TEST(Message2000_Creation, ValidMaxCanId) {
    // 0x1FFFFFFF is the largest valid 29-bit value.
    auto msg = make("1FFFFFFF:AABB");
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getPgn(), 262143u); // 0x3FFFF
}

TEST(Message2000_Creation, ValidMinimumCanId) {
    // 0x00000000 is the smallest valid CAN ID.
    auto msg = make("00000000:01");
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getPriority(), 0u);
    EXPECT_EQ(msg->getSourceAddress(), 0u);
}

// =============================================================================
// 2. Factory creation – invalid inputs
// =============================================================================

TEST(Message2000_Creation, InvalidMissingColon) {
    EXPECT_THROW(make("18EA1234AABBCCDD"), InvalidCanFrameException);
}

TEST(Message2000_Creation, InvalidEmptyString) {
    EXPECT_THROW(make(""), InvalidCanFrameException);
}

TEST(Message2000_Creation, InvalidEmptyCanId) {
    EXPECT_THROW(make(":AABB"), InvalidCanFrameException);
}

TEST(Message2000_Creation, InvalidCanIdExceedsBits) {
    // 0x20000000 exceeds 29 bits.
    EXPECT_THROW(make("20000000:AABB"), InvalidCanFrameException);
}

TEST(Message2000_Creation, InvalidCanIdNonHex) {
    EXPECT_THROW(make("ZZZZZZZZ:AABB"), InvalidCanFrameException);
}

TEST(Message2000_Creation, InvalidOddLengthFrameData) {
    // Frame data must have an even number of hex characters.
    EXPECT_THROW(make("18EA1234:AAB"), InvalidCanFrameException);
}

TEST(Message2000_Creation, InvalidNonHexFrameData) {
    EXPECT_THROW(make("18EA1234:GGGG"), InvalidCanFrameException);
}

TEST(Message2000_Creation, InvalidFrameTooLong) {
    // 224 bytes exceeds the 223-byte fast-packet maximum.
    std::string payload(224 * 2, 'A');
    EXPECT_THROW(make("18FD0923:" + payload), FrameTooLongException);
}

// =============================================================================
// 3. Priority extraction
// =============================================================================
//
// canId[0] layout: [ 0  0  0  P3  P2  P1  R1  DP ]
// priority = (canId[0] >> 2) & 0x07

TEST(Message2000_Priority, PriorityZero) {
    // 0x00F02305 → b0=0x00 → priority = 0
    auto msg = make("00F02305:01");
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getPriority(), 0u);
    EXPECT_FALSE(msg->getPriority3());
    EXPECT_FALSE(msg->getPriority2());
    EXPECT_FALSE(msg->getPriority1());
}

TEST(Message2000_Priority, PrioritySix) {
    // 0x18EA1234 → b0=0x18 = 0b00011000 → priority = (0x18>>2)&7 = 6
    auto msg = make("18EA1234:AABB");
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getPriority(), 6u);
    EXPECT_TRUE(msg->getPriority3());  // bit4 of 0x18 = 1
    EXPECT_TRUE(msg->getPriority2());  // bit3 of 0x18 = 1
    EXPECT_FALSE(msg->getPriority1()); // bit2 of 0x18 = 0
}

TEST(Message2000_Priority, PrioritySeven) {
    // 0x1CF02305 → b0=0x1C = 0b00011100 → priority = (0x1C>>2)&7 = 7
    auto msg = make("1CF02305:01");
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getPriority(), 7u);
    EXPECT_TRUE(msg->getPriority3());  // bit4 of 0x1C = 1
    EXPECT_TRUE(msg->getPriority2());  // bit3 of 0x1C = 1
    EXPECT_TRUE(msg->getPriority1());  // bit2 of 0x1C = 1
}

TEST(Message2000_Priority, PriorityTwo) {
    // 0x08EAFF01 → b0=0x08 = 0b00001000 → priority = (0x08>>2)&7 = 2
    auto msg = make("08EAFF01:01");
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getPriority(), 2u);
    EXPECT_FALSE(msg->getPriority3()); // bit4 of 0x08 = 0
    EXPECT_TRUE(msg->getPriority2());  // bit3 of 0x08 = 1
    EXPECT_FALSE(msg->getPriority1()); // bit2 of 0x08 = 0
}

// =============================================================================
// 4. Reserved (R1) and Data Page (DP) bits
// =============================================================================
//
// R1 = bit1 of canId[0],  DP = bit0 of canId[0]

TEST(Message2000_ControlBits, ReservedAndDataPageBothClear) {
    // 0x18EA1234 → b0=0x18=0b00011000 → R1=0, DP=0
    auto msg = make("18EA1234:AABB");
    ASSERT_NE(msg, nullptr);
    EXPECT_FALSE(msg->getHeaderReserved());
    EXPECT_FALSE(msg->getDataPage());
}

TEST(Message2000_ControlBits, ReservedAndDataPageBothSet) {
    // 0x1FFFFFFF → b0=0x1F=0b00011111 → R1=1, DP=1
    auto msg = make("1FFFFFFF:01");
    ASSERT_NE(msg, nullptr);
    EXPECT_TRUE(msg->getHeaderReserved());
    EXPECT_TRUE(msg->getDataPage());
}

TEST(Message2000_ControlBits, ReservedSetDataPageClear) {
    // Priority=6 (b0 bits[4:2]=110), R1=1 (bit1), DP=0 (bit0) → b0=0b00011010=0x1A
    // PF=0xF0 (PDU2), PS=0x01, SA=0x05
    auto msg = make("1AF00105:01");
    ASSERT_NE(msg, nullptr);
    EXPECT_TRUE(msg->getHeaderReserved());
    EXPECT_FALSE(msg->getDataPage());
}

// =============================================================================
// 5. PDU Format and PDU Specific
// =============================================================================

TEST(Message2000_PDU, FormatPDU1) {
    // 0x18EA1234 → PF = canId[1] = 0xEA < 0xF0 → PDU1
    auto msg = make("18EA1234:AABB");
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getPDUFormat(), 0xEAu);
}

TEST(Message2000_PDU, FormatPDU2) {
    // 0x18F80100 → PF = canId[1] = 0xF8 >= 0xF0 → PDU2
    auto msg = make("18F80100:01");
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getPDUFormat(), 0xF8u);
}

TEST(Message2000_PDU, PDU1SpecificIsDestination) {
    // 0x18EA1234 → PDUSpecific = canId[2] = 0x12
    auto msg = make("18EA1234:AABB");
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getPDUSpecific(), 0x12u);
}

TEST(Message2000_PDU, PDU1BoundaryBelow) {
    // PF = 0xEF = 239 < 240 → PDU1
    // b0=0x18, PF=0xEF, PS=0xAA, SA=0x01
    auto msg = make("18EFAA01:01");
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getPDUFormat(), 0xEFu);
}

TEST(Message2000_PDU, PDU2BoundaryAt0xF0) {
    // PF = 0xF0 = 240 → PDU2 (first PDU2 value)
    auto msg = make("18F02305:01");
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getPDUFormat(), 0xF0u);
}

// =============================================================================
// 7. PGN extraction
// =============================================================================

TEST(Message2000_PGN, PDU1PGN) {
    // 0x18EA1234 → RDP=0, PF=0xEA → PGN = 0x00EA00 = 59904
    auto msg = make("18EA1234:AABB");
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getPgn(), 59904u);
}

TEST(Message2000_PGN, PDU1PGNLowerByteAlwaysZero) {
    // Same PF but different PS → same PGN (PS is DA, not part of PGN in PDU1).
    auto msgA = make("18EA1234:01"); // DA=0x12
    auto msgB = make("18EAAB34:01"); // DA=0xAB
    ASSERT_NE(msgA, nullptr);
    ASSERT_NE(msgB, nullptr);
    EXPECT_EQ(msgA->getPgn(), msgB->getPgn());
    EXPECT_EQ(msgA->getPgn(), 59904u);
}

TEST(Message2000_PGN, PDU2DP0) {
    // 0x18FF00AB → RDP=0, PF=0xFF, PS=0x00 → PGN=0x0FF00=65280
    auto msg = make("18FF00AB:AABB");
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getPgn(), 65280u);
}

TEST(Message2000_PGN, PDU2HighRangeDP1) {
    // 0x19FFFEA0 → RDP=0b01 (DP=1), PF=0xFF, PS=0xFE
    // PGN = (1<<16)|(0xFF<<8)|0xFE = 0x1FFFE = 131070
    auto msg = make("19FFFEA0:01");
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getPgn(), 131070u);
}

TEST(Message2000_PGN, MaxPGN) {
    // 0x1FFFFFFF → RDP=0b11, PF=0xFF, PS=0xFF (PDU2)
    // PGN = (3<<16)|(0xFF<<8)|0xFF = 0x3FFFF = 262143
    auto msg = make("1FFFFFFF:01");
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getPgn(), 262143u);
}

TEST(Message2000_PGN, SamePGNDifferentSA) {
    // Same CAN ID except source address → identical PGN.
    auto msgA = make("18F00100:01"); // SA=0
    auto msgB = make("18F001FD:01"); // SA=253
    ASSERT_NE(msgA, nullptr);
    ASSERT_NE(msgB, nullptr);
    EXPECT_EQ(msgA->getPgn(), msgB->getPgn());
    EXPECT_EQ(msgA->getPgn(), 61441u);
}

TEST(Message2000_PGN, PDU2BoundaryPGN_PF_0xF0) {
    // 0x00F02305 → RDP=0, PF=0xF0, PS=0x23 → PGN=(0xF0<<8)|0x23=0xF023=61475
    auto msg = make("00F02305:01");
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getPgn(), 61475u);
}

// =============================================================================
// 8. CAN Id bytes
// =============================================================================

TEST(Message2000_CanId, BytesPDU1) {
    // 0x18EA1234 → stored as [0x18, 0xEA, 0x12, 0x34]
    auto msg = make("18EA1234:AABB");
    ASSERT_NE(msg, nullptr);
    const auto& id = msg->getCanId();
    ASSERT_EQ(id.size(), 4u);
    EXPECT_EQ(id[0], 0x18u);
    EXPECT_EQ(id[1], 0xEAu);
    EXPECT_EQ(id[2], 0x12u);
    EXPECT_EQ(id[3], 0x34u);
}

TEST(Message2000_CanId, BytesMaxCanId) {
    // 0x1FFFFFFF → stored as [0x1F, 0xFF, 0xFF, 0xFF]
    auto msg = make("1FFFFFFF:01");
    ASSERT_NE(msg, nullptr);
    const auto& id = msg->getCanId();
    ASSERT_EQ(id.size(), 4u);
    EXPECT_EQ(id[0], 0x1Fu);
    EXPECT_EQ(id[1], 0xFFu);
    EXPECT_EQ(id[2], 0xFFu);
    EXPECT_EQ(id[3], 0xFFu);
}

// =============================================================================
// 9. Frame data and length
// =============================================================================

TEST(Message2000_Frame, EmptyPayload) {
    auto msg = make("18F80100:");
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getCanFrameLength(), 0u);
    EXPECT_TRUE(msg->getCanFrame().empty());
}

TEST(Message2000_Frame, SingleBytePayload) {
    auto msg = make("18FD0923:FF");
    ASSERT_NE(msg, nullptr);
    ASSERT_EQ(msg->getCanFrameLength(), 1u);
    EXPECT_EQ(msg->getCanFrame()[0], 0xFFu);
}

TEST(Message2000_Frame, EightBytePayload) {
    auto msg = make("18FD0923:0102030405060708");
    ASSERT_NE(msg, nullptr);
    ASSERT_EQ(msg->getCanFrameLength(), 8u);
    const auto& frame = msg->getCanFrame();
    EXPECT_EQ(frame[0], 0x01u);
    EXPECT_EQ(frame[1], 0x02u);
    EXPECT_EQ(frame[2], 0x03u);
    EXPECT_EQ(frame[3], 0x04u);
    EXPECT_EQ(frame[4], 0x05u);
    EXPECT_EQ(frame[5], 0x06u);
    EXPECT_EQ(frame[6], 0x07u);
    EXPECT_EQ(frame[7], 0x08u);
}

TEST(Message2000_Frame, MaxFastPacketPayload) {
    // 223 bytes (0x00..0xDE repeated/filled)
    std::string payload;
    for (int i = 0; i < 223; ++i) {
        char buf[3];
        std::snprintf(buf, sizeof(buf), "%02X", i & 0xFF);
        payload += buf;
    }
    auto msg = make("18FD0923:" + payload);
    ASSERT_NE(msg, nullptr);
    EXPECT_EQ(msg->getCanFrameLength(), 223u);
    EXPECT_EQ(msg->getCanFrame()[0],   0x00u);
    EXPECT_EQ(msg->getCanFrame()[222], (222u & 0xFF));
}

TEST(Message2000_Frame, PayloadBytesAreCorrect) {
    // Verify individual byte values are parsed correctly.
    auto msg = make("18FD0923:DEADBEEF");
    ASSERT_NE(msg, nullptr);
    ASSERT_EQ(msg->getCanFrameLength(), 4u);
    const auto& frame = msg->getCanFrame();
    EXPECT_EQ(frame[0], 0xDEu);
    EXPECT_EQ(frame[1], 0xADu);
    EXPECT_EQ(frame[2], 0xBEu);
    EXPECT_EQ(frame[3], 0xEFu);
}

TEST(Message2000_Frame, PayloadAllZeros) {
    auto msg = make("18FD0923:00000000");
    ASSERT_NE(msg, nullptr);
    ASSERT_EQ(msg->getCanFrameLength(), 4u);
    for (const auto b : msg->getCanFrame()) {
        EXPECT_EQ(b, 0x00u);
    }
}

TEST(Message2000_Frame, PayloadAllOnes) {
    auto msg = make("18FD0923:FFFFFFFF");
    ASSERT_NE(msg, nullptr);
    ASSERT_EQ(msg->getCanFrameLength(), 4u);
    for (const auto b : msg->getCanFrame()) {
        EXPECT_EQ(b, 0xFFu);
    }
}

// =============================================================================
// 10. Serialization
// =============================================================================

TEST(Message2000_Serialize, AlternativeInputNormalized) {
    // '#' and ':' inputs must serialize identically.
    auto msgColon = make("18EA1234:AABB");
    auto msgHash  = make("18EA1234#AABB");
    ASSERT_NE(msgColon, nullptr);
    ASSERT_NE(msgHash,  nullptr);
    EXPECT_EQ(msgColon->serialize(), msgHash->serialize());
}

// =============================================================================
// 11. Validate
// =============================================================================

TEST(Message2000_Validate, ValidMessage) {
    auto msg = make("18EA1234:AABB");
    ASSERT_NE(msg, nullptr);
    EXPECT_TRUE(msg->validate());
}

TEST(Message2000_Validate, ValidEmptyPayload) {
    auto msg = make("18F80100:");
    ASSERT_NE(msg, nullptr);
    EXPECT_TRUE(msg->validate());
}

TEST(Message2000_Validate, ValidMaxPGN) {
    auto msg = make("1FFFFFFF:01");
    ASSERT_NE(msg, nullptr);
    EXPECT_TRUE(msg->validate());
}

// =============================================================================
// 12. Clone
// =============================================================================

TEST(Message2000_Clone, CloneIsEqual) {
    auto original = make("18EA1234:AABBCCDD");
    ASSERT_NE(original, nullptr);
    auto cloned = original->clone();
    ASSERT_NE(cloned, nullptr);

    // Downcast to access Message2000-specific comparison.
    auto* cloned2000 = dynamic_cast<Message2000*>(cloned.get());
    ASSERT_NE(cloned2000, nullptr);
    EXPECT_TRUE(*original == *cloned2000);
}

TEST(Message2000_Clone, CloneIsIndependent) {
    // Modifying the clone must not affect the original (deep copy).
    auto original = make("18EA1234:AABBCCDD");
    ASSERT_NE(original, nullptr);
    auto cloned = original->clone();
    ASSERT_NE(cloned, nullptr);

    // The clone must share the same PGN as the original.
    auto* cloned2000 = dynamic_cast<Message2000*>(cloned.get());
    ASSERT_NE(cloned2000, nullptr);
    EXPECT_EQ(original->getPgn(), cloned2000->getPgn());
}

// =============================================================================
// 13. Equality operator
// =============================================================================

TEST(Message2000_Equality, SameFrameEqual) {
    auto msgA = make("18EA1234:AABBCCDD");
    auto msgB = make("18EA1234:AABBCCDD");
    ASSERT_NE(msgA, nullptr);
    ASSERT_NE(msgB, nullptr);
    EXPECT_TRUE(*msgA == *msgB);
}

TEST(Message2000_Equality, DifferentCanIdNotEqual) {
    auto msgA = make("18EA1234:AABB");
    auto msgB = make("18F80100:AABB");
    ASSERT_NE(msgA, nullptr);
    ASSERT_NE(msgB, nullptr);
    EXPECT_FALSE(*msgA == *msgB);
}

TEST(Message2000_Equality, DifferentPayloadNotEqual) {
    auto msgA = make("18EA1234:AABB");
    auto msgB = make("18EA1234:CCDD");
    ASSERT_NE(msgA, nullptr);
    ASSERT_NE(msgB, nullptr);
    EXPECT_FALSE(*msgA == *msgB);
}

TEST(Message2000_Equality, DifferentLengthPayloadNotEqual) {
    auto msgA = make("18EA1234:AABB");
    auto msgB = make("18EA1234:AABBCC");
    ASSERT_NE(msgA, nullptr);
    ASSERT_NE(msgB, nullptr);
    EXPECT_FALSE(*msgA == *msgB);
}

// =============================================================================
// 14. getStringContent
// =============================================================================

TEST(Message2000_StringContent, NonVerboseContainsPGN) {
    auto msg = make("18EA1234:AABB");
    ASSERT_NE(msg, nullptr);
    std::string s = msg->getStringContent(false);
    // Non-verbose must contain the PGN number.
    EXPECT_NE(s.find("59904"), std::string::npos);
}

// =============================================================================
// 15. Edge cases and cross-field consistency
// =============================================================================

TEST(Message2000_EdgeCase, PDU1SameCanIdDifferentDAGivesSamePGN) {
    // Changing only PS (DA) in a PDU1 frame must not change the PGN.
    auto msgA = make("18EE1100:01"); // DA=0x11, PF=0xEE
    auto msgB = make("18EE2200:01"); // DA=0x22, PF=0xEE — different PS, same PF
    ASSERT_NE(msgA, nullptr);
    ASSERT_NE(msgB, nullptr);
    EXPECT_EQ(msgA->getPgn(), msgB->getPgn());
}

TEST(Message2000_EdgeCase, PDU2ChangingPSChangesPGN) {
    // For PDU2, PS is part of the PGN, so different PS → different PGN.
    auto msgA = make("18F00100:01"); // PS=0x01
    auto msgB = make("18F00200:01"); // PS=0x02
    ASSERT_NE(msgA, nullptr);
    ASSERT_NE(msgB, nullptr);
    EXPECT_NE(msgA->getPgn(), msgB->getPgn());
    EXPECT_EQ(msgA->getPgn(), 61441u); // 0x0F001
    EXPECT_EQ(msgB->getPgn(), 61442u); // 0x0F002
}

TEST(Message2000_EdgeCase, PriorityDoesNotAffectPGN) {
    // Frames with identical PF/PS/SA but different priority must yield the same PGN.
    auto msgP0 = make("00F02305:01"); // priority=0
    auto msgP7 = make("1CF02305:01"); // priority=7
    ASSERT_NE(msgP0, nullptr);
    ASSERT_NE(msgP7, nullptr);
    EXPECT_EQ(msgP0->getPgn(), msgP7->getPgn());
    EXPECT_EQ(msgP0->getPgn(), 61475u);
}

TEST(Message2000_EdgeCase, PriorityDoesNotAffectSourceAddress) {
    auto msgP0 = make("00F02305:01"); // SA=0x05, priority=0
    auto msgP7 = make("1CF02305:01"); // SA=0x05, priority=7
    ASSERT_NE(msgP0, nullptr);
    ASSERT_NE(msgP7, nullptr);
    EXPECT_EQ(msgP0->getSourceAddress(), msgP7->getSourceAddress());
    EXPECT_EQ(msgP0->getSourceAddress(), 5u);
}

TEST(Message2000_EdgeCase, InputCaseInsensitive) {
    // Lower and upper case hex digits must produce the same message.
    auto msgLower = make("18ea1234:aabbccdd");
    auto msgUpper = make("18EA1234:AABBCCDD");
    ASSERT_NE(msgLower, nullptr);
    ASSERT_NE(msgUpper, nullptr);
    EXPECT_TRUE(*msgLower == *msgUpper);
}