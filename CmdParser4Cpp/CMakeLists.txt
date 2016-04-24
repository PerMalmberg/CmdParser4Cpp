cmake_minimum_required(VERSION 3.4)
project(CmdParser4Cpp)

if(${CMAKE_MAKE_PROGRAM} MATCHES ".*(MSBuild|msdev|devenv|nmake).*")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall /wd4514 /wd4820 /wd4710 /wd4711")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -fno-exceptions")
endif()

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

# Output final binaries in the dist folder so they easilly can be distributed together with the header files.
set_target_properties(${PROJECT_NAME}
		PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/dist/bin/${CMAKE_BUILD_TYPE}"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/dist/bin/${CMAKE_BUILD_TYPE}"
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/dist/bin/${CMAKE_BUILD_TYPE}"
		)