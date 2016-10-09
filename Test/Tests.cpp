// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#include <CmdParser4Cpp/CmdParser4Cpp.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <Catch/include/catch.hpp>
#include <CmdParser4Cpp/XMLConfigurationReader.h>
#include <CmdParser4Cpp/UnboundStringLength.h>
#include "SystemOutputParseResult.h"
#include "SystemOutputUsageFormatter.h"

using namespace std;


TEST_CASE( "Basic parsing" )
{
	GIVEN( "A parser" )
	{
		SystemOutputParseResult msg;
		CmdParser4Cpp p( msg );

		WHEN( "it accepts a single argument with two parameters" )
		{
			p.Accept( "-m" ).AsString( 2 );
			p.Accept( "-b" ).AsBoolean( 1 );

			THEN( "it it accepts two parameters" )
			{
				bool res = p.Parse( std::vector<std::string>( {"-m", "", "one", "two", "-b", "1"} ) );

				REQUIRE( res );
				REQUIRE( 2 == p.GetAvailableStringParameterCount( "-m" ) );
				REQUIRE( 1 == p.GetAvailableBooleanParameterCount( "-b" ) );
				REQUIRE( string( "one" ) == p.GetString( "-m" ) );
				REQUIRE( string( "two" ) == p.GetString( "-m", 1 ) );
				REQUIRE( p.GetBool( "-b" ) );
			}
			AND_WHEN( "asked for unknown argument it returns null" )
			{
				REQUIRE( nullptr == p.GetString( "-multi", 2 ) );
			}
		}
	}
}

TEST_CASE( "Missing parameter parsing" )
{
	GIVEN( "A parser" )
	{
		SystemOutputParseResult msg;
		CmdParser4Cpp p( msg );

		WHEN( "it accepts a single argument with two parameters" )
		{
			p.Accept( "-m" ).AsString( 2 );

			THEN( "it it requires two parameters" )
			{
				REQUIRE_FALSE( p.Parse( std::vector<std::string>( {"-m", "", "one"} ) ) );
			}
			AND_THEN( "is succeeds with two parameters" )
			{
				REQUIRE( p.Parse( std::vector<std::string>( {"-m", "", "one", "two"} ) ) );
			}
		}
	}
}

SCENARIO( "Argument specified multiple times" )
{
	GIVEN( "A parser" )
	{
		SystemOutputParseResult msg;
		CmdParser4Cpp p( msg );

		WHEN( "setup with alias" )
		{
			p.Accept( "-q" ).AsString( 1 ).WithAlias( "-Q" );

			THEN( "called with both primary and alias names" )
			{
				REQUIRE_FALSE( p.Parse( std::vector<std::string>( {"-q", "Foo", "-Q", "Bar"} ) ) );
				const string& s = msg.GetParseResult();
				REQUIRE( strstr( s.c_str(), "The argument '-q' is specified multiple times." ) != nullptr );
			}
		}
	}
}

SCENARIO( "Missing mandatory argument" )
{
	GIVEN( "A parser" )
	{
		SystemOutputParseResult msg;
		CmdParser4Cpp p( msg );
		WHEN( "setup with mandatory argument" )
		{
			p.Accept( "-q" ).AsString( 1 ).SetMandatory();
			p.Accept( "-Q" ).AsString( 1 );

			THEN( "if called with missing argument, it fails" )
			{
				REQUIRE_FALSE( p.Parse( std::vector<std::string>( {"-Q", "Bar"} ) ) );
				const string& s = msg.GetParseResult();
				REQUIRE( strstr( s.c_str(), "missing" ) != nullptr );
			}
			AND_THEN( "if called with mandatory argument, it succeeds" )
			{
				REQUIRE( p.Parse( std::vector<std::string>( {"-q", "Foo"} ) ) );
				const std::string& s = msg.GetParseResult();
				REQUIRE( s.length() == 0 );
			}
		}
	}
}

SCENARIO( "Leftovers on the command line" )
{
	GIVEN( "Parser setup for one argument with one parameter" )
	{
		SystemOutputParseResult msg;
		CmdParser4Cpp p( msg );
		p.Accept( "-Q" ).AsString( 1 );

		WHEN( "called with extra parameters" )
		{
			REQUIRE_FALSE( p.Parse( std::vector<std::string>( {"-Q", "Bar", "some", "extra"} ) ) );
			const std::string& s = msg.GetParseResult();
			REQUIRE( strstr( s.c_str(), "Unknown arguments: some extra" ) != nullptr );
		}
		AND_WHEN( "called with correct number of parameters" )
		{
			REQUIRE( p.Parse( std::vector<std::string>( {"-Q", "Bar"} ) ) );
		}
	}
}

