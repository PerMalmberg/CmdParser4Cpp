#pragma once

#include <vector>


namespace com {
namespace codezeal {
namespace commandline {

// Interface used by the parser to notify the application of parse errors/events.
class IParseResult {
public:
	virtual ~IParseResult() = 0;

	virtual std::string GetParseResult() = 0;

	virtual void UnknownArguments( std::vector<std::string> leftovers ) = 0;
	virtual void NotEnoughParameters( std::string argument, int minParameterCount ) = 0;
	virtual void FailedToParseArgument( std::string argument ) = 0;
};

inline IParseResult::~IParseResult() {}

} // END commandline
} // END codezeal
} // com