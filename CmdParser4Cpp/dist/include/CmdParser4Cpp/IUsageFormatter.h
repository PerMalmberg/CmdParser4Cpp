// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include <string>
#include <vector>

namespace cmdparser4cpp {

class CmdParser4Cpp;
class Argument;

class IUsageFormatter 
{
public:
	virtual ~IUsageFormatter() {}

	virtual void PrepareMandatory( const std::string& primaryName, bool hasVariableParameterCount, int maxArgumentCount, const std::vector<std::string>& aliases, const std::vector<std::string>& dependencyNames, const std::string& description ) = 0;
	virtual void PrepareNonMandatory( const std::string& primaryName, bool hasVariableParameterCount, int maxArgumentCount, const std::vector<std::string>& aliases, const std::vector<std::string>& dependencyNames, const std::string& description ) = 0;
	virtual std::string ToString() const = 0;
};

} // END cmdparser4cpp