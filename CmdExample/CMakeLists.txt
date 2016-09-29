cmake_minimum_required(VERSION 3.4)
project(CmdExample)

include(../externals/CMakeUtils/CMakeUtils.txt)

set(CMAKE_CXX_STANDARD 14)

if( IS_WINDOWS )
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall /wd4514 /wd4820 /wd4710 /wd4711")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -lpthread")
endif()

include_directories("${CMAKE_CURRENT_LIST_DIR}/../CmdParser4Cpp/dist/include")

set(SOURCE_FILES
		CmdExample.cpp
		SystemOutputParseResult.cpp
		SystemOutputParseResult.h
		SystemOutputUsageFormatter.cpp
		SystemOutputUsageFormatter.h
		)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} CmdParser4Cpp)

set_target_properties(${PROJECT_NAME}
		PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/dist/bin"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/dist/bin"
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/dist/bin"
		)