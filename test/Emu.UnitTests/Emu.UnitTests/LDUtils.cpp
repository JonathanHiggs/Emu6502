#include <gtest/gtest.h>

#include <Emu.UnitTests/LDUtils.hpp>


using namespace Emu;


namespace
{

    void ArrangeLoadRegisterImmediate(
        Emu::Memory & memory,
        Emu::Byte opCode,
        Emu::Byte value)
    {
        memory.WriteByte(0xFFFC, opCode);
        memory.WriteByte(0xFFFD, value);
    }

    void ArrangeLoadRegisterZeroPage(
        Emu::Memory & memory,
        Emu::Byte opCode,
        Emu::Byte value)
    {
        // Arrange
        Byte address = 0x42;

        memory.WriteByte(0xFFFC, opCode);
        memory.WriteByte(0xFFFD, address);
        memory.WriteByte(address, value);

    }

    void AssertLD(
        Emu::CPU & cpu,
        Emu::CPU & initialCpu,
        Emu::Byte value,
        Emu::Byte Emu::CPU::* reg,
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

}


void TestLoadRegisterImmediate(
    Emu::CPU & cpu,
    Emu::Memory & memory,
    Emu::Byte opCode,
    Emu::Byte Emu::CPU::*reg)
{
    // Arrange
    constexpr uint32 expectedCycles = 2u;
    constexpr Emu::Byte value = 0x23;

    ArrangeLoadRegisterImmediate(memory, opCode, value);

    CPU initial = cpu;

    // Act
    auto cyclesUsed = cpu.Execute(expectedCycles, memory);

    // Assert
    AssertLD(cpu, initial, value, reg, cyclesUsed, expectedCycles);
}


void TestLoadRegisterImmediate_WithNegativeValue(
    Emu::CPU & cpu,
    Emu::Memory & memory,
    Emu::Byte opCode,
    Emu::Byte Emu::CPU::*reg)
{
    // Arrange
    constexpr uint32 expectedCycles = 2u;
    constexpr Emu::Byte value = 0xC3;

    ArrangeLoadRegisterImmediate(memory, opCode, value);

    CPU initial = cpu;

    // Act
    auto cyclesUsed = cpu.Execute(expectedCycles, memory);

    // Assert
    AssertLD(cpu, initial, value, reg, cyclesUsed, expectedCycles);
}


void TestLoadRegisterImmediate_WithZeroValue(
    Emu::CPU & cpu,
    Emu::Memory & memory,
    Emu::Byte opCode,
    Emu::Byte Emu::CPU::*reg)
{
    // Arrange
    constexpr uint32 expectedCycles = 2u;
    constexpr Emu::Byte value = 0x00;

    ArrangeLoadRegisterImmediate(memory, opCode, value);

    CPU initial = cpu;

    // Act
    auto cyclesUsed = cpu.Execute(expectedCycles, memory);

    // Assert
    AssertLD(cpu, initial, value, reg, cyclesUsed, expectedCycles);
}


void TestLoadRegisterZeroPage(
    Emu::CPU & cpu,
    Emu::Memory & memory,
    Emu::Byte opCode,
    Emu::Byte Emu::CPU::*reg)
{
    // Arrange
    constexpr uint32 expectedCycles = 3u;
    constexpr Emu::Byte value = 0x23;

    ArrangeLoadRegisterZeroPage(memory, opCode, value);

    CPU initial = cpu;

    // Act
    auto cyclesUsed = cpu.Execute(expectedCycles, memory);

    // Assert
    AssertLD(cpu, initial, value, reg, cyclesUsed, expectedCycles);
}


void TestLoadRegisterZeroPage_WithNegativeValue(
    Emu::CPU & cpu,
    Emu::Memory & memory,
    Emu::Byte opCode,
    Emu::Byte Emu::CPU::*reg)
{
    // Arrange
    constexpr uint32 expectedCycles = 3u;
    constexpr Emu::Byte value = 0xD3;

    ArrangeLoadRegisterZeroPage(memory, opCode, value);

    CPU initial = cpu;

    // Act
    auto cyclesUsed = cpu.Execute(expectedCycles, memory);

    // Assert
    AssertLD(cpu, initial, value, reg, cyclesUsed, expectedCycles);
}


void TestLoadRegisterZeroPage_WithZeroValue(
    Emu::CPU & cpu,
    Emu::Memory & memory,
    Emu::Byte opCode,
    Emu::Byte Emu::CPU::*reg)
{
    // Arrange
    constexpr uint32 expectedCycles = 3u;
    constexpr Emu::Byte value = 0x00;

    ArrangeLoadRegisterZeroPage(memory, opCode, value);

    CPU initial = cpu;

    // Act
    auto cyclesUsed = cpu.Execute(expectedCycles, memory);

    // Assert
    AssertLD(cpu, initial, value, reg, cyclesUsed, expectedCycles);
}