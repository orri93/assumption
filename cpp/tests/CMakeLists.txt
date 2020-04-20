set(assumption_cpp_tests_target assumptiontestcpp)

list(APPEND assumption_cpp_tests_source
  "general.cpp")
list(APPEND assumption_cpp_tests_include
  ${gos_assumption_gmock_include_dir}
  ${gos_assumption_gtest_include_dir}
  ${assumption_cpp_include})
list(APPEND assumption_cpp_tests_libraries
  ${gos_assumption_google_test_libraries})

if (GOS_ASSUMPTION_WITH_BOOST)
  list(APPEND assumption_cpp_tests_source
    "boost.cpp")
  list(APPEND assumption_cpp_tests_include
    ${gos_assumption_boost_include})
  list(APPEND assumption_cpp_tests_libraries
    ${gos_assumption_boost_libraries})
endif ()

add_executable(${assumption_cpp_tests_target}
  ${assumption_cpp_tests_source})

if (GOS_ASSUMPTION_WITH_BOOST)
  list(APPEND assumption_cpp_tests_definitions
    _GOS_ASSUMPTION_BOOST_HEADER_)
endif ()

if (GOS_ASSUMPTION_COUT)
  list(APPEND assumption_cpp_tests_definitions
    _GOS_ASSUMPTION_COUT_)
endif ()

if (GOS_ASSUMPTION_SET_CHECK_WITH_VARIABLE)
  list(APPEND assumption_cpp_tests_definitions
    _GOS_ASSUMPTION_SET_CHECK_WITH_VARIABLE_)
endif ()

if (GOS_ASSUMPTION_SET_CHECK_FROM_DEFAULT)
  list(APPEND assumption_cpp_tests_definitions
    _GOS_ASSUMPTION_SET_CHECK_FROM_DEFAULT_)
endif ()

if (GOS_ASSUMPTION_COMPARE_WITH_FRIEND)
  list(APPEND assumption_cpp_tests_definitions
    _GOS_ASSUMPTION_COMPARE_WITH_FRIEND_)
endif ()

if (GOS_ASSUMPTION_BOOST_STATE_MACHINE)
  list(APPEND assumption_cpp_tests_definitions
    _GOS_ASSUMPTION_BOOST_STATE_MACHINE_
    BOOST_MPL_LIMIT_VECTOR_SIZE=32)
endif ()

if (assumption_cpp_tests_definitions)
  target_compile_definitions(${assumption_cpp_tests_target} PUBLIC
    ${assumption_cpp_tests_definitions})
endif ()

target_include_directories(${assumption_cpp_tests_target} PUBLIC
  ${assumption_cpp_tests_include})

if (GOS_ASSUMPTION_CXX_14_STANDARD)
  set_target_properties(${assumption_cpp_tests_target} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)
else ()
  set_target_properties(${assumption_cpp_tests_target} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)
endif ()

target_link_libraries(${assumption_cpp_tests_target}
  ${assumption_cpp_tests_libraries})

add_test(NAME assumption_cpp_unit_test COMMAND ${assumption_cpp_tests_target})
