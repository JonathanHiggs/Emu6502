#include <gtest/gtest.h>

#include <Emu/CPU.hpp>
#include <Emu/Memory.hpp>

using namespace Emu;


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