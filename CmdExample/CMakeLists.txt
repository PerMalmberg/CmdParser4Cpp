cmake_minimum_required(VERSION 3.4)
project(CmdExample)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -static-libgcc -static-libstdc++ -static -lpthread")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

include_directories("${CMAKE_CURRENT_LIST_DIR}/../CmdParser4Cpp/dist/include")

set(libCmdParser4Cpp "${CMAKE_CURRENT_LIST_DIR}/../CmdParser4Cpp/dist/bin/libCmdParser4Cpp.a")

set(SOURCE_FILES
		CmdExample.cpp
		DummyResult.cpp
		DummyResult.h
		DummyUsage.cpp
		DummyUsage.h
		stdafx.cpp
		stdafx.h
		targetver.h)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${libCmdParser4Cpp})

set_target_properties(${PROJECT_NAME}
		PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/dist/bin"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/dist/bin"
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/dist/bin"
		)