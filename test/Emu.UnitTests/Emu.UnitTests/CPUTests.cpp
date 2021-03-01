#include <gtest/gtest.h>

#include <Emu/CPU.hpp>
#include <Emu/Memory.hpp>

using namespace Emu;


class CPUTests : public testing::Test
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


// TEST_F(CPUTests, RunBasicProgram)
// {
//     memory.WriteByte(0xFFFC, CPU::INS_JSR);
//     memory.WriteWord(0xFFFD, 0x4243);
//     memory.WriteByte(0x4243, CPU::INS_LDA_IM);
//     memory.WriteByte(0x4244, 0x84);

//     cpu.Execute(9, memory);

//     EXPECT_EQ(cpu.A, 0x84);

//     //ASSERT_EQ(cpu.UnhandledInstruction, false);
//     //ASSERT_EQ(cpu.CycleOverflow, false);
// }


TEST_F(CPUTests, LDA_IM_CanLoadAValueIntoARegister)
{
    // Arrange
    Byte expected = 0x74;
    memory.WriteByte(0xFFFC, CPU::INS_LDA_IM);
    memory.WriteByte(0xFFFD, expected);

    CPU initial = cpu;

    // Act
    cpu.Execute(2, memory);

    // Assert
    EXPECT_EQ(cpu.A, expected);
    EXPECT_FALSE(cpu.ZeroFlag);
    EXPECT_FALSE(cpu.NegativeFlag);

    EXPECT_EQ(cpu.CarryFlag,        initial.CarryFlag);
    EXPECT_EQ(cpu.IRQDisableFlag,   initial.IRQDisableFlag);
    EXPECT_EQ(cpu.DecimalMode,      initial.DecimalMode);
    EXPECT_EQ(cpu.BreakCommand,     initial.BreakCommand);
    EXPECT_EQ(cpu.OverflowFlag,     initial.OverflowFlag);

    ASSERT_FALSE(cpu.UnhandledInstruction);
    ASSERT_FALSE(cpu.CycleOverflow);
}


TEST_F(CPUTests, LDA_IM_WithNegativeValue_CanLoadAValueIntoARegister)
{
    // Arrange
    Byte expected = 0x84;
    memory.WriteByte(0xFFFC, CPU::INS_LDA_IM);
    memory.WriteByte(0xFFFD, expected);

    CPU initial = cpu;

    // Act
    cpu.Execute(2, memory);

    // Assert
    EXPECT_EQ(cpu.A, expected);
    EXPECT_FALSE(cpu.ZeroFlag);
    EXPECT_TRUE(cpu.NegativeFlag);

    EXPECT_EQ(cpu.CarryFlag,        initial.CarryFlag);
    EXPECT_EQ(cpu.IRQDisableFlag,   initial.IRQDisableFlag);
    EXPECT_EQ(cpu.DecimalMode,      initial.DecimalMode);
    EXPECT_EQ(cpu.BreakCommand,     initial.BreakCommand);
    EXPECT_EQ(cpu.OverflowFlag,     initial.OverflowFlag);

    ASSERT_FALSE(cpu.UnhandledInstruction);
    ASSERT_FALSE(cpu.CycleOverflow);
}


TEST_F(CPUTests, LDA_IM_WithZeroValue_CanLoadAValueIntoARegister)
{
    // Arrange
    Byte expected = 0x00;
    memory.WriteByte(0xFFFC, CPU::INS_LDA_IM);
    memory.WriteByte(0xFFFD, expected);

    CPU initial = cpu;

    // Act
    cpu.Execute(2, memory);

    // Assert
    EXPECT_EQ(cpu.A, expected);
    EXPECT_TRUE(cpu.ZeroFlag);
    EXPECT_FALSE(cpu.NegativeFlag);

    EXPECT_EQ(cpu.CarryFlag,        initial.CarryFlag);
    EXPECT_EQ(cpu.IRQDisableFlag,   initial.IRQDisableFlag);
    EXPECT_EQ(cpu.DecimalMode,      initial.DecimalMode);
    EXPECT_EQ(cpu.BreakCommand,     initial.BreakCommand);
    EXPECT_EQ(cpu.OverflowFlag,     initial.OverflowFlag);

    ASSERT_FALSE(cpu.UnhandledInstruction);
    ASSERT_FALSE(cpu.CycleOverflow);
}


