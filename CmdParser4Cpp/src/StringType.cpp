// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file. 

#include "StringType.h"
#include "Argument.h"

namespace cmdparser4cpp {

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
StringType::StringType( CmdParser4Cpp& parser, Argument& argument, int minParameterCount, int maxParameterCount,
                        std::unique_ptr<Limit<int>> limit )
		: BaseType( parser, argument, minParameterCount, maxParameterCount, std::move( limit ) )
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

	if( res )
	{
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
bool
StringType::CheckLimits()
{
	bool res = true;

	for( size_t i = 0; res && i < myResults.size(); ++i )
	{
		// As we are looping only valid indexes we don't ever expect to receive the default value
		const char* v = GetResult( i, "" );
		std::string s( v );
		int len = static_cast<int>( s.length() );
		res = len >= myLimit->GetLower() && len <= myLimit->GetUpper();
		if( !res ) {
			myParser.GetMessagerParser().InvalidStringLength( myArgument.GetPrimaryName(),  myLimit->GetLower(), myLimit->GetUpper() );
		}
	}

	return res;
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

	if( index >= 0 && index < static_cast<int>( myResults.size() ) )
	{
		return myResults.at( static_cast<size_t>( index ) ).c_str();
	}
	else
	{
		return defaultValue;
	}
}

} // END cmdparser4cpp