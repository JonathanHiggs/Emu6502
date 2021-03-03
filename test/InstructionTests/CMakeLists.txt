set(FILES
    Emu/UnitTests/CPUFixture.hpp
    Emu/UnitTests/CPUTests.cpp
    Emu/UnitTests/LDA_INDX_Tests.cpp
    Emu/UnitTests/LDA_INDY_Tests.cpp
    Emu/UnitTests/LoadRegisterTests.cpp
    Emu/UnitTests/main.cpp
)

add_executable(InstructionTests ${FILES})

target_include_directories(InstructionTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(InstructionTests PRIVATE
    Emu
    GTest::gtest
    GTest::gtest_main
)

#gtest_discover_tests(Emu_UnitTests)

add_test(NAME InstructionTests COMMAND InstructionTests)