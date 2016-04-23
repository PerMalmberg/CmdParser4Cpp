#pragma once

#include "IParseResult.h"

using namespace com::codezeal::commandline;

class DummyResult : public IParseResult
{
public:
	DummyResult();
	~DummyResult();

	virtual std::string GetParseResult() const { return "Not implemented"; }

	virtual void UnknownArguments( const std::vector<std::string>& leftovers ) {}
	virtual void NotEnoughParameters( const std::string& argument, int minParameterCount ) {}
	virtual void FailedToParseArgument( const std::string& argument ) {}
	virtual void ArgumentSpecifiedMultipleTimes( const std::string& argument ) {}
	virtual void MissingMandatoryArgument( const std::string& argument ) {}
	virtual void NoSuchArgumentDefined( const std::string& argument, const std::string& dependsOn ) {}
	virtual void MissingDependentArgument( const std::string& argument, const std::string& dependsOn ) {}
	virtual void NoSuchMutuallyExclusiveArgumentDefined( const std::string& argument, const std::string& missing ) {}
	virtual void ArgumentsAreMutuallyExclusive( const std::string& argument, const std::string& blocker ) {}
};

