cmake_minimum_required(VERSION 3.10)
project(lab5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=maybe-uninitialized")

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(include)

add_library(${CMAKE_PROJECT_NAME}_lib include/memory_resource.cpp)
target_include_directories(${CMAKE_PROJECT_NAME}_lib PUBLIC include)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE ${CMAKE_PROJECT_NAME}_lib)

enable_testing()
add_executable(tests tests/tests.cpp)
target_link_libraries(tests ${CMAKE_PROJECT_NAME}_lib gtest_main)
add_test(NAME MyProjectTests COMMAND tests)