// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include <string>
#include <vector>

namespace cmdparser4cpp {

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

} // END cmdparser4cpp