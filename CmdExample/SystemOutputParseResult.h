// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file. 

#pragma once

#include "CmdParser4Cpp/IParseResult.h"

class SystemOutputParseResult : public cmdparser4cpp::IParseResult
{
public:
	SystemOutputParseResult();
	~SystemOutputParseResult();

	std::string GetParseResult() const override;

	void UnknownArguments( const std::vector<std::string>& leftovers ) override;
	void NotEnoughParameters( const std::string& argument, int minParameterCount ) override;
	void FailedToParseArgument( const std::string& argument ) override;
	void ArgumentSpecifiedMultipleTimes( const std::string& argument ) override;
	void MissingMandatoryArgument( const std::string& argument ) override;
	void NoSuchArgumentDefined( const std::string& argument, const std::string& dependsOn ) override;
	void MissingDependentArgument( const std::string& argument, const std::string& dependsOn ) override;
	void NoSuchMutuallyExclusiveArgumentDefined( const std::string& argument, const std::string& missing ) override;
	void ArgumentsAreMutuallyExclusive( const std::string& argument, const std::string& blocker ) override;
	void ArgumentMissingType( const std::string& argument ) override;
	void FailedToLoadConfiguration( const std::string& fileNameArgument ) override;
	void InvalidStringLength( const std::string& argument, int lower, int upper ) override;
	void InvalidParameterValue( const std::string& argument, int lower, int upper ) override;
private:
	std::vector<std::string> myLines;
};