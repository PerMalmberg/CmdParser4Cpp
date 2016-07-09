// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file. 

#pragma once

#include <vector>
#include <string>

namespace cmdparser4cpp {

// Interface used by the parser to notify the application of parse errors/events.
class IParseResult {
public:
	virtual ~IParseResult() = 0;

	virtual std::string GetParseResult() const = 0;

	virtual void UnknownArguments( const std::vector<std::string>& leftovers ) = 0;
	virtual void NotEnoughParameters( const std::string& argument, int minParameterCount ) = 0;
	virtual void FailedToParseArgument( const std::string& argument ) = 0;
	virtual void ArgumentSpecifiedMultipleTimes( const std::string& argument ) = 0;
	virtual void MissingMandatoryArgument( const std::string& argument ) = 0;
	virtual void NoSuchArgumentDefined( const std::string& argument, const std::string& dependsOn ) = 0;
	virtual void MissingDependentArgument( const std::string& argument, const std::string& dependsOn ) = 0;
	virtual void NoSuchMutuallyExclusiveArgumentDefined( const std::string& argument, const std::string& missing ) = 0;
	virtual void ArgumentsAreMutuallyExclusive( const std::string& argument, const std::string& blocker ) = 0;
	virtual void ArgumentMissingType( const std::string& argument ) = 0;
	virtual void FailedToLoadConfiguration( const std::string& fileNameArgument ) = 0;
	//virtual void InvalidStringLength( const std::string& argument, int lower, int upper );
	//virtual void InvalidParameterValue( const std::string& argument, int lower, int upper );
};

inline IParseResult::~IParseResult() {}

} // END cmdparser4cpp