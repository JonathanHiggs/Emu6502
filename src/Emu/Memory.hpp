#pragma once

#include <Emu/includes.hpp>


namespace Emu
{

    struct Memory
    {
        static constexpr uint32 MAX_MEMORY = 1024 * 64;
        Byte Data[MAX_MEMORY];

        void Initialize()
        {
            memset(&Data, 0, MAX_MEMORY);
        }

        Byte ReadByte(uint32 address) const
        {
            return Data[address];
        }

        void WriteByte(uint32 address, Byte value)
        {
            Data[address] = value;
        }

        Word ReadWord(uint32 address) const
        {
            Word word = Data[address];
            word |= (((Word)Data[address + 1]) << 8);
            return word;
        }

        void WriteWord(uint32 address, Word value)
        {
            Data[address] = value & 0xFF;
            Data[address + 1] = value >> 8;
        }
    };

}