#include <gtest/gtest.h>

#include <Emu/CPU.hpp>
#include <Emu/Memory.hpp>

namespace Emu::UnitTests
{

    class LoadRegisterFixture : public testing::Test
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

        void AssertRegisters(
            CPU & initialCpu,
            Byte value,
            Byte CPU::* reg,
            uint32 cyclesUsed,
            uint32 expectedCycles)
        {
            EXPECT_EQ(cpu.*reg, value);

            if (value == 0x00)
                EXPECT_TRUE(cpu.ZeroFlag);
            else
                EXPECT_FALSE(cpu.ZeroFlag);

            if (value >= 0x80)
                EXPECT_TRUE(cpu.NegativeFlag);
            else
                EXPECT_FALSE(cpu.NegativeFlag);

            EXPECT_EQ(cyclesUsed, expectedCycles);

            EXPECT_EQ(cpu.CarryFlag,        initialCpu.CarryFlag);
            EXPECT_EQ(cpu.IRQDisableFlag,   initialCpu.IRQDisableFlag);
            EXPECT_EQ(cpu.DecimalMode,      initialCpu.DecimalMode);
            EXPECT_EQ(cpu.BreakCommand,     initialCpu.BreakCommand);
            EXPECT_EQ(cpu.OverflowFlag,     initialCpu.OverflowFlag);

            ASSERT_FALSE(cpu.UnhandledInstruction);
            ASSERT_FALSE(cpu.CycleOverflow);
        }

        void ImmediateTest(
            Byte opCode,
            Byte CPU::* resultRegister,
            uint32 expectedCycles,
            Byte value)
        {
            // Arrange
            memory.WriteByte(0xFFFC, opCode);
            memory.WriteByte(0xFFFD, value);

            CPU initial = cpu;

            // Act
            auto cyclesUsed = cpu.Execute(expectedCycles, memory);

            // Assert
            AssertRegisters(initial, value, resultRegister, cyclesUsed, expectedCycles);
        }

        void Immediate_WithPositiveValue(Byte opCode, Byte CPU::* resultRegister)
        { ImmediateTest(opCode, resultRegister, 2u, 0x23); }

        void Immediate_WithNegativeValue(Byte opCode, Byte CPU::* resultRegister)
        { ImmediateTest(opCode, resultRegister, 2u, 0xC3); }

        void Immediate_WithZeroValue(Byte opCode, Byte CPU::* resultRegister)
        { ImmediateTest(opCode, resultRegister, 2u, 0x00); }


        void ZeroPageTest(
            Byte opCode,
            Byte CPU::* resultRegister,
            uint32 expectedCycles,
            Byte value)
        {
            // Arrange
            Byte address = 0x42;

            memory.WriteByte(0xFFFC, opCode);
            memory.WriteByte(0xFFFD, address);
            memory.WriteByte(address, value);

            CPU initial = cpu;

            // Act
            auto cyclesUsed = cpu.Execute(expectedCycles, memory);

            // Assert
            AssertRegisters(initial, value, resultRegister, cyclesUsed, expectedCycles);
        }

        void ZeroPage_WithPositiveValue(Byte opCode, Byte CPU::*reg)
        { ZeroPageTest(opCode, reg, 3u, 0x23); }

        void ZeroPage_WithNegativeValue(Byte opCode, Byte CPU::*reg)
        { ZeroPageTest(opCode, reg, 3u, 0xD3); }

        void ZeroPage_WithZeroValue(Byte opCode, Byte CPU::*reg)
        { ZeroPageTest(opCode, reg, 3u, 0x00); }


        void ZeroPageOffsetTest(
            Byte opCode,
            Byte CPU::* offsetRegister,
            Byte CPU::* resultRegister,
            uint32 expectedCycles,
            Byte value,
            bool wrapAddress = false)
        {
            // Arrange
            Byte registerValue = wrapAddress ? 0xFF : 0x05;
            Byte baseAddress = 0x42;
            Byte offsetAddress = (baseAddress + registerValue) % 0x100;

            cpu.*offsetRegister = registerValue;
            memory.WriteByte(0xFFFC, opCode);
            memory.WriteByte(0xFFFD, baseAddress);
            memory.WriteByte(offsetAddress, value);

            CPU initial = cpu;

            // Act
            auto cyclesUsed = cpu.Execute(expectedCycles, memory);

            // Assert
            AssertRegisters(initial, value, resultRegister, cyclesUsed, expectedCycles);
        }

