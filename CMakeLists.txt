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

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -g3")
set(LIB64 "lib64")
if("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -fsanitize=address -fsanitize=leak")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

set(LIB64 "lib")
if("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
  set(LIB64 "lib64")
endif()
echo_all_cmake_variable_values()

set(box2d_DIR "${CMAKE_SOURCE_DIR}/system/${CMAKE_SYSTEM_NAME}/${LIB64}/cmake/box2d")
find_package(box2d REQUIRED)

set(ixwebsocket_DIR "${CMAKE_SOURCE_DIR}/system/${CMAKE_SYSTEM_NAME}/lib/cmake/ixwebsocket")
find_package(ixwebsocket REQUIRED)

set(protobuf_DIR "${CMAKE_SOURCE_DIR}/system/${CMAKE_SYSTEM_NAME}/${LIB64}/cmake/protobuf")
find_package(protobuf CONFIG REQUIRED)

set(EXECUTABLE_NAME "GGJ2021")

add_executable(${EXECUTABLE_NAME}
  src/proto/box2d.pb.h
  src/proto/box2d.pb.cc
  src/FileUtils.cpp
  src/FileUtils.hpp
  src/WebSocketClientManager.cpp
  src/WebSocketClientManager.hpp
  src/WebSocketSerializer.cpp
  src/WebSocketSerializer.hpp
  src/WebHTTPServer.cpp
  src/WebHTTPServer.hpp
  src/WebSocketServer.cpp
  src/WebSocketServer.hpp
  src/GameManager.cpp
  src/GameManager.hpp
  src/GameObject.hpp
  src/GOPlayer.cpp
  src/GOPlayer.hpp
  src/WebSocketClient.cpp
  src/WebSocketClient.hpp
  src/GOBall.cpp
  src/GOBall.hpp
  src/GOGoal.cpp
  src/GOGoal.hpp
  src/main.cpp
)

target_link_libraries(${EXECUTABLE_NAME}
  box2d::box2d
  ixwebsocket::ixwebsocket
  protobuf::libprotobuf
)
