#include <Emu/UnitTests/CPUFixture.hpp>
#include <Emu/UnitTests/LDUtils.hpp>


using namespace Emu;
using namespace Emu::UnitTests;


// LDA_IM
TEST_F(CPUFixture, LDA_IM_CanLoadValueIntoRegister)
{ TestLoadRegisterImmediate(cpu, memory, CPU::INS_LDA_IM, &CPU::A); }

TEST_F(CPUFixture, LDA_IM_WithNegativeValue_CanLoadValueIntoRegister)
{ TestLoadRegisterImmediate_WithNegativeValue(cpu, memory, CPU::INS_LDA_IM, &CPU::A); }

TEST_F(CPUFixture, LDA_IM_WithZeroValue_CanLoadValueIntoRegister)
{ TestLoadRegisterImmediate_WithZeroValue(cpu, memory, CPU::INS_LDA_IM, &CPU::A); }


// LDA_ZP
TEST_F(CPUFixture, LDA_ZP_CanLoadValueIntoRegister)
{ TestLoadRegisterZeroPage(cpu, memory, CPU::INS_LDA_ZP, &CPU::A); }

TEST_F(CPUFixture, LDA_ZP_WithNegativeValue_CanLoadValueIntoRegister)
{ TestLoadRegisterZeroPage_WithNegativeValue(cpu, memory, CPU::INS_LDA_ZP, &CPU::A); }

TEST_F(CPUFixture, LDA_ZP_WithZeroValue_CanLoadValueIntoRegister)
{ TestLoadRegisterZeroPage_WithZeroValue(cpu, memory, CPU::INS_LDA_ZP, &CPU::A); }


// LDA_ZPX
TEST_F(CPUFixture, LDA_ZPX_CanLoadValueIntoRegister)
{ TestLoadRegisterZeroPageOffset(cpu, memory, CPU::INS_LDA_ZPX, &CPU::X, &CPU::A); }

TEST_F(CPUFixture, LDA_ZPX_WithNegativeValue_CanLoadValueIntoRegister)
{ TestLoadRegisterZeroPageOffset_WithNegativeValue(cpu, memory, CPU::INS_LDA_ZPX, &CPU::X, &CPU::A); }

TEST_F(CPUFixture, LDA_ZPX_WithZeroValue_CanLoadValueIntoRegister)
{ TestLoadRegisterZeroPageOffset_WithZeroValue(cpu, memory, CPU::INS_LDA_ZPX, &CPU::X, &CPU::A); }

TEST_F(CPUFixture, LDA_ZPX_WithWrappedAddress_CanLoadValueIntoRegister)
{ TestLoadRegisterZeroPageOffset_WithWrappedAddress(cpu, memory, CPU::INS_LDA_ZPX, &CPU::X, &CPU::A); }


// LDA_ABS
TEST_F(CPUFixture, LDA_ABS_CanLoadValueIntoRegister)
{ TestLoadRegisterAbsolute(cpu, memory, CPU::INS_LDA_ABS, &CPU::A); }

TEST_F(CPUFixture, LDA_ABS_WithNegativeValue_CanLoadValueIntoRegister)
{ TestLoadRegisterAbsolute_WithNegativeValue(cpu, memory, CPU::INS_LDA_ABS, &CPU::A); }

TEST_F(CPUFixture, LDA_ABS_WithZeroValue_CanLoadValueIntoRegister)
{ TestLoadRegisterAbsolute_WithZeroValue(cpu, memory, CPU::INS_LDA_ABS, &CPU::A); }


// LDA_ABSX
TEST_F(CPUFixture, LDA_ABSX_CanLoadValueIntoRegister)
{ TestLoadRegisterAbsoluteX(cpu, memory, CPU::INS_LDA_ABSX, &CPU::A); }

TEST_F(CPUFixture, LDA_ABSX_WithNegativeValue_CanLoadValueIntoRegister)
{ TestLoadRegisterAbsoluteX_WithNegativeValue(cpu, memory, CPU::INS_LDA_ABSX, &CPU::A); }

TEST_F(CPUFixture, LDA_ABSX_WithZeroValue_CanLoadValueIntoRegister)
{ TestLoadRegisterAbsoluteX_WithZeroValue(cpu, memory, CPU::INS_LDA_ABSX, &CPU::A); }

TEST_F(CPUFixture, LDA_ABSX_WithCrossPageBoundary_TakesFiveClockCycles)
{ TestLoadRegisterAbsoluteX_WithCrossPageBoundary(cpu, memory, CPU::INS_LDA_ABSX, &CPU::A); }
