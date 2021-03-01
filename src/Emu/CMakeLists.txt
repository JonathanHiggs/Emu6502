set(FILES
    Emu/CPU.hpp
    Emu/Emu.cpp
    Emu/includes.hpp
    Emu/Memory.hpp
)

add_library(Emu STATIC ${FILES})

target_include_directories(Emu PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Emu PUBLIC
    fmt::fmt
)