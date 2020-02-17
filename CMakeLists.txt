cmake_minimum_required(VERSION 3.8.2)

project(Assumption VERSION 1.0 DESCRIPTION "Assumption Project" LANGUAGES C CXX)

# Doxygen Build
option(BUILD_DOC "Build Documentation" ON)
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
option(BUILD_TESTS "Build tests" ON)

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

if (BUILD_TESTS)

  if (MSVC)
    #if (NOV_FORCE_SHARED_CRT)
      set(gtest_force_shared_crt ON CACHE BOOL "Use Shared CRT")
    #endif ()
  endif (MSVC)

  mark_as_advanced(
    BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
    gmock_build_tests gtest_build_samples gtest_build_tests
    gtest_disable_pthreads gtest_hide_internal_symbols)

  add_subdirectory(extern/googletest)

endif ()

add_subdirectory(cpp)