SCENARIO( "Using boolean arguments" )
{
	GIVEN( "A parser set up for boolean arguments" )
	{
		SystemOutputParseResult msg;
		CmdParser4Cpp p( msg );
		p.Accept( "/b" ).AsBoolean( 4 );
		p.Accept( "/bar" ).AsBoolean( 1 ).WithAlias( "/foo" );

		WHEN( "called with valid parameters" )
		{
			REQUIRE( p.Parse( std::vector<std::string>( {"/b", "true", "1", "0", "false", "/foo", "false"} ) ) );

			THEN( "parsing succeeds" )
			{
				REQUIRE( p.GetBool( "/b", 0 ) );
				REQUIRE( p.GetBool( "/b", 1 ) );
			}
			AND_THEN( "only provided parameters are available" )
			{
				REQUIRE_FALSE( p.GetBool( "/b", 2 ) );
				REQUIRE_FALSE( p.GetBool( "/b", 3 ) );
				REQUIRE_FALSE( p.GetBool( "/bar", 0 ) );
			}
		}
		AND_WHEN( "called with invalid parameters" )
		{
			REQUIRE_FALSE(
					p.Parse( std::vector<std::string>( {"/b", "true", "1", "notbool", "false", "/foo", "false"} ) ) );

			THEN( "argument is reported as problem" )
			{
				const string& s = msg.GetParseResult();
				REQUIRE( strstr( s.c_str(), "Failed to parse argument '/b'" ) != nullptr );
			}
		}
	}
}


SCENARIO( "Single boolean" )
{
	GIVEN( "Parser setup for two single booleans" )
	{
		SystemOutputParseResult msg;
		CmdParser4Cpp p( msg );
		p.Accept( "foo" ).AsSingleBoolean().SetMandatory();
		p.Accept( "bar" ).AsSingleBoolean();

		WHEN( "called with both arguments" )
		{
			REQUIRE( p.Parse( std::vector<std::string>( {"foo", "bar"} ) ) );
			THEN( "both arguments are true" )
			{
				REQUIRE( p.GetBool( "foo", 0 ) );
				REQUIRE( p.GetBool( "bar", 0 ) );
			}
			AND_THEN( "unknown arguments are false" )
			{
				REQUIRE_FALSE( p.GetBool( "XXX", 0 ) );
			}
		}
		AND_WHEN( "called without mandatory it fails" )
		{
			REQUIRE_FALSE( p.Parse( std::vector<std::string>( {"bar"} ) ) );
		}
	}
}

SCENARIO( "Integer" )
{
	GIVEN( "Parser setup for two integers" )
	{
		SystemOutputParseResult msg;
		CmdParser4Cpp p( msg );
		p.Accept( "-a" ).AsInteger( 1 );
		p.Accept( "-b" ).AsInteger( 2 );

		WHEN( "called with both arguments" )
		{
			REQUIRE( p.Parse( std::vector<std::string>( {"-a", "5", "-b", "1", "2"} ) ) );
			THEN( "both arguments return correct count" )
			{
				REQUIRE( p.GetAvailableIntegerParameterCount( "-a" ) == 1 );
				REQUIRE( p.GetAvailableIntegerParameterCount( "-b" ) == 2 );
			}
			AND_THEN( "Correct parameters are returned" )
			{
				REQUIRE( 5 == p.GetInteger( "-a" ) );
				REQUIRE( 1 == p.GetInteger( "-b", 0 ) );
				REQUIRE( 2 == p.GetInteger( "-b", 1, 1000 ) );
				REQUIRE( 0 == p.GetInteger( "-b", 10, 0 ) );
			}
		}
		AND_WHEN( "called with negative numbers" )
		{
			REQUIRE( p.Parse( std::vector<std::string>( {"-a", "-54321"} ) ) );
			THEN( "negative number returned" )
			{
				REQUIRE( -54321 == p.GetInteger( "-a" ) );
			}
		}
		AND_WHEN( "called with explicit positive number" )
		{
			REQUIRE( p.Parse( std::vector<std::string>( {"-a", "+456789"} ) ) );
			THEN( "positive number returned" )
			{
				REQUIRE( 456789 == p.GetInteger( "-a" ) );
			}
		}
		AND_WHEN( "called with invalid integer data" )
		{
			REQUIRE_FALSE( p.Parse( std::vector<std::string>( {"-a", "AA"} ) ) );
		}
	}
}

SCENARIO( "Missing parameters" )
{
	GIVEN( "A parser with boolean argument requiring four parameters" )
	{
		SystemOutputParseResult msg;
		CmdParser4Cpp p( msg );
		p.Accept( "/b" ).AsBoolean( 4 );
		WHEN( "called with too few parameters" )
		{
			REQUIRE_FALSE( p.Parse( std::vector<std::string>( {"/b", "true", "1", "0"} ) ) );

			THEN( "Argument is reported as problematic" )
			{
				const std::string& s = msg.GetParseResult();
				REQUIRE( strstr( s.c_str(), "Not enough parameters, argument '/b'" ) != nullptr );
			}
		}
	}
}

