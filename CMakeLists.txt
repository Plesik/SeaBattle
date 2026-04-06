cmake_minimum_required(VERSION 3.20)

project(SeaBattle VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(SEABATTLE_FETCH_SFML "Download SFML automatically if it is not installed" ON)

set(SEABATTLE_SOURCES
    src/main.cpp
)


set(SEABATTLE_HEADERS
    include/button.hpp
    include/player.hpp
    include/board.hpp
    include/game.hpp
    include/rules.hpp
)

find_package(SFML 2.6 QUIET COMPONENTS graphics window system)

if(NOT SFML_FOUND AND SEABATTLE_FETCH_SFML)
    include(FetchContent)

    message(STATUS "SFML was not found. Fetching SFML 2.6.2 from GitHub...")

    set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(
        sfml
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.2
        GIT_SHALLOW ON
    )

    FetchContent_MakeAvailable(sfml)
endif()

add_executable(SeaBattle ${SEABATTLE_SOURCES} ${SEABATTLE_HEADERS})
target_include_directories(SeaBattle PRIVATE include)

target_link_libraries(SeaBattle PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)

set(SEABATTLE_FONT_SOURCE "${CMAKE_SOURCE_DIR}/assets/ArialRegular.ttf")
set(SEABATTLE_FONT_TARGET "${CMAKE_CURRENT_BINARY_DIR}/assets/ArialRegular.ttf")

if(EXISTS "${SEABATTLE_FONT_SOURCE}")
    configure_file("${SEABATTLE_FONT_SOURCE}" "${SEABATTLE_FONT_TARGET}" COPYONLY)
else()
    message(WARNING "ArialRegular.ttf was not found in the project root. The application may fail to start without the font file.")
endif()

if(MSVC)
    target_compile_options(SeaBattle PRIVATE /W4 /permissive-)
else()
    target_compile_options(SeaBattle PRIVATE -Wall -Wextra -Wpedantic)
endif()
