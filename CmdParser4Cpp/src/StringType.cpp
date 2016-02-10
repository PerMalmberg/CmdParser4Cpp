// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file. 

#include "StringType.h"
#include "CmdParser4Cpp.h"
#include "Argument.h"

namespace com {
namespace codezeal {
namespace commandline {

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
StringType::StringType( CmdParser4Cpp& parser, Argument& argument, int minParameterCount, int maxParameterCount )
	: BaseType( parser, argument, minParameterCount, maxParameterCount )
{
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
StringType::~StringType()
{
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool
StringType::DoTypeParse( const std::string& parameter )
{
	bool res = parameter.length() > 0;

	if( res ) {
		myResults.push_back( parameter );
	}

	return res;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void
StringType::RetrieveResult()
{
	myParser.SetResult( myArgument.GetPrimaryName(), this );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
const char*
StringType::GetResult( int index, const char* defaultValue ) const
{
	// If we want to use a single return, we're forced to cast away
	// the constness of the underlying std::string in myResults
	// and that is worse than two returns.

	if( myResults.size() >= index && index <= myResults.size() ) {
		return myResults.at( index ).c_str();
	}
	else {
		return defaultValue;
	}
}

} // END commandline
} // END codezeal
} // com