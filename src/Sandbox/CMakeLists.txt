set(FILES
    main.cpp
)

add_executable(Sandbox ${FILES})

target_include_directories(Sandbox PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Sandbox PRIVATE
    Emu
)