        void ZeroPageOffset_WithPositiveValue(Byte opCode, Byte CPU::* offsetRegister, Byte CPU::* resultRegister)
        { ZeroPageOffsetTest(opCode, offsetRegister, resultRegister, 4u, 0x42); }

        void ZeroPageOffset_WithNegativeValue(Byte opCode, Byte CPU::* offsetRegister, Byte CPU::* resultRegister)
        { ZeroPageOffsetTest(opCode, offsetRegister, resultRegister, 4u, 0xE2); }

        void ZeroPageOffset_WithZeroValue(Byte opCode, Byte CPU::* offsetRegister, Byte CPU::* resultRegister)
        { ZeroPageOffsetTest(opCode, offsetRegister, resultRegister, 4u, 0x00); }

        void ZeroPageOffset_WithWrappedAddress(Byte opCode, Byte CPU::* offsetRegister, Byte CPU::* resultRegister)
        { ZeroPageOffsetTest(opCode, offsetRegister, resultRegister, 4u, 0x42, true); }


        void AbsoluteTest(
            Byte opCode,
            Byte CPU::* resultRegister,
            uint32 expectedCycles,
            Byte value)
        {
            // Arrange
            Word address = 0x4480;

            memory.WriteByte(0xFFFC, opCode);
            memory.WriteWord(0xFFFD, address);
            memory.WriteByte(address, value);

            CPU initial = cpu;

            // Act
            auto cyclesUsed = cpu.Execute(expectedCycles, memory);

            // Assert
            AssertRegisters(initial, value, resultRegister, cyclesUsed, expectedCycles);
        }

        void Absolute_WithPositiveValue(Byte opCode, Byte CPU::* resultRegister)
        { AbsoluteTest(opCode, resultRegister, 4u, 0x11); }

        void Absolute_WithNegativeValue(Byte opCode, Byte CPU::* resultRegister)
        { AbsoluteTest(opCode, resultRegister, 4u, 0xD1); }

        void Absolute_WithZeroValue(Byte opCode, Byte CPU::* resultRegister)
        { AbsoluteTest(opCode, resultRegister, 4u, 0x00); }


        void AbsoluteXTest(
            Byte opCode,
            Byte CPU::* resultRegister,
            uint32 expectedCycles,
            Byte value,
            bool crossPageBoundary = false)
        {
            assert(resultRegister != &CPU::X);

            // Arrange
            Byte registerValue = crossPageBoundary ? 0xFF : 0x01;
            Word baseAddress = 0x4480;
            Word offsetAddress = baseAddress + registerValue;

            cpu.X = registerValue;
            memory.WriteByte(0xFFFC, opCode);
            memory.WriteWord(0xFFFD, baseAddress);
            memory.WriteByte(offsetAddress, value);

            CPU initial = cpu;

            // Act
            auto cyclesUsed = cpu.Execute(expectedCycles, memory);

            // Assert
            AssertRegisters(initial, value, resultRegister, cyclesUsed, expectedCycles);
        }

        void AbsoluteX_WithPositiveValue(Byte opCode, Byte CPU::* resultRegister)
        { AbsoluteXTest(opCode, resultRegister, 4u, 0x22); }

        void AbsoluteX_WithNegativeValue(Byte opCode, Byte CPU::* resultRegister)
        { AbsoluteXTest(opCode, resultRegister, 4u, 0xA2); }

        void AbsoluteX_WithZeroValue(Byte opCode, Byte CPU::* resultRegister)
        { AbsoluteXTest(opCode, resultRegister, 4u, 0x00); }

        void AbsoluteX_WithCrossPageBoundary(Byte opCode, Byte CPU::* resultRegister)
        { AbsoluteXTest(opCode, resultRegister, 5u, 0x01, true); }


