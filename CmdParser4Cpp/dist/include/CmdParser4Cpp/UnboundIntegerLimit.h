// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include <limits>
#include "NumericLimit.h"

namespace cmdparser4cpp {

class UnboundIntegerLimit : public NumericLimit<int>
{
public:
	UnboundIntegerLimit()
			:NumericLimit(std::numeric_limits<int>::min(), std::numeric_limits<int>::max())
	{
	}
};

}