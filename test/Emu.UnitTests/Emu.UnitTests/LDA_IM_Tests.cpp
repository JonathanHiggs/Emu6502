#include <Emu.UnitTests/CPUFixture.hpp>
#include <Emu.UnitTests/LDUtils.hpp>


TEST_F(CPUFixture, LDA_IM_CanLoadValueIntoRegister)
{
    TestLoadRegisterImmediate(cpu, memory, CPU::INS_LDA_IM, &CPU::A);
}


TEST_F(CPUFixture, LDA_IM_WithNegativeValue_CanLoadValueIntoRegister)
{
    // Arrange
    Byte expected = 0x84;
    constexpr uint32 expectedCycles = 2u;

    memory.WriteByte(0xFFFC, CPU::INS_LDA_IM);
    memory.WriteByte(0xFFFD, expected);

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


TEST_F(CPUFixture, LDA_IM_WithZeroValue_CanLoadValueIntoRegister)
{
    // Arrange
    Byte expected = 0x00;
    constexpr uint32 expectedCycles = 2u;

    memory.WriteByte(0xFFFC, CPU::INS_LDA_IM);
    memory.WriteByte(0xFFFD, expected);

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