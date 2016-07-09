// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.

#pragma once

#include <Constructor.h>

namespace cmdparser4cpp {

class ITypeParser
{
public:
	virtual bool Parse( VectorOfString& allArguments, int argumentIx ) = 0;
	virtual bool HasVariableParameterCount() const = 0;
	virtual int GetMaxParameterCount() const = 0;
	virtual bool IsSucessfullyParsed() const = 0;

	virtual ~ITypeParser() {}
};

}