SCENARIO( "Missing parameters, part 2" )
{
	GIVEN( "A parser with boolean arguments requiring parameters" )
	{
		SystemOutputParseResult msg;
		CmdParser4Cpp p( msg );
		p.Accept( "/b" ).AsBoolean( 2 );
		p.Accept( "/c" ).AsBoolean( 1 );

		WHEN( "called with too few parameters" )
		{
			REQUIRE_FALSE( p.Parse( std::vector<std::string>( {"/b", "true", "/c", "false"} ) ) );

			THEN( "Argument is reported as problematic" )
			{
				REQUIRE( strstr( msg.GetParseResult().c_str(), "Not enough parameters, argument '/b'" ) != nullptr );
			}
		}
	}
}

SCENARIO( "Missing parameters, part 3" )
{
	GIVEN( "A parser with boolean arguments requiring parameters" )
	{
		SystemOutputParseResult msg;
		CmdParser4Cpp p( msg );
		p.Accept( "/a" ).AsBoolean( 1 );
		p.Accept( "/b" ).AsBoolean( 3 );
		p.Accept( "/c" ).AsBoolean( 1 );

		WHEN( "called with too few parameters" )
		{
			REQUIRE_FALSE(
					p.Parse( std::vector<std::string>( {"/b", "true", "false", "/a", "true", "/c", "false"} ) ) );

			THEN( "Argument is reported as problematic" )
			{
				REQUIRE( strstr( msg.GetParseResult().c_str(), "Not enough parameters, argument '/b' requires 3" ) !=
				         nullptr );
			}
		}
	}
}

SCENARIO( "No parameters at all" )
{
	GIVEN( "A parser with mandatory boolean argument requiring four parameters" )
	{
		SystemOutputParseResult msg;
		CmdParser4Cpp p( msg );
		p.Accept( "/b" ).AsBoolean( 4 ).SetMandatory();
		WHEN( "called with no parameters" )
		{
			REQUIRE_FALSE( p.Parse( std::vector<std::string>( {"/b"} ) ) );

			THEN( "Argument is reported as problematic" )
			{
				const std::string& s = msg.GetParseResult();
				REQUIRE( strstr( s.c_str(), "Not enough parameters, argument '/b'" ) != nullptr );
			}
		}
		AND_WHEN( "called with without mandatory argument" )
		{
			REQUIRE_FALSE( p.Parse( std::vector<std::string>( {} ) ) );
			const string& s = msg.GetParseResult();
			REQUIRE( strstr( s.c_str(), "missing" ) != nullptr );
		}
	}
}

SCENARIO( "Description" )
{
	GIVEN( "A parser with set descriptions" )
	{
		SystemOutputParseResult msg;
		CmdParser4Cpp p( msg );

		p.Accept( "single" ).AsSingleBoolean().DescribedAs(
				"AAA BBBBB CCCCCCCCCCC DDDDDDDDDDDDDDE EEEEEEEEEEEEEEEE FFFFFFFFFFF GGGGGGGGGGGGGGG HHHHHHHHHHHH" );
		p.Accept( "/bool" ).AsBoolean( 1 ).WithAlias( std::vector<std::string>( {"/B", "-B", "-b"} ) ).DescribedAs(
				"A Boolean value" ).SetMandatory();
		p.Accept( "/string" ).AsString( 1 ).DescribedAs( "A string argument" );
		p.Accept( "/goo" ).AsBoolean( 1 ).SetMandatory().DescribedAs( "Something something" );
		p.Accept( "/aaa" ).AsString( 1 ).DescribedAs( "Jada Jada Jada" );
		p.Accept( "/bbb" ).AsString( 1, Constructor::NO_PARAMETER_LIMIT, UnboundStringLength() ).DescribedAs(
				"A long non descriptive description without any meaning what so ever" );

		WHEN( "arguments are parsed" )
		{
			REQUIRE( p.Parse( std::vector<std::string>(
					{"/bool", "1", "single", "/goo", "true", "/aaa", "AAA", "/bbb", "123", "456", "789"} ) ) );

			THEN( "parameters can be read" )
			{
				REQUIRE( p.GetBool( "/bool" ) );
				REQUIRE( p.GetBool( "/goo" ) );
				REQUIRE( p.GetBool( "single", 0 ) );
				REQUIRE( string( "AAA" ) == p.GetString( "/aaa", 0, "blah" ) );
				REQUIRE( string( "123" ) == p.GetString( "/bbb", 0, "blah" ) );
				REQUIRE( string( "456" ) == p.GetString( "/bbb", 1, "blah" ) );
				REQUIRE( string( "789" ) == p.GetString( "/bbb", 2, "blah" ) );
			}
		}
		AND_WHEN( "usage is asked for" )
		{
			SystemOutputUsageFormatter usage( "application name" );
			p.GetUsage( usage );
			std::string output = usage.ToString();
			THEN( "output contains descriptions" )
			{
				//cout << output;

				// Can't test outputed text so just verify that it contains the descriptions
				REQUIRE( strstr( output.c_str(), "/aaa" ) != nullptr );
				REQUIRE( strstr( output.c_str(),
				                 "A long non descriptive description without any meaning what so ever" ) != nullptr );
			}
		}
	}
}