TEST_F(CPUTests, LDA_ZP_CanLoadAValueIntoARegister)
{
    // Arrange
    Byte address = 0x42;
    Byte expected = 0x75;
    memory.WriteByte(0xFFFC, CPU::INS_LDA_ZP);
    memory.WriteByte(0xFFFD, address);
    memory.WriteByte(address, expected);

    CPU initial = cpu;

    // Act
    cpu.Execute(3, memory);

    // Assert
    EXPECT_EQ(cpu.A, expected);
    EXPECT_FALSE(cpu.ZeroFlag);
    EXPECT_FALSE(cpu.NegativeFlag);

    EXPECT_EQ(cpu.CarryFlag,        initial.CarryFlag);
    EXPECT_EQ(cpu.IRQDisableFlag,   initial.IRQDisableFlag);
    EXPECT_EQ(cpu.DecimalMode,      initial.DecimalMode);
    EXPECT_EQ(cpu.BreakCommand,     initial.BreakCommand);
    EXPECT_EQ(cpu.OverflowFlag,     initial.OverflowFlag);

    ASSERT_FALSE(cpu.UnhandledInstruction);
    ASSERT_FALSE(cpu.CycleOverflow);
}


TEST_F(CPUTests, LDA_ZP_WithNegativeValue_CanLoadAValueIntoARegister)
{
    // Arrange
    Byte address = 0x42;
    Byte expected = 0x85;
    memory.WriteByte(0xFFFC, CPU::INS_LDA_ZP);
    memory.WriteByte(0xFFFD, address);
    memory.WriteByte(address, expected);

    CPU initial = cpu;

    // Act
    cpu.Execute(3, memory);

    // Assert
    EXPECT_EQ(cpu.A, expected);
    EXPECT_FALSE(cpu.ZeroFlag);
    EXPECT_TRUE(cpu.NegativeFlag);

    EXPECT_EQ(cpu.CarryFlag,        initial.CarryFlag);
    EXPECT_EQ(cpu.IRQDisableFlag,   initial.IRQDisableFlag);
    EXPECT_EQ(cpu.DecimalMode,      initial.DecimalMode);
    EXPECT_EQ(cpu.BreakCommand,     initial.BreakCommand);
    EXPECT_EQ(cpu.OverflowFlag,     initial.OverflowFlag);

    ASSERT_FALSE(cpu.UnhandledInstruction);
    ASSERT_FALSE(cpu.CycleOverflow);
}


TEST_F(CPUTests, LDA_ZP_WithZeroValue_CanLoadAValueIntoARegister)
{
    // Arrange
    Byte address = 0x42;
    Byte expected = 0x00;
    memory.WriteByte(0xFFFC, CPU::INS_LDA_ZP);
    memory.WriteByte(0xFFFD, address);
    memory.WriteByte(address, expected);

    CPU initial = cpu;

    // Act
    cpu.Execute(3, memory);

    // Assert
    EXPECT_EQ(cpu.A, expected);
    EXPECT_TRUE(cpu.ZeroFlag);
    EXPECT_FALSE(cpu.NegativeFlag);

    EXPECT_EQ(cpu.CarryFlag,        initial.CarryFlag);
    EXPECT_EQ(cpu.IRQDisableFlag,   initial.IRQDisableFlag);
    EXPECT_EQ(cpu.DecimalMode,      initial.DecimalMode);
    EXPECT_EQ(cpu.BreakCommand,     initial.BreakCommand);
    EXPECT_EQ(cpu.OverflowFlag,     initial.OverflowFlag);

    ASSERT_FALSE(cpu.UnhandledInstruction);
    ASSERT_FALSE(cpu.CycleOverflow);
}


TEST_F(CPUTests, LDA_ZPX_CanLoadAValueIntoARegister)
{
    // Arrange
    Byte registerValue = 0x05;
    Byte baseAddress = 0x42;
    Byte offsetAddress = baseAddress + registerValue;
    Byte expected = 0x37;

    cpu.X = registerValue;
    memory.WriteByte(0xFFFC, CPU::INS_LDA_ZPX);
    memory.WriteByte(0xFFFD, baseAddress);
    memory.WriteByte(offsetAddress, expected);

    CPU initial = cpu;

    // Act
    cpu.Execute(4, memory);

    // Assert
    EXPECT_EQ(cpu.A, expected);
    EXPECT_FALSE(cpu.ZeroFlag);
    EXPECT_FALSE(cpu.NegativeFlag);

    EXPECT_EQ(cpu.CarryFlag,        initial.CarryFlag);
    EXPECT_EQ(cpu.IRQDisableFlag,   initial.IRQDisableFlag);
    EXPECT_EQ(cpu.DecimalMode,      initial.DecimalMode);
    EXPECT_EQ(cpu.BreakCommand,     initial.BreakCommand);
    EXPECT_EQ(cpu.OverflowFlag,     initial.OverflowFlag);

    ASSERT_FALSE(cpu.UnhandledInstruction);
    ASSERT_FALSE(cpu.CycleOverflow);
}


