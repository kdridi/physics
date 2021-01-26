cmake_minimum_required(VERSION 2.6)

function(echo_all_cmake_variable_values)
  message(STATUS “”)
  get_cmake_property(vs VARIABLES)
  foreach(v ${vs})
    message(STATUS “${v}=’${${v}}'”)
  endforeach(v)
  message(STATUS “”)
endfunction()

project(GGJ2021)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIB64 "lib64")
if("${CMAKE_SYSTEM_NAME}" MATCHES "Darwin")
  set(LIB64 "lib")
endif()
echo_all_cmake_variable_values()

set(box2d_DIR "${CMAKE_SOURCE_DIR}/system/${CMAKE_SYSTEM_NAME}/${LIB64}/cmake/box2d")
find_package(box2d REQUIRED)

set(ixwebsocket_DIR "${CMAKE_SOURCE_DIR}/system/${CMAKE_SYSTEM_NAME}/lib/cmake/ixwebsocket")
find_package(ixwebsocket REQUIRED)


set(EXECUTABLE_NAME "GGJ2021")

add_executable(${EXECUTABLE_NAME}
src/file_utils.cpp
src/file_utils.hpp
src/main.cpp
)

target_link_libraries(${EXECUTABLE_NAME}
  box2d::box2d
  ixwebsocket::ixwebsocket
)
