cmake_minimum_required(VERSION 3.4)
project(Test)

if(${CMAKE_MAKE_PROGRAM} MATCHES ".*(MSBuild|msdev|devenv|nmake).*")
	# Build at warning level 4 on Windows.
	# Treat all warnings as errors
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX /wd4711 /wd4702")
else()
	# Build with all warnings
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++11 -static-libgcc -static-libstdc++ -static -lpthread")
endif()

# Use CMAKE_CURRENT_LIST_DIR instead of CMAKE_SOURCE_DIR to specify relative to where this CMakeLists.txt is located
# as opposed to where the 'main' CMakeLists.txt file is located.
include_directories("${CMAKE_CURRENT_LIST_DIR}/../CmdParser4Cpp/dist/include")

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