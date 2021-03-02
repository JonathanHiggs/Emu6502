#include <Emu/UnitTests/CPUFixture.hpp>
#include <Emu/UnitTests/LDUtils.hpp>


using namespace Emu;
using namespace Emu::UnitTests;


TEST_F(CPUFixture, LDA_IM_CanLoadValueIntoRegister)
{
    TestLoadRegisterImmediate(cpu, memory, CPU::INS_LDA_IM, &CPU::A);
}


TEST_F(CPUFixture, LDA_IM_WithNegativeValue_CanLoadValueIntoRegister)
{
    TestLoadRegisterImmediate_WithNegativeValue(cpu, memory, CPU::INS_LDA_IM, &CPU::A);
}


TEST_F(CPUFixture, LDA_IM_WithZeroValue_CanLoadValueIntoRegister)
{
    TestLoadRegisterImmediate_WithZeroValue(cpu, memory, CPU::INS_LDA_IM, &CPU::A);
}


TEST_F(CPUFixture, LDA_ZP_CanLoadValueIntoRegister)
{
    TestLoadRegisterZeroPage(cpu, memory, CPU::INS_LDA_ZP, &CPU::A);
}


TEST_F(CPUFixture, LDA_ZP_WithNegativeValue_CanLoadValueIntoRegister)
{
    TestLoadRegisterZeroPage_WithNegativeValue(cpu, memory, CPU::INS_LDA_ZP, &CPU::A);
}


TEST_F(CPUFixture, LDA_ZP_WithZeroValue_CanLoadValueIntoRegister)
{
    TestLoadRegisterZeroPage_WithZeroValue(cpu, memory, CPU::INS_LDA_ZP, &CPU::A);
}


TEST_F(CPUFixture, LDA_ZPX_CanLoadValueIntoRegister)
{
    TestLoadRegisterZeroPageOffset(cpu, memory, CPU::INS_LDA_ZPX, &CPU::X, &CPU::A);
}


TEST_F(CPUFixture, LDA_ZPX_WithNegativeValue_CanLoadValueIntoRegister)
{
    TestLoadRegisterZeroPageOffset_WithNegativeValue(cpu, memory, CPU::INS_LDA_ZPX, &CPU::X, &CPU::A);
}


TEST_F(CPUFixture, LDA_ZPX_WithZeroValue_CanLoadValueIntoRegister)
{
    TestLoadRegisterZeroPageOffset_WithZeroValue(cpu, memory, CPU::INS_LDA_ZPX, &CPU::X, &CPU::A);
}


TEST_F(CPUFixture, LDA_ZPX_WithWrappedAddress_CanLoadValueIntoRegister)
{
    TestLoadRegisterZeroPageOffset_WithWrappedAddress(cpu, memory, CPU::INS_LDA_ZPX, &CPU::X, &CPU::A);
}