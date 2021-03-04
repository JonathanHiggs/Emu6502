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

}