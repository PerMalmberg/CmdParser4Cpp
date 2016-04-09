// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file. 

#include "BoolType.h"


namespace com {
namespace codezeal {
namespace commandline {

const std::string BoolType::ZERO( "0" );
const std::string BoolType::ONE( "1" );
const std::string BoolType::TRUE( "true" );
const std::string BoolType::FALSE( "false" );

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
BoolType::BoolType( CmdParser4Cpp& parser, Argument& argument, int minParameterCount, int maxParameterCount )
	: BaseType( parser, argument, minParameterCount, maxParameterCount )
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
	unsigned long int count = myResults.size();

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
		return myResults.at( index ).compare( ONE ) == 0;
	}

	return res;
}

} // END commandline
} // END codezeal
} // com