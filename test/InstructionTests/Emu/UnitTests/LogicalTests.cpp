#include <functional>

#include <gtest/gtest.h>

#include <Emu/CPU.hpp>


namespace Emu::UnitTests
{

    class LogicalFixture : public testing::Test
    {
    public:
        Memory memory;
        CPU cpu;

        void SetUp() override
        {
            cpu.Reset(memory);
        }

        void TearDown() override
        { }

        void AssertRegisters(CPU const & initialCpu, Byte value)
        {
            if (value == 0x00)
                EXPECT_TRUE(cpu.StatusFlags.ZeroFlag);
            else
                EXPECT_FALSE(cpu.StatusFlags.ZeroFlag);

            if (value >= 0x80)
                EXPECT_TRUE(cpu.StatusFlags.NegativeFlag);
            else
                EXPECT_FALSE(cpu.StatusFlags.NegativeFlag);


            EXPECT_EQ(cpu.StatusFlags.CarryFlag,        initialCpu.StatusFlags.CarryFlag);
            EXPECT_EQ(cpu.StatusFlags.IRQDisableFlag,   initialCpu.StatusFlags.IRQDisableFlag);
            EXPECT_EQ(cpu.StatusFlags.DecimalMode,      initialCpu.StatusFlags.DecimalMode);
            EXPECT_EQ(cpu.StatusFlags.BreakCommand,     initialCpu.StatusFlags.BreakCommand);
            EXPECT_EQ(cpu.StatusFlags.OverflowFlag,     initialCpu.StatusFlags.OverflowFlag);

            ASSERT_FALSE(cpu.DebugFlags.UnhandledInstruction);
            ASSERT_FALSE(cpu.DebugFlags.CycleOverflow);
        }

        void TestLogicalOpImmediate(
            Byte opCode,
            Byte accumulatorValue,
            Byte memoryValue,
            Byte (*op)(Byte, Byte),
            Byte initialFlags = 0x00)
        {
            // Arrange
            uint32 cyclesExpected = 2u;
            Byte expectedValue = op(accumulatorValue, memoryValue);

            cpu.A = accumulatorValue;
            memory.WriteByte(0xFFFC, opCode);
            memory.WriteByte(0xFFFD, memoryValue);

            CPU initial = cpu;

            // Act
            auto cyclesUsed = cpu.Execute(cyclesExpected, memory);

            // Assert
            EXPECT_EQ(cpu.A, expectedValue);
            EXPECT_EQ(cyclesUsed, cyclesExpected);

            AssertRegisters(initial, expectedValue);
        }

        void TestLogicalOpZeroPage(
            Byte opCode,
            Byte accumulatorValue,
            Byte memoryValue,
            Byte (*op)(Byte, Byte),
            Byte initialFlags = 0x00)
        {
            // Arrange
            uint32 cyclesExpected = 3u;
            Byte expectedValue = op(accumulatorValue, memoryValue);
            Byte address = 0x42;

            cpu.A = accumulatorValue;
            memory.WriteByte(0xFFFC, opCode);
            memory.WriteByte(0xFFFD, address);
            memory.WriteByte(address, memoryValue);

            CPU initial = cpu;

            // Act
            auto cyclesUsed = cpu.Execute(cyclesExpected, memory);

            // Assert
            EXPECT_EQ(cpu.A, expectedValue);
            EXPECT_EQ(cyclesUsed, cyclesExpected);

            AssertRegisters(initial, expectedValue);
        }

        void TestLogicalOpZeroPageOffsetX(
            Byte opCode,
            Byte accumulatorValue,
            Byte memoryValue,
            Byte (*op)(Byte, Byte),
            bool wrapAddress = false,
            Byte initialFlags = 0x00)
        {
            // Arrange
            uint32 cyclesExpected = 4u;
            Byte expectedValue = op(accumulatorValue, memoryValue);

            Byte offset = wrapAddress ? 0xFF : 0x05;
            Byte baseAddress = 0x56;
            Byte offsetAddress = (baseAddress + offset) % 0x100;

            cpu.A = accumulatorValue;
            cpu.X = offset;
            memory.WriteByte(0xFFFC, opCode);
            memory.WriteByte(0xFFFD, baseAddress);
            memory.WriteByte(offsetAddress, memoryValue);

            CPU initial = cpu;

            // Act
            auto cyclesUsed = cpu.Execute(cyclesExpected, memory);

            // Assert
            EXPECT_EQ(cpu.A, expectedValue);
            EXPECT_EQ(cyclesUsed, cyclesExpected);

            AssertRegisters(initial, expectedValue);
        }

        void TestLogicalOpAbsolute(
            Byte opCode,
            Byte accumulatorValue,
            Byte memoryValue,
            Byte (*op)(Byte, Byte),
            Byte initialFlags = 0x00)
        {
            // Arrange
            uint32 cyclesExpected = 4u;
            Byte expectedValue = op(accumulatorValue, memoryValue);
            Word address = 0x4480;

            cpu.A = accumulatorValue;
            memory.WriteByte(0xFFFC, opCode);
            memory.WriteWord(0xFFFD, address);
            memory.WriteByte(address, memoryValue);

            CPU initial = cpu;

            // Act
            auto cyclesUsed = cpu.Execute(cyclesExpected, memory);

            // Assert
            EXPECT_EQ(cpu.A, expectedValue);
            EXPECT_EQ(cyclesUsed, cyclesExpected);

            AssertRegisters(initial, expectedValue);
        }

