// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.

#pragma once

#include <string>
#include "Limit.h"

namespace cmdparser4cpp {

template<typename T>
class NumericLimit : public Limit<T>
{
public:
	NumericLimit( T lower, T upper )
			: Limit<T>( lower, upper )
	{
	}
};

}
