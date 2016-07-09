// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.

#pragma once

#include <string>
#include "Limit.h"
#include "IParseResult.h"

namespace cmdparser4cpp {

class StringLengthLimit : public Limit<int>
{
public:
	StringLengthLimit( int lower, int upper )
			: Limit( lower, upper )
	{
	}
};

}