        void AbsoluteYTest(
            Byte opCode,
            Byte CPU::* resultRegister,
            uint32 expectedCycles,
            Byte value,
            bool crossPageBoundary = false)
        {
            assert(resultRegister != &CPU::Y);

            // Arrange
            Byte registerValue = crossPageBoundary ? 0xFF : 0x01;
            Word baseAddress = 0x4480;
            Word offsetAddress = baseAddress + registerValue;

            cpu.Y = registerValue;
            memory.WriteByte(0xFFFC, opCode);
            memory.WriteWord(0xFFFD, baseAddress);
            memory.WriteByte(offsetAddress, value);

            CPU initial = cpu;

            // Act
            auto cyclesUsed = cpu.Execute(expectedCycles, memory);

            // Assert
            AssertRegisters(initial, value, resultRegister, cyclesUsed, expectedCycles);
        }

        void AbsoluteY_WithPositiveValue(Byte opCode, Byte CPU::* resultRegister)
        { AbsoluteYTest(opCode, resultRegister, 4u, 0x22); }

        void AbsoluteY_WithNegativeValue(Byte opCode, Byte CPU::* resultRegister)
        { AbsoluteYTest(opCode, resultRegister, 4u, 0xA2); }

        void AbsoluteY_WithZeroValue(Byte opCode, Byte CPU::* resultRegister)
        { AbsoluteYTest(opCode, resultRegister, 4u, 0x00); }

