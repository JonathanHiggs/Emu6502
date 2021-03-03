#include <gtest/gtest.h>

#include <Emu/CPU.hpp>
#include <Emu/Memory.hpp>


namespace Emu::UnitTests
{

    class CPUFixture : public testing::Test
    {
    public:
        Memory memory;
        CPU cpu;

        void SetUp() override
        {
            cpu.Reset(memory);
        }

        void TearDown() override
        { }
    };


    TEST_F(CPUFixture, CPU_WithWithZeroCycles_DoesNothing)
    {
        // Arrange
        constexpr uint32 cycles = 0u;

        // Act
        auto cyclesUsed = cpu.Execute(cycles, memory);

        // Assert
        EXPECT_EQ(cyclesUsed, 0u);
    }


    TEST_F(CPUFixture, CPU_WithFewerCyclesGiven_CanExecuteInstruction)
    {
        // Arrange
        memory.WriteByte(0xFFFC, CPU::INS_LDA_IM);
        memory.WriteByte(0xFFFD, 0x34);
        constexpr uint32 expectedCycles = 2u;

        // Act
        auto cyclesUsed = cpu.Execute(expectedCycles - 1u, memory);

        // Assert
        EXPECT_EQ(cyclesUsed, expectedCycles);
        EXPECT_TRUE(cpu.CycleOverflow);
    }


    TEST_F(CPUFixture, CPU_WithBadInstruction_DoesNotInfiniteLoop)
    {
        // Arrange
        constexpr uint32 expectedCycles = 1u;

        memory.WriteByte(0xFFFC, 0x00);
        memory.WriteByte(0xFFFD, 0x00);

        // Act
        auto cyclesUsed = cpu.Execute(expectedCycles, memory);

        // Assert
        EXPECT_EQ(cyclesUsed, expectedCycles);
    }


    TEST_F(CPUFixture, CPU_WithBadInstruction_SetsUnhandledInstructionBit)
    {
        // Arrange
        constexpr uint32 expectedCycles = 1u;

        memory.WriteByte(0xFFFC, 0x00);

        // Act
        auto cyclesUsed = cpu.Execute(expectedCycles, memory);

        // Assert
        EXPECT_EQ(cyclesUsed, expectedCycles);
        EXPECT_TRUE(cpu.UnhandledInstruction);
    }

}