        void TestLogicalOpAbsoluteXY(
            Byte opCode,
            Byte accumulatorValue,
            Byte memoryValue,
            Byte (*op)(Byte, Byte),
            Byte CPU::* offsetRegister,
            bool crossPageBoundary = false,
            Byte initialFlags = 0x00)
        {
            // Arrange
            uint32 cyclesExpected = crossPageBoundary ? 5u : 4u;
            Byte expectedValue = op(accumulatorValue, memoryValue);

            Byte registerValue = crossPageBoundary ? 0xFF : 0x01;
            Word baseAddress = 0x6580;
            Word offsetAddress = baseAddress + registerValue;

            cpu.A = accumulatorValue;
            cpu.*offsetRegister = registerValue;
            memory.WriteByte(0xFFFC, opCode);
            memory.WriteWord(0xFFFD, baseAddress);
            memory.WriteByte(offsetAddress, memoryValue);

            CPU initial = cpu;

            // Act
            auto cyclesUsed = cpu.Execute(cyclesExpected, memory);

            // Assert
            EXPECT_EQ(cpu.A, expectedValue);
            EXPECT_EQ(cyclesUsed, cyclesExpected);

            AssertRegisters(initial, expectedValue);
        }

        void TestLogicalOpIndirectX(
            Byte opCode,
            Byte accumulatorValue,
            Byte memoryValue,
            Byte (*op)(Byte, Byte),
            Byte initialFlags = 0x00)
        {
            // Arrange
            uint32 cyclesExpected = 6u;
            Byte expectedValue = op(accumulatorValue, memoryValue);

            Byte zeroPageAddress = 0x02;
            Byte zeroPageOffset = 0x04;
            Word indexAddress = zeroPageAddress + zeroPageOffset;
            Word valueAddress = 0x3300;

            cpu.A = accumulatorValue;
            cpu.X = zeroPageOffset;
            memory.WriteByte(0xFFFC, opCode);
            memory.WriteWord(0xFFFD, zeroPageAddress);
            memory.WriteWord(indexAddress, valueAddress);
            memory.WriteByte(valueAddress, memoryValue);

            CPU initial = cpu;

            // Act
            auto cyclesUsed = cpu.Execute(cyclesExpected, memory);

            // Assert
            EXPECT_EQ(cpu.A, expectedValue);
            EXPECT_EQ(cyclesUsed, cyclesExpected);

            AssertRegisters(initial, expectedValue);
        }

        void TestLogicalOpIndirectY(
            Byte opCode,
            Byte accumulatorValue,
            Byte memoryValue,
            Byte (*op)(Byte, Byte),
            bool crossPageBoundary = false,
            Byte initialFlags = 0x00)
        {
            // Arrange
            uint32 cyclesExpected = crossPageBoundary ? 6u : 5u;
            Byte expectedValue = op(accumulatorValue, memoryValue);

            Byte offsetValue = 0x12;
            Byte zeroPageAddress = 0x04;
            Word indexAddress = crossPageBoundary ? 0x22FE : 0x2222;
            Word valueAddress = indexAddress + offsetValue;

            cpu.A = accumulatorValue;
            cpu.Y = offsetValue;
            memory.WriteByte(0xFFFC, opCode);
            memory.WriteWord(0xFFFD, zeroPageAddress);
            memory.WriteWord(zeroPageAddress, indexAddress);
            memory.WriteByte(valueAddress, memoryValue);

            CPU initial = cpu;

            // Act
            auto cyclesUsed = cpu.Execute(cyclesExpected, memory);

            // Assert
            EXPECT_EQ(cpu.A, expectedValue);
            EXPECT_EQ(cyclesUsed, cyclesExpected);

            AssertRegisters(initial, expectedValue);
        }

        void TestBitOpZeroPage(Byte value, Byte pattern)
        {
            // Arrange
            uint32 cyclesExpected = 3u;
            bool zeroFlag = (value & pattern) == 0;
            bool overflowFlag = (value & 1 << 6) > 0;
            bool negativeFlag = (value & 1 << 7) > 0;
            Word zeroPageAddress = 0x0042;

            cpu.StatusFlags.OverflowFlag = !overflowFlag;
            cpu.StatusFlags.NegativeFlag = !negativeFlag;

            cpu.A = pattern;
            memory.WriteByte(0xFFFC, CPU::INS_BIT_ZP);
            memory.WriteByte(0xFFFD, (Byte)zeroPageAddress);
            memory.WriteByte(zeroPageAddress, value);

            CPU initial = cpu;

            // Act
            auto cyclesUsed = cpu.Execute(cyclesExpected, memory);

            // Assert
            EXPECT_EQ(cyclesUsed, cyclesExpected);

            EXPECT_EQ(cpu.StatusFlags.ZeroFlag, zeroFlag);
            EXPECT_EQ(cpu.StatusFlags.OverflowFlag, overflowFlag);
            EXPECT_EQ(cpu.StatusFlags.NegativeFlag, negativeFlag);

            EXPECT_EQ(cpu.StatusFlags.CarryFlag,        initial.StatusFlags.CarryFlag);
            EXPECT_EQ(cpu.StatusFlags.IRQDisableFlag,   initial.StatusFlags.IRQDisableFlag);
            EXPECT_EQ(cpu.StatusFlags.DecimalMode,      initial.StatusFlags.DecimalMode);
            EXPECT_EQ(cpu.StatusFlags.BreakCommand,     initial.StatusFlags.BreakCommand);

            ASSERT_FALSE(cpu.DebugFlags.UnhandledInstruction);
            ASSERT_FALSE(cpu.DebugFlags.CycleOverflow);
        }

