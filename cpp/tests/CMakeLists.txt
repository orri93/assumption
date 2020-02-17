cmake_minimum_required(VERSION 3.8.2)

project(AssumptionCppTests
  VERSION 1.0
  DESCRIPTION "Assumption C++ Test Project"
  LANGUAGES CXX)

option(BUILD_BOOST_TEST
  "Build boost dependent tests" ON)
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

list(APPEND assumptioncpptests_source "general.cpp")
list(APPEND assumptioncpptests_libraries
  gtest
  gmock_main)

if (BUILD_BOOST_TEST)
  #find_package(Boost 1.71 COMPONENTS headers)
  # Find Boost headers only
  find_package(Boost)
  if (Boost_FOUND)
    message(STATUS "Boost headers where found so adding boost tests.")
    list(APPEND assumption_cpp_include ${Boost_INCLUDE_DIRS})
    list(APPEND assumptioncpptests_source "boost.cpp")
  else ()
    message(WARNING "Boost needs to be installed to build the boost tests.")
  endif ()
  find_package(Boost OPTIONAL_COMPONENTS
    date_time
    system
    regex)
  if (Boost_SYSTEM_FOUND)
    list(APPEND assumptioncpptests_libraries
      Boost::date_time
      Boost::regex)
    message(STATUS "Boost system where found so adding boost tests.")    
  else ()
    message(WARNING "Boost system needs to be installed to build the boost tests.")
  endif ()
endif ()

add_executable(assumptioncpptests ${assumptioncpptests_source})

if (Boost_FOUND)
  target_compile_definitions(assumptioncpptests PUBLIC 
    _GOS_ASSUMPTION_BOOST_HEADER_)
endif ()

if (Boost_SYSTEM_FOUND)
  target_compile_definitions(assumptioncpptests PUBLIC 
    _GOS_ASSUMPTION_BOOST_SYSTEM_)
endif ()

if (GOS_ASSUMPTION_COUT)
  target_compile_definitions(assumptioncpptests PUBLIC
    _GOS_ASSUMPTION_COUT_)
endif ()

if (GOS_ASSUMPTION_SET_CHECK_WITH_VARIABLE)
  target_compile_definitions(assumptioncpptests PUBLIC 
    _GOS_ASSUMPTION_SET_CHECK_WITH_VARIABLE_)
endif ()

if (GOS_ASSUMPTION_SET_CHECK_FROM_DEFAULT)
  target_compile_definitions(assumptioncpptests PUBLIC 
    _GOS_ASSUMPTION_SET_CHECK_FROM_DEFAULT_)
endif ()

if (GOS_ASSUMPTION_COMPARE_WITH_FRIEND)
  target_compile_definitions(assumptioncpptests PUBLIC 
    _GOS_ASSUMPTION_COMPARE_WITH_FRIEND_)
endif ()

if (GOS_ASSUMPTION_BOOST_STATE_MACHINE)
  target_compile_definitions(assumptioncpptests PUBLIC 
    _GOS_ASSUMPTION_BOOST_STATE_MACHINE_)
  target_compile_definitions(assumptioncpptests PUBLIC
    BOOST_MPL_LIMIT_VECTOR_SIZE=32)
endif ()

target_include_directories(assumptioncpptests PUBLIC ${assumption_cpp_include})

if (GOS_ASSUMPTION_CXX_14_STANDARD)
  set_target_properties(assumptioncpptests PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)
else ()
  set_target_properties(assumptioncpptests PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)
endif ()

target_link_libraries(assumptioncpptests ${assumptioncpptests_libraries})

add_test(NAME AssumptionCppTest COMMAND assumptioncpptests)