SCENARIO( "Variable parameter count" )
{
	GIVEN( "A parser setup for argument with no parameter limit" )
	{
		SystemOutputParseResult msg;
		CmdParser4Cpp p( msg );
		p.Accept( "/b" ).AsBoolean( 1, Constructor::NO_PARAMETER_LIMIT );

		WHEN( "called with arbitrary number of parameters" )
		{
			REQUIRE( p.Parse( std::vector<std::string>( {"/b", "1", "0", "1", "true", "false"} ) ) );

			THEN( "parameters are available" )
			{
				REQUIRE( 5 == p.GetAvailableBooleanParameterCount( "/b" ) );
				REQUIRE( p.GetBool( "/b", 0, false ) );
				REQUIRE_FALSE( p.GetBool( "/b", 1, true ) );
				REQUIRE( p.GetBool( "/b", 2, false ) );
				REQUIRE( p.GetBool( "/b", 3, false ) );
				REQUIRE_FALSE( p.GetBool( "/b", 4, true ) );
			}
		}
	}
}

SCENARIO( "Variable parameter in middle" )
{
	GIVEN( "A parser setup for argument with no parameter limit" )
	{
		SystemOutputParseResult msg;
		CmdParser4Cpp p( msg );
		p.Accept( "single" ).AsSingleBoolean().DescribedAs(
				"AAA BBBBB CCCCCCCCCCC DDDDDDDDDDDDDDE EEEEEEEEEEEEEEEE FFFFFFFFFFF GGGGGGGGGGGGGGG HHHHHHHHHHHH" );
		p.Accept( "/bool" ).AsBoolean( 1 ).WithAlias( std::vector<std::string>( {"/B", "-B", "-b"} ) ).DescribedAs(
				"A Boolean value" ).SetMandatory();
		p.Accept( "/string" ).AsString( 1 ).DescribedAs( "A string argument" );
		p.Accept( "/goo" ).AsBoolean( 1 ).SetMandatory().DescribedAs( "-gle?" );
		p.Accept( "/aaa" ).AsString( 3 ).DescribedAs( "Jada Jada Jada" ).WithAlias( "-A" );
		p.Accept( "/bbb" ).AsString( 1, Constructor::NO_PARAMETER_LIMIT, UnboundStringLength() ).DescribedAs(
				"A long non descriptive description without any meaning what so ever" );

		WHEN( "called with arbitrary number of parameters in the middle of the command line" )
		{
			REQUIRE( p.Parse( std::vector<std::string>(
					{"/bool", "1", "single", "/goo", "true", "/bbb", "AAA", "BBB", "CCC", "-A", "123", "456",
					 "789"} ) ) );

			THEN( "parameters are available for multi-parameter argument" )
			{
				REQUIRE( string( "AAA" ) == p.GetString( "/bbb", 0 ) );
				REQUIRE( string( "BBB" ) == p.GetString( "/bbb", 1 ) );
				REQUIRE( string( "CCC" ) == p.GetString( "/bbb", 2 ) );
				REQUIRE( nullptr == p.GetString( "/bbb", 3 ) );
			}
			AND_THEN( "other parameters are available too" )
			{
				REQUIRE( p.GetBool( "/bool" ) );
			}
			AND_THEN( "other parameters are available too, also as aliases" )
			{
				REQUIRE( string( "456" ) == p.GetString( "/aaa", 1 ) );
			}
		}
	}
}

