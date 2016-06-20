// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file. 

#pragma once

#include <cctype>
#include "BaseType.h"
#include "CmdParser4Cpp.h"
#include "Argument.h"
#include <regex>

namespace cmdparser4cpp {

class IntegerType : public BaseType
{
public:
	bool DoTypeParse( const std::string& parameter ) override;
	int GetResult( int index, int defaultValue ) const;

	IntegerType( CmdParser4Cpp& parser, Argument& argument, int minParameterCount, int maxParameterCount );
	~IntegerType();

protected:
	void RetrieveResult() override;

private:
	std::regex myMatcher;


	IntegerType( const IntegerType& ) = delete;
	IntegerType& operator=( const IntegerType& ) = delete;
};


} // END cmdparser4cpp