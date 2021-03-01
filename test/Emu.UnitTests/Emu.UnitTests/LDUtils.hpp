#include <Emu/CPU.hpp>
#include <Emu/Memory.hpp>


void TestLoadRegisterImmediate(
    Emu::CPU & cpu,
    Emu::Memory & memory,
    Emu::Byte opCode,
    Emu::Byte Emu::CPU::*reg);


void TestLoadRegisterImmediate_WithNegativeValue(
    Emu::CPU & cpu,
    Emu::Memory & memory,
    Emu::Byte opCode,
    Emu::Byte Emu::CPU::*reg);


void TestLoadRegisterImmediate_WithZeroValue(
    Emu::CPU & cpu,
    Emu::Memory & memory,
    Emu::Byte opCode,
    Emu::Byte Emu::CPU::*reg);


void TestLoadRegisterZeroPage(
    Emu::CPU & cpu,
    Emu::Memory & memory,
    Emu::Byte opCode,
    Emu::Byte Emu::CPU::*reg);


void TestLoadRegisterZeroPage_WithNegativeValue(
    Emu::CPU & cpu,
    Emu::Memory & memory,
    Emu::Byte opCode,
    Emu::Byte Emu::CPU::*reg);


void TestLoadRegisterZeroPage_WithZeroValue(
    Emu::CPU & cpu,
    Emu::Memory & memory,
    Emu::Byte opCode,
    Emu::Byte Emu::CPU::*reg);