        void TestBitOpAbsolute(Byte value, Byte pattern)
        {
            // Arrange
            uint32 cyclesExpected = 4u;
            bool zeroFlag = (value & pattern) == 0;
            bool overflowFlag = (value & 1 << 6) > 0;
            bool negativeFlag = (value & 1 << 7) > 0;
            Word address = 0x4480;

            cpu.StatusFlags.OverflowFlag = !overflowFlag;
            cpu.StatusFlags.NegativeFlag = !negativeFlag;

            cpu.A = pattern;
            memory.WriteByte(0xFFFC, CPU::INS_BIT_ABS);
            memory.WriteWord(0xFFFD, address);
            memory.WriteByte(address, value);

            CPU initial = cpu;

            // Act
            auto cyclesUsed = cpu.Execute(cyclesExpected, memory);

            // Assert
            EXPECT_EQ(cyclesUsed, cyclesExpected);

            EXPECT_EQ(cpu.StatusFlags.ZeroFlag, zeroFlag);
            EXPECT_EQ(cpu.StatusFlags.OverflowFlag, overflowFlag);
            EXPECT_EQ(cpu.StatusFlags.NegativeFlag, negativeFlag);

            EXPECT_EQ(cpu.StatusFlags.CarryFlag,        initial.StatusFlags.CarryFlag);
            EXPECT_EQ(cpu.StatusFlags.IRQDisableFlag,   initial.StatusFlags.IRQDisableFlag);
            EXPECT_EQ(cpu.StatusFlags.DecimalMode,      initial.StatusFlags.DecimalMode);
            EXPECT_EQ(cpu.StatusFlags.BreakCommand,     initial.StatusFlags.BreakCommand);

            ASSERT_FALSE(cpu.DebugFlags.UnhandledInstruction);
            ASSERT_FALSE(cpu.DebugFlags.CycleOverflow);
        }

    };


    Byte OpAND(Byte a, Byte b) { return a & b; }
    Byte OpORA(Byte a, Byte b) { return a | b; }
    Byte OpEOR(Byte a, Byte b) { return a ^ b; }


    TEST_F(LogicalFixture, INS_AND_IM_WithSelf)
    { TestLogicalOpImmediate(CPU::INS_AND_IM, 0x42, 0x42, OpAND); }

    TEST_F(LogicalFixture, INS_AND_IM_WithIdentity)
    { TestLogicalOpImmediate(CPU::INS_AND_IM, 0x42, 0xFF, OpAND); }

    TEST_F(LogicalFixture, INS_AND_IM_WithZero)
    { TestLogicalOpImmediate(CPU::INS_AND_IM, 0x42, 0x00, OpAND); }

    TEST_F(LogicalFixture, INS_AND_IM_WithNegative)
    { TestLogicalOpImmediate(CPU::INS_AND_IM, 0x42, 0x80, OpAND); }

    TEST_F(LogicalFixture, INS_AND_IM_WithInitialFlags)
    { TestLogicalOpImmediate(CPU::INS_AND_IM, 0x42, 0x80, OpAND, 0xFF); }


    TEST_F(LogicalFixture, INS_AND_ZP_WithSelf)
    { TestLogicalOpZeroPage(CPU::INS_AND_ZP, 0x43, 0x43, OpAND); }

    TEST_F(LogicalFixture, INS_AND_ZP_WithIdentity)
    { TestLogicalOpZeroPage(CPU::INS_AND_ZP, 0x43, 0xFF, OpAND); }

    TEST_F(LogicalFixture, INS_AND_ZP_WithZero)
    { TestLogicalOpZeroPage(CPU::INS_AND_ZP, 0x43, 0x00, OpAND); }

    TEST_F(LogicalFixture, INS_AND_ZP_WithNegative)
    { TestLogicalOpZeroPage(CPU::INS_AND_ZP, 0x43, 0x80, OpAND); }

    TEST_F(LogicalFixture, INS_AND_ZP_WithInitialFlags)
    { TestLogicalOpZeroPage(CPU::INS_AND_ZP, 0x43, 0x80, OpAND, 0xFF); }


    TEST_F(LogicalFixture, INS_AND_ZPX_WithSelf)
    { TestLogicalOpZeroPageOffsetX(CPU::INS_AND_ZPX, 0x43, 0x43, OpAND); }

    TEST_F(LogicalFixture, INS_AND_ZPX_WithIdentity)
    { TestLogicalOpZeroPageOffsetX(CPU::INS_AND_ZPX, 0x43, 0xFF, OpAND); }

    TEST_F(LogicalFixture, INS_AND_ZPX_WithZero)
    { TestLogicalOpZeroPageOffsetX(CPU::INS_AND_ZPX, 0x43, 0x00, OpAND); }

    TEST_F(LogicalFixture, INS_AND_ZPX_WithNegative)
    { TestLogicalOpZeroPageOffsetX(CPU::INS_AND_ZPX, 0x43, 0x80, OpAND); }

    TEST_F(LogicalFixture, INS_AND_ZPX_WithWrapAddress)
    { TestLogicalOpZeroPageOffsetX(CPU::INS_AND_ZPX, 0x43, 0x72, OpAND, true); }

