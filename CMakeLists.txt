cmake_minimum_required(VERSION 3.28.1)

project(not_a_3d_game)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/not-a-3d-game)

option(BUILD_GUI_APP "" ON)

set(SDL_SHARED ON CACHE BOOL "" FORCE)
set(SDL_STATIC OFF CACHE BOOL "" FORCE)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)

set(COMMON_SOURCES
        src/game_logic/game_grid/game_grid.cpp
        src/core/game.cpp
        src/game_logic/player/player.cpp
        src/graphics/game_graphic.cpp
        src/graphics/debug_graphics.cpp
        src/core/input_handler/input_handler.cpp
        src/game_logic/world.cpp
        src/core/file_managers/worlds_manager.cpp
        src/core/file_managers/logs_manager.cpp
)

if(BUILD_GUI_APP AND WIN32)
    add_executable(not-a-3d-game WIN32 ${COMMON_SOURCES} src/main.cpp)
else()
    add_executable(not-a-3d-game ${COMMON_SOURCES} src/main.cpp)
endif()

target_include_directories(not-a-3d-game PRIVATE src)
target_link_libraries(not-a-3d-game PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)

add_custom_command(TARGET not-a-3d-game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:not-a-3d-game>
        $<TARGET_FILE_DIR:not-a-3d-game>
        COMMAND_EXPAND_LISTS

        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:not-a-3d-game>/resources"

        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/test_worlds"
        "$<TARGET_FILE_DIR:not-a-3d-game>/worlds"
)
