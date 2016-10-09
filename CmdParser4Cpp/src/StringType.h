// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include <string>
#include "BaseType.h"

namespace cmdparser4cpp {

class CmdParser4Cpp;
class Argument;

class StringType : public BaseType<int>
{
public:
	StringType( CmdParser4Cpp& parser, Argument& argument, int minParameterCount, int maxParameterCount, std::unique_ptr<Limit<int>> limit );
	~StringType();

	const char* GetResult( int index, const char* defaultValue ) const;

protected:
	bool DoTypeParse( const std::string& parameter ) override;
	void RetrieveResult() override;
	bool CheckLimits() override;

private:
	StringType( const StringType& ) = delete;
	StringType& operator=( const StringType& ) = delete;
};

} // END cmdparser4cpp