cmake_minimum_required(VERSION 3.4)
project(CmdParser4Cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

include_directories("dist/include")

set(SOURCE_FILES
		src/Argument.cpp
		src/Argument.h
		src/BaseType.cpp
		src/BaseType.h
		src/BoolType.cpp
		src/BoolType.h
		src/CmdParser4Cpp.cpp
		src/Constructor.cpp
		src/SingleBoolType.cpp
		src/SingleBoolType.h
		src/StringType.cpp
		src/StringType.h)

# Create a static library
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

set_target_properties(${PROJECT_NAME}
		PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/dist/bin"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SORUCE_DIR}/dist/bin"
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SORUCE_DIR}/dist/bin"
		)