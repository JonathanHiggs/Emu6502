cmake_minimum_required(VERSION 3.16)

project(Emu6502)

SET(CMAKE_GENERATOR_PLATFORM x64)
SET(CMAKE_CXX_STANDARD 17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS "/EHsc /WX /W4 /wd4005 /wd4100 /wd4189 /wd4458")
set(CMAKE_CXX_FLAGS_DEBUG "/Od /Oi /Zi /JMC")

find_package(fmt CONFIG REQUIRED)
find_package(GTest REQUIRED)
include(GoogleTest)

enable_testing()

add_subdirectory(src)
add_subdirectory(test)