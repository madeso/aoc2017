cmake_minimum_required(VERSION 3.1.0)
project (aoc)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

macro(day name)
  add_executable(${name} ${name}.cc)
  add_executable(${name}-test ${name}.cc)
  target_compile_definitions(${name}-test PRIVATE UNITTEST)
  add_custom_command(
    TARGET ${name}-test
    COMMENT "Running tests for ${name}"
    POST_BUILD 
    COMMAND ${name}-test
  )
  ADD_CUSTOM_COMMAND(
    TARGET ${name}
    COMMENT "Running ${name}"
    POST_BUILD
    COMMAND ${name}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endmacro(day)

day(day1)
day(day2)
day(day3)
day(day4)
