// Command Parser4Cpp
// Copyright (C) Per Malmberg 2016

#include "../dist/include/CmdParser4Cpp.h"
#include "Argument.h"

namespace com {
namespace codezeal {
namespace commandline {

// This is the constructor of a class that has been exported.
// see CmdParser4Cpp.h for the class definition
CmdParser4Cpp::CmdParser4Cpp( const char* argumentPrefix, const IParseResult& parseResult )
	: myArgumentPrefix( argumentPrefix ),
	myParseResult( parseResult ),
	myArguments()
{
	
}

const Constructor 
CmdParser4Cpp::Accept( std::string argumentName )
{
	Argument* a = new Argument();
	myArguments.insert( { argumentName, a } );
	Constructor c( *a, *this );

	// Return a copy for further use.
	return c;
}

CmdParser4Cpp::~CmdParser4Cpp()
{
	// Deallocate our arguments
	for( auto& pair : myArguments ) {
		delete pair.second;
	}
}

void 
CmdParser4Cpp::Parse( int argc, char * argv[] )
{
}


} // END commandline
} // END codezeal
} // com