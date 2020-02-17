cmake_minimum_required(VERSION 3.8.2)

project(AssumptionCpp
  VERSION 1.0
  DESCRIPTION "Assumption C++ Project"
  LANGUAGES CXX)

option(BUILD_TESTS "Build tests" ON)

list(APPEND assumption_cpp_include "${CMAKE_CURRENT_SOURCE_DIR}/include")

if (BUILD_TESTS)
  add_subdirectory(tests)
endif ()