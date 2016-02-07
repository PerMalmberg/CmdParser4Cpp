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
	std::string GetPrimaryName() const;
	void AddAlias( const std::string& alias ) { myNames.push_back( alias ); }

private:
	BaseType* myType;
	bool myExistsOnCommandLine;
	VectorOfString myNames;
	
	Argument( const Argument& ) = delete;
	Argument& operator=( const Argument& ) = delete;
};

} // END commandline
} // END codezeal
} // com