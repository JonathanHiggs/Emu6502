#include <Emu.UnitTests/CPUFixture.hpp>


TEST_F(CPUFixture, LDA_ZP_CanLoadValueIntoRegister)
{
    // Arrange
    Byte address = 0x42;
    Byte expected = 0x75;
    constexpr uint32 expectedCycles = 3u;

    memory.WriteByte(0xFFFC, CPU::INS_LDA_ZP);
    memory.WriteByte(0xFFFD, address);
    memory.WriteByte(address, expected);

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


TEST_F(CPUFixture, LDA_ZP_WithNegativeValue_CanLoadValueIntoRegister)
{
    // Arrange
    Byte address = 0x42;
    Byte expected = 0x85;
    constexpr uint32 expectedCycles = 3u;

    memory.WriteByte(0xFFFC, CPU::INS_LDA_ZP);
    memory.WriteByte(0xFFFD, address);
    memory.WriteByte(address, expected);

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


TEST_F(CPUFixture, LDA_ZP_WithZeroValue_CanLoadValueIntoRegister)
{
    // Arrange
    Byte address = 0x42;
    Byte expected = 0x00;
    constexpr uint32 expectedCycles = 3u;

    memory.WriteByte(0xFFFC, CPU::INS_LDA_ZP);
    memory.WriteByte(0xFFFD, address);
    memory.WriteByte(address, expected);

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