#pragma once

#include <string>
#include <vector>

namespace com {
namespace codezeal {
namespace commandline {

class CmdParser4Cpp;
class Argument;

class IUsageFormatter 
{
public:
	virtual ~IUsageFormatter() {}

	virtual void PrepareMandatory( const std::string& primaryName, bool hasVariableParameterCount, int maxArgumentCount, const std::vector<std::string>& aliases, const std::string& description ) = 0;
	virtual void PrepareNonMandatory( const std::string& primaryName, bool hasVariableParameterCount, int maxArgumentCount, const std::vector<std::string>& aliases, const std::string& description ) = 0;
	virtual std::string ToString() const = 0;
};

} // END commandline
} // END codezeal
} // com