#pragma once

#include <Emu/Memory.hpp>


namespace Emu
{

    struct CPUStatusFlags
    {
        Byte CarryFlag : 1;
        Byte ZeroFlag : 1;
        Byte IRQDisableFlag : 1;    // Interrupt disable
        Byte DecimalMode : 1;
        Byte BreakCommand : 1;
        Byte Unused : 1;
        Byte OverflowFlag : 1;
        Byte NegativeFlag : 1;
    };

    struct CPUDebugFlags
    {
        Byte UnhandledInstruction : 1;
        Byte CycleOverflow : 1;
    };

    struct CPU
    {
        Word PC;        // Program Counter
        Byte SP;        // Stack Pointer
        Byte A, X, Y;   // Registers

        union
        {
            Byte Status;
            CPUStatusFlags StatusFlags;
        };

        union
        {
            Byte DebugStatus;
            CPUDebugFlags DebugFlags;
        };

        void Reset(Memory & memory, Word programCounter = 0xFFFC)
        {
            PC = programCounter;
            SP = 0xFF;

            Status = 0;
            DebugStatus = 0;

            A = X = Y = 0;

            memory.Initialize();
        }

        inline Byte FetchByte(uint32 & cycles, Memory const & memory)
        {
            auto value = memory.ReadByte(PC);
            ++PC;
            --cycles;
            return value;
        }

        inline Word FetchAddressAbsolute(uint32 & cycles, Memory const & memory)
        {
            return FetchWord(cycles, memory);
        }

        inline Word FetchAddressAbsoluteX(uint32 & cycles, Memory const & memory, bool useCycleAnyway = false)
        {
            auto address = FetchAddressAbsolute(cycles, memory);
            cycles -= useCycleAnyway || ((address & 0xFF) + X) > 0xFF ? 1 : 0;
            address += X;
            return address;
        }

        inline Word FetchAddressAbsoluteY(uint32 & cycles, Memory const & memory, bool useCycleAnyway = false)
        {
            auto address = FetchAddressAbsolute(cycles, memory);
            cycles -= useCycleAnyway || ((address & 0xFF) + Y) > 0xFF ? 1 : 0;
            address += Y;
            return address;
        }

        inline Word FetchAddressIndirectX(uint32 & cycles, Memory const & memory)
        {
            auto zeroPageAddress = FetchAddressZeroPage(cycles, memory);
            --cycles;
            zeroPageAddress += X;
            return ReadWord(cycles, zeroPageAddress, memory);
        }

        inline Word FetchAddressIndirectY(uint32 & cycles, Memory const & memory, bool useCycleAnyway = false)
        {
            auto zeroPageAddress = FetchAddressZeroPage(cycles, memory);
            auto address = ReadWord(cycles, zeroPageAddress, memory);
            cycles -= useCycleAnyway || ((address & 0xFF) + Y) > 0xFF ? 1 : 0;
            address += Y;
            return address;
        }

        inline Word FetchAddressZeroPage(uint32 & cycles, Memory const & memory)
        {
            return FetchByte(cycles, memory);
        }

        inline Word FetchAddressZeroPageX(uint32 & cycles, Memory const & memory)
        {
            --cycles;
            return (FetchAddressZeroPage(cycles, memory) + X) & 0x00FF;
        }

        inline Word FetchAddressZeroPageY(uint32 & cycles, Memory const & memory)
        {
            --cycles;
            return (FetchAddressZeroPage(cycles, memory) + Y) & 0x00FF;
        }

        inline Byte ReadByte(uint32 & cycles, Word const address, Memory const & memory) const
        {
            auto value = memory.ReadByte(address);
            --cycles;
            return value;
        }

        inline void WriteByte(uint32 & cycles, Word const address, Byte const value, Memory & memory)
        {
            memory.WriteByte(address, value);
            cycles -= 1;
        }

        inline Word FetchWord(uint32 & cycles, Memory const & memory)
        {
            auto value = memory.ReadWord(PC);

            PC += 2;
            cycles -= 2;

            return value;
        }

