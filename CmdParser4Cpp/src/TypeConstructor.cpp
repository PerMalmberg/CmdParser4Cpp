// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#include <CmdParser4Cpp/NumericLimit.h>
#include <CmdParser4Cpp/UnboundIntegerLimit.h>
#include <CmdParser4Cpp/UnboundStringLength.h>
#include "CmdParser4Cpp/TypeConstructor.h"
#include "Argument.h"
#include "StringType.h"
#include "BoolType.h"
#include "SingleBoolType.h"
#include "IntegerType.h"

namespace cmdparser4cpp {

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
TypeConstructor::TypeConstructor( Argument& argument, CmdParser4Cpp& parser )
		: myParser( parser ),
		  myArgument( argument )
{
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
TypeConstructor::~TypeConstructor()
{
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
const Constructor
TypeConstructor::AsString( int parameterCount ) const
{
	return AsString( parameterCount, UnboundStringLength() );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
const Constructor
TypeConstructor::AsString( int parameterCount, const StringLengthLimit& lengths ) const
{
	return AsString( parameterCount, parameterCount, lengths );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
const Constructor
TypeConstructor::AsString( int minimumParameterCount, int maximumParameterCount,
                           const StringLengthLimit& lengths ) const
{
	myArgument.SetArgumentType( new StringType( myParser, myArgument, minimumParameterCount, maximumParameterCount, std::unique_ptr<Limit<int>>( new NumericLimit<int>( lengths.GetLower(), lengths.GetUpper() ) ) ) );
	return Constructor( myArgument );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
const Constructor
TypeConstructor::AsBoolean( int parameterCount ) const
{
	return AsBoolean( parameterCount, parameterCount );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
const Constructor
TypeConstructor::AsBoolean( int minimumParameterCount, int maximumParameterCount ) const
{
	myArgument.SetArgumentType( new BoolType( myParser, myArgument, minimumParameterCount, maximumParameterCount ) );
	return Constructor( myArgument );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
const Constructor
TypeConstructor::AsSingleBoolean() const
{
	myArgument.SetArgumentType( new SingleBoolType( myParser, myArgument ) );
	return Constructor( myArgument );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
const Constructor
TypeConstructor::AsInteger( int parameterCount ) const
{
	return AsInteger( parameterCount, parameterCount, UnboundIntegerLimit() );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
const Constructor
TypeConstructor::AsInteger( int parameterCount, const NumericLimit<int>& limits ) const
{
	return AsInteger( parameterCount, parameterCount, limits );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
const Constructor
TypeConstructor::AsInteger( int minimumParameterCount, int maximumParameterCount,
                            const NumericLimit<int>& limits ) const
{
	myArgument.SetArgumentType( new IntegerType( myParser, myArgument, minimumParameterCount, maximumParameterCount, std::unique_ptr<Limit<int>>( new NumericLimit<int>( limits ) ) ) );
	return Constructor( myArgument );
}


} // END cmdparser4cpp