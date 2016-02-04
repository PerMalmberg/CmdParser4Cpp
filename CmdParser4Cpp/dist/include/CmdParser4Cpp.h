#pragma once

#include <string>
#include <unordered_map>
#include "IParseResult.h"
#include "Constructor.h"

namespace com {
namespace codezeal {
namespace commandline {

class CmdParser4Cpp
{
public:
	CmdParser4Cpp( const char* argumentPrefix, const IParseResult& parseResult );

	virtual ~CmdParser4Cpp();

	// Defines an argument with the provided argument name.
	// Use the returned Constructor object to further define the argument properties.
	const Constructor Accept( std::string argumentName );

	// Parses the command line arguments
	void Parse( int argc, char* argv[] );
	
private:
	std::string myArgumentPrefix;
	const IParseResult& myParseResult;
	std::unordered_map<std::string, Argument*> myArguments;

	// Precent copying
	CmdParser4Cpp( const CmdParser4Cpp& ) = delete;
	CmdParser4Cpp& operator=( const CmdParser4Cpp& ) = delete;
};

} // END commandline
} // END codezeal
} // com