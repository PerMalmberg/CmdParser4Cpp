#include "../dist/Include/Constructor.h"
#include "Argument.h"
#include "../dist/Include/CmdParser4Cpp.h"
#include "BaseType.h"

namespace com {
namespace codezeal {
namespace commandline {


Constructor::Constructor( Argument& argument, CmdParser4Cpp& parser )
	: myParser(parser),
	myArgument(argument)
{
}


Constructor::~Constructor()
{
}

const Constructor& 
Constructor::AsString( int parameterCount ) const
{
	return AsString( parameterCount, parameterCount );
}

const Constructor&
Constructor::AsString( int minimumParameterCount, int maximumParameterCount ) const
{
	BaseType<int> foo(myParser, myArgument, 1, 1);
	//qqq myArgument.SetType( )
	return *this;
}

} // END commandline
} // END codezeal
} // com