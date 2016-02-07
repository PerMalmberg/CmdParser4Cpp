#include "Argument.h"

namespace com {
namespace codezeal {
namespace commandline {

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
Argument::Argument( const std::string& argumentName )
	: myType( 0 ),
	myExistsOnCommandLine( false ),
	myNames()
{
	myNames.push_back( argumentName );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
Argument::~Argument()
{
	if( myType != 0 ) {
		delete myType;
	}
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void Argument::SetArgumentType( BaseType* type )
{
	myType = type;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool
Argument::Parse( VectorOfString& arguments )
{
	bool result = true;
	int hits = 0;

	int ix = FindArgument( arguments, hits );

	if( ix >= 0 ) {
		// Argument found, parse it
		myExistsOnCommandLine = true;
		result = myType->Parse( arguments, ix );
	}

	return result;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int
Argument::FindArgument( VectorOfString& allArguments, int& hitCount )
{
	int foundIx = -1;

	hitCount = 0;

	// Loop all our names and test against all provided arguments.
	for( const auto& name : myNames ) {
		for( int ix = 0; ix < allArguments.size(); ++ix ) {
			if(  allArguments.at(ix).compare( name ) == 0 ) {
				if( foundIx == -1 ) {
					// First hit
					foundIx = ix;
				}

				++hitCount;
			}
		}
	}

	return foundIx;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
std::string
Argument::GetPrimaryName() const
{
	return myNames.at( 0 );
}

} // END commandline
} // END codezeal
} // com
