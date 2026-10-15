cmake_minimum_required(VERSION 3.14)
project(Snake CXX)

file(MAKE_DIRECTORY 
include
src
test
)

file(TOUCH "include/Snake.hpp")
file(TOUCH "src/Snake.cpp")
file(TOUCH "test/SnakeTest.cpp")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(snakeProject src/main.cpp src/Snake.cpp test/SnakeTest.cpp)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

enable_testing()

include_directories(include)
add_executable(unit_tests src/Snake.cpp test/SnakeTest.cpp)
target_link_libraries(unit_tests PRIVATE gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_tests)