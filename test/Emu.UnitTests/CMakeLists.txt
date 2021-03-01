set(FILES
    Emu.UnitTests/CPUFixture.hpp
    Emu.UnitTests/CPUTests.cpp
    Emu.UnitTests/LDA_ABS_Tests.cpp
    Emu.UnitTests/LDA_ABSX_Tests.cpp
    Emu.UnitTests/LDA_ABSY_Tests.cpp
    Emu.UnitTests/LDA_INDX_Tests.cpp
    Emu.UnitTests/LDA_INDY_Tests.cpp
    Emu.UnitTests/LDA_ZPX_Tests.cpp
    Emu.UnitTests/LDATests.cpp
    Emu.UnitTests/LDUtils.cpp
    Emu.UnitTests/LDUtils.hpp
    Emu.UnitTests/LDXTests.cpp
    Emu.UnitTests/LDYTests.cpp
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