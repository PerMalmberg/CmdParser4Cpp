// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include <cctype>
#include "BaseType.h"
#include "CmdParser4Cpp/CmdParser4Cpp.h"
#include "Argument.h"
#include <regex>

namespace cmdparser4cpp {

class IntegerType : public BaseType<int>
{
public:
	bool DoTypeParse( const std::string& parameter ) override;
	int GetResult( int index, int defaultValue ) const;

	IntegerType( CmdParser4Cpp& parser, Argument& argument, int minParameterCount, int maxParameterCount, std::unique_ptr<Limit<int>> limit );
	~IntegerType();

protected:
	void RetrieveResult() override;
	bool CheckLimits() override;

private:
	std::regex myMatcher;


	IntegerType( const IntegerType& ) = delete;
	IntegerType& operator=( const IntegerType& ) = delete;
};


} // END cmdparser4cpp