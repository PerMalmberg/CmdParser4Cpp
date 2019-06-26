cmake_minimum_required(VERSION 3.4)
project(Test)

set(EXTERNALS ${CMAKE_CURRENT_LIST_DIR}/../externals)

include(${EXTERNALS}/CMakeUtils/CMakeUtils.txt)

if( IS_WINDOWS )
	# Build at warning level 4 on Windows.
	# Treat all warnings as errors
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX /wd4711 /wd4702")
else()
	# Build with all warnings
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++11")
endif()

set(SOURCE_FILES
		main.cpp
		Tests.cpp
		SystemOutputParseResult.h
		SystemOutputParseResult.cpp
		SystemOutputUsageFormatter.h
		SystemOutputUsageFormatter.cpp
		)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} CmdParser4Cpp)

set(EXTERNALS ${CMAKE_CURRENT_LIST_DIR}/../externals)
target_include_directories(${PROJECT_NAME} PRIVATE
											${EXTERNALS}/Catch2/single_include
											${EXTERNALS})

# Output final binaries in the dist folder so they easily can be distributed together with the header files.
set_target_properties(${PROJECT_NAME}
		PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/dist/bin/${CMAKE_BUILD_TYPE}"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/dist/bin/${CMAKE_BUILD_TYPE}"
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/dist/bin/${CMAKE_BUILD_TYPE}"
		)