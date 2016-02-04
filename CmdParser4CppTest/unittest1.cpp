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
		const IParseResult& msg = SystemOutputParseResult();

		CmdParser4Cpp& p = CmdParser4Cpp( "-", msg );
		p.Accept( "-m" ).AsString( 2 );
		/*assertTrue( p.parse( "-m", VAR_OPT_SOME_FOLDER_FOO, OTHER_PATH ) );
		assertEquals( 2, p.getAvailableStringParameterCount( "-m" ) );
		assertEquals( VAR_OPT_SOME_FOLDER_FOO, p.getString( "-m" ) );
		assertEquals( OTHER_PATH, p.getString( "-m", 1 ) );

		assertEquals( null, p.getString( "-multi", 2 ) );*/

	}

};
}