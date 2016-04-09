#pragma once

#include "IUsageFormatter.h"

using namespace com::codezeal::commandline;

class DummyUsage : public IUsageFormatter
{
public:
	DummyUsage();
	~DummyUsage();

	virtual void PrepareMandatory( const std::string& primaryName, bool hasVariableParameterCount, int maxArgumentCount, const std::vector<std::string>& aliases, const std::string& description ) {}
	virtual void PrepareNonMandatory( const std::string& primaryName, bool hasVariableParameterCount, int maxArgumentCount, const std::vector<std::string>& aliases, const std::string& description ) {}
	virtual std::string ToString() { return ""; }
};

