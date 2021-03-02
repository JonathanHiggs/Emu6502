#pragma once

#include <Emu/Memory.hpp>


namespace Emu
{

    struct CPU
    {
        Word PC;
        Word SP;

        // Registers
        Byte A, X, Y;

        Word CarryFlag : 1;
        Word ZeroFlag : 1;
        Word IRQDisableFlag : 1;
        Word DecimalMode : 1;
        Word BreakCommand : 1;
        Word OverflowFlag : 1;
        Word NegativeFlag : 1;
        // Extra debug flags
        Word UnhandledInstruction : 1;
        Word CycleOverflow : 1;

        void Reset(Memory & memory)
        {
            PC = 0xFFFC;
            SP = 0x0100;

            CarryFlag = 0;
            ZeroFlag = 0;
            IRQDisableFlag = 0;
            DecimalMode = 0;
            BreakCommand = 0;
            OverflowFlag = 0;
            NegativeFlag = 0;
            UnhandledInstruction = 0;
            CycleOverflow = 0;

            A = X = Y = 0;

            memory.Initialize();
        }

        Byte FetchByte(uint32 & cycles, Memory & memory)
        {
            auto value = memory.ReadByte(PC);
            ++PC;
            --cycles;
            return value;
        }

        Byte ReadByte(uint32 & cycles, Word address, Memory & memory)
        {
            auto value = memory.ReadByte(address);
            --cycles;
            return value;
        }

        Word FetchWord(uint32 & cycles, Memory & memory)
        {
            auto value = memory.ReadWord(PC);

            PC += 2;
            cycles -= 2;

            return value;
        }

        Word ReadWord(uint32 & cycles, Word address, Memory & memory)
        {
            auto value = memory.ReadWord(address);
            cycles -= 2;
            return value;
        }

        void WriteWord(uint32 & cycles, Word address, Word value, Memory & memory)
        {
            memory.WriteWord(address, value);
            cycles -= 2;
        }

        void LDSetStatus(Byte CPU::*reg)
        {
            ZeroFlag = *this.*reg == 0;
            NegativeFlag = (*this.*reg & 1 << 7) > 0;
        }

        // opcodes
        static constexpr Byte INS_LDA_IM    = 0xA9;
        static constexpr Byte INS_LDA_ZP    = 0xA5;
        static constexpr Byte INS_LDA_ZPX   = 0xB5;
        static constexpr Byte INS_LDA_ABS   = 0xAD;
        static constexpr Byte INS_LDA_ABSX  = 0xBD;
        static constexpr Byte INS_LDA_ABSY  = 0xB9;
        static constexpr Byte INS_LDA_INDX  = 0xA1;
        static constexpr Byte INS_LDA_INDY  = 0xB1;

        static constexpr Byte INS_LDX_IM    = 0xA2;
        static constexpr Byte INS_LDX_ZP    = 0xA6;
        static constexpr Byte INS_LDX_ZPY   = 0xB2;
        static constexpr Byte INS_LDX_ABS   = 0xAE;
        static constexpr Byte INS_LDX_ABSY  = 0xBE;

        static constexpr Byte INS_LDY_IM    = 0xA0;
        static constexpr Byte INS_LDY_ZP    = 0xA4;
        static constexpr Byte INS_LDY_ZPX   = 0xB4;
        static constexpr Byte INS_LDY_ABS   = 0xAC;
        static constexpr Byte INS_LDY_ABSY  = 0xBC;


        static constexpr Byte INS_JSR       = 0x20;

