#pragma once

namespace com {
namespace codezeal {
namespace commandline {

template<typename T>
class BaseType
{
public:
	BaseType( CmdParser4Cpp& parser, Argument& argument, int minParameterCount, int maxParameterCount );

private:
	BaseType( const BaseType<T>& ) = delete;
};

template<typename T>
BaseType<T>::BaseType( CmdParser4Cpp& parser, Argument& argument, int minParameterCount, int maxParameterCount )
{

}


} // END commandline
} // END codezeal
} // com