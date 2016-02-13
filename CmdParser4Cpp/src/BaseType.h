// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file. 

#pragma once

#include "CmdParser4Cpp.h"


namespace com {
namespace codezeal {
namespace commandline {

class BaseType
{
public:
	virtual bool Parse( VectorOfString& allArguments, int argumentIx );

	virtual ~BaseType() {}

	int GetAvailableParameterCount() const { return myResults.size(); }
	int GetMaxParameterCount() const { return myMaxParameterCount; }
	int HasVariableParameterCount() const { return myMinParameterCount != myMaxParameterCount; }
	virtual bool IsSucessfullyParsed() const;

protected:
	BaseType( CmdParser4Cpp& parser, Argument& argument, int minParameterCount, int maxParameterCount );

	virtual bool DoTypeParse( const std::string& parameter ) = 0;
	virtual void RetrieveResult() = 0;

	CmdParser4Cpp& myParser;
	Argument& myArgument;
	int myMinParameterCount;
	int myMaxParameterCount;
	VectorOfString myResults;

private:
	bool HasEnoughParametersLeft( VectorOfString arguments, int argumentIx );		

	BaseType( const BaseType& ) = delete;
	BaseType& operator=( const BaseType& ) = delete;
};

} // END commandline
} // END codezeal
} // com