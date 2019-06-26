cmake_minimum_required(VERSION 3.4)
project(CmdParser4Cpp)
include(../externals/CMakeUtils/CMakeUtils.txt)

set(CMAKE_CXX_STANDARD 14)

if ( IS_WINDOWS )
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall /wd4514 /wd4820 /wd4710 /wd4711")
else ()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fno-exceptions")
endif ()

#Disable exceptions in pugixml
add_definitions(-DPUGIXML_NO_EXCEPTIONS)

include_directories("${CMAKE_CURRENT_LIST_DIR}/../externals/")

set(SOURCE_FILES
		src/Argument.cpp
		src/Argument.h
		src/BaseType.h
		src/BoolType.cpp
		src/BoolType.h
		src/CmdParser4Cpp.cpp
		src/TypeConstructor.cpp
		src/Constructor.cpp
		src/SingleBoolType.cpp
		src/SingleBoolType.h
		src/StringType.cpp
		src/StringType.h
		src/IntegerType.cpp
		src/IntegerType.h
		src/ITypeParser.h
		src/XMLConfigurationReader.cpp
		dist/include/CmdParser4Cpp
		dist/include/CmdParser4Cpp/TypeConstructor.h
		dist/include/CmdParser4Cpp/UnboundStringLength.h
		dist/include/CmdParser4Cpp/CmdParser4Cpp.h
		dist/include/CmdParser4Cpp/IUsageFormatter.h
		dist/include/CmdParser4Cpp/XMLConfigurationReader.h
		dist/include/CmdParser4Cpp/IParseResult.h
		dist/include/CmdParser4Cpp/Limit.h
		dist/include/CmdParser4Cpp/NumericLimit.h
		dist/include/CmdParser4Cpp/UnboundIntegerLimit.h
		dist/include/CmdParser4Cpp/Constructor.h
		dist/include/CmdParser4Cpp/StringLengthLimit.h
		dist/include/CmdParser4Cpp/IConfigurationReader.h
		../externals/pugixml/src/pugixml.cpp
		)

# Create a static library
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC "dist/include")

# Output final binaries in the dist folder so they easily can be distributed together with the header files.
set_target_properties(${PROJECT_NAME}
		PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/dist/bin/${CMAKE_BUILD_TYPE}"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/dist/bin/${CMAKE_BUILD_TYPE}"
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/dist/bin/${CMAKE_BUILD_TYPE}"
		)