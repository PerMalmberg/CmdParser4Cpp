#pragma once

namespace com {
namespace codezeal {
namespace commandline {

// Interface used by the parser to notify the application of parse errors/events.
class IParseResult {
public:
	virtual ~IParseResult() = 0;
};

inline IParseResult::~IParseResult() {}

} // END commandline
} // END codezeal
} // com