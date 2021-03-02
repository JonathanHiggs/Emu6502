#include <gtest/gtest.h>

#include <Emu/UnitTests/LDUtils.hpp>


namespace Emu::UnitTests
{

    namespace
    {

        void ArrangeLoadRegisterImmediate(
            Memory & memory,
            Byte opCode,
            Byte value)
        {
            memory.WriteByte(0xFFFC, opCode);
            memory.WriteByte(0xFFFD, value);
        }

        void ArrangeLoadRegisterZeroPage(
            Memory & memory,
            Byte opCode,
            Byte value)
        {
            // Arrange
            Byte address = 0x42;

            memory.WriteByte(0xFFFC, opCode);
            memory.WriteByte(0xFFFD, address);
            memory.WriteByte(address, value);
        }

        void ArrangeLoadRegisterZeroPageOffset(
            CPU & cpu,
            Memory & memory,
            Byte opCode,
            Byte value,
            Byte CPU::* offsetRegister,
            bool wrapAddress = false)
        {
            Byte registerValue = wrapAddress ? 0xFF : 0x05;
            Byte baseAddress = 0x42;
            Byte offsetAddress = (baseAddress + registerValue) % 0x100;

            cpu.*offsetRegister = registerValue;
            memory.WriteByte(0xFFFC, opCode);
            memory.WriteByte(0xFFFD, baseAddress);
            memory.WriteByte(offsetAddress, value);
        }

        void AssertLD(
            CPU & cpu,
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

    }


    void TestLoadRegisterImmediate(
        CPU & cpu,
        Memory & memory,
        Byte opCode,
        Byte CPU::*reg)
    {
        // Arrange
        constexpr uint32 expectedCycles = 2u;
        constexpr Byte value = 0x23;

        ArrangeLoadRegisterImmediate(memory, opCode, value);

        CPU initial = cpu;

        // Act
        auto cyclesUsed = cpu.Execute(expectedCycles, memory);

        // Assert
        AssertLD(cpu, initial, value, reg, cyclesUsed, expectedCycles);
    }


    void TestLoadRegisterImmediate_WithNegativeValue(
        CPU & cpu,
        Memory & memory,
        Byte opCode,
        Byte CPU::*reg)
    {
        // Arrange
        constexpr uint32 expectedCycles = 2u;
        constexpr Byte value = 0xC3;

        ArrangeLoadRegisterImmediate(memory, opCode, value);

        CPU initial = cpu;

        // Act
        auto cyclesUsed = cpu.Execute(expectedCycles, memory);

        // Assert
        AssertLD(cpu, initial, value, reg, cyclesUsed, expectedCycles);
    }


    void TestLoadRegisterImmediate_WithZeroValue(
        CPU & cpu,
        Memory & memory,
        Byte opCode,
        Byte CPU::*reg)
    {
        // Arrange
        constexpr uint32 expectedCycles = 2u;
        constexpr Byte value = 0x00;

        ArrangeLoadRegisterImmediate(memory, opCode, value);

        CPU initial = cpu;

        // Act
        auto cyclesUsed = cpu.Execute(expectedCycles, memory);

        // Assert
        AssertLD(cpu, initial, value, reg, cyclesUsed, expectedCycles);
    }


    void TestLoadRegisterZeroPage(
        CPU & cpu,
        Memory & memory,
        Byte opCode,
        Byte CPU::*reg)
    {
        // Arrange
        constexpr uint32 expectedCycles = 3u;
        constexpr Byte value = 0x23;

        ArrangeLoadRegisterZeroPage(memory, opCode, value);

        CPU initial = cpu;

        // Act
        auto cyclesUsed = cpu.Execute(expectedCycles, memory);

        // Assert
        AssertLD(cpu, initial, value, reg, cyclesUsed, expectedCycles);
    }


    void TestLoadRegisterZeroPage_WithNegativeValue(
        CPU & cpu,
        Memory & memory,
        Byte opCode,
        Byte CPU::*reg)
    {
        // Arrange
        constexpr uint32 expectedCycles = 3u;
        constexpr Byte value = 0xD3;

        ArrangeLoadRegisterZeroPage(memory, opCode, value);

        CPU initial = cpu;

        // Act
        auto cyclesUsed = cpu.Execute(expectedCycles, memory);

        // Assert
        AssertLD(cpu, initial, value, reg, cyclesUsed, expectedCycles);
    }


    void TestLoadRegisterZeroPage_WithZeroValue(
        CPU & cpu,
        Memory & memory,
        Byte opCode,
        Byte CPU::*reg)
    {
        // Arrange
        constexpr uint32 expectedCycles = 3u;
        constexpr Byte value = 0x00;

        ArrangeLoadRegisterZeroPage(memory, opCode, value);

        CPU initial = cpu;

        // Act
        auto cyclesUsed = cpu.Execute(expectedCycles, memory);

        // Assert
        AssertLD(cpu, initial, value, reg, cyclesUsed, expectedCycles);
    }


    void TestLoadRegisterZeroPageOffset(
        CPU & cpu,
        Memory & memory,
        Byte opCode,
        Byte CPU::* offsetRegister,
        Byte CPU::* resultRegister)
    {
        // Arrange
        constexpr uint32 expectedCycles = 4u;
        constexpr Byte value = 0x42;

        ArrangeLoadRegisterZeroPageOffset(cpu, memory, opCode, value, offsetRegister);

        CPU initial = cpu;

        // Act
        auto cyclesUsed = cpu.Execute(expectedCycles, memory);

        // Assert
        AssertLD(cpu, initial, value, resultRegister, cyclesUsed, expectedCycles);
    }


    void TestLoadRegisterZeroPageOffset_WithNegativeValue(
        CPU & cpu,
        Memory & memory,
        Byte opCode,
        Byte CPU::* offsetRegister,
        Byte CPU::* resultRegister)
    {
        // Arrange
        constexpr uint32 expectedCycles = 4u;
        constexpr Byte value = 0xE2;

        ArrangeLoadRegisterZeroPageOffset(cpu, memory, opCode, value, offsetRegister);

        CPU initial = cpu;

        // Act
        auto cyclesUsed = cpu.Execute(expectedCycles, memory);

        // Assert
        AssertLD(cpu, initial, value, resultRegister, cyclesUsed, expectedCycles);
    }


    void TestLoadRegisterZeroPageOffset_WithZeroValue(
        CPU & cpu,
        Memory & memory,
        Byte opCode,
        Byte CPU::* offsetRegister,
        Byte CPU::* resultRegister)
    {
        // Arrange
        constexpr uint32 expectedCycles = 4u;
        constexpr Byte value = 0x00;

        ArrangeLoadRegisterZeroPageOffset(cpu, memory, opCode, value, offsetRegister);

        CPU initial = cpu;

        // Act
        auto cyclesUsed = cpu.Execute(expectedCycles, memory);

        // Assert
        AssertLD(cpu, initial, value, resultRegister, cyclesUsed, expectedCycles);
    }


    void TestLoadRegisterZeroPageOffset_WithWrappedAddress(
        CPU & cpu,
        Memory & memory,
        Byte opCode,
        Byte CPU::* offsetRegister,
        Byte CPU::* resultRegister)
    {
        // Arrange
        constexpr uint32 expectedCycles = 4u;
        constexpr Byte value = 0x42;

        ArrangeLoadRegisterZeroPageOffset(cpu, memory, opCode, value, offsetRegister, true);

        CPU initial = cpu;

        // Act
        auto cyclesUsed = cpu.Execute(expectedCycles, memory);

        // Assert
        AssertLD(cpu, initial, value, resultRegister, cyclesUsed, expectedCycles);
    }

}