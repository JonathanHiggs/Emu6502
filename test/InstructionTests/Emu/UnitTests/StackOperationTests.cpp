#include <gtest/gtest.h>

#include <Emu/CPU.hpp>


namespace Emu::UnitTests
{

    class StackOperationsFixture : public testing::Test
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

        void AssertRegisters(
            CPU const & initialCpu,
            Byte value,
            Byte CPU::* reg)
        {
            EXPECT_EQ(cpu.*reg, value);

            if (reg == &CPU::A || reg == &CPU::X)
            {
                // Set flags if storing in the A or X registers
                if (value == 0x00)
                    EXPECT_TRUE(cpu.StatusFlags.ZeroFlag);
                else
                    EXPECT_FALSE(cpu.StatusFlags.ZeroFlag);

                if (value >= 0x80)
                    EXPECT_TRUE(cpu.StatusFlags.NegativeFlag);
                else
                    EXPECT_FALSE(cpu.StatusFlags.NegativeFlag);
            }
            else
            {
                EXPECT_EQ(cpu.StatusFlags.ZeroFlag,     initialCpu.StatusFlags.ZeroFlag);
                EXPECT_EQ(cpu.StatusFlags.NegativeFlag, initialCpu.StatusFlags.NegativeFlag);
            }

            // Rest unchanged
            EXPECT_EQ(cpu.StatusFlags.CarryFlag,        initialCpu.StatusFlags.CarryFlag);
            EXPECT_EQ(cpu.StatusFlags.IRQDisableFlag,   initialCpu.StatusFlags.IRQDisableFlag);
            EXPECT_EQ(cpu.StatusFlags.DecimalMode,      initialCpu.StatusFlags.DecimalMode);
            EXPECT_EQ(cpu.StatusFlags.BreakCommand,     initialCpu.StatusFlags.BreakCommand);
            EXPECT_EQ(cpu.StatusFlags.OverflowFlag,     initialCpu.StatusFlags.OverflowFlag);

            ASSERT_FALSE(cpu.DebugFlags.UnhandledInstruction);
            ASSERT_FALSE(cpu.DebugFlags.CycleOverflow);
        }

        void TestTransferStackPointer(
            Byte opCode,
            Byte value,
            Byte CPU::* fromRegister,
            Byte CPU::* toRegister,
            Byte initialFlags = 0x0)
        {
            // Arrange
            uint32 cyclesExpected = 2u;

            cpu.*fromRegister = value;
            cpu.Status = initialFlags;
            memory.WriteByte(0xFFFC, opCode);

            CPU initial = cpu;

            // Act
            auto cyclesUsed = cpu.Execute(cyclesExpected, memory);

            // Assert
            EXPECT_EQ(cpu.*toRegister, cpu.SP);

            EXPECT_EQ(cyclesUsed, cyclesExpected);

            AssertRegisters(initial, value, toRegister);
        }

        void TestPushRegisterToStack(
            Byte opCode,
            Byte value,
            Byte CPU::* fromRegister,
            Byte initialFlags = 0x0)
        {
            // Arrange
            uint32 cyclesExpected = 3u;

            cpu.*fromRegister = value;

            if (fromRegister != &CPU::Status)
                cpu.Status = initialFlags;

            memory.WriteByte(0xFFFC, opCode);

            CPU initial = cpu;

            // Act
            auto cyclesUsed = cpu.Execute(cyclesExpected, memory);

            // Assert
            EXPECT_EQ(cpu.SP, initial.SP - 1);
            EXPECT_EQ(cpu.PeekByteInStack(memory), value);

            EXPECT_EQ(cyclesUsed, cyclesExpected);
            EXPECT_EQ(cpu.Status, initial.Status);

            ASSERT_FALSE(cpu.DebugFlags.UnhandledInstruction);
            ASSERT_FALSE(cpu.DebugFlags.CycleOverflow);
        }

