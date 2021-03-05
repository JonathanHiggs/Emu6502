#include <gtest/gtest.h>

#include <Emu/CPU.hpp>


namespace Emu::UnitTests
{

    class JumpLocationFixture : public testing::Test
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


    TEST_F(JumpLocationFixture, INS_JMP_ABS_CanJumpLocation)
    {
        // Arrange
        uint32 cyclesExpected = 3u;
        Word jumpAddress = 0x4480;

        memory.WriteByte(0xFFFC, CPU::INS_JMP_ABS);
        memory.WriteWord(0xFFFD, jumpAddress);

        CPU initial = cpu;

        // Act
        auto cyclesUsed = cpu.Execute(cyclesExpected, memory);

        // Assert
        EXPECT_EQ(cpu.PC, jumpAddress);
        EXPECT_EQ(cpu.SP, initial.SP);
        AssertFlags(initial, cyclesUsed, cyclesExpected);
    }


    TEST_F(JumpLocationFixture, INS_JMP_ABS_JumpAndExecuteInstruction)
    {
        // Arrange
        uint32 cyclesExpected = 3u + 2u;
        Byte value = 0x42;
        Word jumpAddress = 0x4480;

        memory.WriteByte(0xFFFC,            CPU::INS_JMP_ABS);
        memory.WriteWord(0xFFFD,            jumpAddress);
        memory.WriteByte(jumpAddress,       CPU::INS_LDA_IM);
        memory.WriteByte(jumpAddress + 1,   value);

        CPU initial = cpu;

        // Act
        auto cyclesUsed = cpu.Execute(cyclesExpected, memory);

        // Assert
        EXPECT_EQ(cpu.A, value);
    }


    TEST_F(JumpLocationFixture, INS_JMP_IND_CanJumpLocation)
    {
        // Arrange
        uint32 cyclesExpected = 5u;
        Word pointerAddress = 0x4470;
        Word jumpAddress = 0x4480;

        memory.WriteByte(0xFFFC,            CPU::INS_JMP_IND);
        memory.WriteWord(0xFFFD,            pointerAddress);
        memory.WriteWord(pointerAddress,    jumpAddress);

        CPU initial = cpu;

        // Act
        auto cyclesUsed = cpu.Execute(cyclesExpected, memory);

        // Assert
        EXPECT_EQ(cpu.PC, jumpAddress);
        EXPECT_EQ(cpu.SP, initial.SP);
        AssertFlags(initial, cyclesUsed, cyclesExpected);
    }


    TEST_F(JumpLocationFixture, INS_JMP_IND_JumpAndExecuteInstruction)
    {
        // Arrange
        uint32 cyclesExpected = 5u + 2u;
        Byte value = 0x43;
        Word pointerAddress = 0x4470;
        Word jumpAddress = 0x4480;

        memory.WriteByte(0xFFFC,            CPU::INS_JMP_IND);
        memory.WriteWord(0xFFFD,            pointerAddress);
        memory.WriteWord(pointerAddress,    jumpAddress);
        memory.WriteByte(jumpAddress,       CPU::INS_LDA_IM);
        memory.WriteByte(jumpAddress + 1,   value);

        CPU initial = cpu;

        // Act
        auto cyclesUsed = cpu.Execute(cyclesExpected, memory);

        // Assert
        EXPECT_EQ(cpu.A, value);
    }

}