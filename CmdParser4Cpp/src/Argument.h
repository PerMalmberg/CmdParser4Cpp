// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file. 

#pragma once

#include <typeinfo>
#include "BaseType.h"

namespace com {
namespace codezeal {
namespace commandline {

class Argument
{
public:
	Argument( const std::string& argumentName );
	virtual ~Argument();

	void SetArgumentType( BaseType* type );
	bool Parse( VectorOfString& arguments );

	int FindArgument( VectorOfString& arguments, int& hitCount );
	
	const std::string& GetPrimaryName() const;
	void AddAlias( const std::string& alias ) { myNames.push_back( alias ); }
	const VectorOfString GetAliases() const;

	bool HasVariableParameterCount() const { return myType->HasVariableParameterCount(); }
	int GetMaxParameterCount() const { return myType->GetMaxParameterCount(); }

	void SetMandatory() { myIsMandatory = true; }
	bool IsMandatory() const { return myIsMandatory; }
	bool IsSuccessFullyParsed() const { return myExistsOnCommandLine && myType->IsSucessfullyParsed(); }

	void SetDescription( const std::string& desc ) { myDescription = desc; }
	const std::string& GetDescription() const { return myDescription; }

private:
	BaseType* myType;
	bool myExistsOnCommandLine;
	VectorOfString myNames;
	bool myIsMandatory;
	std::string myDescription;
	
	Argument( const Argument& ) = delete;
	Argument& operator=( const Argument& ) = delete;
};

} // END commandline
} // END codezeal
} // com