        inline Word ReadWord(uint32 & cycles, Word const address, Memory const & memory) const
        {
            auto value = memory.ReadWord(address);
            cycles -= 2;
            return value;
        }

        inline void WriteWord(uint32 & cycles, Word const address, Word const value, Memory & memory)
        {
            memory.WriteWord(address, value);
            cycles -= 2;
        }

        inline void LoadRegisterSetStatus(Byte & reg)
        {
            StatusFlags.ZeroFlag = reg == 0;
            StatusFlags.NegativeFlag = (reg & 1 << 7) > 0;
        }

        inline Word StackPointerAddress()
        {
            return 0x0100 | SP;
        }

        inline void PushWordToStack(uint32 & cycles, Word const value, Memory & memory)
        {
            // Note: Possible error if the stack is overflowing
            Word stackAddress = StackPointerAddress() - 1;
            WriteWord(cycles, stackAddress, value, memory);
            SP -= 2;
        }

        inline Word PeekWordInStack(Memory & memory)
        {
            uint32 cycles = 0u;
            Word stackAddress = StackPointerAddress() + 1;
            return ReadWord(cycles, stackAddress, memory);
        }

        inline Word PopWordFromStack(uint32 & cycles, Memory & memory)
        {
            // Note: Possible error if the stack is overflowing
            Word stackAddress = StackPointerAddress() + 1;
            auto value = ReadWord(cycles, stackAddress, memory);
            SP += 2;
            return value;
        }

        inline void PushByteToStack(uint32 & cycles, Byte const value, Memory & memory)
        {
            auto stackAddress = StackPointerAddress();
            WriteByte(cycles, stackAddress, value, memory);
            --SP;
        }

        inline Byte PopByteFromStack(uint32 & cycles, Memory & memory)
        {
            auto stackAddress = StackPointerAddress();
            auto value = ReadByte(cycles, stackAddress, memory);
            --SP;
            return value;
        }

        // opcodes
        static constexpr Byte INS_JSR       = 0x20;

        static constexpr Byte INS_RTS       = 0x60;

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
        static constexpr Byte INS_LDY_ABSX  = 0xBC;

        static constexpr Byte INS_STA_ZP    = 0x85;
        static constexpr Byte INS_STA_ZPX   = 0x95;
        static constexpr Byte INS_STA_ABS   = 0x8D;
        static constexpr Byte INS_STA_ABSX  = 0x9D;
        static constexpr Byte INS_STA_ABSY  = 0x99;
        static constexpr Byte INS_STA_INDX  = 0x81;
        static constexpr Byte INS_STA_INDY  = 0x91;

        static constexpr Byte INS_STX_ZP    = 0x86;
        static constexpr Byte INS_STX_ZPY   = 0x96;
        static constexpr Byte INS_STX_ABS   = 0x8E;

        static constexpr Byte INS_STY_ZP    = 0x84;
        static constexpr Byte INS_STY_ZPX   = 0x94;
        static constexpr Byte INS_STY_ABS   = 0x8C;


