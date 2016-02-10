// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file. 

#pragma once

#include <string>
#include "BaseType.h"

namespace com {
namespace codezeal {
namespace commandline {

class CmdParser4Cpp;
class Argument;

class StringType : public BaseType
{
public:
	StringType( CmdParser4Cpp& parser, Argument& argument, int minParameterCount, int maxParameterCount );
	~StringType();

	const char* GetResult( int index, const char* defaultValue ) const;

protected:
	bool DoTypeParse( const std::string& parameter ) override;
	void RetrieveResult() override;

private:
};

} // END commandline
} // END codezeal
} // com