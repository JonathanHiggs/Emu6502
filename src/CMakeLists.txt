set(FILES
    Emu/CPU.hpp
    Emu/includes.hpp
    Emu/main.cpp
    Emu/Memory.hpp
)

add_executable(Emu ${FILES})

target_include_directories(Emu PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Emu PRIVATE
    fmt::fmt
)