// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file. 

#include "IntegerType.h"

namespace cmdparser4cpp {


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
IntegerType::IntegerType( CmdParser4Cpp& parser, Argument& argument, int minParameterCount, int maxParameterCount,
                          std::unique_ptr<Limit<int>> limit )
		: BaseType( parser, argument, minParameterCount, maxParameterCount, std::move( limit ) ),
		  myMatcher( R"!!(^([+-]{0,1}\d*)$)!!" )
{
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
IntegerType::~IntegerType()
{
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool
IntegerType::DoTypeParse( const std::string& parameter )
{
	size_t count = myResults.size();

	if( std::regex_match( parameter, myMatcher ) )
	{
		myResults.push_back( parameter );
	}

	// Any new results added?
	return count < myResults.size();
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void
IntegerType::RetrieveResult()
{
	myParser.SetResult( myArgument.GetPrimaryName(), this );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool
IntegerType::CheckLimits()
{
	bool res = true;

	for( int i = 0; res && i < static_cast<int>( myResults.size() ); ++i )
	{
		// As we are looping only valid indexes we don't ever expect to receive the default value
		int v = GetResult( i, 0 );
		res = v >= myLimit->GetLower() && v <= myLimit->GetUpper();
		if( !res ) {
			//myParser.GetMessagerParser().InvalidParameterValue( myArgument.GetPrimaryName(), myLimit->GetLower(), myLimit->GetUpper() );
		}
	}

	return res;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int
IntegerType::GetResult( int index, int defaultValue ) const
{
	int res = defaultValue;

	if( index >= 0 && index < static_cast<int>( myResults.size() ) )
	{
		res = atoi( myResults.at( static_cast<size_t>( index ) ).c_str() );
	}

	return res;
}

} // END cmdparser4cpp