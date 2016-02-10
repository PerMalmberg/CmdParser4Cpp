// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file. 

#include "CppUnitTest.h"
#include "CmdParser4Cpp.h"
#include "SystemOutputParseResult.h"


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

};
}