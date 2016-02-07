#pragma once

#include <string>

namespace com {
namespace codezeal {
namespace commandline {

class CmdParser4Cpp;
class Argument;

// Class used for creating fluent initializations of the held argument.
// This class is intentionally copyable!
class Constructor
{
public:
	Constructor( Argument& argument, CmdParser4Cpp& parser );
	virtual ~Constructor();

	const Constructor& AsString( int parameterCount ) const;
	const Constructor& AsString( int minimumParameterCount, int maximumParameterCount ) const;

	const Constructor& WithAlias( const std::string& alias ) const;

private:
	CmdParser4Cpp& myParser;
	Argument& myArgument;
};

} // END commandline
} // END codezeal
} // com