SCENARIO( "Multiple muti-parameter arguments" )
{
	GIVEN( "Parser setup for multiple multi-parameter arguments" )
	{
		SystemOutputParseResult msg;
		CmdParser4Cpp p( msg );
		p.Accept( "/multi1" ).AsBoolean( 1, 3 );
		p.Accept( "/multi2" ).AsBoolean( 1, 3 );

		WHEN( "Called with multiple parameters" )
		{
			REQUIRE( p.Parse( std::vector<std::string>( {"/multi1", "1", "0", "/multi2", "0", "true"} ) ) );

			THEN( "Parameters are available" )
			{
				REQUIRE( p.GetBool( "/multi1", 0 ) );
				REQUIRE_FALSE( p.GetBool( "/multi1", 1 ) );
				REQUIRE_FALSE( p.GetBool( "/multi2", 0 ) );
				REQUIRE( p.GetBool( "/multi2", 1 ) );
			}
			AND_THEN( "Parameters outside provided parameters are not available and gives the default value" )
			{
				REQUIRE( p.GetBool( "/multi1", p.GetAvailableBooleanParameterCount( string( "/multi1" ) ), true ) );
				REQUIRE( p.GetBool( "/multi1", p.GetAvailableBooleanParameterCount( string( "/multi2" ) ), true ) );

				REQUIRE_FALSE(
						p.GetBool( "/multi1", p.GetAvailableBooleanParameterCount( string( "/multi1" ) ), false ) );
				REQUIRE_FALSE(
						p.GetBool( "/multi1", p.GetAvailableBooleanParameterCount( string( "/multi2" ) ), false ) );
			}
		}

	}
}

SCENARIO( "Same argument multiple times" )
{
	GIVEN( "Parser for two arguments" )
	{
		SystemOutputParseResult msg;
		CmdParser4Cpp p( msg );
		p.Accept( "/multi1" ).AsBoolean( 1, 3 );
		p.Accept( "/multi2" ).AsBoolean( 1, 3 );

		WHEN( "Called with same argument twice" )
		{
			REQUIRE_FALSE( p.Parse( std::vector<std::string>( {"/multi1", "1", "/multi1", "1"} ) ) );
			THEN( "parsing fails" )
			{
				REQUIRE(
						strstr( msg.GetParseResult().c_str(), "The argument '/multi1' is specified multiple times" ) !=
						nullptr );
			}
		}
	}
}

SCENARIO( "Missing dependency" )
{
	GIVEN( "Dependent arguments" )
	{
		SystemOutputParseResult msg;
		CmdParser4Cpp p( msg );
		p.Accept( "-first" ).AsBoolean( 1 ).DependsOn( "-second" );
		p.Accept( "-second" ).AsBoolean( 1 );

		WHEN( "Called without dependency" )
		{
			REQUIRE_FALSE( p.Parse( std::vector<std::string>( {"-first", "false"} ) ) );

			THEN( "Missing dependency is reported" )
			{
				REQUIRE( strstr( msg.GetParseResult().c_str(),
				                 "Argument '-first' depends on '-second', but the latter is missing" ) != nullptr );
			}
		}
	}
}

SCENARIO( "Two-way dependency" )
{
	GIVEN( "Dependent arguments" )
	{
		SystemOutputParseResult msg;
		CmdParser4Cpp p( msg );
		p.Accept( "-first" ).AsBoolean( 1 ).DependsOn( "-second" );
		p.Accept( "-second" ).AsBoolean( 1 ).DependsOn( "-first" );

		WHEN( "Called with dependency" )
		{
			REQUIRE( p.Parse( std::vector<std::string>( {"-first", "false", "-second", "true"} ) ) );

			THEN( "No error reported" )
			{
				REQUIRE( 0 == static_cast<int>(msg.GetParseResult().length()) );
			}
			AND_THEN( "Arguments available" )
			{
				REQUIRE_FALSE( p.GetBool( "-first" ) );
				REQUIRE( p.GetBool( "-second" ) );
			}
		}
	}
}

SCENARIO( "Two-way dependency fails" )
{
	GIVEN( "Dependent arguments" )
	{
		SystemOutputParseResult msg;
		CmdParser4Cpp p( msg );
		p.Accept( "-first" ).AsBoolean( 1 ).DependsOn( "-second" );
		p.Accept( "-second" ).AsBoolean( 1 ).DependsOn( "-first" );

		WHEN( "Called without dependency" )
		{
			REQUIRE_FALSE( p.Parse( std::vector<std::string>( {"-second", "true"} ) ) );

			THEN( "Missing dependancy reported" )
			{
				REQUIRE( strstr( msg.GetParseResult().c_str(),
				                 "Argument '-second' depends on '-first', but the latter is missing" ) != nullptr );
			}
		}
	}
}

SCENARIO( "Two-way dependency programming error" )
{
	GIVEN( "Dependent arguments, but one is not defined by the programmer" )
	{
		SystemOutputParseResult msg;
		CmdParser4Cpp p( msg );
		p.Accept( "-first" ).AsBoolean( 1 ).DependsOn( "-second" );

		WHEN( "Called without dependency" )
		{
			REQUIRE_FALSE( p.Parse( std::vector<std::string>( {"-first", "false"} ) ) );

			THEN( "Missing dependancy reported" )
			{
				REQUIRE( strstr( msg.GetParseResult().c_str(),
				                 "Argument '-first' depends on '-second', but no such argument is defined - contact the author of the application" ) !=
				         nullptr );
			}
		}
	}
}

