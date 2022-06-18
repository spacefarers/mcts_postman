cmake_minimum_required(VERSION 3.16)
project(wfc)

set(CMAKE_CXX_STANDARD 20)

add_executable(mcts newPostman.cpp)
add_executable(ua wfcPostman.cpp)
add_executable(check checkPath.cpp)
add_executable(graph makeGraph.cpp)
add_executable(rand randomPostman.cpp)