// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.

#pragma once

#include <limits>
#include "NumericLimit.h"
#include "StringLengthLimit.h"

namespace cmdparser4cpp {

class UnboundStringLength : public StringLengthLimit
{
public:
	UnboundStringLength()
			:StringLengthLimit(1, std::numeric_limits<int>::max())
	{
	}
};

}