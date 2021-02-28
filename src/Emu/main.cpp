// https://www.youtube.com/watch?v=qJgsuQoy9bc&ab_channel=DavePoo

#include <Emu/CPU.hpp>
#include <Emu/Memory.hpp>


int main()
{
    Emu::Memory memory;
    Emu::CPU cpu;

    cpu.Reset(memory);

    memory.WriteByte(0xFFFC, Emu::CPU::INS_JSR);
    memory.WriteWord(0xFFFD, 0x4243);
    memory.WriteByte(0x4243, Emu::CPU::INS_LDA_IM);
    memory.WriteByte(0x4244, 0x84);

    cpu.Execute(9, memory);

    cpu.DumpState();

    return 0;
}