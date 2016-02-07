#pragma once
#include "IParseResult.h"

namespace com {
namespace codezeal {
namespace commandline {

class SystemOutputParseResult : public IParseResult
{
public:
	SystemOutputParseResult();
	~SystemOutputParseResult();

	std::string GetParseResult() override;

	void UnknownArguments( std::vector<std::string> leftovers ) override;
	void NotEnoughParameters( std::string argument, int minParameterCount ) override;
	void FailedToParseArgument( std::string argument ) override;

private:
	std::vector<std::string> myLines;
};

} // END commandline
} // END codezeal
} // com