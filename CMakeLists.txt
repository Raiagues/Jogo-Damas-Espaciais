cmake_minimum_required(VERSION 3.16)
project(SpaceCheckers VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
option(BUILD_GAME "Build the Allegro desktop game" ON)

add_library(checkers src/game.c)
target_include_directories(checkers PUBLIC src)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
    target_compile_options(checkers PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(BUILD_GAME)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(ALLEGRO REQUIRED IMPORTED_TARGET
        allegro-5>=5.2 allegro_image-5 allegro_primitives-5
        allegro_audio-5 allegro_acodec-5)
    add_executable(damas-espaciais src/main.c)
    target_link_libraries(damas-espaciais PRIVATE checkers PkgConfig::ALLEGRO)
    # Keep runtime resources beside the executable, including multi-config builds.
    add_custom_command(TARGET damas-espaciais POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/imagens" "$<TARGET_FILE_DIR:damas-espaciais>/imagens"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/sons" "$<TARGET_FILE_DIR:damas-espaciais>/sons")
endif()

include(CTest)
if(BUILD_TESTING)
    add_executable(test-game tests/test_game.c)
    target_link_libraries(test-game PRIVATE checkers)
    add_test(NAME game-rules COMMAND test-game)
endif()
