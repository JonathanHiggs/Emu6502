#include <gtest/gtest.h>

#include <Emu/CPU.hpp>


namespace Emu::UnitTests
{

    class ReturnSubroutineFixture : public testing::Test
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


    TEST_F(ReturnSubroutineFixture, INS_RTS_CanJumpToSubroutine)
    {
        // Arrange
        uint32 cyclesExpected = 6u;
        Word programCounter = 0x0200;
        Word returnAddress = 0x0300;

        memory.WriteByte(programCounter, CPU::INS_RTS);
        memory.WriteWord(0x01FE, returnAddress);

        cpu.PC = programCounter;
        cpu.SP = 0xFF - 2;          // 2 byte word added to stack

        CPU initial = cpu;

        // Act
        auto cyclesUsed = cpu.Execute(cyclesExpected, memory);

        // Assert
        EXPECT_EQ(cpu.PC - 1, returnAddress);
        EXPECT_EQ(cpu.SP, 0xFF);
        AssertFlags(initial, cyclesUsed, cyclesExpected);
    }

}