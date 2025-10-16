cmake_minimum_required(VERSION 3.10)
project(lab2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=maybe-uninitialized")

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.0
  TLS_VERIFY false
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(seven_lib src/seven.cpp)
add_library(vector_lib src/vector.cpp)

target_include_directories(seven_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(vector_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_executable(main main.cpp)
target_link_libraries(main seven_lib vector_lib)


enable_testing()
add_executable(tests tests/tests.cpp)
target_link_libraries(tests seven_lib vector_lib gtest_main)
add_test(NAME tests COMMAND tests)