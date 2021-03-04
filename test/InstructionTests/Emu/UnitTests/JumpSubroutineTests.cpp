#include <gtest/gtest.h>

#include <Emu/CPU.hpp>


namespace Emu::UnitTests
{

    class JumpSubroutineFixture : public testing::Test
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

        void AssertFlags(CPU const & initialCpu, uint32 cyclesUsed, uint32 cyclesExpected)
        {
            EXPECT_EQ(cpu.Status, initialCpu.Status);
            EXPECT_EQ(cpu.DebugStatus, initialCpu.DebugStatus);
            EXPECT_EQ(cyclesUsed, cyclesExpected);
        }

    };


    TEST_F(JumpSubroutineFixture, INS_JSR_CanJumpToSubroutine)
    {
        // Arrange
        uint32 cyclesExpected = 6u;
        Word subroutineAddress = 0x0200;
        Word returnAddress = 0xFFFE;    // PC = 0xFFFD + 1 after ins, return PC - 1

        memory.WriteByte(0xFFFC, CPU::INS_JSR);
        memory.WriteWord(0xFFFD, subroutineAddress);

        CPU initial = cpu;

        // Act
        auto cyclesUsed = cpu.Execute(cyclesExpected, memory);

        // Assert
        EXPECT_EQ(cpu.PC, subroutineAddress);
        EXPECT_EQ(cpu.SP, initial.SP - 2);
        EXPECT_EQ(cpu.PeekWordInStack(memory), returnAddress);
        AssertFlags(initial, cyclesUsed, cyclesExpected);
    }


    TEST_F(JumpSubroutineFixture, CanJumpAndReturn)
    {
        // Arrange
        uint32 cyclesExpected = 6u + 6u;
        Word subroutineAddress = 0x0200;
        Byte value = 0x42;

        cpu.Reset(memory, 0xFFFC);

        memory.WriteByte(subroutineAddress, CPU::INS_RTS);

        memory.WriteByte(0xFFFC, CPU::INS_JSR);
        memory.WriteWord(0xFFFD, subroutineAddress);

        CPU initial = cpu;

        // Act
        auto cyclesUsed = cpu.Execute(cyclesExpected, memory);

        // Assert
        EXPECT_EQ(cpu.SP, initial.SP);
        AssertFlags(initial, cyclesUsed, cyclesExpected);
    }


    TEST_F(JumpSubroutineFixture, CanJumpReturnAndNextInstruction)
    {
        // Arrange
        uint32 cyclesExpected = 6u + 6u + 2u;
        Word subroutineAddress = 0x0200;
        Byte value = 0x42;

        cpu.Reset(memory, 0xFFF0);

        memory.WriteByte(subroutineAddress, CPU::INS_RTS);

        memory.WriteByte(0xFFF0, CPU::INS_JSR);
        memory.WriteWord(0xFFF1, subroutineAddress);
        memory.WriteByte(0xFFF3, CPU::INS_LDA_IM);
        memory.WriteByte(0xFFF4, value);

        CPU initial = cpu;

        // Act
        auto cyclesUsed = cpu.Execute(cyclesExpected, memory);

        // Assert
        EXPECT_EQ(cpu.A, value);
    }

}