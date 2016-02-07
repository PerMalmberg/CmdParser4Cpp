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
		std::string s = msg.GetParseResult();
		Assert::IsTrue( strstr( s.c_str(), "The argument '-q' is specified multiple times." ) != 0 );
	}

};
}