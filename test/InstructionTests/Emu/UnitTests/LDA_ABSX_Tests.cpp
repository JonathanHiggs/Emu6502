#include <Emu/UnitTests/CPUFixture.hpp>


using namespace Emu;
using namespace Emu::UnitTests;


TEST_F(CPUFixture, LDA_ABSX_CanLoadValueIntoRegister)
{
    // Arrange
    Byte registerValue = 0x01;
    Word baseAddress = 0x4480;
    Word offsetAddress = baseAddress + registerValue;
    Byte expected = 0x22;
    constexpr uint32 expectedCycles = 4u;

    cpu.X = registerValue;
    memory.WriteByte(0xFFFC, CPU::INS_LDA_ABSX);
    memory.WriteWord(0xFFFD, baseAddress);
    memory.WriteByte(offsetAddress, expected);

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


TEST_F(CPUFixture, LDA_ABSX_WithNegativeValue_CanLoadValueIntoRegister)
{
    // Arrange
    Byte registerValue = 0x01;
    Word baseAddress = 0x4480;
    Word offsetAddress = baseAddress + registerValue;
    Byte expected = 0xA2;
    constexpr uint32 expectedCycles = 4u;

    cpu.X = registerValue;
    memory.WriteByte(0xFFFC, CPU::INS_LDA_ABSX);
    memory.WriteWord(0xFFFD, baseAddress);
    memory.WriteByte(offsetAddress, expected);

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


TEST_F(CPUFixture, LDA_ABSX_WithZeroValue_CanLoadValueIntoRegister)
{
    // Arrange
    Byte registerValue = 0x01;
    Word baseAddress = 0x4480;
    Word offsetAddress = baseAddress + registerValue;
    Byte expected = 0x00;
    constexpr uint32 expectedCycles = 4u;

    cpu.X = registerValue;
    memory.WriteByte(0xFFFC, CPU::INS_LDA_ABSX);
    memory.WriteWord(0xFFFD, baseAddress);
    memory.WriteByte(offsetAddress, expected);

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


TEST_F(CPUFixture, LDA_ABSX_WithCrossPageBoundary_TakesFiveClockCycles)
{
    // Arrange
    Byte registerValue = 0xFF;
    Word baseAddress = 0x4402;
    Word offsetAddress = baseAddress + registerValue;
    Byte expected = 0x07;
    constexpr uint32 expectedCycles = 5u;

    cpu.X = registerValue;
    memory.WriteByte(0xFFFC, CPU::INS_LDA_ABSX);
    memory.WriteWord(0xFFFD, baseAddress);
    memory.WriteByte(offsetAddress, expected);

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


TEST_F(CPUFixture, LDA_ABSX_WithCrossPageBoundary2_TakesFiveClockCycles)
{
    // Arrange
    Byte registerValue = 0x06;
    Word baseAddress = 0x44FE;
    Word offsetAddress = baseAddress + registerValue;
    Byte expected = 0x07;
    constexpr uint32 expectedCycles = 5u;

    cpu.X = registerValue;
    memory.WriteByte(0xFFFC, CPU::INS_LDA_ABSX);
    memory.WriteWord(0xFFFD, baseAddress);
    memory.WriteByte(offsetAddress, expected);

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