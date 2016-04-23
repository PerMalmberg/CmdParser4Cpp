cmake_minimum_required(VERSION 3.4)
project(Test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -static-libgcc -static-libstdc++ -static -lpthread")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")


include_directories("${CMAKE_CURRENT_LIST_DIR}/../CmdParser4Cpp/dist/include")

set(libCmdParser4Cpp "${CMAKE_CURRENT_LIST_DIR}/../CmdParser4Cpp/dist/bin/libCmdParser4Cpp.a")


set(SOURCE_FILES
		main.cpp
		Tests.cpp
		SystemOutputParseResult.h
		SystemOutputParseResult.cpp
		SystemOutputUsageFormatter.h
		SystemOutputUsageFormatter.cpp
		)


add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${libCmdParser4Cpp})

set_target_properties(${PROJECT_NAME}
		PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/dist/bin"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/dist/bin"
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/dist/bin"
		)