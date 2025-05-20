cmake_minimum_required(VERSION 3.15)
project(SeaBattle)

# Настройка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Указываем путь к SFML вручную (если CMake не находит автоматически)
set(SFML_DIR "C:/Users/danii/source/SFML-2.6.1/lib/cmake/SFML")  # Замените на ваш путь к SFML!

# 2. Поиск SFML
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)
if(NOT SFML_FOUND)
    message(FATAL_ERROR "SFML не найден! Укажите правильный SFML_DIR.")
endif()

# 3. Исходные файлы
set(SOURCES
    main.cpp
    player.hpp
    board.hpp
)

# 4. Исполняемый файл
add_executable(SeaBattle ${SOURCES})

# 5. Подключение SFML
target_link_libraries(SeaBattle
    sfml-graphics
    sfml-window
    sfml-system
)

# 6. Копирование шрифта и DLL (Windows)
if (WIN32)
    # Шрифт
    if(EXISTS "${CMAKE_SOURCE_DIR}/ArialRegular.ttf")
        configure_file(
            ${CMAKE_SOURCE_DIR}/ArialRegular.ttf
            ${CMAKE_BINARY_DIR}/ArialRegular.ttf
            COPYONLY
        )
    else()
        message(WARNING "Файл шрифта ArialRegular.ttf не найден в корне проекта!")
    endif()

    # DLL SFML (если не в PATH)
    if(SFML_DIR)
        file(GLOB SFML_DLLS
            "${SFML_DIR}/../../bin/*.dll"  # Путь к DLL относительно SFML_DIR
        )
        file(COPY ${SFML_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
    endif()
endif()