SCENARIO( "Blocking arguments" )
{
	GIVEN( "Parser with blocking arguments" )
	{
		SystemOutputParseResult msg;
		CmdParser4Cpp p( msg );
		p.Accept( "-first" ).AsSingleBoolean().BlockedBy( "-second" );
		p.Accept( "-second" ).AsSingleBoolean().BlockedBy( "-first" );

		WHEN( "Called with only one argument" )
		{
			REQUIRE( p.Parse( std::vector<std::string>( {"-first"} ) ) );
		}
		AND_WHEN( "Called with only other argument" )
		{
			REQUIRE( p.Parse( std::vector<std::string>( {"-second"} ) ) );
		}
		AND_WHEN( "Called with both arguments" )
		{
			REQUIRE_FALSE( p.Parse( std::vector<std::string>( {"-first", "-second"} ) ) );
			THEN( "Mutually exclusive arguments are reported" )
			{
				REQUIRE( strstr( msg.GetParseResult().c_str(), "mutually exclusive" ) != nullptr );
			}
		}
	}
}

SCENARIO( "Blocking argument missing" )
{
	GIVEN( "Parser with missing blocking argument" )
	{
		SystemOutputParseResult msg;
		CmdParser4Cpp p( msg );
		p.Accept( "-first" ).AsSingleBoolean().BlockedBy( "-doesnotexist" );
		p.Accept( "-second" ).AsSingleBoolean().BlockedBy( "-first" );

		WHEN( "Parse attempted" )
		{
			REQUIRE_FALSE( p.Parse( std::vector<std::string>( {"-first"} ) ) );

			THEN( "error reported" )
			{
				REQUIRE( strstr( msg.GetParseResult().c_str(), "doesnotexist" ) != nullptr );
			}
		}

	}
}

SCENARIO( "Hidden arguments" )
{
	GIVEN( "Parser with hidden arguments" )
	{
		SystemOutputParseResult msg;
		CmdParser4Cpp p( msg );
		p.Accept( "-first" ).AsSingleBoolean().DescribedAs( "The first argument" );
		p.Accept( "-second" ).AsSingleBoolean().SetHidden().DescribedAs( "The hidden argument" );

		WHEN( "Usage is called" )
		{
			SystemOutputUsageFormatter usage( "application name" );
			p.GetUsage( usage );
			const string& output = usage.ToString();

			THEN( "Hidden argument is not reported" )
			{
				REQUIRE( strstr( output.c_str(), "The hidden argument" ) == nullptr );
			}
			AND_THEN( "Non-hidden arguments are reported" )
			{
				REQUIRE( strstr( output.c_str(), "The first argument" ) != nullptr );
			}
		}
	}
}

SCENARIO( "Missing argument type" )
{
	GIVEN( "A parser with argument that has no set type" )
	{
		SystemOutputParseResult msg;
		CmdParser4Cpp p( msg );
		p.Accept( "-first" );

		WHEN( "parsing is done" )
		{
			REQUIRE_FALSE( p.Parse( std::vector<std::string>( {"-first"} ) ) );

			THEN( "Problem reported" )
			{
				REQUIRE( strstr( msg.GetParseResult().c_str(), "is missing type information" ) != nullptr );
			}
		}
	}
}

SCENARIO( "Garbage on command line" )
{
	GIVEN( "Properly setup parser" )
	{
		SystemOutputParseResult msg;
		CmdParser4Cpp p( msg );
		p.Accept( "-first" ).AsSingleBoolean();

		WHEN( "given only garbage on command line" )
		{
			REQUIRE_FALSE( p.Parse( std::vector<std::string>( {"jada", "Jada"} ) ) );

			THEN( "Unknown arguments named" )
			{
				const std::string& s = msg.GetParseResult();
				REQUIRE( strstr( s.c_str(), "jada Jada" ) != nullptr );
			}
		}
	}
}

SCENARIO( "Garbage before first command" )
{
	GIVEN( "Properly setup parser" )
	{
		SystemOutputParseResult msg;
		CmdParser4Cpp p( msg );
		p.Accept( "-first" ).AsSingleBoolean();

		WHEN( "given garbage on before first command" )
		{
			REQUIRE_FALSE( p.Parse( std::vector<std::string>( {"jada", "-first"} ) ) );

			THEN( "Unknown arguments named" )
			{
				const std::string& s = msg.GetParseResult();
				REQUIRE( strstr( s.c_str(), "jada" ) != nullptr );
			}
		}
	}
}

