// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#include "SingleBoolType.h"
#include "BoolType.h"

namespace cmdparser4cpp {

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
SingleBoolType::SingleBoolType( CmdParser4Cpp& parser, Argument& argument )
	: BoolType( parser, argument, 0, 0 )
{
	// Add a result here as we won't parse any data with this type.
	myResults.push_back( ONE );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool SingleBoolType::IsSucessfullyParsed() const
{
	return true;
}


} // END cmdparser4cpp