TEST_F(CPUTests, LDA_ZPX_WithNegativeValue_CanLoadAValueIntoARegister)
{
    // Arrange
    Byte registerValue = 0x05;
    Byte baseAddress = 0x42;
    Byte offsetAddress = baseAddress + registerValue;
    Byte expected = 0x97;

    cpu.X = registerValue;
    memory.WriteByte(0xFFFC, CPU::INS_LDA_ZPX);
    memory.WriteByte(0xFFFD, baseAddress);
    memory.WriteByte(offsetAddress, expected);

    CPU initial = cpu;

    // Act
    cpu.Execute(4, memory);

    // Assert
    EXPECT_EQ(cpu.A, expected);
    EXPECT_FALSE(cpu.ZeroFlag);
    EXPECT_TRUE(cpu.NegativeFlag);

    EXPECT_EQ(cpu.CarryFlag,        initial.CarryFlag);
    EXPECT_EQ(cpu.IRQDisableFlag,   initial.IRQDisableFlag);
    EXPECT_EQ(cpu.DecimalMode,      initial.DecimalMode);
    EXPECT_EQ(cpu.BreakCommand,     initial.BreakCommand);
    EXPECT_EQ(cpu.OverflowFlag,     initial.OverflowFlag);

    ASSERT_FALSE(cpu.UnhandledInstruction);
    ASSERT_FALSE(cpu.CycleOverflow);
}


TEST_F(CPUTests, LDA_ZPX_WithZeroValue_CanLoadAValueIntoARegister)
{
    // Arrange
    Byte registerValue = 0x05;
    Byte baseAddress = 0x42;
    Byte offsetAddress = baseAddress + registerValue;
    Byte expected = 0x00;

    cpu.X = registerValue;
    memory.WriteByte(0xFFFC, CPU::INS_LDA_ZPX);
    memory.WriteByte(0xFFFD, baseAddress);
    memory.WriteByte(offsetAddress, expected);

    CPU initial = cpu;

    // Act
    cpu.Execute(4, memory);

    // Assert
    EXPECT_EQ(cpu.A, expected);
    EXPECT_TRUE(cpu.ZeroFlag);
    EXPECT_FALSE(cpu.NegativeFlag);

    EXPECT_EQ(cpu.CarryFlag,        initial.CarryFlag);
    EXPECT_EQ(cpu.IRQDisableFlag,   initial.IRQDisableFlag);
    EXPECT_EQ(cpu.DecimalMode,      initial.DecimalMode);
    EXPECT_EQ(cpu.BreakCommand,     initial.BreakCommand);
    EXPECT_EQ(cpu.OverflowFlag,     initial.OverflowFlag);

    ASSERT_FALSE(cpu.UnhandledInstruction);
    ASSERT_FALSE(cpu.CycleOverflow);
}


TEST_F(CPUTests, LDA_ZPX_WithWrappedAddress_CanLoadAValueIntoARegister)
{
    // Arrange
    Byte registerValue = 0xFF;
    Byte baseAddress = 0x80;
    Byte offsetAddress = (baseAddress + registerValue) % 0x100;
    Byte expected = 0x24;

    cpu.X = registerValue;
    memory.WriteByte(0xFFFC, CPU::INS_LDA_ZPX);
    memory.WriteByte(0xFFFD, baseAddress);
    memory.WriteByte(offsetAddress, expected);

    CPU initial = cpu;

    // Act
    cpu.Execute(4, memory);

    // Assert
    EXPECT_EQ(cpu.A, expected);
    EXPECT_FALSE(cpu.ZeroFlag);
    EXPECT_FALSE(cpu.NegativeFlag);

    EXPECT_EQ(cpu.CarryFlag,        initial.CarryFlag);
    EXPECT_EQ(cpu.IRQDisableFlag,   initial.IRQDisableFlag);
    EXPECT_EQ(cpu.DecimalMode,      initial.DecimalMode);
    EXPECT_EQ(cpu.BreakCommand,     initial.BreakCommand);
    EXPECT_EQ(cpu.OverflowFlag,     initial.OverflowFlag);

    ASSERT_FALSE(cpu.UnhandledInstruction);
    ASSERT_FALSE(cpu.CycleOverflow);
}