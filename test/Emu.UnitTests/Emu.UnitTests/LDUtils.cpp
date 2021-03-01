#include <gtest/gtest.h>

#include <Emu.UnitTests/LDUtils.hpp>


using namespace Emu;


void TestLoadRegisterImmediate(
    CPU & cpu,
    Memory & memory,
    Byte opCode,
    Byte CPU::*reg)
{
    // Arrange
    Byte expected = 0x74;
    constexpr uint32 expectedCycles = 2u;

    memory.WriteByte(0xFFFC, opCode);
    memory.WriteByte(0xFFFD, expected);

    CPU initial = cpu;

    // Act
    auto cyclesUsed = cpu.Execute(expectedCycles, memory);

    // Assert
    EXPECT_EQ(cpu.*reg, expected);
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