#include <gtest/gtest.h>

#include <Emu/UnitTests/LDUtils.hpp>


namespace Emu::UnitTests
{

    namespace
    {

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


    void TestLoadRegisterImmediateImpl(
        CPU & cpu,
        Memory & memory,
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
        AssertLD(cpu, initial, value, resultRegister, cyclesUsed, expectedCycles);
    }

    void TestLoadRegisterImmediate(CPU & cpu, Memory & memory, Byte opCode, Byte CPU::* resultRegister)
    { TestLoadRegisterImmediateImpl(cpu, memory, opCode, resultRegister, 2u, 0x23); }

    void TestLoadRegisterImmediate_WithNegativeValue(CPU & cpu, Memory & memory, Byte opCode, Byte CPU::* resultRegister)
    { TestLoadRegisterImmediateImpl(cpu, memory, opCode, resultRegister, 2u, 0xC3); }

    void TestLoadRegisterImmediate_WithZeroValue(CPU & cpu, Memory & memory, Byte opCode, Byte CPU::* resultRegister)
    { TestLoadRegisterImmediateImpl(cpu, memory, opCode, resultRegister, 2u, 0x00); }


    void TestLoadRegisterZeroPageImpl(
        CPU & cpu,
        Memory & memory,
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
        AssertLD(cpu, initial, value, resultRegister, cyclesUsed, expectedCycles);
    }

    void TestLoadRegisterZeroPage(CPU & cpu, Memory & memory, Byte opCode, Byte CPU::*reg)
    { TestLoadRegisterZeroPageImpl(cpu, memory, opCode, reg, 3u, 0x23); }

    void TestLoadRegisterZeroPage_WithNegativeValue(CPU & cpu, Memory & memory, Byte opCode, Byte CPU::*reg)
    { TestLoadRegisterZeroPageImpl(cpu, memory, opCode, reg, 3u, 0xD3); }

    void TestLoadRegisterZeroPage_WithZeroValue(CPU & cpu, Memory & memory, Byte opCode, Byte CPU::*reg)
    { TestLoadRegisterZeroPageImpl(cpu, memory, opCode, reg, 3u, 0x00); }


    void TestLoadRegisterZeroPageOffsetImpl(
        CPU & cpu,
        Memory & memory,
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
        AssertLD(cpu, initial, value, resultRegister, cyclesUsed, expectedCycles);
    }

    void TestLoadRegisterZeroPageOffset(CPU & cpu, Memory & memory, Byte opCode, Byte CPU::* offsetRegister, Byte CPU::* resultRegister)
    { TestLoadRegisterZeroPageOffsetImpl(cpu, memory, opCode, offsetRegister, resultRegister, 4u, 0x42); }

    void TestLoadRegisterZeroPageOffset_WithNegativeValue(CPU & cpu, Memory & memory, Byte opCode, Byte CPU::* offsetRegister, Byte CPU::* resultRegister)
    { TestLoadRegisterZeroPageOffsetImpl(cpu, memory, opCode, offsetRegister, resultRegister, 4u, 0xE2); }

    void TestLoadRegisterZeroPageOffset_WithZeroValue(CPU & cpu, Memory & memory, Byte opCode, Byte CPU::* offsetRegister, Byte CPU::* resultRegister)
    { TestLoadRegisterZeroPageOffsetImpl(cpu, memory, opCode, offsetRegister, resultRegister, 4u, 0x00); }

    void TestLoadRegisterZeroPageOffset_WithWrappedAddress(CPU & cpu, Memory & memory, Byte opCode, Byte CPU::* offsetRegister, Byte CPU::* resultRegister)
    { TestLoadRegisterZeroPageOffsetImpl(cpu, memory, opCode, offsetRegister, resultRegister, 4u, 0x42, true); }


    void TestLoadRegisterAbsoluteImpl(
        CPU & cpu,
        Memory & memory,
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
        AssertLD(cpu, initial, value, resultRegister, cyclesUsed, expectedCycles);
    }

    void TestLoadRegisterAbsolute(CPU & cpu, Memory & memory, Byte opCode, Byte CPU::* resultRegister)
    { TestLoadRegisterAbsoluteImpl(cpu, memory, opCode, resultRegister, 4u, 0x11); }

    void TestLoadRegisterAbsolute_WithNegativeValue(CPU & cpu, Memory & memory, Byte opCode, Byte CPU::* resultRegister)
    { TestLoadRegisterAbsoluteImpl(cpu, memory, opCode, resultRegister, 4u, 0xD1); }

    void TestLoadRegisterAbsolute_WithZeroValue(CPU & cpu, Memory & memory, Byte opCode, Byte CPU::* resultRegister)
    { TestLoadRegisterAbsoluteImpl(cpu, memory, opCode, resultRegister, 4u, 0x00); }


    void TestLoadRegisterAbsoluteXImpl(
        CPU & cpu,
        Memory & memory,
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
        AssertLD(cpu, initial, value, resultRegister, cyclesUsed, expectedCycles);
    }

    void TestLoadRegisterAbsoluteX(CPU & cpu, Memory & memory, Byte opCode, Byte CPU::* resultRegister)
    { TestLoadRegisterAbsoluteXImpl(cpu, memory, opCode, resultRegister, 4u, 0x22); }

    void TestLoadRegisterAbsoluteX_WithNegativeValue(CPU & cpu, Memory & memory, Byte opCode, Byte CPU::* resultRegister)
    { TestLoadRegisterAbsoluteXImpl(cpu, memory, opCode, resultRegister, 4u, 0xA2); }

    void TestLoadRegisterAbsoluteX_WithZeroValue(CPU & cpu, Memory & memory, Byte opCode, Byte CPU::* resultRegister)
    { TestLoadRegisterAbsoluteXImpl(cpu, memory, opCode, resultRegister, 4u, 0x00); }

    void TestLoadRegisterAbsoluteX_WithCrossPageBoundary(CPU & cpu, Memory & memory, Byte opCode, Byte CPU::* resultRegister)
    { TestLoadRegisterAbsoluteXImpl(cpu, memory, opCode, resultRegister, 5u, 0x01, true); }

}