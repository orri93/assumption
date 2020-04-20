cmake_minimum_required(VERSION 3.8.2)

project(Assumption VERSION 1.0 DESCRIPTION "Assumption Project" LANGUAGES C CXX)

# Doxygen Build
option(BUILD_DOC "Build Documentation" ON)

option(GOS_ASSUMPTION_WITH_BOOST
  "Build assumption that depend on boost" ON)
option(GOS_ASSUMPTION_COUT
  "Assumption print to out" OFF)
option(GOS_ASSUMPTION_SET_CHECK_WITH_VARIABLE
  "Assumption set check with variable" ON)
option(GOS_ASSUMPTION_SET_CHECK_FROM_DEFAULT
  "Assumption set check from default" OFF)
option(GOS_ASSUMPTION_COMPARE_WITH_FRIEND
  "Assumption compare with friend" ON)
option(GOS_ASSUMPTION_CXX_14_STANDARD
  "Assumption C++ 14 standard" OFF)

option(GOS_ASSUMPTION_BOOST_STATE_MACHINE
  "Assumption Boost State Machine" OFF)


if (BUILD_DOC)
  find_package(Doxygen)
  if(DOXYGEN_FOUND)
    set(BUILD_DOC_DIR ${CMAKE_SOURCE_DIR}/build/docs)
    if(NOT EXISTS ${BUILD_DOC_DIR})
      file(MAKE_DIRECTORY ${BUILD_DOC_DIR})
    endif()

    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    message(STATUS "Doxygen build started")
    add_custom_target(Doxygen ALL
      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  #    COMMAND "Generating API documentation with Doxygen"
  #    VERBATIM
    )
  else ()
    message(WARNING
      "Doxygen needs to be installed to generate the documentation.")
  endif ()
endif ()

if (GOS_ASSUMPTION_WITH_BOOST)
  set(Boost_USE_MULTITHREADED        ON)
  set(Boost_USE_STATIC_RUNTIME      OFF)
  if (BUILD_SHARED_LIBS)
    set(Boost_USE_STATIC_LIBS       OFF)    
  else ()
    set(Boost_USE_STATIC_LIBS        ON)
  endif ()
  list(APPEND gos_assumption_boost_package
    regex
    system
    date_time)
  list(REMOVE_DUPLICATES gos_assumption_boost_package)
  if (gos_assumption_boost_package)
    find_package(Boost 1.71 COMPONENTS REQUIRED
      ${gos_assumption_boost_package})
  else ()
    find_package(Boost 1.71)
  endif ()
  set(gos_assumption_boost_include ${Boost_INCLUDE_DIRS})
  list(APPEND gos_assumption_boost_libraries
    Boost::date_time
    Boost::regex)
endif ()

if (MSVC)
  #if (NOV_FORCE_SHARED_CRT)
    set(gtest_force_shared_crt ON CACHE BOOL "Use Shared CRT")
  #endif ()
endif (MSVC)

add_subdirectory(extern/googletest)

set(gos_assumption_gmock_include_dir
  ${CMAKE_CURRENT_SOURCE_DIR}/extern/googletest/googlemock/include)
set(gos_assumption_gtest_include_dir
  ${CMAKE_CURRENT_SOURCE_DIR}/extern/googletest/googletest/include)
list(APPEND gos_assumption_google_test_libraries
  gmock_main
  gmock
  gtest)

enable_testing()

add_subdirectory(cpp)