SCENARIO( "XML configuration" )
{
	GIVEN( "Properly setup parser" )
	{
		SystemOutputParseResult msg;
		CmdParser4Cpp p( msg );
		p.Accept( "-first" ).AsInteger( 3 );

		WHEN( "Provided with configuration file and the value is in text-format as the child of the node" )
		{
			std::string cfgStr = R"!!(
									<Settings>
										<First><![CDATA[40]]></First>
										<First>41</First>
										<First><![CDATA[42]]></First>
									</Settings>)!!";
			std::shared_ptr<XMLConfigurationReader> cfg = std::make_shared<XMLConfigurationReader>( cfgStr );
			cfg->SetMatcher( "-first", XMLConfigurationReader::NodeMatcher( "/Settings/First" ) );

			REQUIRE( p.Parse( std::vector<std::string>(), cfg ) );

			THEN( "Argument read from configuration" )
			{
				REQUIRE( p.GetInteger( "-first" ) == 40 );
				REQUIRE( p.GetInteger( "-first", 1 ) == 41 );
				REQUIRE( p.GetInteger( "-first", 2 ) == 42 );
			}
		}
		AND_WHEN( "The wanted value is in an attribute and we search using attribute name/value pair" )
		{
			std::string cfgStr =
					R"!!(
						"<Settings>
							<First Key="KeyName" Value="1234"/>
							<First Key="KeyName" Value="5678"/>
							<First Key="KeyName" Value="9012"/>
						</Settings>")!!";


			std::shared_ptr<XMLConfigurationReader> cfg = std::make_shared<XMLConfigurationReader>( cfgStr );
			cfg->SetMatcher( "-first",
			                 XMLConfigurationReader::NodeMatcher( "/Settings/First", "Value", "Key", "KeyName" ) );

			REQUIRE( p.Parse( std::vector<std::string>(), cfg ) );

			THEN( "Argument read from configuration" )
			{
				REQUIRE( p.GetInteger( "-first" ) == 1234 );
				REQUIRE( p.GetInteger( "-first", 1 ) == 5678 );
				REQUIRE( p.GetInteger( "-first", 2 ) == 9012 );
			}
		}
		AND_WHEN( "The wanted value is in an attribute but we don't care about first matching name/value pair" )
		{
			std::string cfgStr =
					R"!!(
						"<Settings>
							<First Key="Foo" Value="70"/>
							<First Key="Bar" Value="80"/>
							<First Key="FooBar" Value="90"/>
						</Settings>")!!";


			std::shared_ptr<XMLConfigurationReader> cfg = std::make_shared<XMLConfigurationReader>( cfgStr );
			cfg->SetMatcher( "-first", XMLConfigurationReader::NodeMatcher( "/Settings/First", "Value" ) );

			REQUIRE( p.Parse( std::vector<std::string>(), cfg ) );

			THEN( "Argument read from configuration" )
			{
				REQUIRE( p.GetInteger( "-first" ) == 70 );
				REQUIRE( p.GetInteger( "-first", 1 ) == 80 );
				REQUIRE( p.GetInteger( "-first", 2 ) == 90 );
			}
		}
		AND_WHEN( "The wanted value is the child data and we search using attribute name/value pair" )
		{
			std::string cfgStr =
					R"!!(
						"<Settings>
							<First Key="KeyName">100</First>
							<First Key="KeyName">200</First>
							<First Key="KeyName">300</First>
						</Settings>")!!";


			std::shared_ptr<XMLConfigurationReader> cfg = std::make_shared<XMLConfigurationReader>( cfgStr );
			cfg->SetMatcher( "-first",
			                 XMLConfigurationReader::NodeMatcher( "/Settings/First", "Key", "KeyName" ) );

			REQUIRE( p.Parse( std::vector<std::string>(), cfg ) );

			THEN( "Argument read from configuration" )
			{
				REQUIRE( p.GetInteger( "-first" ) == 100 );
				REQUIRE( p.GetInteger( "-first", 1 ) == 200 );
				REQUIRE( p.GetInteger( "-first", 2 ) == 300 );
			}
		}
		AND_WHEN( "Configuration file is missing entries" )
		{
			std::string cfgStr = "<Settings><First>55</First><First>56</First></Settings>";
			std::shared_ptr<XMLConfigurationReader> cfg = std::make_shared<XMLConfigurationReader>( cfgStr );
			cfg->SetMatcher( "-first", XMLConfigurationReader::NodeMatcher( "/Settings/First" ) );

			THEN( "Parsing fails" )
			{
				REQUIRE_FALSE( p.Parse( std::vector<std::string>(), cfg ) );
			}
		}
		AND_WHEN( "Configuration file is invalid" )
		{
			std::string cfgStr = "Not XML";
			std::shared_ptr<XMLConfigurationReader> cfg = std::make_shared<XMLConfigurationReader>( cfgStr );
			cfg->SetMatcher( "-first", XMLConfigurationReader::NodeMatcher( "/Settings/First" ) );

			THEN( "Parsing fails" )
			{
				REQUIRE_FALSE( p.Parse( std::vector<std::string>(), cfg ) );
			}
		}
	}
}

