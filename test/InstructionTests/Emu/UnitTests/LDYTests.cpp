#include <Emu/UnitTests/CPUFixture.hpp>
#include <Emu/UnitTests/LDUtils.hpp>


using namespace Emu;
using namespace Emu::UnitTests;


// LDY_IM
TEST_F(CPUFixture, LDY_IM_CanLoadValueIntoRegister)
{ TestLoadRegisterImmediate(cpu, memory, CPU::INS_LDY_IM, &CPU::Y); }

TEST_F(CPUFixture, LDY_IM_WithNegativeValue_CanLoadValueIntoRegister)
{ TestLoadRegisterImmediate_WithNegativeValue(cpu, memory, CPU::INS_LDY_IM, &CPU::Y); }

TEST_F(CPUFixture, LDY_IM_WithZeroValue_CanLoadValueIntoRegister)
{ TestLoadRegisterImmediate_WithZeroValue(cpu, memory, CPU::INS_LDY_IM, &CPU::Y); }


// LDY_ZP
TEST_F(CPUFixture, LDY_ZP_CanLoadValueIntoRegister)
{ TestLoadRegisterZeroPage(cpu, memory, CPU::INS_LDY_ZP, &CPU::Y); }

TEST_F(CPUFixture, LDY_ZP_WithNegativeValue_CanLoadValueIntoRegister)
{ TestLoadRegisterZeroPage_WithNegativeValue(cpu, memory, CPU::INS_LDY_ZP, &CPU::Y); }

TEST_F(CPUFixture, LDY_ZP_WithZeroValue_CanLoadValueIntoRegister)
{ TestLoadRegisterZeroPage_WithZeroValue(cpu, memory, CPU::INS_LDY_ZP, &CPU::Y); }


// LDY_ZPX
TEST_F(CPUFixture, LDY_ZPX_CanLoadValueIntoRegister)
{ TestLoadRegisterZeroPageOffset(cpu, memory, CPU::INS_LDY_ZPX, &CPU::X, &CPU::Y); }

TEST_F(CPUFixture, LDY_ZPX_WithNegativeValue_CanLoadValueIntoRegister)
{ TestLoadRegisterZeroPageOffset_WithNegativeValue(cpu, memory, CPU::INS_LDY_ZPX, &CPU::X, &CPU::Y); }

TEST_F(CPUFixture, LDY_ZPX_WithZeroValue_CanLoadValueIntoRegister)
{ TestLoadRegisterZeroPageOffset_WithZeroValue(cpu, memory, CPU::INS_LDY_ZPX, &CPU::X, &CPU::Y); }

TEST_F(CPUFixture, LDY_ZPX_WithWrappedAddress_CanLoadValueIntoRegister)
{ TestLoadRegisterZeroPageOffset_WithWrappedAddress(cpu, memory, CPU::INS_LDY_ZPX, &CPU::X, &CPU::Y); }


// LDY_ABS
TEST_F(CPUFixture, LDY_ABS_CanLoadValueIntoRegister)
{ TestLoadRegisterAbsolute(cpu, memory, CPU::INS_LDY_ABS, &CPU::Y); }

TEST_F(CPUFixture, LDY_ABS_WithNegativeValue_CanLoadValueIntoRegister)
{ TestLoadRegisterAbsolute_WithNegativeValue(cpu, memory, CPU::INS_LDY_ABS, &CPU::Y); }

TEST_F(CPUFixture, LDY_ABS_WithZeroValue_CanLoadValueIntoRegister)
{ TestLoadRegisterAbsolute_WithZeroValue(cpu, memory, CPU::INS_LDY_ABS, &CPU::Y); }


// LDY_ABSX
TEST_F(CPUFixture, LDY_ABSX_CanLoadValueIntoRegister)
{ TestLoadRegisterAbsoluteX(cpu, memory, CPU::INS_LDY_ABSX, &CPU::Y); }

TEST_F(CPUFixture, LDY_ABSX_WithNegativeValue_CanLoadValueIntoRegister)
{ TestLoadRegisterAbsoluteX_WithNegativeValue(cpu, memory, CPU::INS_LDY_ABSX, &CPU::Y); }

TEST_F(CPUFixture, LDY_ABSX_WithZeroValue_CanLoadValueIntoRegister)
{ TestLoadRegisterAbsoluteX_WithZeroValue(cpu, memory, CPU::INS_LDY_ABSX, &CPU::Y); }

TEST_F(CPUFixture, LDY_ABSX_WithCrossPageBoundary_TakesFiveClockCycles)
{ TestLoadRegisterAbsoluteX_WithCrossPageBoundary(cpu, memory, CPU::INS_LDY_ABSX, &CPU::Y); }