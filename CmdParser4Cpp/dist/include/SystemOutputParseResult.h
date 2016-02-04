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
};

} // END commandline
} // END codezeal
} // com