        void AbsoluteY_WithCrossPageBoundary(Byte opCode, Byte CPU::* resultRegister)
        { AbsoluteYTest(opCode, resultRegister, 5u, 0x01, true); }
    };


    /* --------- LDA --------- */
    // LDA_IM
    TEST_F(LoadRegisterFixture, LDA_IM_WithPositiveValue)
    { Immediate_WithPositiveValue(CPU::INS_LDA_IM, &CPU::A); }

    TEST_F(LoadRegisterFixture, LDA_IM_WithNegativeValue)
    { Immediate_WithNegativeValue(CPU::INS_LDA_IM, &CPU::A); }

    TEST_F(LoadRegisterFixture, LDA_IM_WithZeroValue)
    { Immediate_WithZeroValue(CPU::INS_LDA_IM, &CPU::A); }


    // LDA_ZP
    TEST_F(LoadRegisterFixture, LDA_ZP_WithPositiveValue)
    { ZeroPage_WithPositiveValue(CPU::INS_LDA_ZP, &CPU::A); }

    TEST_F(LoadRegisterFixture, LDA_ZP_WithNegativeValue)
    { ZeroPage_WithNegativeValue(CPU::INS_LDA_ZP, &CPU::A); }

    TEST_F(LoadRegisterFixture, LDA_ZP_WithZeroValue)
    { ZeroPage_WithZeroValue(CPU::INS_LDA_ZP, &CPU::A); }


    // LDA_ZPX
    TEST_F(LoadRegisterFixture, LDA_ZPX_WithPositiveValue)
    { ZeroPageOffset_WithPositiveValue(CPU::INS_LDA_ZPX, &CPU::X, &CPU::A); }

    TEST_F(LoadRegisterFixture, LDA_ZPX_WithNegativeValue)
    { ZeroPageOffset_WithNegativeValue(CPU::INS_LDA_ZPX, &CPU::X, &CPU::A); }

    TEST_F(LoadRegisterFixture, LDA_ZPX_WithZeroValue)
    { ZeroPageOffset_WithZeroValue(CPU::INS_LDA_ZPX, &CPU::X, &CPU::A); }

    TEST_F(LoadRegisterFixture, LDA_ZPX_WithWrappedAddress)
    { ZeroPageOffset_WithWrappedAddress(CPU::INS_LDA_ZPX, &CPU::X, &CPU::A); }


    // LDA_ABS
    TEST_F(LoadRegisterFixture, LDA_ABS_WithPositiveValue)
    { Absolute_WithPositiveValue(CPU::INS_LDA_ABS, &CPU::A); }

    TEST_F(LoadRegisterFixture, LDA_ABS_WithNegativeValue)
    { Absolute_WithNegativeValue(CPU::INS_LDA_ABS, &CPU::A); }

    TEST_F(LoadRegisterFixture, LDA_ABS_WithZeroValue)
    { Absolute_WithZeroValue(CPU::INS_LDA_ABS, &CPU::A); }


    // LDA_ABSX
    TEST_F(LoadRegisterFixture, LDA_ABSX_WithPositiveValue)
    { AbsoluteX_WithPositiveValue(CPU::INS_LDA_ABSX, &CPU::A); }

    TEST_F(LoadRegisterFixture, LDA_ABSX_WithNegativeValue)
    { AbsoluteX_WithNegativeValue(CPU::INS_LDA_ABSX, &CPU::A); }

    TEST_F(LoadRegisterFixture, LDA_ABSX_WithZeroValue)
    { AbsoluteX_WithZeroValue(CPU::INS_LDA_ABSX, &CPU::A); }

    TEST_F(LoadRegisterFixture, LDA_ABSX_WithCrossPageBoundary)
    { AbsoluteX_WithCrossPageBoundary(CPU::INS_LDA_ABSX, &CPU::A); }


    // LDA_ABSY
    TEST_F(LoadRegisterFixture, LDA_ABSY_WithPositiveValue)
    { AbsoluteY_WithPositiveValue(CPU::INS_LDA_ABSY, &CPU::A); }

    TEST_F(LoadRegisterFixture, LDA_ABSY_WithNegativeValue)
    { AbsoluteY_WithNegativeValue(CPU::INS_LDA_ABSY, &CPU::A); }

    TEST_F(LoadRegisterFixture, LDA_ABSY_WithZeroValue)
    { AbsoluteY_WithZeroValue(CPU::INS_LDA_ABSY, &CPU::A); }

    TEST_F(LoadRegisterFixture, LDA_ABSY_WithCrossPageBoundary)
    { AbsoluteY_WithCrossPageBoundary(CPU::INS_LDA_ABSY, &CPU::A); }


    /* --------- LDX --------- */
    // LDX_IM
    TEST_F(LoadRegisterFixture, LDX_IM_WithPositiveValue)
    { Immediate_WithPositiveValue(CPU::INS_LDX_IM, &CPU::X); }

    TEST_F(LoadRegisterFixture, LDX_IM_WithNegativeValue)
    { Immediate_WithNegativeValue(CPU::INS_LDX_IM, &CPU::X); }

    TEST_F(LoadRegisterFixture, LDX_IM_WithZeroValue)
    { Immediate_WithZeroValue(CPU::INS_LDX_IM, &CPU::X); }


    // LDX_ZP
    TEST_F(LoadRegisterFixture, LDX_ZP_WithPositiveValue)
    { ZeroPage_WithPositiveValue(CPU::INS_LDX_ZP, &CPU::X); }

    TEST_F(LoadRegisterFixture, LDX_ZP_WithNegativeValue)
    { ZeroPage_WithNegativeValue(CPU::INS_LDX_ZP, &CPU::X); }

    TEST_F(LoadRegisterFixture, LDX_ZP_WithZeroValue)
    { ZeroPage_WithZeroValue(CPU::INS_LDX_ZP, &CPU::X); }


    // LDX_ZPY
    TEST_F(LoadRegisterFixture, LDX_ZPY_WithPositiveValue)
    { ZeroPageOffset_WithPositiveValue(CPU::INS_LDX_ZPY, &CPU::Y, &CPU::X); }

    TEST_F(LoadRegisterFixture, LDX_ZPY_WithNegativeValue)
    { ZeroPageOffset_WithNegativeValue(CPU::INS_LDX_ZPY, &CPU::Y, &CPU::X); }

    TEST_F(LoadRegisterFixture, LDX_ZPY_WithZeroValue)
    { ZeroPageOffset_WithZeroValue(CPU::INS_LDX_ZPY, &CPU::Y, &CPU::X); }

    TEST_F(LoadRegisterFixture, LDX_ZPY_WithWrappedAddress)
    { ZeroPageOffset_WithWrappedAddress(CPU::INS_LDX_ZPY, &CPU::Y, &CPU::X); }


    // LDX_ABS
    TEST_F(LoadRegisterFixture, LDX_ABS_WithPositiveValue)
    { Absolute_WithPositiveValue(CPU::INS_LDX_ABS, &CPU::X); }

    TEST_F(LoadRegisterFixture, LDX_ABS_WithNegativeValue)
    { Absolute_WithNegativeValue(CPU::INS_LDX_ABS, &CPU::X); }

    TEST_F(LoadRegisterFixture, LDX_ABS_WithZeroValue)
    { Absolute_WithZeroValue(CPU::INS_LDX_ABS, &CPU::X); }


    // LDX_ABSY
    TEST_F(LoadRegisterFixture, LDX_ABSY_WithPositiveValue)
    { AbsoluteY_WithPositiveValue(CPU::INS_LDX_ABSY, &CPU::X); }

    TEST_F(LoadRegisterFixture, LDX_ABSY_WithNegativeValue)
    { AbsoluteY_WithNegativeValue(CPU::INS_LDX_ABSY, &CPU::X); }

    TEST_F(LoadRegisterFixture, LDX_ABSY_WithZeroValue)
    { AbsoluteY_WithZeroValue(CPU::INS_LDX_ABSY, &CPU::X); }

    TEST_F(LoadRegisterFixture, LDX_ABSY_WithCrossPageBoundary)
    { AbsoluteY_WithCrossPageBoundary(CPU::INS_LDX_ABSY, &CPU::X); }


    /* --------- LDY --------- */
    // LDY_IM
    TEST_F(LoadRegisterFixture, LDY_IM_WithPositiveValue)
    { Immediate_WithPositiveValue(CPU::INS_LDY_IM, &CPU::Y); }

    TEST_F(LoadRegisterFixture, LDY_IM_WithNegativeValue)
    { Immediate_WithNegativeValue(CPU::INS_LDY_IM, &CPU::Y); }

    TEST_F(LoadRegisterFixture, LDY_IM_WithZeroValue)
    { Immediate_WithZeroValue(CPU::INS_LDY_IM, &CPU::Y); }


    // LDY_ZP
    TEST_F(LoadRegisterFixture, LDY_ZP_WithPositiveValue)
    { ZeroPage_WithPositiveValue(CPU::INS_LDY_ZP, &CPU::Y); }

    TEST_F(LoadRegisterFixture, LDY_ZP_WithNegativeValue)
    { ZeroPage_WithNegativeValue(CPU::INS_LDY_ZP, &CPU::Y); }

    TEST_F(LoadRegisterFixture, LDY_ZP_WithZeroValue)
    { ZeroPage_WithZeroValue(CPU::INS_LDY_ZP, &CPU::Y); }


    // LDY_ZPX
    TEST_F(LoadRegisterFixture, LDY_ZPX_WithPositiveValue)
    { ZeroPageOffset_WithPositiveValue(CPU::INS_LDY_ZPX, &CPU::X, &CPU::Y); }

    TEST_F(LoadRegisterFixture, LDY_ZPX_WithNegativeValue)
    { ZeroPageOffset_WithNegativeValue(CPU::INS_LDY_ZPX, &CPU::X, &CPU::Y); }

    TEST_F(LoadRegisterFixture, LDY_ZPX_WithZeroValue)
    { ZeroPageOffset_WithZeroValue(CPU::INS_LDY_ZPX, &CPU::X, &CPU::Y); }

    TEST_F(LoadRegisterFixture, LDY_ZPX_WithWrappedAddress)
    { ZeroPageOffset_WithWrappedAddress(CPU::INS_LDY_ZPX, &CPU::X, &CPU::Y); }


    // LDY_ABS
    TEST_F(LoadRegisterFixture, LDY_ABS_WithPositiveValue)
    { Absolute_WithPositiveValue(CPU::INS_LDY_ABS, &CPU::Y); }

    TEST_F(LoadRegisterFixture, LDY_ABS_WithNegativeValue)
    { Absolute_WithNegativeValue(CPU::INS_LDY_ABS, &CPU::Y); }

    TEST_F(LoadRegisterFixture, LDY_ABS_WithZeroValue)
    { Absolute_WithZeroValue(CPU::INS_LDY_ABS, &CPU::Y); }


    // LDY_ABSX
    TEST_F(LoadRegisterFixture, LDY_ABSX_WithPositiveValue)
    { AbsoluteX_WithPositiveValue(CPU::INS_LDY_ABSX, &CPU::Y); }

    TEST_F(LoadRegisterFixture, LDY_ABSX_WithNegativeValue)
    { AbsoluteX_WithNegativeValue(CPU::INS_LDY_ABSX, &CPU::Y); }

    TEST_F(LoadRegisterFixture, LDY_ABSX_WithZeroValue)
    { AbsoluteX_WithZeroValue(CPU::INS_LDY_ABSX, &CPU::Y); }

    TEST_F(LoadRegisterFixture, LDY_ABSX_WithCrossPageBoundary)
    { AbsoluteX_WithCrossPageBoundary(CPU::INS_LDY_ABSX, &CPU::Y); }

}