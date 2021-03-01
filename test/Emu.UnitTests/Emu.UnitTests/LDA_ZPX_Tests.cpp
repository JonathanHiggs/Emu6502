#include <Emu.UnitTests/CPUFixture.hpp>


TEST_F(CPUFixture, LDA_ZPX_CanLoadValueIntoRegister)
{
    // Arrange
    Byte registerValue = 0x05;
    Byte baseAddress = 0x42;
    Byte offsetAddress = baseAddress + registerValue;
    Byte expected = 0x37;
    constexpr uint32 expectedCycles = 4u;

    cpu.X = registerValue;
    memory.WriteByte(0xFFFC, CPU::INS_LDA_ZPX);
    memory.WriteByte(0xFFFD, baseAddress);
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


TEST_F(CPUFixture, LDA_ZPX_WithNegativeValue_CanLoadValueIntoRegister)
{
    // Arrange
    Byte registerValue = 0x05;
    Byte baseAddress = 0x42;
    Byte offsetAddress = baseAddress + registerValue;
    Byte expected = 0x97;
    constexpr uint32 expectedCycles = 4u;

    cpu.X = registerValue;
    memory.WriteByte(0xFFFC, CPU::INS_LDA_ZPX);
    memory.WriteByte(0xFFFD, baseAddress);
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


TEST_F(CPUFixture, LDA_ZPX_WithZeroValue_CanLoadValueIntoRegister)
{
    // Arrange
    Byte registerValue = 0x05;
    Byte baseAddress = 0x42;
    Byte offsetAddress = baseAddress + registerValue;
    Byte expected = 0x00;
    constexpr uint32 expectedCycles = 4u;

    cpu.X = registerValue;
    memory.WriteByte(0xFFFC, CPU::INS_LDA_ZPX);
    memory.WriteByte(0xFFFD, baseAddress);
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


TEST_F(CPUFixture, LDA_ZPX_WithWrappedAddress_CanLoadValueIntoRegister)
{
    // Arrange
    Byte registerValue = 0xFF;
    Byte baseAddress = 0x80;
    Byte offsetAddress = (baseAddress + registerValue) % 0x100;
    Byte expected = 0x24;
    constexpr uint32 expectedCycles = 4u;

    cpu.X = registerValue;
    memory.WriteByte(0xFFFC, CPU::INS_LDA_ZPX);
    memory.WriteByte(0xFFFD, baseAddress);
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