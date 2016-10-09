// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#pragma once

#include <cctype>
#include "BoolType.h"
#include "CmdParser4Cpp/CmdParser4Cpp.h"
#include "Argument.h"

namespace cmdparser4cpp {

class SingleBoolType : public BoolType
{
public:
	SingleBoolType( CmdParser4Cpp& parser, Argument& argument );
	bool IsSucessfullyParsed() const override;

private:
	SingleBoolType( const SingleBoolType& ) = delete;
	SingleBoolType& operator=( const SingleBoolType& ) = delete;

};


} // END cmdparser4cpp