        uint32 Execute(uint32 cycles, Memory & memory)
        {
            uint32 startCycles = cycles;

            while (cycles > 0)
            {
                if (cycles > startCycles)
                {
                    // Detect cycles overflow
                    CycleOverflow = 1;
                    return startCycles - cycles;
                }

                auto instruction = FetchByte(cycles, memory);

                switch (instruction)
                {
                // LDA
                case INS_LDA_IM:
                {
                    A = FetchByte(cycles, memory);
                    LDSetStatus(&CPU::A);
                } break;

                case INS_LDA_ZP:
                {
                    auto zeroPageAddress = FetchByte(cycles, memory);
                    A = ReadByte(cycles, zeroPageAddress, memory);
                    LDSetStatus(&CPU::A);
                } break;

                case INS_LDA_ZPX:
                {
                    auto zeroPageAddress = FetchByte(cycles, memory);
                    zeroPageAddress += X;
                    --cycles;
                    A = ReadByte(cycles, zeroPageAddress, memory);
                    LDSetStatus(&CPU::A);
                } break;

                case INS_LDA_ABS:
                {
                    auto address = FetchWord(cycles, memory);
                    A = ReadByte(cycles, address, memory);
                    LDSetStatus(&CPU::A);
                } break;

                case INS_LDA_ABSX:
                {
                    auto address = FetchWord(cycles, memory);
                    cycles -= ((address & 0xFF) + X) > 0xFF ? 1 : 0;
                    address += X;
                    A = ReadByte(cycles, address, memory);
                    LDSetStatus(&CPU::A);
                } break;

                case INS_LDA_ABSY:
                {
                    auto address = FetchWord(cycles, memory);
                    cycles -= ((address & 0xFF) + Y) > 0xFF ? 1 : 0;
                    address += Y;
                    A = ReadByte(cycles, address, memory);
                    LDSetStatus(&CPU::A);
                } break;

                case INS_LDA_INDX:
                {
                    auto zeroPageAddress = (Word)FetchByte(cycles, memory);
                    --cycles;
                    zeroPageAddress += X;
                    auto address = ReadWord(cycles, zeroPageAddress, memory);
                    A = ReadByte(cycles, address, memory);
                    LDSetStatus(&CPU::A);
                } break;

                case INS_LDA_INDY:
                {
                    auto zeroPageAddress = (Word)FetchByte(cycles, memory);
                    auto address = ReadWord(cycles, zeroPageAddress, memory);
                    cycles -= ((address & 0xFF) + Y) > 0xFF ? 1 : 0;
                    address += Y;
                    A = ReadByte(cycles, address, memory);
                    LDSetStatus(&CPU::A);
                } break;


                // LDX
                case INS_LDX_IM:
                {
                    X = FetchByte(cycles, memory);
                    LDSetStatus(&CPU::X);
                } break;

                case INS_LDX_ZP:
                {
                    auto zeroPageAddress = FetchByte(cycles, memory);
                    X = ReadByte(cycles, zeroPageAddress, memory);
                    LDSetStatus(&CPU::X);
                } break;

                case INS_LDX_ZPY:
                {
                    auto zeroPageAddress = FetchByte(cycles, memory);
                    zeroPageAddress += Y;
                    --cycles;
                    X = ReadByte(cycles, zeroPageAddress, memory);
                    LDSetStatus(&CPU::X);
                } break;


                // LDY
                case INS_LDY_IM:
                {
                    Y = FetchByte(cycles, memory);
                    LDSetStatus(&CPU::Y);
                } break;

                case INS_LDY_ZP:
                {
                    auto zeroPageAddress = FetchByte(cycles, memory);
                    Y = ReadByte(cycles, zeroPageAddress, memory);
                    LDSetStatus(&CPU::Y);
                } break;

                case INS_LDY_ZPX:
                {
                    auto zeroPageAddress = FetchByte(cycles, memory);
                    zeroPageAddress += X;
                    --cycles;
                    Y = ReadByte(cycles, zeroPageAddress, memory);
                    LDSetStatus(&CPU::Y);
                } break;


                // JSR
                case INS_JSR:
                {
                    auto address = FetchWord(cycles, memory);
                    WriteWord(cycles, SP, PC - 1, memory);
                    ++SP;
                    PC = address;
                    --cycles;
                } break;

                default:
                {
                    UnhandledInstruction = 1;
                    fmt::print("Instruction not handled: {:x}\n", instruction);
                    return startCycles - cycles;
                }
                }
            }

            return startCycles - cycles;
        }

        void DumpState()
        {
            fmt::print("PC: {:x}\nSP: {:x}\nA:  {:x}\nX:  {:x}\nY:  {:x}", PC, SP, A, X, Y);
        }

    };

}