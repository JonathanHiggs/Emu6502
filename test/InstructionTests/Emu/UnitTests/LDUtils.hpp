#include <Emu/CPU.hpp>
#include <Emu/Memory.hpp>


namespace Emu::UnitTests
{

    void TestLoadRegisterImmediate(
        CPU & cpu,
        Memory & memory,
        Byte opCode,
        Byte CPU::*reg);


    void TestLoadRegisterImmediate_WithNegativeValue(
        CPU & cpu,
        Memory & memory,
        Byte opCode,
        Byte CPU::*reg);


    void TestLoadRegisterImmediate_WithZeroValue(
        CPU & cpu,
        Memory & memory,
        Byte opCode,
        Byte CPU::*reg);


    void TestLoadRegisterZeroPage(
        CPU & cpu,
        Memory & memory,
        Byte opCode,
        Byte CPU::*reg);


    void TestLoadRegisterZeroPage_WithNegativeValue(
        CPU & cpu,
        Memory & memory,
        Byte opCode,
        Byte CPU::*reg);


    void TestLoadRegisterZeroPage_WithZeroValue(
        CPU & cpu,
        Memory & memory,
        Byte opCode,
        Byte CPU::*reg);


    void TestLoadRegisterZeroPageOffset(
        CPU & cpu,
        Memory & memory,
        Byte opCode,
        Byte CPU::* offsetRegister,
        Byte CPU::* resultRegister);


    void TestLoadRegisterZeroPageOffset_WithNegativeValue(
        CPU & cpu,
        Memory & memory,
        Byte opCode,
        Byte CPU::* offsetRegister,
        Byte CPU::* resultRegister);


    void TestLoadRegisterZeroPageOffset_WithZeroValue(
        CPU & cpu,
        Memory & memory,
        Byte opCode,
        Byte CPU::* offsetRegister,
        Byte CPU::* resultRegister);


    void TestLoadRegisterZeroPageOffset_WithWrappedAddress(
        CPU & cpu,
        Memory & memory,
        Byte opCode,
        Byte CPU::* offsetRegister,
        Byte CPU::* resultRegister);

}