cmake_minimum_required(VERSION 3.4)
project(CmdParser4CppTest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

include_directories("${CMAKE_SOURCE_DIR}/../CmdParser4Cpp/dist/include")

set(libCmdParser4Cpp "${CMAKE_SOURCE_DIR}/../CmdParser4Cpp/dist/bin/libCmdParser4Cpp.a")

set(SOURCE_FILES
    ParserTest.cpp
    stdafx.cpp
    stdafx.h
    SystemOutputParseResult.cpp
    SystemOutputParseResult.h
    SystemOutputUsageFormatter.cpp
    SystemOutputUsageFormatter.h
    targetver.h)

add_executable(CmdParser4CppTest ${SOURCE_FILES})