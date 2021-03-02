#include <Emu/UnitTests/CPUFixture.hpp>


using namespace Emu;
using namespace Emu::UnitTests;


TEST_F(CPUFixture, LDA_INDY_LoadsValueIntoRegister)
{
    // Arrange
    Byte offsetValue = 0x04;
    Word zeroPageAddress = 0x0002;
    Word indexAddress = 0x4480;
    Word valueAddress = indexAddress + offsetValue;
    Byte expected = 0x22;
    constexpr uint32 expectedCycles = 5u;

    cpu.Y = offsetValue;
    memory.WriteByte(0xFFFC, CPU::INS_LDA_INDY);
    memory.WriteByte(0xFFFD, (Byte)zeroPageAddress);
    memory.WriteWord(zeroPageAddress, indexAddress);
    memory.WriteByte(valueAddress, expected);

    CPU initial = cpu;

    // Act
    auto cyclesUsed = cpu.Execute(expectedCycles, memory);

    // Assert
    EXPECT_EQ(cpu.A, expected);
    EXPECT_FALSE(cpu.ZeroFlag);
    EXPECT_FALSE(cpu.NegativeFlag);
    EXPECT_EQ(cyclesUsed, expectedCycles);

    EXPECT_EQ(cpu.CarryFlag,        initial.CarryFlag);
    EXPECT_EQ(cpu.IRQDisableFlag,   initial.IRQDisableFlag);
    EXPECT_EQ(cpu.DecimalMode,      initial.DecimalMode);
    EXPECT_EQ(cpu.BreakCommand,     initial.BreakCommand);
    EXPECT_EQ(cpu.OverflowFlag,     initial.OverflowFlag);

    ASSERT_FALSE(cpu.UnhandledInstruction);
    ASSERT_FALSE(cpu.CycleOverflow);
}


TEST_F(CPUFixture, LDA_INDY_WithNegativeValue_LoadsValueIntoRegister)
{
    // Arrange
    Byte offsetValue = 0x04;
    Word zeroPageAddress = 0x0002;
    Word indexAddress = 0x4480;
    Word valueAddress = indexAddress + offsetValue;
    Byte expected = 0xC2;
    constexpr uint32 expectedCycles = 5u;

    cpu.Y = offsetValue;
    memory.WriteByte(0xFFFC, CPU::INS_LDA_INDY);
    memory.WriteByte(0xFFFD, (Byte)zeroPageAddress);
    memory.WriteWord(zeroPageAddress, indexAddress);
    memory.WriteByte(valueAddress, expected);

    CPU initial = cpu;

    // Act
    auto cyclesUsed = cpu.Execute(expectedCycles, memory);

    // Assert
    EXPECT_EQ(cpu.A, expected);
    EXPECT_FALSE(cpu.ZeroFlag);
    EXPECT_TRUE(cpu.NegativeFlag);
    EXPECT_EQ(cyclesUsed, expectedCycles);

    EXPECT_EQ(cpu.CarryFlag,        initial.CarryFlag);
    EXPECT_EQ(cpu.IRQDisableFlag,   initial.IRQDisableFlag);
    EXPECT_EQ(cpu.DecimalMode,      initial.DecimalMode);
    EXPECT_EQ(cpu.BreakCommand,     initial.BreakCommand);
    EXPECT_EQ(cpu.OverflowFlag,     initial.OverflowFlag);

    ASSERT_FALSE(cpu.UnhandledInstruction);
    ASSERT_FALSE(cpu.CycleOverflow);
}