    TEST_F(LogicalFixture, INS_AND_ZPX_WithInitialFlags)
    { TestLogicalOpZeroPageOffsetX(CPU::INS_AND_ZPX, 0x43, 0x72, OpAND, false, 0xFF); }


    TEST_F(LogicalFixture, INS_AND_ABS_WithSelf)
    { TestLogicalOpAbsolute(CPU::INS_AND_ABS, 0x43, 0x43, OpAND); }

    TEST_F(LogicalFixture, INS_AND_ABS_WithIdentity)
    { TestLogicalOpAbsolute(CPU::INS_AND_ABS, 0x43, 0xFF, OpAND); }

    TEST_F(LogicalFixture, INS_AND_ABS_WithZero)
    { TestLogicalOpAbsolute(CPU::INS_AND_ABS, 0x43, 0x00, OpAND); }

    TEST_F(LogicalFixture, INS_AND_ABS_WithNegative)
    { TestLogicalOpAbsolute(CPU::INS_AND_ABS, 0x43, 0x80, OpAND); }

    TEST_F(LogicalFixture, INS_AND_ABS_WithInitialFlags)
    { TestLogicalOpAbsolute(CPU::INS_AND_ABS, 0x43, 0x80, OpAND, 0xFF); }


    TEST_F(LogicalFixture, INS_AND_ABSX_WithSelf)
    { TestLogicalOpAbsoluteXY(CPU::INS_AND_ABSX, 0x43, 0x43, OpAND, &CPU::X); }

    TEST_F(LogicalFixture, INS_AND_ABSX_WithIdentity)
    { TestLogicalOpAbsoluteXY(CPU::INS_AND_ABSX, 0x43, 0xFF, OpAND, &CPU::X); }

    TEST_F(LogicalFixture, INS_AND_ABSX_WithZero)
    { TestLogicalOpAbsoluteXY(CPU::INS_AND_ABSX, 0x43, 0x00, OpAND, &CPU::X); }

    TEST_F(LogicalFixture, INS_AND_ABSX_WithNegative)
    { TestLogicalOpAbsoluteXY(CPU::INS_AND_ABSX, 0x43, 0x80, OpAND, &CPU::X); }

    TEST_F(LogicalFixture, INS_AND_ABSX_WithCrossPageBoundary)
    { TestLogicalOpAbsoluteXY(CPU::INS_AND_ABSX, 0x43, 0x80, OpAND, &CPU::X, true); }

    TEST_F(LogicalFixture, INS_AND_ABSX_WithInitialFlags)
    { TestLogicalOpAbsoluteXY(CPU::INS_AND_ABSX, 0x43, 0x80, OpAND, &CPU::X, false, 0xFF); }


    TEST_F(LogicalFixture, INS_AND_ABSY_WithSelf)
    { TestLogicalOpAbsoluteXY(CPU::INS_AND_ABSY, 0x43, 0x43, OpAND, &CPU::Y); }

    TEST_F(LogicalFixture, INS_AND_ABSY_WithIdentity)
    { TestLogicalOpAbsoluteXY(CPU::INS_AND_ABSY, 0x43, 0xFF, OpAND, &CPU::Y); }

    TEST_F(LogicalFixture, INS_AND_ABSY_WithZero)
    { TestLogicalOpAbsoluteXY(CPU::INS_AND_ABSY, 0x43, 0x00, OpAND, &CPU::Y); }

    TEST_F(LogicalFixture, INS_AND_ABSY_WithNegative)
    { TestLogicalOpAbsoluteXY(CPU::INS_AND_ABSY, 0x43, 0x80, OpAND, &CPU::Y); }

    TEST_F(LogicalFixture, INS_AND_ABSY_WithCrossPageBoundary)
    { TestLogicalOpAbsoluteXY(CPU::INS_AND_ABSY, 0x43, 0x80, OpAND, &CPU::Y, true); }

    TEST_F(LogicalFixture, INS_AND_ABSY_WithInitialFlags)
    { TestLogicalOpAbsoluteXY(CPU::INS_AND_ABSY, 0x43, 0x80, OpAND, &CPU::Y, false, 0xFF); }


    TEST_F(LogicalFixture, INS_AND_INDX_WithSelf)
    { TestLogicalOpIndirectX(CPU::INS_AND_INDX, 0x43, 0x43, OpAND); }

    TEST_F(LogicalFixture, INS_AND_INDX_WithIdentity)
    { TestLogicalOpIndirectX(CPU::INS_AND_INDX, 0x43, 0xFF, OpAND); }

    TEST_F(LogicalFixture, INS_AND_INDX_WithZero)
    { TestLogicalOpIndirectX(CPU::INS_AND_INDX, 0x43, 0x00, OpAND); }

    TEST_F(LogicalFixture, INS_AND_INDX_WithNegative)
    { TestLogicalOpIndirectX(CPU::INS_AND_INDX, 0x43, 0x80, OpAND); }

    TEST_F(LogicalFixture, INS_AND_INDX_WithInitialFlags)
    { TestLogicalOpIndirectX(CPU::INS_AND_INDX, 0x43, 0x80, OpAND, 0xFF); }


    TEST_F(LogicalFixture, INS_AND_INDY_WithSelf)
    { TestLogicalOpIndirectY(CPU::INS_AND_INDY, 0x43, 0x43, OpAND); }

