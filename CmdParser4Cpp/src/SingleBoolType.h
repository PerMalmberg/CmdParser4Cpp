// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file. 

#pragma once

#pragma once

#include <cctype>
#include "BoolType.h"
#include "CmdParser4Cpp.h"
#include "Argument.h"

namespace com {
namespace codezeal {
namespace commandline {

class SingleBoolType : public BoolType
{
public:
	SingleBoolType( CmdParser4Cpp& parser, Argument& argument );
	bool IsSucessfullyParsed() const override;

private:
	SingleBoolType( const SingleBoolType& ) = delete;
	SingleBoolType& operator=( const SingleBoolType& ) = delete;

};


} // END commandline
} // END codezeal
} // com