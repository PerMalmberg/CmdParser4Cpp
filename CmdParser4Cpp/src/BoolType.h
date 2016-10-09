// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include <cctype>
#include "BaseType.h"
#include "CmdParser4Cpp/CmdParser4Cpp.h"
#include "Argument.h"

namespace cmdparser4cpp {

class BoolType : public BaseType<bool>
{
public:
	static constexpr const char* ZERO{"0"};
	static constexpr const char* ONE{"1"};
	static constexpr const char* TRUE{"true"};
	static constexpr const char* FALSE{"false"};

	bool DoTypeParse( const std::string& parameter ) override;

	bool GetResult( int index, bool defaultValue ) const;

	bool CheckLimits() override
	{
		// A boolean type can never be outside limits
		return true;
	}

	BoolType( CmdParser4Cpp& parser, Argument& argument, int minParameterCount, int maxParameterCount );

	~BoolType();

protected:
	void RetrieveResult() override;

private:
	bool icompare( const std::string& s, const std::string& s2 );

	static bool icomparer( char c, char c2 )
	{ return std::tolower( c ) == std::tolower( c2 ); }

	BoolType( const BoolType& ) = delete;

	BoolType& operator=( const BoolType& ) = delete;
};


} // END cmdparser4cpp