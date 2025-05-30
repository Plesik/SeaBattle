cmake_minimum_required(VERSION 3.15)
project(SeaBattle)

# ��������� ��������� C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. ��������� ���� � SFML ������� (���� CMake �� ������� �������������)
set(SFML_DIR "C:/Users/danii/source/SFML-2.6.1/lib/cmake/SFML")  # �������� �� ��� ���� � SFML!

# 2. ����� SFML
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)
if(NOT SFML_FOUND)
    message(FATAL_ERROR "SFML �� ������! ������� ���������� SFML_DIR.")
endif()

# 3. �������� �����
set(SOURCES
    main.cpp
    button.hpp
    player.hpp
    board.hpp
    game.hpp
    rules.hpp
)

# 4. ����������� ����
add_executable(SeaBattle ${SOURCES})

# 5. ����������� SFML
target_link_libraries(SeaBattle
    sfml-graphics
    sfml-window
    sfml-system
)

# 6. ����������� ������ � DLL (Windows)
if (WIN32)
    # �����
    if(EXISTS "${CMAKE_SOURCE_DIR}/ArialRegular.ttf")
        configure_file(
            ${CMAKE_SOURCE_DIR}/ArialRegular.ttf
            ${CMAKE_BINARY_DIR}/ArialRegular.ttf
            COPYONLY
        )
    else()
        message(WARNING "���� ������ ArialRegular.ttf �� ������ � ����� �������!")
    endif()

    # DLL SFML (���� �� � PATH)
    if(SFML_DIR)
        file(GLOB SFML_DLLS
            "${SFML_DIR}/../../bin/*.dll"  # ���� � DLL ������������ SFML_DIR
        )
        file(COPY ${SFML_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
    endif()
endif()