    TEST_F(LogicalFixture, INS_AND_INDY_WithIdentity)
    { TestLogicalOpIndirectY(CPU::INS_AND_INDY, 0x43, 0xFF, OpAND); }

    TEST_F(LogicalFixture, INS_AND_INDY_WithZero)
    { TestLogicalOpIndirectY(CPU::INS_AND_INDY, 0x43, 0x00, OpAND); }

    TEST_F(LogicalFixture, INS_AND_INDY_WithNegative)
    { TestLogicalOpIndirectY(CPU::INS_AND_INDY, 0x43, 0x80, OpAND); }

    TEST_F(LogicalFixture, INS_AND_INDY_WithCrossPageBoundary)
    { TestLogicalOpIndirectY(CPU::INS_AND_INDY, 0x43, 0x80, OpAND, true); }

    TEST_F(LogicalFixture, INS_AND_INDY_WithInitialFlags)
    { TestLogicalOpIndirectY(CPU::INS_AND_INDY, 0x43, 0x80, OpAND, false, 0xFF); }



    TEST_F(LogicalFixture, INS_ORA_IM_WithSelf)
    { TestLogicalOpImmediate(CPU::INS_ORA_IM, 0x42, 0x42, OpORA); }

    TEST_F(LogicalFixture, INS_ORA_IM_WithIdentity)
    { TestLogicalOpImmediate(CPU::INS_ORA_IM, 0x42, 0xFF, OpORA); }

    TEST_F(LogicalFixture, INS_ORA_IM_WithZero)
    { TestLogicalOpImmediate(CPU::INS_ORA_IM, 0x42, 0x00, OpORA); }

    TEST_F(LogicalFixture, INS_ORA_IM_WithNegative)
    { TestLogicalOpImmediate(CPU::INS_ORA_IM, 0x42, 0x80, OpORA); }

    TEST_F(LogicalFixture, INS_ORA_IM_WithInitialFlags)
    { TestLogicalOpImmediate(CPU::INS_ORA_IM, 0x42, 0x80, OpORA, 0xFF); }


    TEST_F(LogicalFixture, INS_ORA_ZP_WithSelf)
    { TestLogicalOpZeroPage(CPU::INS_ORA_ZP, 0x43, 0x43, OpORA); }

    TEST_F(LogicalFixture, INS_ORA_ZP_WithIdentity)
    { TestLogicalOpZeroPage(CPU::INS_ORA_ZP, 0x43, 0xFF, OpORA); }

    TEST_F(LogicalFixture, INS_ORA_ZP_WithZero)
    { TestLogicalOpZeroPage(CPU::INS_ORA_ZP, 0x43, 0x00, OpORA); }

    TEST_F(LogicalFixture, INS_ORA_ZP_WithNegative)
    { TestLogicalOpZeroPage(CPU::INS_ORA_ZP, 0x43, 0x80, OpORA); }

    TEST_F(LogicalFixture, INS_ORA_ZP_WithInitialFlags)
    { TestLogicalOpZeroPage(CPU::INS_ORA_ZP, 0x43, 0x80, OpORA, 0xFF); }


    TEST_F(LogicalFixture, INS_ORA_ZPX_WithSelf)
    { TestLogicalOpZeroPageOffsetX(CPU::INS_ORA_ZPX, 0x43, 0x43, OpORA); }

    TEST_F(LogicalFixture, INS_ORA_ZPX_WithIdentity)
    { TestLogicalOpZeroPageOffsetX(CPU::INS_ORA_ZPX, 0x43, 0xFF, OpORA); }

    TEST_F(LogicalFixture, INS_ORA_ZPX_WithZero)
    { TestLogicalOpZeroPageOffsetX(CPU::INS_ORA_ZPX, 0x43, 0x00, OpORA); }

    TEST_F(LogicalFixture, INS_ORA_ZPX_WithNegative)
    { TestLogicalOpZeroPageOffsetX(CPU::INS_ORA_ZPX, 0x43, 0x80, OpORA); }

    TEST_F(LogicalFixture, INS_ORA_ZPX_WithWrapAddress)
    { TestLogicalOpZeroPageOffsetX(CPU::INS_ORA_ZPX, 0x43, 0x72, OpORA, true); }

    TEST_F(LogicalFixture, INS_ORA_ZPX_WithInitialFlags)
    { TestLogicalOpZeroPageOffsetX(CPU::INS_ORA_ZPX, 0x43, 0x72, OpORA, false, 0xFF); }


    TEST_F(LogicalFixture, INS_ORA_ABS_WithSelf)
    { TestLogicalOpAbsolute(CPU::INS_ORA_ABS, 0x43, 0x43, OpORA); }

    TEST_F(LogicalFixture, INS_ORA_ABS_WithIdentity)
    { TestLogicalOpAbsolute(CPU::INS_ORA_ABS, 0x43, 0xFF, OpORA); }

    TEST_F(LogicalFixture, INS_ORA_ABS_WithZero)
    { TestLogicalOpAbsolute(CPU::INS_ORA_ABS, 0x43, 0x00, OpORA); }

    TEST_F(LogicalFixture, INS_ORA_ABS_WithNegative)
    { TestLogicalOpAbsolute(CPU::INS_ORA_ABS, 0x43, 0x80, OpORA); }