        uint32 Execute(uint32 cycles, Memory & memory)
        {
            auto LoadRegisterImmediate = [&cycles, &memory, this](Byte & reg)
            {
                reg = FetchByte(cycles, memory);
                LoadRegisterSetStatus(reg);
            };

            /* Load a register from the address */
            auto LoadRegister = [&cycles, &memory, this](Byte & reg, Word const & address)
            {
                reg = ReadByte(cycles, address, memory);
                LoadRegisterSetStatus(reg);
            };

            auto StoreRegister = [&cycles, &memory, this](Byte & reg, Word const & address)
            { WriteByte(cycles, address, reg, memory); };

            uint32 startCycles = cycles;

            while (cycles > 0)
            {
                if (cycles > startCycles)
                {
                    // Detect cycles overflow
                    DebugFlags.CycleOverflow = 1;
                    return startCycles - cycles;
                }

                auto instruction = FetchByte(cycles, memory);

                switch (instruction)
                {
                // Load Register
                case INS_LDA_IM:    LoadRegisterImmediate(A);                                       break;
                case INS_LDA_ZP:    LoadRegister(A, FetchAddressZeroPage(cycles, memory));          break;
                case INS_LDA_ZPX:   LoadRegister(A, FetchAddressZeroPageX(cycles, memory));         break;
                case INS_LDA_ABS:   LoadRegister(A, FetchAddressAbsolute(cycles, memory));          break;
                case INS_LDA_ABSX:  LoadRegister(A, FetchAddressAbsoluteX(cycles, memory));         break;
                case INS_LDA_ABSY:  LoadRegister(A, FetchAddressAbsoluteY(cycles, memory));         break;
                case INS_LDA_INDX:  LoadRegister(A, FetchAddressIndirectX(cycles, memory));         break;
                case INS_LDA_INDY:  LoadRegister(A, FetchAddressIndirectY(cycles, memory));         break;
                // LDX
                case INS_LDX_IM:    LoadRegisterImmediate(X);                                       break;
                case INS_LDX_ZP:    LoadRegister(X, FetchAddressZeroPage(cycles, memory));          break;
                case INS_LDX_ZPY:   LoadRegister(X, FetchAddressZeroPageY(cycles, memory));         break;
                case INS_LDX_ABS:   LoadRegister(X, FetchAddressAbsolute(cycles, memory));          break;
                case INS_LDX_ABSY:  LoadRegister(X, FetchAddressAbsoluteY(cycles, memory));         break;
                // LDY
                case INS_LDY_IM:    LoadRegisterImmediate(Y);                                       break;
                case INS_LDY_ZP:    LoadRegister(Y, FetchAddressZeroPage(cycles, memory));          break;
                case INS_LDY_ZPX:   LoadRegister(Y, FetchAddressZeroPageX(cycles, memory));         break;
                case INS_LDY_ABS:   LoadRegister(Y, FetchAddressAbsolute(cycles, memory));          break;
                case INS_LDY_ABSX:  LoadRegister(Y, FetchAddressAbsoluteX(cycles, memory));         break;

                // StoreRegister
                case INS_STA_ZP:    StoreRegister(A, FetchAddressZeroPage(cycles, memory));         break;
                case INS_STA_ZPX:   StoreRegister(A, FetchAddressZeroPageX(cycles, memory));        break;
                case INS_STA_ABS:   StoreRegister(A, FetchAddressAbsolute(cycles, memory));         break;
                case INS_STA_ABSX:  StoreRegister(A, FetchAddressAbsoluteX(cycles, memory, true));  break;
                case INS_STA_ABSY:  StoreRegister(A, FetchAddressAbsoluteY(cycles, memory, true));  break;
                case INS_STA_INDX:  StoreRegister(A, FetchAddressIndirectX(cycles, memory));        break;
                case INS_STA_INDY:  StoreRegister(A, FetchAddressIndirectY(cycles, memory, true));  break;

                case INS_STX_ZP:    StoreRegister(X, FetchAddressZeroPage(cycles, memory));         break;
                case INS_STX_ZPY:   StoreRegister(X, FetchAddressZeroPageY(cycles, memory));        break;
                case INS_STX_ABS:   StoreRegister(X, FetchAddressAbsolute(cycles, memory));         break;

                case INS_STY_ZP:    StoreRegister(Y, FetchAddressZeroPage(cycles, memory));         break;
                case INS_STY_ZPX:   StoreRegister(Y, FetchAddressZeroPageX(cycles, memory));        break;
                case INS_STY_ABS:   StoreRegister(Y, FetchAddressAbsolute(cycles, memory));         break;

                // JSR
                case INS_JSR:
                {
                    auto routineAddress = FetchAddressAbsolute(cycles, memory);
                    PushWordToStack(cycles, PC - 1, memory);
                    PC = routineAddress;
                    --cycles;
                } break;

                // RTS
                case INS_RTS:
                {
                    auto returnAddress = PopWordFromStack(cycles, memory);
                    PC = returnAddress + 1;
                    cycles -= 3;
                } break;

                default:
                {
                    DebugFlags.UnhandledInstruction = 1;
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