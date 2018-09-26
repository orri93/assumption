project(${CMAKE_PROJECT_NAME}_tests)

# Clone Google Test

set(GOOGLETEST_DIR /opt/googletest/lib)
set(GOOGLETEST_INCLUDE /opt/googletest/include)

add_subdirectory(${GOOGLETEST_INCLUDE})

include_directories(${GOOGLETEST_INCLUDE}

add_executable(Unit_Test_run
  test.cpp
)

target_link_libraries(Unit_Tests_run gtest gtest_main)
#target_link_libraries(Unit_Tests_run ${CMAKE_PROJECT_NAME}_lib)

