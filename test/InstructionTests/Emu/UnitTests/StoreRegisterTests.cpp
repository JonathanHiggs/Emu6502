#include <gtest/gtest.h>

#include <Emu/CPU.hpp>


namespace Emu::UnitTests
{

    class StoreRegisterFixture : public testing::Test
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
            uint32 cyclesUsed,
            uint32 cyclesExpected)
        {
            EXPECT_EQ(cpu.Status, initialCpu.Status);
            EXPECT_EQ(cpu.DebugStatus, initialCpu.DebugStatus);
            EXPECT_EQ(cyclesUsed, cyclesExpected);
        }


        void StoreZeroPage(Byte opCode, Byte CPU::* storeRegister)
        {
            // Arrange
            uint32 cyclesExpected = 3u;
            Byte value = 0xFF;
            Byte zeroPageAddress = 0x10;
            Word address = (Word)zeroPageAddress;

            cpu.*storeRegister = value;
            memory.WriteByte(0xFFFC, opCode);
            memory.WriteByte(0xFFFD, zeroPageAddress);

            CPU initial = cpu;

            // Act
            auto cyclesUsed = cpu.Execute(cyclesExpected, memory);

            // Assert
            EXPECT_EQ(memory.ReadByte(address), value);
            EXPECT_EQ(cyclesUsed, cyclesExpected);
            AssertRegisters(initial, cyclesUsed, cyclesExpected);
        }


        void StoreZeroPageOffset(
            Byte opCode,
            Byte CPU::* storeRegister,
            Byte CPU::* offsetRegister,
            bool withWrap = false)
        {
            // Only apply wrap logic with X
            assert(!(withWrap && offsetRegister == &CPU::Y));

            // Arrange
            uint32 cyclesExpected = 4u;
            Byte value = 0xFE;
            Byte zeroPageAddress = 0x10;
            Byte zeroPageOffset = withWrap ? 0xFF : 0x20;
            Word address = (Word)zeroPageAddress + (Word)zeroPageOffset;

            if (withWrap) address &= 0x00FF;

            cpu.*offsetRegister = zeroPageOffset;
            cpu.*storeRegister = value;
            memory.WriteByte(0xFFFC, opCode);
            memory.WriteByte(0xFFFD, zeroPageAddress);

            CPU initial = cpu;

            // Act
            auto cyclesUsed = cpu.Execute(cyclesExpected, memory);

            // Assert
            EXPECT_EQ(memory.ReadByte(address), value);
            EXPECT_EQ(cyclesUsed, cyclesExpected);
            AssertRegisters(initial, cyclesUsed, cyclesExpected);
        }


        void StoreAbsolute(Byte opCode, Byte CPU::* storeRegister)
        {
            // Arrange
            uint32 cyclesExpected = 4u;
            Byte value = 0xFD;
            Word address = 0x4480;

            cpu.*storeRegister = value;
            memory.WriteByte(0xFFFC, opCode);
            memory.WriteWord(0xFFFD, address);

            CPU initial = cpu;

            // Act
            auto cyclesUsed = cpu.Execute(cyclesExpected, memory);

            // Assert
            EXPECT_EQ(memory.ReadByte(address), value);
            EXPECT_EQ(cyclesUsed, cyclesExpected);
            AssertRegisters(initial, cyclesUsed, cyclesExpected);
        }


        void StoreAbsoluteOffset(Byte opCode, Byte CPU::* offsetRegister)
        {
            // Arrange
            uint32 cyclesExpected = 5u;
            Byte value = 0xFC;
            Byte addressOffset = 0x20;
            Word baseAddress = 0x4470;
            Word address = baseAddress + addressOffset;

            cpu.A = value;      // Store register
            cpu.*offsetRegister = addressOffset;
            memory.WriteByte(0xFFFC, opCode);
            memory.WriteWord(0xFFFD, baseAddress);

            CPU initial = cpu;

            // Act
            auto cyclesUsed = cpu.Execute(cyclesExpected, memory);

            // Assert
            EXPECT_EQ(memory.ReadByte(address), value);
            EXPECT_EQ(cyclesUsed, cyclesExpected);
            AssertRegisters(initial, cyclesUsed, cyclesExpected);
        }


        void StoreIndirectX(Byte opCode)
        {
            // Arrange
            uint32 cyclesExpected = 6u;
            Byte value = 0xFB;
            Byte zeroPageAddress = 0x20;
            Byte zeroPageOffset = 0x04;
            Word indexAddress = (Word)(zeroPageAddress + zeroPageOffset);
            Word address = 0x4460;

            cpu.A = value;      // Store register
            cpu.X = zeroPageOffset;
            memory.WriteByte(0xFFFC, opCode);
            memory.WriteByte(0xFFFD, zeroPageAddress);
            memory.WriteWord(indexAddress, address);

            CPU initial = cpu;

            // Act
            auto cyclesUsed = cpu.Execute(cyclesExpected, memory);

            // Assert
            EXPECT_EQ(memory.ReadByte(address), value);
            EXPECT_EQ(cyclesUsed, cyclesExpected);
            AssertRegisters(initial, cyclesUsed, cyclesExpected);
        }


        void StoreIndirectY(Byte opCode, bool crossPageBoundary = false)
        {
            // Arrange
            uint32 cyclesExpected = 6u;
            Byte value = 0xFB;
            Byte offsetValue = 0x20;
            Byte zeroPageAddress = 0x04;
            Word indexAddress = crossPageBoundary ? 0x44FF : 0x4400;
            Word address = indexAddress + offsetValue;

            cpu.A = value;      // Store register
            cpu.Y = offsetValue;
            memory.WriteByte(0xFFFC, opCode);
            memory.WriteByte(0xFFFD, zeroPageAddress);
            memory.WriteWord((Word)zeroPageAddress, indexAddress);

            CPU initial = cpu;

            // Act
            auto cyclesUsed = cpu.Execute(cyclesExpected, memory);

            // Assert
            EXPECT_EQ(memory.ReadByte(address), value);
            EXPECT_EQ(cyclesUsed, cyclesExpected);
            AssertRegisters(initial, cyclesUsed, cyclesExpected);
        }
    };


    /* --------- STA --------- */
    TEST_F(StoreRegisterFixture, STA_ZP)
    { StoreZeroPage(CPU::INS_STA_ZP, &CPU::A); }

    TEST_F(StoreRegisterFixture, STA_ZPX)
    { StoreZeroPageOffset(CPU::INS_STA_ZPX, &CPU::A, &CPU::X); }

    TEST_F(StoreRegisterFixture, STA_ZPX_WithWrap)
    { StoreZeroPageOffset(CPU::INS_STA_ZPX, &CPU::A, &CPU::X, true); }

    TEST_F(StoreRegisterFixture, STA_ABS)
    { StoreAbsolute(CPU::INS_STA_ABS, &CPU::A); }

    TEST_F(StoreRegisterFixture, STA_ABSX)
    { StoreAbsoluteOffset(CPU::INS_STA_ABSX, &CPU::X); }

    TEST_F(StoreRegisterFixture, STA_ABSY)
    { StoreAbsoluteOffset(CPU::INS_STA_ABSY, &CPU::Y); }

    TEST_F(StoreRegisterFixture, STA_INDX)
    { StoreIndirectX(CPU::INS_STA_INDX); }

    TEST_F(StoreRegisterFixture, STA_INDY)
    { StoreIndirectY(CPU::INS_STA_INDY); }

    TEST_F(StoreRegisterFixture, STA_INDY_WithPageCross)
    { StoreIndirectY(CPU::INS_STA_INDY, true); }


    /* --------- STX --------- */
    TEST_F(StoreRegisterFixture, STX_ZP)
    { StoreZeroPage(CPU::INS_STX_ZP, &CPU::X); }

    TEST_F(StoreRegisterFixture, STX_ZPY)
    { StoreZeroPageOffset(CPU::INS_STX_ZPY, &CPU::X, &CPU::Y); }

    TEST_F(StoreRegisterFixture, STX_ABS)
    { StoreAbsolute(CPU::INS_STX_ABS, &CPU::X); }


    /* --------- STY --------- */
    TEST_F(StoreRegisterFixture, STY_ZP)
    { StoreZeroPage(CPU::INS_STY_ZP, &CPU::Y); }

    TEST_F(StoreRegisterFixture, STY_ZPX)
    { StoreZeroPageOffset(CPU::INS_STY_ZPX, &CPU::Y, &CPU::X); }

    TEST_F(StoreRegisterFixture, STY_ZPX_WithWrap)
    { StoreZeroPageOffset(CPU::INS_STY_ZPX, &CPU::Y, &CPU::X, true); }

    TEST_F(StoreRegisterFixture, STY_ABS)
    { StoreAbsolute(CPU::INS_STY_ABS, &CPU::Y); }

}