    TEST_F(LogicalFixture, INS_ORA_ABS_WithInitialFlags)
    { TestLogicalOpAbsolute(CPU::INS_ORA_ABS, 0x43, 0x80, OpORA, 0xFF); }


    TEST_F(LogicalFixture, INS_ORA_ABSX_WithSelf)
    { TestLogicalOpAbsoluteXY(CPU::INS_ORA_ABSX, 0x43, 0x43, OpORA, &CPU::X); }

    TEST_F(LogicalFixture, INS_ORA_ABSX_WithIdentity)
    { TestLogicalOpAbsoluteXY(CPU::INS_ORA_ABSX, 0x43, 0xFF, OpORA, &CPU::X); }

    TEST_F(LogicalFixture, INS_ORA_ABSX_WithZero)
    { TestLogicalOpAbsoluteXY(CPU::INS_ORA_ABSX, 0x43, 0x00, OpORA, &CPU::X); }

    TEST_F(LogicalFixture, INS_ORA_ABSX_WithNegative)
    { TestLogicalOpAbsoluteXY(CPU::INS_ORA_ABSX, 0x43, 0x80, OpORA, &CPU::X); }

    TEST_F(LogicalFixture, INS_ORA_ABSX_WithCrossPageBoundary)
    { TestLogicalOpAbsoluteXY(CPU::INS_ORA_ABSX, 0x43, 0x80, OpORA, &CPU::X, true); }

    TEST_F(LogicalFixture, INS_ORA_ABSX_WithInitialFlags)
    { TestLogicalOpAbsoluteXY(CPU::INS_ORA_ABSX, 0x43, 0x80, OpORA, &CPU::X, false, 0xFF); }


    TEST_F(LogicalFixture, INS_ORA_ABSY_WithSelf)
    { TestLogicalOpAbsoluteXY(CPU::INS_ORA_ABSY, 0x43, 0x43, OpORA, &CPU::Y); }

    TEST_F(LogicalFixture, INS_ORA_ABSY_WithIdentity)
    { TestLogicalOpAbsoluteXY(CPU::INS_ORA_ABSY, 0x43, 0xFF, OpORA, &CPU::Y); }

    TEST_F(LogicalFixture, INS_ORA_ABSY_WithZero)
    { TestLogicalOpAbsoluteXY(CPU::INS_ORA_ABSY, 0x43, 0x00, OpORA, &CPU::Y); }

    TEST_F(LogicalFixture, INS_ORA_ABSY_WithNegative)
    { TestLogicalOpAbsoluteXY(CPU::INS_ORA_ABSY, 0x43, 0x80, OpORA, &CPU::Y); }

    TEST_F(LogicalFixture, INS_ORA_ABSY_WithCrossPageBoundary)
    { TestLogicalOpAbsoluteXY(CPU::INS_ORA_ABSY, 0x43, 0x80, OpORA, &CPU::Y, true); }

    TEST_F(LogicalFixture, INS_ORA_ABSY_WithInitialFlags)
    { TestLogicalOpAbsoluteXY(CPU::INS_ORA_ABSY, 0x43, 0x80, OpORA, &CPU::Y, false, 0xFF); }


    TEST_F(LogicalFixture, INS_ORA_INDX_WithSelf)
    { TestLogicalOpIndirectX(CPU::INS_ORA_INDX, 0x43, 0x43, OpORA); }

    TEST_F(LogicalFixture, INS_ORA_INDX_WithIdentity)
    { TestLogicalOpIndirectX(CPU::INS_ORA_INDX, 0x43, 0xFF, OpORA); }

    TEST_F(LogicalFixture, INS_ORA_INDX_WithZero)
    { TestLogicalOpIndirectX(CPU::INS_ORA_INDX, 0x43, 0x00, OpORA); }

    TEST_F(LogicalFixture, INS_ORA_INDX_WithNegative)
    { TestLogicalOpIndirectX(CPU::INS_ORA_INDX, 0x43, 0x80, OpORA); }

    TEST_F(LogicalFixture, INS_ORA_INDX_WithInitialFlags)
    { TestLogicalOpIndirectX(CPU::INS_ORA_INDX, 0x43, 0x80, OpORA, 0xFF); }


    TEST_F(LogicalFixture, INS_ORA_INDY_WithSelf)
    { TestLogicalOpIndirectY(CPU::INS_ORA_INDY, 0x43, 0x43, OpORA); }

    TEST_F(LogicalFixture, INS_ORA_INDY_WithIdentity)
    { TestLogicalOpIndirectY(CPU::INS_ORA_INDY, 0x43, 0xFF, OpORA); }

    TEST_F(LogicalFixture, INS_ORA_INDY_WithZero)
    { TestLogicalOpIndirectY(CPU::INS_ORA_INDY, 0x43, 0x00, OpORA); }

    TEST_F(LogicalFixture, INS_ORA_INDY_WithNegative)
    { TestLogicalOpIndirectY(CPU::INS_ORA_INDY, 0x43, 0x80, OpORA); }

    TEST_F(LogicalFixture, INS_ORA_INDY_WithCrossPageBoundary)
    { TestLogicalOpIndirectY(CPU::INS_ORA_INDY, 0x43, 0x80, OpORA, true); }

    TEST_F(LogicalFixture, INS_ORA_INDY_WithInitialFlags)
    { TestLogicalOpIndirectY(CPU::INS_ORA_INDY, 0x43, 0x80, OpORA, false, 0xFF); }



