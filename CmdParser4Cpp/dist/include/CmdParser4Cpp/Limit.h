// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include <string>
#include "IParseResult.h"

namespace cmdparser4cpp {

template<typename T>
class Limit
{
public:
	Limit( T lower, T upper )
			: myLower( lower ), myUpper( upper )
	{
	}

	virtual ~Limit()
	{ }

	T GetLower() const
	{
		return myLower;
	}

	T GetUpper() const
	{
		return myUpper;
	}

private:
	T myLower;
	T myUpper;
};

}