        void TestPopStackToRegister(
            Byte opCode,
            Byte value,
            Byte CPU::* toRegister,
            Byte initialFlags = 0x0)
        {
            // Arrange
            uint32 cyclesExpected = 4u;

            uint32 cycles = 0u;
            cpu.PushByteToStack(cycles, value, memory);

            if (toRegister != &CPU::Status)
                cpu.Status = initialFlags;

            memory.WriteByte(0xFFFC, opCode);

            CPU initial = cpu;

            // Act
            auto cyclesUsed = cpu.Execute(cyclesExpected, memory);

            // Assert
            EXPECT_EQ(cpu.SP, initial.SP + 1);
            EXPECT_EQ(cpu.SP, 0xFF);

            EXPECT_EQ(cpu.*toRegister, value);

            EXPECT_EQ(cyclesUsed, cyclesExpected);

            if (toRegister != &CPU::Status)
                AssertRegisters(initial, value, toRegister);

            ASSERT_FALSE(cpu.DebugFlags.UnhandledInstruction);
            ASSERT_FALSE(cpu.DebugFlags.CycleOverflow);
        }

    };


    TEST_F(StackOperationsFixture, INS_TSX_WithPositiveValue)
    { TestTransferStackPointer(CPU::INS_TSX, 0x42, &CPU::SP, &CPU::X); }

    TEST_F(StackOperationsFixture, INS_TSX_WithNegativeValue)
    { TestTransferStackPointer(CPU::INS_TSX, 0xC2, &CPU::SP, &CPU::X); }

    TEST_F(StackOperationsFixture, INS_TSX_WithZeroValue)
    { TestTransferStackPointer(CPU::INS_TSX, 0x00, &CPU::SP, &CPU::X); }

    TEST_F(StackOperationsFixture, INS_TSX_WithStatusFlags)
    { TestTransferStackPointer(CPU::INS_TSX, 0x00, &CPU::SP, &CPU::X, 0xF); }


    TEST_F(StackOperationsFixture, INS_TSA_WithPositiveValue)
    { TestTransferStackPointer(CPU::INS_TSA, 0x42, &CPU::SP, &CPU::A); }

    TEST_F(StackOperationsFixture, INS_TSA_WithNegativeValue)
    { TestTransferStackPointer(CPU::INS_TSA, 0xC2, &CPU::SP, &CPU::A); }

    TEST_F(StackOperationsFixture, INS_TSA_WithZeroValue)
    { TestTransferStackPointer(CPU::INS_TSA, 0x00, &CPU::SP, &CPU::A); }

    TEST_F(StackOperationsFixture, INS_TSA_WithStatusFlags)
    { TestTransferStackPointer(CPU::INS_TSA, 0x00, &CPU::SP, &CPU::A, 0xF); }


    TEST_F(StackOperationsFixture, INS_TXS_WithPositiveValue)
    { TestTransferStackPointer(CPU::INS_TXS, 0x42, &CPU::X, &CPU::SP); }

    TEST_F(StackOperationsFixture, INS_TXS_WithNegativeValue)
    { TestTransferStackPointer(CPU::INS_TXS, 0xC2, &CPU::X, &CPU::SP); }

    TEST_F(StackOperationsFixture, INS_TXS_WithZeroValue)
    { TestTransferStackPointer(CPU::INS_TXS, 0x00, &CPU::X, &CPU::SP); }

    TEST_F(StackOperationsFixture, INS_TXS_WithStatusFlags)
    { TestTransferStackPointer(CPU::INS_TXS, 0x00, &CPU::X, &CPU::SP, 0xF); }


    TEST_F(StackOperationsFixture, INS_PHA)
    { TestPushRegisterToStack(CPU::INS_PHA, 0x44, &CPU::A); }

    TEST_F(StackOperationsFixture, INS_PHA_WithStatusFlags)
    { TestPushRegisterToStack(CPU::INS_PHA, 0x44, &CPU::A, 0xF); }


    TEST_F(StackOperationsFixture, INS_PHP)
    { TestPushRegisterToStack(CPU::INS_PHP, 0x44, &CPU::Status); }


    TEST_F(StackOperationsFixture, INS_PLA_WithPositiveValue)
    { TestPopStackToRegister(CPU::INS_PLA, 0x45, &CPU::A); }

    TEST_F(StackOperationsFixture, INS_PLA_WithNegativeValue)
    { TestPopStackToRegister(CPU::INS_PLA, 0xF5, &CPU::A); }

    TEST_F(StackOperationsFixture, INS_PLA_WithZeroValue)
    { TestPopStackToRegister(CPU::INS_PLA, 0x00, &CPU::A); }

    TEST_F(StackOperationsFixture, INS_PLA_WithStatusFlags)
    { TestPopStackToRegister(CPU::INS_PLA, 0x45, &CPU::A, 0xF); }


    TEST_F(StackOperationsFixture, INS_PLP)
    { TestPopStackToRegister(CPU::INS_PLP, 0x42, &CPU::Status); }

}