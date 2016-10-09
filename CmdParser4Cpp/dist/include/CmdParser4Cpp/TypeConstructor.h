// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include <string>
#include <vector>
#include "NumericLimit.h"
#include "StringLengthLimit.h"
#include "Constructor.h"

namespace cmdparser4cpp {

class CmdParser4Cpp;
class Argument;
typedef std::vector<std::string> VectorOfString;

// Class used for creating fluent initializations of the held argument.
// This class is intentionally copyable!
class TypeConstructor
{
public:
	TypeConstructor( Argument& argument, CmdParser4Cpp& parser );
	virtual ~TypeConstructor();

	const Constructor AsString( int parameterCount ) const;
	const Constructor AsString( int parameterCount, const StringLengthLimit& lengths ) const;
	const Constructor AsString( int minimumParameterCount, int maximumParameterCount, const StringLengthLimit& lengths ) const;

	const Constructor AsBoolean( int parameterCount ) const;
	const Constructor AsBoolean( int minimumParameterCount, int maximumParameterCount ) const;

	const Constructor AsSingleBoolean() const;

	const Constructor AsInteger( int parameterCount ) const;
	const Constructor AsInteger( int parameterCount, const NumericLimit<int>& limits ) const;
	const Constructor AsInteger( int minimumParameterCount, int maximumParameterCount, const NumericLimit<int>& limits ) const;


private:
	CmdParser4Cpp& myParser;
	Argument& myArgument;

	Constructor& operator=( const Constructor& ) = delete;
};

} // END cmdparser4cpp