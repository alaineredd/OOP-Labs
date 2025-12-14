cmake_minimum_required(VERSION 3.14)
project(dungeon)

set(CMAKE_CXX_STANDARD 17)

# Автозагрузка GoogleTest с подавлением предупреждения
cmake_policy(SET CMP0135 NEW)
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

# Проверяем существование файлов
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/observer.cpp")
    message(FATAL_ERROR "File not found: src/observer.cpp")
endif()

# Основная программа
add_executable(main
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/npc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/npc_factory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/visitor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/observer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dungeon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/game.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/os.cpp
)
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Тесты
add_executable(tests
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/npc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/npc_factory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/visitor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/observer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dungeon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/os.cpp
)
target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Связываем с библиотеками
find_package(Threads REQUIRED)
target_link_libraries(main PRIVATE Threads::Threads)
target_link_libraries(tests PRIVATE Threads::Threads gtest gtest_main)