SCENARIO( "XML configuration from command line" )
{
	GIVEN( "Properly setup parser" )
	{
		SystemOutputParseResult msg;
		CmdParser4Cpp p( msg );
		p.Accept( "-first" ).AsInteger( 3 );
		p.Accept( "-config" ).AsString( 1 ).SetMandatory();

		const std::string file = "./config.xml";
		std::string cfgStr =
				R"!!(
				"<Settings>
					<Foo>
						<First>88</First>
						<First>89</First>
						<First>90</First>
					</Foo>
				</Settings>")!!";

		ofstream f;
		remove( file.c_str() );
		f.open( file, std::ofstream::binary );

		REQUIRE( f.good() );
		if( f.good() )
		{
			f.write( cfgStr.c_str(), cfgStr.length() );
			f.close();
		}

		std::shared_ptr<XMLConfigurationReader> cfg = std::make_shared<XMLConfigurationReader>();
		cfg->SetMatcher( "-first", XMLConfigurationReader::NodeMatcher( "/Settings/Foo/First" ) );

		WHEN( "Configuration file is specified and available" )
		{
			THEN( "Parsing succeeds" )
			{
				REQUIRE( p.Parse( std::vector<std::string>( {"-config", file} ), cfg, "-config" ) );
				REQUIRE( p.GetInteger( "-first" ) == 88 );
				REQUIRE( p.GetInteger( "-first", 1 ) == 89 );
				REQUIRE( p.GetInteger( "-first", 2 ) == 90 );
			}

			remove( file.c_str() );
		}
		AND_WHEN( "Configuration file is missing" )
		{
			THEN( "Parsing fails" )
			{
				REQUIRE_FALSE( p.Parse( std::vector<std::string>( {"-config",
				                                                   "./non-existingFile.xml"} ), cfg, "-config" ) );
				std::string result = msg.GetParseResult();
				REQUIRE(
						strstr( result.c_str(), "Could not load the configuration specified by argument" ) != nullptr );
			}
		}
	}
}

SCENARIO( "Integer limits" )
{
	GIVEN( "Properly setup parser" )
	{
		SystemOutputParseResult msg;
		CmdParser4Cpp p( msg );
		p.Accept( "-first" ).AsInteger( 1, NumericLimit<int>( 4, 5 ) );

		WHEN( "Called with value inside limits" )
		{
			REQUIRE( p.Parse( std::vector<std::string>( {"-first", "4"} ) ) );
		}
		AND_WHEN( "Called with value inside limits" )
		{
			REQUIRE( p.Parse( std::vector<std::string>( {"-first", "5"} ) ) );
		}
		AND_WHEN( "Called with value outside limits" )
		{
			REQUIRE_FALSE( p.Parse( std::vector<std::string>( {"-first", "3"} ) ) );
			std::string s = msg.GetParseResult();
			REQUIRE( strstr( s.c_str(), "Parameter of argument " ) != nullptr );
		}
		AND_WHEN( "Called with value outside limits" )
		{
			REQUIRE_FALSE( p.Parse( std::vector<std::string>( {"-first", "6"} ) ) );
			std::string s = msg.GetParseResult();
			REQUIRE( strstr( s.c_str(), "Parameter of argument " ) != nullptr );
		}
	}
}

SCENARIO( "String lengths" )
{
	GIVEN( "Properly setup parser" )
	{
		SystemOutputParseResult msg;
		CmdParser4Cpp p( msg );
		p.Accept( "-first" ).AsString( 1, StringLengthLimit( 4, 5 ) );

		WHEN( "Called with value inside limits" )
		{
			REQUIRE( p.Parse( std::vector<std::string>( {"-first", "AAAA"} ) ) );
		}
		AND_WHEN( "Called with value inside limits" )
		{
			REQUIRE( p.Parse( std::vector<std::string>( {"-first", "AAAAA"} ) ) );
		}
		AND_WHEN( "Called with value outside limits" )
		{
			REQUIRE_FALSE( p.Parse( std::vector<std::string>( {"-first", "AAA"} ) ) );
			std::string s = msg.GetParseResult();
			REQUIRE( strstr( s.c_str(), "String length of argument" ) != nullptr );
		}
		AND_WHEN( "Called with value outside limits" )
		{
			REQUIRE_FALSE( p.Parse( std::vector<std::string>( {"-first", "AAAAAA"} ) ) );
			std::string s = msg.GetParseResult();
			REQUIRE( strstr( s.c_str(), "String length of argument" ) != nullptr );
		}
	}
}