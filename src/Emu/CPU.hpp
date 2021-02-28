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

        Byte ReadByte(uint32 & cycles, Byte address, Memory & memory)
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

        void WriteWord(uint32 & cycles, uint32 address, Word value, Memory & memory)
        {
            memory.WriteWord(address, value);
            cycles -= 2;
        }

        void LDASetStatus()
        {
            ZeroFlag = A == 0;
            NegativeFlag = (A & 1 << 7) > 0;
        }

        // opcodes
        static constexpr Byte INS_LDA_IM    = 0xA9;
        static constexpr Byte INS_LDA_ZP    = 0xA5;
        static constexpr Byte INS_LDA_ZPX   = 0xB5;
        static constexpr Byte INS_LDA_ABS   = 0xAD;
        static constexpr Byte INS_LDA_ABSX  = 0xBD;
        static constexpr Byte INS_LDA_ABSY  = 0xB9;

        static constexpr Byte INS_JSR       = 0x20;

        void Execute(uint32 cycles, Memory & memory)
        {
            while (cycles > 0)
            {
                auto instruction = FetchByte(cycles, memory);

                switch (instruction)
                {
                case INS_LDA_IM:
                {
                    A = FetchByte(cycles, memory);
                    LDASetStatus();
                } break;

                case INS_LDA_ZP:
                {
                    auto zeroPageAddress = FetchByte(cycles, memory);
                    A = ReadByte(cycles, zeroPageAddress, memory);
                    LDASetStatus();
                } break;

                case INS_LDA_ZPX:
                {
                    auto zeroPageAddress = FetchByte(cycles, memory);
                    zeroPageAddress += X;
                    --cycles;
                    A = ReadByte(cycles, zeroPageAddress, memory);
                    LDASetStatus();
                } break;

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
                    fmt::print("Instruction not handled: {:x}\n", instruction);
                }
                }
            }
        }

        void DumpState()
        {
            fmt::print("PC: {:x}\nSP: {:x}\nA:  {:x}\nX:  {:x}\nY:  {:x}", PC, SP, A, X, Y);
        }

    };

}