set(FILES
    Emu.UnitTests/CPUTests.cpp
    Emu.UnitTests/main.cpp
)

add_executable(Emu_UnitTests ${FILES})

target_include_directories(Emu_UnitTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Emu_UnitTests PRIVATE
    Emu
    GTest::gtest
    GTest::gtest_main
)

#gtest_discover_tests(Emu_UnitTests)

add_test(NAME Emu_UnitTests COMMAND Emu_UnitTests)