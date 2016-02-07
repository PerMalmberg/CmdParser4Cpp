#pragma once

#include <cctype>
#include "BaseType.h"
#include "CmdParser4Cpp.h"
#include "Argument.h"

namespace com {
namespace codezeal {
namespace commandline {

class BoolType : public BaseType
{
public:
	static const std::string ZERO;
	static const std::string ONE;
	static const std::string TRUE;
	static const std::string FALSE;

	bool DoTypeParse( const std::string& parameter ) override;
	bool GetResult( int index, bool defaultValue ) const;

	BoolType( CmdParser4Cpp& parser, Argument& argument, int minParameterCount, int maxParameterCount );
	~BoolType();

protected:
	void RetrieveResult() override;

private:
	bool icompare( const std::string& s, const std::string& s2 );
	static bool icomparer( char c, char c2 ) { return std::tolower( c ) == std::tolower( c2 ); }
};


} // END commandline
} // END codezeal
} // com