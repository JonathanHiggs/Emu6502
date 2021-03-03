#include <Emu/UnitTests/CPUFixture.hpp>
#include <Emu/UnitTests/LDUtils.hpp>


using namespace Emu;
using namespace Emu::UnitTests;


// LDX_IM
TEST_F(CPUFixture, LDX_IM_CanLoadValueIntoRegister)
{ TestLoadRegisterImmediate(cpu, memory, CPU::INS_LDX_IM, &CPU::X); }

TEST_F(CPUFixture, LDX_IM_WithNegativeValue_CanLoadValueIntoRegister)
{ TestLoadRegisterImmediate_WithNegativeValue(cpu, memory, CPU::INS_LDX_IM, &CPU::X); }

TEST_F(CPUFixture, LDX_IM_WithZeroValue_CanLoadValueIntoRegister)
{ TestLoadRegisterImmediate_WithZeroValue(cpu, memory, CPU::INS_LDX_IM, &CPU::X); }


// LDX_ZP
TEST_F(CPUFixture, LDX_ZP_CanLoadValueIntoRegister)
{ TestLoadRegisterZeroPage(cpu, memory, CPU::INS_LDX_ZP, &CPU::X); }

TEST_F(CPUFixture, LDX_ZP_WithNegativeValue_CanLoadValueIntoRegister)
{ TestLoadRegisterZeroPage_WithNegativeValue(cpu, memory, CPU::INS_LDX_ZP, &CPU::X); }

TEST_F(CPUFixture, LDX_ZP_WithZeroValue_CanLoadValueIntoRegister)
{ TestLoadRegisterZeroPage_WithZeroValue(cpu, memory, CPU::INS_LDX_ZP, &CPU::X); }


// LDX_ZPY
TEST_F(CPUFixture, LDX_ZPY_CanLoadValueIntoRegister)
{ TestLoadRegisterZeroPageOffset(cpu, memory, CPU::INS_LDX_ZPY, &CPU::Y, &CPU::X); }

TEST_F(CPUFixture, LDX_ZPY_WithNegativeValue_CanLoadValueIntoRegister)
{ TestLoadRegisterZeroPageOffset_WithNegativeValue(cpu, memory, CPU::INS_LDX_ZPY, &CPU::Y, &CPU::X); }

TEST_F(CPUFixture, LDX_ZPY_WithZeroValue_CanLoadValueIntoRegister)
{ TestLoadRegisterZeroPageOffset_WithZeroValue(cpu, memory, CPU::INS_LDX_ZPY, &CPU::Y, &CPU::X); }

TEST_F(CPUFixture, LDX_ZPY_WithWrappedAddress_CanLoadValueIntoRegister)
{ TestLoadRegisterZeroPageOffset_WithWrappedAddress(cpu, memory, CPU::INS_LDX_ZPY, &CPU::Y, &CPU::X); }


// LDX_ABS
TEST_F(CPUFixture, LDX_ABS_CanLoadValueIntoRegister)
{ TestLoadRegisterAbsolute(cpu, memory, CPU::INS_LDX_ABS, &CPU::X); }

TEST_F(CPUFixture, LDX_ABS_WithNegativeValue_CanLoadValueIntoRegister)
{ TestLoadRegisterAbsolute_WithNegativeValue(cpu, memory, CPU::INS_LDX_ABS, &CPU::X); }

TEST_F(CPUFixture, LDX_ABS_WithZeroValue_CanLoadValueIntoRegister)
{ TestLoadRegisterAbsolute_WithZeroValue(cpu, memory, CPU::INS_LDX_ABS, &CPU::X); }