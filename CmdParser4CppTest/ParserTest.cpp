// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file. 

#include "CppUnitTest.h"
#include "CmdParser4Cpp.h"
#include "SystemOutputParseResult.h"
#include "SystemOutputUsageFormatter.H"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace com::codezeal::commandline;

namespace CmdParser4CppTest {
TEST_CLASS( CmdParser4CppTest )
{
public:
	TEST_METHOD( testParse )
	{
		SystemOutputParseResult msg;

		CmdParser4Cpp p( "-", msg );
		p.Accept( "-m" ).AsString( 2 );

		Assert::IsTrue( p.Parse( std::vector<std::string>( { "-m", "", "one", "two" } ) ) );
		Assert::AreEqual( 2, p.GetAvailableStringParameterCount( "-m" ) );

		Assert::AreEqual( "one", p.GetString( "-m" ) );
		Assert::AreEqual( "two", p.GetString( "-m", 1 ) );

		Assert::AreEqual( nullptr, p.GetString( "-multi", 2 ) );
	}


	TEST_METHOD( testSpecifiedMultipleTimes )
	{
		SystemOutputParseResult msg;

		CmdParser4Cpp p( "-", msg );
		p.Accept( "-q" ).AsString( 1 ).WithAlias( "-Q" );
		Assert::IsFalse( p.Parse( std::vector<std::string>( { "-q", "Foo", "-Q", "Bar" } ) ) );
		std::string& s = msg.GetParseResult();
		Assert::IsTrue( strstr( s.c_str(), "The argument '-q' is specified multiple times." ) != nullptr );
	}

	TEST_METHOD( testMissingMandatory )
	{
		SystemOutputParseResult msg;

		CmdParser4Cpp p( "-", msg );

		p.Accept( "-q" ).AsString( 1 ).SetMandatory();
		p.Accept( "-Q" ).AsString( 1 );
		Assert::IsFalse( p.Parse( std::vector<std::string>( { "-Q", "Bar" } ) ) );
		std::string& s = msg.GetParseResult();
		Assert::IsTrue( strstr( s.c_str(), "The mandatory argument" ) != nullptr );
	}

	TEST_METHOD( testLeftOvers )
	{
		SystemOutputParseResult msg;

		CmdParser4Cpp p( "-", msg );
		p.Accept( "-Q" ).AsString( 1 );
		Assert::IsFalse( p.Parse( std::vector<std::string>( { "-Q", "Bar", "some", "extra" } ) ) );
		std::string& s = msg.GetParseResult();
		Assert::IsTrue( strstr( s.c_str(), "Unknown arguments: some extra" ) != nullptr );
	}

	TEST_METHOD( testBoolean )
	{
		SystemOutputParseResult msg;

		CmdParser4Cpp p( "/", msg );

		p.Accept( "/b" ).AsBoolean( 4 );
		p.Accept( "/bar" ).WithAlias( "/foo" ).AsBoolean( 1 );
		Assert::IsTrue( p.Parse( std::vector<std::string>( { "/b", "true", "1", "0", "false", "/foo", "false" } ) ) );
		Assert::IsTrue( p.GetBool( "/b", 0 ) );
		Assert::IsTrue( p.GetBool( "/b", 1 ) );
		Assert::IsFalse( p.GetBool( "/b", 2 ) );
		Assert::IsFalse( p.GetBool( "/b", 3 ) );
		Assert::IsFalse( p.GetBool( "/bar", 0 ) );
	}

	TEST_METHOD( testSingleBoolean )
	{
		SystemOutputParseResult msg;

		CmdParser4Cpp& p = CmdParser4Cpp( "-", msg );

		p.Accept( "foo" ).AsSingleBoolean();
		p.Accept( "bar" ).AsSingleBoolean();
		Assert::IsTrue( p.Parse( std::vector<std::string>( { "foo", "bar" } ) ) );
		Assert::IsTrue( p.GetBool( "foo", 0 ) );
		Assert::IsTrue( p.GetBool( "bar", 0 ) );
		Assert::IsFalse( p.GetBool( "XXX", 0 ) );
	}

	TEST_METHOD( testFailedParse )
	{
		SystemOutputParseResult msg;

		CmdParser4Cpp p( "/", msg );

		p.Accept( "/b" ).AsBoolean( 4 );
		Assert::IsFalse( p.Parse( std::vector<std::string>( { "/b", "true", "1", "Nope", "false" } ) ) );
		std::string& s = msg.GetParseResult();
		Assert::IsTrue( strstr( s.c_str(), "Failed to parse argument '/b'" ) != nullptr );
	}

	TEST_METHOD( testMissingParameters )
	{
		SystemOutputParseResult msg;

		CmdParser4Cpp p( "/", msg );
		p.Accept( "/b" ).AsBoolean( 4 );
		Assert::IsFalse( p.Parse( std::vector<std::string>( { "/b", "true", "1", "Nope" } ) ) );
		std::string& s = msg.GetParseResult();
		Assert::IsTrue( strstr( s.c_str(), "Not enough parameters, argument '/b'" ) != nullptr );
	}

	TEST_METHOD( testMissingParameters2 )
	{
		SystemOutputParseResult msg;

		CmdParser4Cpp p( "/", msg );
		p.Accept( "/b" ).AsBoolean( 4 );
		Assert::IsFalse( p.Parse( std::vector<std::string>( { "/b" } ) ) );
		std::string& s = msg.GetParseResult();
		Assert::IsTrue( strstr( s.c_str(), "Not enough parameters, argument" ) != nullptr );
	}

	TEST_METHOD( testNoInputWithMandatory )
	{
		SystemOutputParseResult msg;

		CmdParser4Cpp p( "/", msg );
		p.Accept( "/b" ).AsBoolean( 1 ).SetMandatory();
		Assert::IsFalse( p.Parse( std::vector<std::string>( { "" } ) ) );
		std::string& s = msg.GetParseResult();
		Assert::IsTrue( strstr( s.c_str(), "The mandatory argument" ) != nullptr );
	}

	TEST_METHOD( testDescription )
	{
		SystemOutputParseResult msg;

		CmdParser4Cpp p( "/", msg );
		p.Accept( "single" ).AsSingleBoolean().DescribedAs( "AAA BBBBB CCCCCCCCCCC DDDDDDDDDDDDDDE EEEEEEEEEEEEEEEE FFFFFFFFFFF GGGGGGGGGGGGGGG HHHHHHHHHHHH" );
		p.Accept( "/bool" ).AsBoolean( 1 ).WithAlias( std::vector<std::string>( { "/B", "-B", "-b" } ) ).DescribedAs( "A Boolean value" ).SetMandatory();
		p.Accept( "/string" ).AsString( 1 ).DescribedAs( "A string argument" );
		p.Accept( "/goo" ).AsBoolean( 1 ).SetMandatory().DescribedAs( "Something something" );
		p.Accept( "/aaa" ).AsString( 1 ).DescribedAs( "Jada Jada Jada" );
		p.Accept( "/bbb" ).AsString( 1, Constructor::NO_PARAMETER_LIMIT ).DescribedAs( "A long non descriptive description without any meaning what so ever" );
		Assert::IsTrue( p.Parse( std::vector<std::string>( { "/bool", "1", "single", "/goo", "true", "/aaa", "AAA", "/bbb", "123", "456", "789" } ) ) );
		Assert::AreEqual( true, p.GetBool( "/bool" ) );
		Assert::AreEqual( true, p.GetBool( "/goo" ) );
		Assert::AreEqual( true, p.GetBool( "single", 0 ) );
		Assert::AreEqual( "AAA", p.GetString( "/aaa", 0, "blah" ) );
		Assert::AreEqual( "123", p.GetString( "/bbb", 0, "blah" ) );
		Assert::AreEqual( "456", p.GetString( "/bbb", 1, "blah" ) );
		Assert::AreEqual( "789", p.GetString( "/bbb", 2, "blah" ) );

		SystemOutputUsageFormatter usage( "application name" );
		p.GetUsage( usage );
		std::string output = usage.ToString();
		// Can't really 'test' the output string, so debug and verify correctness manually.
	}

	TEST_METHOD( testVariableParameterCount )
	{
		SystemOutputParseResult msg;

		CmdParser4Cpp p( "/", msg );
		p.Accept( "/b" ).AsBoolean( 1, Constructor::NO_PARAMETER_LIMIT );
		Assert::IsTrue( p.Parse( std::vector<std::string>( { "/b", "1", "0", "1", "true", "false" } ) ) );
		Assert::AreEqual( 5, p.GetAvailableBooleanParameterCount( "/b" ) );
		Assert::IsTrue( p.GetBool( "/b", 0, false ) );
		Assert::IsFalse( p.GetBool( "/b", 1, true ) );
		Assert::IsTrue( p.GetBool( "/b", 2, false ) );
		Assert::IsTrue( p.GetBool( "/b", 3, false ) );
		Assert::IsFalse( p.GetBool( "/b", 4, true ) );
	}

	TEST_METHOD( testMultiArgumentInMiddle )
	{
		SystemOutputParseResult msg;

		CmdParser4Cpp p( "/", msg );
		p.Accept( "single" ).AsSingleBoolean().DescribedAs( "AAA BBBBB CCCCCCCCCCC DDDDDDDDDDDDDDE EEEEEEEEEEEEEEEE FFFFFFFFFFF GGGGGGGGGGGGGGG HHHHHHHHHHHH" );
		p.Accept( "/bool" ).AsBoolean( 1 ).WithAlias( std::vector<std::string>( { "/B", "-B", "-b" } ) ).DescribedAs( "A Boolean value" ).SetMandatory();
		p.Accept( "/string" ).AsString( 1 ).DescribedAs( "A string argument" );
		p.Accept( "/goo" ).AsBoolean( 1 ).SetMandatory().DescribedAs( "-gle?" );
		p.Accept( "/aaa" ).AsString( 3 ).DescribedAs( "Jada Jada Jada" );
		p.Accept( "/bbb" ).AsString( 1, Constructor::NO_PARAMETER_LIMIT ).DescribedAs( "A long non descriptive description without any meaning what so ever" );
		Assert::IsTrue( p.Parse( std::vector<std::string>( { "/bool", "1", "single", "/goo", "true", "/bbb", "AAA", "BBB", "CCC", "/aaa", "123", "456", "789" } ) ) );

		Assert::AreEqual( "AAA", p.GetString( "/bbb", 0 ) );
		Assert::AreEqual( "BBB", p.GetString( "/bbb", 1 ) );
		Assert::AreEqual( "CCC", p.GetString( "/bbb", 2 ) );
		Assert::AreEqual( nullptr, p.GetString( "/bbb", 3 ) );
	}

	TEST_METHOD( testNotEnoughParameters )
	{
		SystemOutputParseResult msg;

		CmdParser4Cpp p( "/", msg );
		p.Accept( "/b" ).AsBoolean( 2 );
		p.Accept( "/c" ).AsBoolean( 1 );
		Assert::IsFalse( p.Parse( std::vector<std::string>( { "/b", "true", "/c", "false" } ) ) );
		Assert::IsTrue( strstr( msg.GetParseResult().c_str(), "Not enough parameters, argument '/b'" ) != nullptr );
	}

	TEST_METHOD( testNotEnoughParameters2 )
	{
		SystemOutputParseResult msg;

		CmdParser4Cpp p( "/", msg );
		p.Accept( "/a" ).AsBoolean( 1 );
		p.Accept( "/b" ).AsBoolean( 3 );
		p.Accept( "/c" ).AsBoolean( 1 );
		Assert::IsFalse( p.Parse( std::vector<std::string>( { "/b", "true", "false", "/a", "true", "/c", "false" } ) ) );
		Assert::IsTrue( strstr( msg.GetParseResult().c_str(), "Not enough parameters, argument '/b' requires 3" ) != nullptr );
	}

	TEST_METHOD( testMultipleMulti )
	{
		SystemOutputParseResult msg;

		CmdParser4Cpp p( "/", msg );
		p.Accept( "/multi1" ).AsBoolean( 1, 3 );
		p.Accept( "/multi2" ).AsBoolean( 1, 3 );
		Assert::IsTrue( p.Parse( std::vector<std::string>( { "/multi1", "1", "0", "/multi2", "0", "true" } ) ) );
		Assert::AreEqual( true, p.GetBool( "/multi1", 0 ) );
		Assert::AreEqual( false, p.GetBool( "/multi1", 1 ) );
		Assert::AreEqual( false, p.GetBool( "/multi2", 0 ) );
		Assert::AreEqual( true, p.GetBool( "/multi2", 1 ) );
	}

	TEST_METHOD( testSameArgumentMultipleTimes )
	{
		SystemOutputParseResult msg;

		CmdParser4Cpp p( "/", msg );
		p.Accept( "/multi1" ).AsBoolean( 1, 3 );
		p.Accept( "/multi2" ).AsBoolean( 1, 3 );
		Assert::IsFalse( p.Parse( std::vector<std::string>( { "/multi1", "1", "/multi1", "1" } ) ) );

		Assert::IsTrue( strstr( msg.GetParseResult().c_str(), "The argument '/multi1' is specified multiple times" ) != nullptr );
	}

	TEST_METHOD( testDependsOnMissing )
	{
		SystemOutputParseResult msg;

		CmdParser4Cpp p( "/", msg );
		p.Accept( "-first" ).DependsOn( "-second" ).AsBoolean( 1 );
		p.Accept( "-second" ).AsBoolean( 1 );
		Assert::IsFalse( p.Parse( std::vector<std::string>( { "-first", "false" } ) ) );
		Assert::IsTrue( strstr( msg.GetParseResult().c_str(), "Argument '-first' depends on '-second', but the latter is missing" ) != nullptr );
	}

	TEST_METHOD( testDependsTwoWay )
	{
		SystemOutputParseResult msg;

		CmdParser4Cpp p( "-", msg );
		p.Accept( "-first" ).DependsOn( "-second" ).AsBoolean( 1 );
		p.Accept( "-second" ).DependsOn( "-first" ).AsBoolean( 1 );
		Assert::IsTrue( p.Parse( std::vector<std::string>( { "-first", "false", "-second", "true" } ) ) );
		Assert::AreEqual( 0, static_cast<int>(msg.GetParseResult().length()) );
	}

	TEST_METHOD( testDependsTwoWayFail )
	{
		SystemOutputParseResult msg;

		CmdParser4Cpp p( "-", msg );
		p.Accept( "-first" ).DependsOn( "-second" ).AsBoolean( 1 );
		p.Accept( "-second" ).DependsOn( "-first" ).AsBoolean( 1 );
		Assert::IsFalse( p.Parse( std::vector<std::string>( { "-second", "true" } ) ) );
		Assert::IsTrue( strstr( msg.GetParseResult().c_str(), "Argument '-second' depends on '-first', but the latter is missing" ) != nullptr );
	}

	TEST_METHOD( testDependsOnProgrammingError )
	{
		SystemOutputParseResult msg;

		CmdParser4Cpp p( "-", msg );
		p.Accept( "-first" ).DependsOn( "-second" ).AsBoolean( 1 );
		Assert::IsFalse( p.Parse( std::vector<std::string>( { "-first", "false" } ) ) );
		Assert::IsTrue( strstr( msg.GetParseResult().c_str(), "Argument '-first' depends on '-second', but no such argument is defined - contact the author of the application" ) != nullptr );
	}

	TEST_METHOD( testBlockedByOK )
	{
		SystemOutputParseResult msg;

		CmdParser4Cpp p( "-", msg );
		p.Accept( "-first" ).BlockedBy( "-second" ).AsSingleBoolean();
		p.Accept( "-second" ).BlockedBy( "-first" ).AsSingleBoolean();
		Assert::IsTrue( p.Parse( std::vector<std::string>( { "-first" } ) ) );
	}

	TEST_METHOD( testBlockedByFAIL )
	{
		SystemOutputParseResult msg;

		CmdParser4Cpp p( "-", msg );
		p.Accept( "-first" ).BlockedBy( "-second" ).AsSingleBoolean();
		p.Accept( "-second" ).BlockedBy( "-first" ).AsSingleBoolean();
		Assert::IsFalse( p.Parse( std::vector<std::string>( { "-first", "-second" } ) ) );
		Assert::IsTrue( strstr( msg.GetParseResult().c_str(), "mutually exclusive" ) != nullptr );
	}

	TEST_METHOD( testGitExample )
	{
		SystemOutputParseResult msg;

		CmdParser4Cpp p( "-", msg );
		p.Accept( "-argument" ).AsBoolean( 1 ).SetMandatory().DescribedAs( "An argument that accept a single boolean parameter" );
		p.Accept( "-multi" ).AsString( 1, 4 ).DescribedAs( "An optional argument that accept one to four argument." );
		// The name of the argument, or any prefix characters, doesn't really matter, here we use double dash.
		p.Accept( "--otherArgument" ).WithAlias( std::vector<std::string>( { "-o", "-O" } ) ).AsSingleBoolean().DescribedAs( "An optional argument that takes no parameters" );
		// Arguments with variable parameters are only accepted as the last argument on the commandline.
		Assert::IsTrue( p.Parse( std::vector<std::string>( { "-argument", "true", "-O", "-multi", "parameter1", "parameter2", "parameter3" } ) ) );
		// Verify the number of parameters that can be read for the different arguments.
		Assert::AreEqual( 1, p.GetAvailableBooleanParameterCount( "--otherArgument" ) );
		Assert::AreEqual( 3, p.GetAvailableStringParameterCount( "-multi" ) );
		Assert::AreEqual( 1, p.GetAvailableBooleanParameterCount( "-argument" ) );

		// Read the values from the parser.
		Assert::AreEqual( true, p.GetBool( "-argument", 0 ) );
		Assert::AreEqual( "parameter1", p.GetString( "-multi", 0 ) );
		Assert::AreEqual( "parameter2", p.GetString( "-multi", 1 ) );
		Assert::AreEqual( "parameter3", p.GetString( "-multi", 2 ) );
		Assert::AreEqual( nullptr, p.GetString( "-multi", 4 ) );

		SystemOutputUsageFormatter usage( "MyCmdString" );
		p.GetUsage( usage );

		std::cout << usage.ToString();
	}

	TEST_METHOD( testGitExample2 )
	{
		SystemOutputParseResult msg;

		CmdParser4Cpp p( "-", msg );

		p.Accept( "-argument" ).AsBoolean( 1 ).SetMandatory().DescribedAs( "An argument that accept a single boolean parameter" );
		p.Accept( "-multi" ).AsString( 1, 4 ).DescribedAs( "An optional argument that accept one to four argument." );

		// Missing mandatory argument
		Assert::IsFalse( p.Parse( std::vector<std::string>( { "-multi", "parameter1", "parameter2", "parameter3" } ) ) );

		std::cout << msg.GetParseResult();
	}

	TEST_METHOD( testHideFromUsage )
	{

	}
};
}