// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file. 

#pragma once

#include <string>
#include <vector>

namespace com {
namespace codezeal {
namespace commandline {

class CmdParser4Cpp;
class Argument;
typedef std::vector<std::string> VectorOfString;

// Class used for creating fluent initializations of the held argument.
// This class is intentionally copyable!
class Constructor
{
public:
	static const int NO_PARAMETER_LIMIT;

	Constructor( Argument& argument, CmdParser4Cpp& parser );
	virtual ~Constructor();

	const Constructor& AsString( int parameterCount ) const;
	const Constructor& AsString( int minimumParameterCount, int maximumParameterCount ) const;

	const Constructor& AsBoolean( int parameterCount ) const;
	const Constructor& AsBoolean( int minimumParameterCount, int maximumParameterCount ) const;

	const Constructor& AsSingleBoolean() const;

	const Constructor& WithAlias( const std::string& alias ) const;
	const Constructor& WithAlias( const VectorOfString& alias ) const;
	const Constructor& SetMandatory() const;
	const Constructor& DescribedAs( const std::string& desc ) const;
	const Constructor& DependsOn( const std::string& primaryName ) const;
	const Constructor& BlockedBy( const std::string& primaryName ) const;
	const Constructor& SetHidden() const;

private:
	CmdParser4Cpp& myParser;
	Argument& myArgument;

	Constructor& operator=( const Constructor& ) = delete;
};

} // END commandline
} // END codezeal
} // com