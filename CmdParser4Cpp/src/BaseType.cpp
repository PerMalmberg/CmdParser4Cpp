#include "BaseType.h"

namespace com {
namespace codezeal {
namespace commandline {

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
BaseType::BaseType( CmdParser4Cpp& parser, Argument& argument, int minParameterCount, int maxParameterCount )
	: myParser( parser ),
	myArgument( argument ),
	myMinParameterCount( minParameterCount ),
	myMaxParameterCount( maxParameterCount ),
	myResults()
{
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool
BaseType::Parse( VectorOfString& allArguments, int argumentIx )
{
	// Save and remove the argument name
	std::string argumentName = allArguments.at( argumentIx );
	allArguments.erase( allArguments.begin() + argumentIx );

	bool res = HasEnoughParametersLeft( allArguments, argumentIx );

	if( res ) {
		// We only do this if the current type takes at least one parameter
		for( int currentParameter = 0;
		res && currentParameter < myMaxParameterCount // Don't take too many parameters
			&& allArguments.size() > 0 // Still some left
			&& argumentIx < allArguments.size() // Not yet reached end
			;
			++currentParameter
			) {

			// Get the next parameter from the front, i.e. where our parameters start.
			std::string parameter = allArguments.at( argumentIx );
			allArguments.erase( allArguments.begin() + argumentIx );
			res = DoTypeParse( parameter );
		}
	}
	else {
		myParser.GetMessagerParser().NotEnoughParameters( argumentName, myMinParameterCount );
	}

	res = IsSucessfullyParsed();

	if( res ) {
		RetrieveResult();
	}
	else {
		myParser.GetMessagerParser().FailedToParseArgument( argumentName );
	}

	return res;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool
BaseType::IsSucessfullyParsed() const
{
	return GetAvailableParameterCount() >= myMinParameterCount && GetAvailableParameterCount() <= myMaxParameterCount;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool
BaseType::HasEnoughParametersLeft( VectorOfString arguments, int argumentIx )
{
	// Calculate remaining arguments until the prefix is found
	int remaining = 0;

	bool done = false;

	for( int i = argumentIx; !done && i < arguments.size(); ++i ) {
		if( strncmp( arguments.at( i ).c_str(), myParser.GetArgumentPrefix().c_str(), myParser.GetArgumentPrefix().length() ) == 0 ) {
			done = true;
		}
		else {
			++remaining;
		}
	}

	return remaining >= myMinParameterCount;
}

} // END commandline
} // END codezeal
} // com

