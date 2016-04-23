cmake_minimum_required(VERSION 3.4)
project(CmdParser4Cpp)

add_subdirectory(CmdParser4Cpp)
add_subdirectory(Test)
add_subdirectory(CmdExample)

# CmdExample and test both depends on CmdParser4Cpp
add_dependencies(CmdExample CmdParser4Cpp)
add_dependencies(Test CmdParser4Cpp)