    TEST_F(LogicalFixture, INS_EOR_IM_WithSelf)
    { TestLogicalOpImmediate(CPU::INS_EOR_IM, 0x42, 0x42, OpEOR); }

    TEST_F(LogicalFixture, INS_EOR_IM_WithIdentity)
    { TestLogicalOpImmediate(CPU::INS_EOR_IM, 0x42, 0xFF, OpEOR); }

    TEST_F(LogicalFixture, INS_EOR_IM_WithZero)
    { TestLogicalOpImmediate(CPU::INS_EOR_IM, 0x42, 0x00, OpEOR); }

    TEST_F(LogicalFixture, INS_EOR_IM_WithNegative)
    { TestLogicalOpImmediate(CPU::INS_EOR_IM, 0x42, 0x80, OpEOR); }

    TEST_F(LogicalFixture, INS_EOR_IM_WithInitialFlags)
    { TestLogicalOpImmediate(CPU::INS_EOR_IM, 0x42, 0x80, OpEOR, 0xFF); }


    TEST_F(LogicalFixture, INS_EOR_ZP_WithSelf)
    { TestLogicalOpZeroPage(CPU::INS_EOR_ZP, 0x43, 0x43, OpEOR); }

    TEST_F(LogicalFixture, INS_EOR_ZP_WithIdentity)
    { TestLogicalOpZeroPage(CPU::INS_EOR_ZP, 0x43, 0xFF, OpEOR); }

    TEST_F(LogicalFixture, INS_EOR_ZP_WithZero)
    { TestLogicalOpZeroPage(CPU::INS_EOR_ZP, 0x43, 0x00, OpEOR); }

    TEST_F(LogicalFixture, INS_EOR_ZP_WithNegative)
    { TestLogicalOpZeroPage(CPU::INS_EOR_ZP, 0x43, 0x80, OpEOR); }

    TEST_F(LogicalFixture, INS_EOR_ZP_WithInitialFlags)
    { TestLogicalOpZeroPage(CPU::INS_EOR_ZP, 0x43, 0x80, OpEOR, 0xFF); }


    TEST_F(LogicalFixture, INS_EOR_ZPX_WithSelf)
    { TestLogicalOpZeroPageOffsetX(CPU::INS_EOR_ZPX, 0x43, 0x43, OpEOR); }

    TEST_F(LogicalFixture, INS_EOR_ZPX_WithIdentity)
    { TestLogicalOpZeroPageOffsetX(CPU::INS_EOR_ZPX, 0x43, 0xFF, OpEOR); }

    TEST_F(LogicalFixture, INS_EOR_ZPX_WithZero)
    { TestLogicalOpZeroPageOffsetX(CPU::INS_EOR_ZPX, 0x43, 0x00, OpEOR); }

    TEST_F(LogicalFixture, INS_EOR_ZPX_WithNegative)
    { TestLogicalOpZeroPageOffsetX(CPU::INS_EOR_ZPX, 0x43, 0x80, OpEOR); }

    TEST_F(LogicalFixture, INS_EOR_ZPX_WithWrapAddress)
    { TestLogicalOpZeroPageOffsetX(CPU::INS_EOR_ZPX, 0x43, 0x72, OpEOR, true); }

    TEST_F(LogicalFixture, INS_EOR_ZPX_WithInitialFlags)
    { TestLogicalOpZeroPageOffsetX(CPU::INS_EOR_ZPX, 0x43, 0x72, OpEOR, false, 0xFF); }


    TEST_F(LogicalFixture, INS_EOR_ABS_WithSelf)
    { TestLogicalOpAbsolute(CPU::INS_EOR_ABS, 0x43, 0x43, OpEOR); }

    TEST_F(LogicalFixture, INS_EOR_ABS_WithIdentity)
    { TestLogicalOpAbsolute(CPU::INS_EOR_ABS, 0x43, 0xFF, OpEOR); }

    TEST_F(LogicalFixture, INS_EOR_ABS_WithZero)
    { TestLogicalOpAbsolute(CPU::INS_EOR_ABS, 0x43, 0x00, OpEOR); }

    TEST_F(LogicalFixture, INS_EOR_ABS_WithNegative)
    { TestLogicalOpAbsolute(CPU::INS_EOR_ABS, 0x43, 0x80, OpEOR); }

    TEST_F(LogicalFixture, INS_EOR_ABS_WithInitialFlags)
    { TestLogicalOpAbsolute(CPU::INS_EOR_ABS, 0x43, 0x80, OpEOR, 0xFF); }


    TEST_F(LogicalFixture, INS_EOR_ABSX_WithSelf)
    { TestLogicalOpAbsoluteXY(CPU::INS_EOR_ABSX, 0x43, 0x43, OpEOR, &CPU::X); }

    TEST_F(LogicalFixture, INS_EOR_ABSX_WithIdentity)
    { TestLogicalOpAbsoluteXY(CPU::INS_EOR_ABSX, 0x43, 0xFF, OpEOR, &CPU::X); }

    TEST_F(LogicalFixture, INS_EOR_ABSX_WithZero)
    { TestLogicalOpAbsoluteXY(CPU::INS_EOR_ABSX, 0x43, 0x00, OpEOR, &CPU::X); }

    TEST_F(LogicalFixture, INS_EOR_ABSX_WithNegative)
    { TestLogicalOpAbsoluteXY(CPU::INS_EOR_ABSX, 0x43, 0x80, OpEOR, &CPU::X); }

