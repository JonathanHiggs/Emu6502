#include <Emu.UnitTests/CPUFixture.hpp>


TEST_F(CPUFixture, LDA_INDX_LoadsValueIntoRegister)
{
    // Arrange
    Byte offsetValue = 0x04;
    Byte zeroPageAddress = 0x0002;
    Word indexAddress = (Word)(zeroPageAddress + offsetValue);
    Word valueAddress = 0x4480;
    Byte expected = 0x22;
    constexpr uint32 expectedCycles = 6u;

    cpu.X = offsetValue;
    memory.WriteByte(0xFFFC, CPU::INS_LDA_INDX);
    memory.WriteByte(0xFFFD, zeroPageAddress);
    memory.WriteWord(indexAddress, valueAddress);
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


TEST_F(CPUFixture, LDA_INDX_WithNegativeValue_LoadsValueIntoRegister)
{
    // Arrange
    Byte offsetValue = 0x04;
    Byte zeroPageAddress = 0x0002;
    Word indexAddress = (Word)(zeroPageAddress + offsetValue);
    Word valueAddress = 0x4480;
    Byte expected = 0xB2;
    constexpr uint32 expectedCycles = 6u;

    cpu.X = offsetValue;
    memory.WriteByte(0xFFFC, CPU::INS_LDA_INDX);
    memory.WriteByte(0xFFFD, zeroPageAddress);
    memory.WriteWord(indexAddress, valueAddress);
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


TEST_F(CPUFixture, LDA_INDX_WithZeroValue_LoadsValueIntoRegister)
{
    // Arrange
    Byte offsetValue = 0x04;
    Byte zeroPageAddress = 0x0002;
    Word indexAddress = (Word)(zeroPageAddress + offsetValue);
    Word valueAddress = 0x4480;
    Byte expected = 0x00;
    constexpr uint32 expectedCycles = 6u;

    cpu.X = offsetValue;
    memory.WriteByte(0xFFFC, CPU::INS_LDA_INDX);
    memory.WriteByte(0xFFFD, zeroPageAddress);
    memory.WriteWord(indexAddress, valueAddress);
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