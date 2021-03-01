#pragma once


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <limits>

#include <fmt/format.h>


namespace Emu
{

    typedef uint8_t     uint8;
    typedef uint16_t    uint16;
    typedef uint32_t    uint32;
    typedef uint64_t    uint64;

    typedef int8_t      int8;
    typedef int16_t     int16;
    typedef int32_t     int32;
    typedef int64_t     int64;

    typedef bool        bool1;
    typedef int32_t     bool32;

    typedef float       real32;
    typedef double      real64;

    typedef char        char8;
    typedef char16_t    char16;
    typedef char32_t    char32;


    using Byte = uint8;
    using Word = uint16;

}