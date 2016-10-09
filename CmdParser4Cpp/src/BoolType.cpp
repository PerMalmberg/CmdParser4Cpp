// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#include "BoolType.h"

namespace cmdparser4cpp {

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
BoolType::BoolType( CmdParser4Cpp& parser, Argument& argument, int minParameterCount, int maxParameterCount )
	: BaseType( parser, argument, minParameterCount, maxParameterCount, std::unique_ptr<Limit<bool>>( new Limit<bool>( false, true ) ) )
{
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
BoolType::~BoolType()
{
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool
BoolType::DoTypeParse( const std::string& parameter )
{
	size_t count = myResults.size();

	// Push ONE or ZERO to the results, we use ONE to compare in GetResult()
	if( icompare( ZERO, parameter ) || icompare( ONE, parameter ) ) {
		myResults.push_back( icompare( ONE, parameter ) ? ONE : ZERO );
	}
	else if( icompare( TRUE, parameter ) || icompare( FALSE, parameter ) ) {
		myResults.push_back( icompare( TRUE, parameter ) ? ONE : ZERO );
	}

	// Any new results added?
	return count < myResults.size();
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool
BoolType::icompare( const std::string& s, const std::string& s2 )
{
	return std::equal( s.begin(), s.end(), s2.begin(), icomparer );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void
BoolType::RetrieveResult()
{
	myParser.SetResult( myArgument.GetPrimaryName(), this );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool
BoolType::GetResult( int index, bool defaultValue ) const
{
	bool res = defaultValue;

	if( index >= 0 && index < static_cast<int>( myResults.size() ) ) {
		return myResults.at( static_cast<size_t>( index ) ).compare( ONE ) == 0;
	}

	return res;
}

} // END cmdparser4cpp