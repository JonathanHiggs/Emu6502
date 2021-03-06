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

        inline Word StackPointerAddress() const
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

        inline Word PeekWordInStack(Memory & memory) const
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
            cycles -= 3;
            return value;
        }

        inline void PushByteToStack(uint32 & cycles, Byte const value, Memory & memory)
        {
            auto stackAddress = StackPointerAddress();
            WriteByte(cycles, stackAddress, value, memory);
            --SP;
        }

        inline Byte PeekByteInStack(Memory & memory) const
        {
            uint32 cycles = 0u;
            Word stackAddress = StackPointerAddress() + 0x1;
            return ReadByte(cycles, stackAddress, memory);
        }

        inline Byte PopByteFromStack(uint32 & cycles, Memory & memory)
        {
            ++SP;
            auto stackAddress = StackPointerAddress();
            auto value = ReadByte(cycles, stackAddress, memory);
            cycles -= 2;
            return value;
        }

        // opcodes
        static constexpr Byte INS_AND_IM    = 0x29;
        static constexpr Byte INS_AND_ZP    = 0x25;
        static constexpr Byte INS_AND_ZPX   = 0x35;
        static constexpr Byte INS_AND_ABS   = 0x2D;
        static constexpr Byte INS_AND_ABSX  = 0x3D;
        static constexpr Byte INS_AND_ABSY  = 0x39;
        static constexpr Byte INS_AND_INDX  = 0x21;
        static constexpr Byte INS_AND_INDY  = 0x31;

        static constexpr Byte INS_EOR_IM    = 0x49;
        static constexpr Byte INS_EOR_ZP    = 0x45;
        static constexpr Byte INS_EOR_ZPX   = 0x55;
        static constexpr Byte INS_EOR_ABS   = 0x4D;
        static constexpr Byte INS_EOR_ABSX  = 0x5D;
        static constexpr Byte INS_EOR_ABSY  = 0x59;
        static constexpr Byte INS_EOR_INDX  = 0x41;
        static constexpr Byte INS_EOR_INDY  = 0x51;

        static constexpr Byte INS_JMP_ABS   = 0x4C;
        static constexpr Byte INS_JMP_IND   = 0x6C;

        static constexpr Byte INS_JSR       = 0x20;

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

        static constexpr Byte INS_ORA_IM    = 0x09;
        static constexpr Byte INS_ORA_ZP    = 0x05;
        static constexpr Byte INS_ORA_ZPX   = 0x15;
        static constexpr Byte INS_ORA_ABS   = 0x0D;
        static constexpr Byte INS_ORA_ABSX  = 0x1D;
        static constexpr Byte INS_ORA_ABSY  = 0x19;
        static constexpr Byte INS_ORA_INDX  = 0x01;
        static constexpr Byte INS_ORA_INDY  = 0x11;

        static constexpr Byte INS_PHA       = 0x48;
        static constexpr Byte INS_PHP       = 0x08;
        static constexpr Byte INS_PLA       = 0x68;
        static constexpr Byte INS_PLP       = 0x28;

        static constexpr Byte INS_RTS       = 0x60;

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

        static constexpr Byte INS_TSX       = 0xBA;
        static constexpr Byte INS_TSA       = 0x8A;
        static constexpr Byte INS_TXS       = 0x9A;


        inline static Byte OpAND(Byte a, Byte b) { return a & b; }
        inline static Byte OpORA(Byte a, Byte b) { return a | b; }
        inline static Byte OpEOR(Byte a, Byte b) { return a ^ b; }


        uint32 Execute(uint32 cycles, Memory & memory)
        {
            auto LoadRegisterImmediate = [&cycles, &memory, this](Byte & reg)
            { LoadRegisterSetStatus(reg = FetchByte(cycles, memory)); };

            /* Load a register from the address */
            auto LoadRegister = [&cycles, &memory, this](Byte & reg, Word const & address)
            { LoadRegisterSetStatus(reg = ReadByte(cycles, address, memory)); };

            auto StoreRegister = [&cycles, &memory, this](Byte & reg, Word const & address)
            { WriteByte(cycles, address, reg, memory); };

            auto LogicalOp = [&cycles, &memory, this](Word const & address, Byte (*op)(Byte, Byte))
            { LoadRegisterSetStatus(A = op(A, ReadByte(cycles, address, memory))); };

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
                case INS_AND_IM:    A = A & FetchByte(cycles, memory); LoadRegisterSetStatus(A);            break;
                case INS_AND_ZP:    LogicalOp(FetchAddressZeroPage(cycles, memory), OpAND);                 break;
                case INS_AND_ZPX:   LogicalOp(FetchAddressZeroPageX(cycles, memory), OpAND);                break;
                case INS_AND_ABS:   LogicalOp(FetchAddressAbsolute(cycles, memory), OpAND);                 break;
                case INS_AND_ABSX:  LogicalOp(FetchAddressAbsoluteX(cycles, memory), OpAND);                break;
                case INS_AND_ABSY:  LogicalOp(FetchAddressAbsoluteY(cycles, memory), OpAND);                break;
                case INS_AND_INDX:  LogicalOp(FetchAddressIndirectX(cycles, memory), OpAND);                break;
                case INS_AND_INDY:  LogicalOp(FetchAddressIndirectY(cycles, memory), OpAND);                break;

                case INS_EOR_IM:    A = A ^ FetchByte(cycles, memory); LoadRegisterSetStatus(A);            break;
                case INS_EOR_ZP:    LogicalOp(FetchAddressZeroPage(cycles, memory), OpEOR);                 break;
                case INS_EOR_ZPX:   LogicalOp(FetchAddressZeroPageX(cycles, memory), OpEOR);                break;
                case INS_EOR_ABS:   LogicalOp(FetchAddressAbsolute(cycles, memory), OpEOR);                 break;
                case INS_EOR_ABSX:  LogicalOp(FetchAddressAbsoluteX(cycles, memory), OpEOR);                break;
                case INS_EOR_ABSY:  LogicalOp(FetchAddressAbsoluteY(cycles, memory), OpEOR);                break;
                case INS_EOR_INDX:  LogicalOp(FetchAddressIndirectX(cycles, memory), OpEOR);                break;
                case INS_EOR_INDY:  LogicalOp(FetchAddressIndirectY(cycles, memory), OpEOR);                break;

                case INS_JMP_ABS:   PC = FetchAddressAbsolute(cycles, memory);                              break;
                case INS_JMP_IND:   PC = ReadWord(cycles, FetchAddressAbsolute(cycles, memory), memory);    break;

                case INS_JSR:
                {
                    auto routineAddress = FetchAddressAbsolute(cycles, memory);
                    PushWordToStack(cycles, PC - 1, memory);
                    PC = routineAddress;
                    --cycles;
                } break;

                case INS_LDA_IM:    LoadRegisterImmediate(A);                                               break;
                case INS_LDA_ZP:    LoadRegister(A, FetchAddressZeroPage(cycles, memory));                  break;
                case INS_LDA_ZPX:   LoadRegister(A, FetchAddressZeroPageX(cycles, memory));                 break;
                case INS_LDA_ABS:   LoadRegister(A, FetchAddressAbsolute(cycles, memory));                  break;
                case INS_LDA_ABSX:  LoadRegister(A, FetchAddressAbsoluteX(cycles, memory));                 break;
                case INS_LDA_ABSY:  LoadRegister(A, FetchAddressAbsoluteY(cycles, memory));                 break;
                case INS_LDA_INDX:  LoadRegister(A, FetchAddressIndirectX(cycles, memory));                 break;
                case INS_LDA_INDY:  LoadRegister(A, FetchAddressIndirectY(cycles, memory));                 break;

                case INS_LDX_IM:    LoadRegisterImmediate(X);                                               break;
                case INS_LDX_ZP:    LoadRegister(X, FetchAddressZeroPage(cycles, memory));                  break;
                case INS_LDX_ZPY:   LoadRegister(X, FetchAddressZeroPageY(cycles, memory));                 break;
                case INS_LDX_ABS:   LoadRegister(X, FetchAddressAbsolute(cycles, memory));                  break;
                case INS_LDX_ABSY:  LoadRegister(X, FetchAddressAbsoluteY(cycles, memory));                 break;

                case INS_LDY_IM:    LoadRegisterImmediate(Y);                                               break;
                case INS_LDY_ZP:    LoadRegister(Y, FetchAddressZeroPage(cycles, memory));                  break;
                case INS_LDY_ZPX:   LoadRegister(Y, FetchAddressZeroPageX(cycles, memory));                 break;
                case INS_LDY_ABS:   LoadRegister(Y, FetchAddressAbsolute(cycles, memory));                  break;
                case INS_LDY_ABSX:  LoadRegister(Y, FetchAddressAbsoluteX(cycles, memory));                 break;

                case INS_ORA_IM:    A = A | FetchByte(cycles, memory); LoadRegisterSetStatus(A);            break;
                case INS_ORA_ZP:    LogicalOp(FetchAddressZeroPage(cycles, memory), OpORA);                 break;
                case INS_ORA_ZPX:   LogicalOp(FetchAddressZeroPageX(cycles, memory), OpORA);                break;
                case INS_ORA_ABS:   LogicalOp(FetchAddressAbsolute(cycles, memory), OpORA);                 break;
                case INS_ORA_ABSX:  LogicalOp(FetchAddressAbsoluteX(cycles, memory), OpORA);                break;
                case INS_ORA_ABSY:  LogicalOp(FetchAddressAbsoluteY(cycles, memory), OpORA);                break;
                case INS_ORA_INDX:  LogicalOp(FetchAddressIndirectX(cycles, memory), OpORA);                break;
                case INS_ORA_INDY:  LogicalOp(FetchAddressIndirectY(cycles, memory), OpORA);                break;

                case INS_PHA:       PushByteToStack(--cycles, A, memory);                                   break;
                case INS_PHP:       PushByteToStack(--cycles, Status, memory);                              break;
                case INS_PLA:       A = PopByteFromStack(cycles, memory); LoadRegisterSetStatus(A);         break;
                case INS_PLP:       Status = PopByteFromStack(cycles, memory); LoadRegisterSetStatus(A);    break;

                case INS_RTS:       PC = PopWordFromStack(cycles, memory) + 1;                              break;

                case INS_STA_ZP:    StoreRegister(A, FetchAddressZeroPage(cycles, memory));                 break;
                case INS_STA_ZPX:   StoreRegister(A, FetchAddressZeroPageX(cycles, memory));                break;
                case INS_STA_ABS:   StoreRegister(A, FetchAddressAbsolute(cycles, memory));                 break;
                case INS_STA_ABSX:  StoreRegister(A, FetchAddressAbsoluteX(cycles, memory, true));          break;
                case INS_STA_ABSY:  StoreRegister(A, FetchAddressAbsoluteY(cycles, memory, true));          break;
                case INS_STA_INDX:  StoreRegister(A, FetchAddressIndirectX(cycles, memory));                break;
                case INS_STA_INDY:  StoreRegister(A, FetchAddressIndirectY(cycles, memory, true));          break;

                case INS_STX_ZP:    StoreRegister(X, FetchAddressZeroPage(cycles, memory));                 break;
                case INS_STX_ZPY:   StoreRegister(X, FetchAddressZeroPageY(cycles, memory));                break;
                case INS_STX_ABS:   StoreRegister(X, FetchAddressAbsolute(cycles, memory));                 break;

                case INS_STY_ZP:    StoreRegister(Y, FetchAddressZeroPage(cycles, memory));                 break;
                case INS_STY_ZPX:   StoreRegister(Y, FetchAddressZeroPageX(cycles, memory));                break;
                case INS_STY_ABS:   StoreRegister(Y, FetchAddressAbsolute(cycles, memory));                 break;

                case INS_TSA:       A = SP; --cycles; LoadRegisterSetStatus(A);                             break;
                case INS_TSX:       X = SP; --cycles; LoadRegisterSetStatus(X);                             break;

                case INS_TXS:       SP = X; --cycles;                                                       break;

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