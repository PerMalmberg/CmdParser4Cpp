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
		IParseResult& msg = SystemOutputParseResult();

		CmdParser4Cpp& p = CmdParser4Cpp( "-", msg );
		p.Accept( "-m" ).AsString( 2 );

		Assert::IsTrue( p.Parse( std::vector<std::string>( { "-m", "", "one", "two" } ) ) );
		Assert::AreEqual( 2, p.GetAvailableStringParameterCount( "-m" ) );

		Assert::AreEqual( "one", p.GetString( "-m" ) );
		Assert::AreEqual( "two", p.GetString( "-m", 1 ) );

		Assert::AreEqual( nullptr, p.GetString( "-multi", 2 ) );
	}


	TEST_METHOD( testSpecifiedMultipleTimes )
	{
		IParseResult& msg = SystemOutputParseResult();

		CmdParser4Cpp& p = CmdParser4Cpp( "-", msg );
		p.Accept( "-q" ).AsString( 1 ).WithAlias( "-Q" );
		Assert::IsFalse( p.Parse( std::vector<std::string>( { "-q", "Foo", "-Q", "Bar" } ) ) );
		std::string& s = msg.GetParseResult();
		Assert::IsTrue( strstr( s.c_str(), "The argument '-q' is specified multiple times." ) != nullptr );
	}

	TEST_METHOD( testMissingMandatory )
	{
		IParseResult& msg = SystemOutputParseResult();

		CmdParser4Cpp& p = CmdParser4Cpp( "-", msg );

		p.Accept( "-q" ).AsString( 1 ).SetMandatory();
		p.Accept( "-Q" ).AsString( 1 );
		Assert::IsFalse( p.Parse( std::vector<std::string>( { "-Q", "Bar" } ) ) );
		std::string& s = msg.GetParseResult();
		Assert::IsTrue( strstr( s.c_str(), "The mandatory argument" ) != nullptr );
	}

	TEST_METHOD( testLeftOvers )
	{
		IParseResult& msg = SystemOutputParseResult();

		CmdParser4Cpp& p = CmdParser4Cpp( "-", msg );
		p.Accept( "-Q" ).AsString( 1 );
		Assert::IsFalse( p.Parse( std::vector<std::string>( { "-Q", "Bar", "some", "extra" } ) ) );
		std::string& s = msg.GetParseResult();
		Assert::IsTrue( strstr( s.c_str(), "Unknown arguments: some extra" ) != nullptr );
	}

	TEST_METHOD( testBoolean )
	{
		IParseResult& msg = SystemOutputParseResult();

		CmdParser4Cpp& p = CmdParser4Cpp( "/", msg );

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
		IParseResult& msg = SystemOutputParseResult();

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
		IParseResult& msg = SystemOutputParseResult();

		CmdParser4Cpp& p = CmdParser4Cpp( "/", msg );

		p.Accept( "/b" ).AsBoolean( 4 );
		Assert::IsFalse( p.Parse( std::vector<std::string>( { "/b", "true", "1", "Nope", "false" } ) ) );
		std::string& s = msg.GetParseResult();
		Assert::IsTrue( strstr( s.c_str(), "Failed to parse argument '/b'" ) != nullptr );
	}

	TEST_METHOD( testMissingParameters )
	{
		IParseResult& msg = SystemOutputParseResult();

		CmdParser4Cpp& p = CmdParser4Cpp( "/", msg );
		p.Accept( "/b" ).AsBoolean( 4 );
		Assert::IsFalse( p.Parse( std::vector<std::string>( { "/b", "true", "1", "Nope" } ) ) );
		std::string& s = msg.GetParseResult();
		Assert::IsTrue( strstr( s.c_str(), "Not enough parameters, argument '/b'" ) != nullptr );
	}

	TEST_METHOD( testMissingParameters2 )
	{
		IParseResult& msg = SystemOutputParseResult();

		CmdParser4Cpp& p = CmdParser4Cpp( "/", msg );
		p.Accept( "/b" ).AsBoolean( 4 );
		Assert::IsFalse( p.Parse( std::vector<std::string>( { "/b" } ) ) );
		std::string& s = msg.GetParseResult();
		Assert::IsTrue( strstr( s.c_str(), "Not enough parameters, argument" ) != nullptr );
	}

	TEST_METHOD( testNoInputWithMandatory )
	{
		IParseResult& msg = SystemOutputParseResult();

		CmdParser4Cpp& p = CmdParser4Cpp( "/", msg );
		p.Accept( "/b" ).AsBoolean( 1 ).SetMandatory();
		Assert::IsFalse( p.Parse( std::vector<std::string>( { "" } ) ) );
		std::string& s = msg.GetParseResult();
		Assert::IsTrue( strstr( s.c_str(), "The mandatory argument" ) != nullptr );
	}

	TEST_METHOD( testDescription )
	{
		IParseResult& msg = SystemOutputParseResult();

		CmdParser4Cpp& p = CmdParser4Cpp( "/", msg );
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



	TEST_METHOD( testHidFromUsage )
	{

	}
};
}