    TEST_F(LogicalFixture, INS_EOR_ABSX_WithCrossPageBoundary)
    { TestLogicalOpAbsoluteXY(CPU::INS_EOR_ABSX, 0x43, 0x80, OpEOR, &CPU::X, true); }

    TEST_F(LogicalFixture, INS_EOR_ABSX_WithInitialFlags)
    { TestLogicalOpAbsoluteXY(CPU::INS_EOR_ABSX, 0x43, 0x80, OpEOR, &CPU::X, false, 0xFF); }


    TEST_F(LogicalFixture, INS_EOR_ABSY_WithSelf)
    { TestLogicalOpAbsoluteXY(CPU::INS_EOR_ABSY, 0x43, 0x43, OpEOR, &CPU::Y); }

    TEST_F(LogicalFixture, INS_EOR_ABSY_WithIdentity)
    { TestLogicalOpAbsoluteXY(CPU::INS_EOR_ABSY, 0x43, 0xFF, OpEOR, &CPU::Y); }

    TEST_F(LogicalFixture, INS_EOR_ABSY_WithZero)
    { TestLogicalOpAbsoluteXY(CPU::INS_EOR_ABSY, 0x43, 0x00, OpEOR, &CPU::Y); }

    TEST_F(LogicalFixture, INS_EOR_ABSY_WithNegative)
    { TestLogicalOpAbsoluteXY(CPU::INS_EOR_ABSY, 0x43, 0x80, OpEOR, &CPU::Y); }

    TEST_F(LogicalFixture, INS_EOR_ABSY_WithCrossPageBoundary)
    { TestLogicalOpAbsoluteXY(CPU::INS_EOR_ABSY, 0x43, 0x80, OpEOR, &CPU::Y, true); }

    TEST_F(LogicalFixture, INS_EOR_ABSY_WithInitialFlags)
    { TestLogicalOpAbsoluteXY(CPU::INS_EOR_ABSY, 0x43, 0x80, OpEOR, &CPU::Y, false, 0xFF); }


    TEST_F(LogicalFixture, INS_EOR_INDX_WithSelf)
    { TestLogicalOpIndirectX(CPU::INS_EOR_INDX, 0x43, 0x43, OpEOR); }

    TEST_F(LogicalFixture, INS_EOR_INDX_WithIdentity)
    { TestLogicalOpIndirectX(CPU::INS_EOR_INDX, 0x43, 0xFF, OpEOR); }

    TEST_F(LogicalFixture, INS_EOR_INDX_WithZero)
    { TestLogicalOpIndirectX(CPU::INS_EOR_INDX, 0x43, 0x00, OpEOR); }

    TEST_F(LogicalFixture, INS_EOR_INDX_WithNegative)
    { TestLogicalOpIndirectX(CPU::INS_EOR_INDX, 0x43, 0x80, OpEOR); }

    TEST_F(LogicalFixture, INS_EOR_INDX_WithInitialFlags)
    { TestLogicalOpIndirectX(CPU::INS_EOR_INDX, 0x43, 0x80, OpEOR, 0xFF); }


    TEST_F(LogicalFixture, INS_EOR_INDY_WithSelf)
    { TestLogicalOpIndirectY(CPU::INS_EOR_INDY, 0x43, 0x43, OpEOR); }

    TEST_F(LogicalFixture, INS_EOR_INDY_WithIdentity)
    { TestLogicalOpIndirectY(CPU::INS_EOR_INDY, 0x43, 0xFF, OpEOR); }

    TEST_F(LogicalFixture, INS_EOR_INDY_WithZero)
    { TestLogicalOpIndirectY(CPU::INS_EOR_INDY, 0x43, 0x00, OpEOR); }

    TEST_F(LogicalFixture, INS_EOR_INDY_WithNegative)
    { TestLogicalOpIndirectY(CPU::INS_EOR_INDY, 0x43, 0x80, OpEOR); }

    TEST_F(LogicalFixture, INS_EOR_INDY_WithCrossPageBoundary)
    { TestLogicalOpIndirectY(CPU::INS_EOR_INDY, 0x43, 0x80, OpEOR, true); }

    TEST_F(LogicalFixture, INS_EOR_INDY_WithInitialFlags)
    { TestLogicalOpIndirectY(CPU::INS_EOR_INDY, 0x43, 0x80, OpEOR, false, 0xFF); }


    TEST_F(LogicalFixture, INS_BIT_ZP_WithSelf)
    { TestBitOpZeroPage(0xCC, 0xCC); }

    TEST_F(LogicalFixture, INS_BIT_ZP_WithOne)
    { TestBitOpZeroPage(0x39, 0xFF); }

    TEST_F(LogicalFixture, INS_BIT_ZP_WithZero)
    { TestBitOpZeroPage(0x42, 0x00); }

    TEST_F(LogicalFixture, INS_BIT_ZP_CheckFlags)
    { TestBitOpZeroPage(0x03, 0xB0); }


    TEST_F(LogicalFixture, INS_BIT_ABS_WithSelf)
    { TestBitOpAbsolute(0xCC, 0xCC); }

    TEST_F(LogicalFixture, INS_BIT_ABS_WithOne)
    { TestBitOpAbsolute(0x39, 0xFF); }

    TEST_F(LogicalFixture, INS_BIT_ABS_WithZero)
    { TestBitOpAbsolute(0xCC, 0x00); }

}