TEST_F(CPUFixture, LDA_INDY_WithZeroValue_LoadsValueIntoRegister)
{
    // Arrange
    Byte offsetValue = 0x04;
    Word zeroPageAddress = 0x0002;
    Word indexAddress = 0x4480;
    Word valueAddress = indexAddress + offsetValue;
    Byte expected = 0x00;
    constexpr uint32 expectedCycles = 5u;

    cpu.Y = offsetValue;
    memory.WriteByte(0xFFFC, CPU::INS_LDA_INDY);
    memory.WriteByte(0xFFFD, (Byte)zeroPageAddress);
    memory.WriteWord(zeroPageAddress, indexAddress);
    memory.WriteByte(valueAddress, expected);

    CPU initial = cpu;

    // Act
    auto cyclesUsed = cpu.Execute(expectedCycles, memory);

    // Assert
    EXPECT_EQ(cpu.A, expected);
    EXPECT_TRUE(cpu.ZeroFlag);
    EXPECT_FALSE(cpu.NegativeFlag);
    EXPECT_EQ(cyclesUsed, expectedCycles);

    EXPECT_EQ(cpu.CarryFlag,        initial.CarryFlag);
    EXPECT_EQ(cpu.IRQDisableFlag,   initial.IRQDisableFlag);
    EXPECT_EQ(cpu.DecimalMode,      initial.DecimalMode);
    EXPECT_EQ(cpu.BreakCommand,     initial.BreakCommand);
    EXPECT_EQ(cpu.OverflowFlag,     initial.OverflowFlag);

    ASSERT_FALSE(cpu.UnhandledInstruction);
    ASSERT_FALSE(cpu.CycleOverflow);
}


TEST_F(CPUFixture, LDA_INDY_WithPageCross_LoadsValueIntoRegister)
{
    // Arrange
    Byte offsetValue = 0x04;
    Word zeroPageAddress = 0x0002;
    Word indexAddress = 0x44FE;
    Word valueAddress = indexAddress + offsetValue;
    Byte expected = 0x22;
    constexpr uint32 expectedCycles = 6u;

    cpu.Y = offsetValue;
    memory.WriteByte(0xFFFC, CPU::INS_LDA_INDY);
    memory.WriteByte(0xFFFD, (Byte)zeroPageAddress);
    memory.WriteWord(zeroPageAddress, indexAddress);
    memory.WriteByte(valueAddress, expected);

    CPU initial = cpu;

    // Act
    auto cyclesUsed = cpu.Execute(expectedCycles, memory);

    // Assert
    EXPECT_EQ(cpu.A, expected);
    EXPECT_FALSE(cpu.ZeroFlag);
    EXPECT_FALSE(cpu.NegativeFlag);
    EXPECT_EQ(cyclesUsed, expectedCycles);

    EXPECT_EQ(cpu.CarryFlag,        initial.CarryFlag);
    EXPECT_EQ(cpu.IRQDisableFlag,   initial.IRQDisableFlag);
    EXPECT_EQ(cpu.DecimalMode,      initial.DecimalMode);
    EXPECT_EQ(cpu.BreakCommand,     initial.BreakCommand);
    EXPECT_EQ(cpu.OverflowFlag,     initial.OverflowFlag);

    ASSERT_FALSE(cpu.UnhandledInstruction);
    ASSERT_FALSE(cpu.CycleOverflow);
}


TEST_F(CPUFixture, LDA_INDY_WithPageCross2_LoadsValueIntoRegister)
{
    // Arrange
    Byte offsetValue = 0xFE;
    Word zeroPageAddress = 0x0002;
    Word indexAddress = 0x4404;
    Word valueAddress = indexAddress + offsetValue;
    Byte expected = 0x22;
    constexpr uint32 expectedCycles = 6u;

    cpu.Y = offsetValue;
    memory.WriteByte(0xFFFC, CPU::INS_LDA_INDY);
    memory.WriteByte(0xFFFD, (Byte)zeroPageAddress);
    memory.WriteWord(zeroPageAddress, indexAddress);
    memory.WriteByte(valueAddress, expected);

    CPU initial = cpu;

    // Act
    auto cyclesUsed = cpu.Execute(expectedCycles, memory);

    // Assert
    EXPECT_EQ(cpu.A, expected);
    EXPECT_FALSE(cpu.ZeroFlag);
    EXPECT_FALSE(cpu.NegativeFlag);
    EXPECT_EQ(cyclesUsed, expectedCycles);

    EXPECT_EQ(cpu.CarryFlag,        initial.CarryFlag);
    EXPECT_EQ(cpu.IRQDisableFlag,   initial.IRQDisableFlag);
    EXPECT_EQ(cpu.DecimalMode,      initial.DecimalMode);
    EXPECT_EQ(cpu.BreakCommand,     initial.BreakCommand);
    EXPECT_EQ(cpu.OverflowFlag,     initial.OverflowFlag);

    ASSERT_FALSE(cpu.UnhandledInstruction);
    ASSERT_FALSE(cpu.CycleOverflow);
}