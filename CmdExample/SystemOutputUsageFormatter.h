#pragma once

#include <sstream>
#include "IUsageFormatter.h"


using namespace cmdparser4cpp;

class SystemOutputUsageFormatter : public IUsageFormatter
{
public:
	explicit SystemOutputUsageFormatter( const std::string& header );
	~SystemOutputUsageFormatter();

	void PrepareMandatory( const std::string& primaryName, bool hasVariableParameterCount, int maxArgumentCount, const std::vector<std::string>& aliases, const std::string& description ) override;
	void PrepareNonMandatory( const std::string& primaryName, bool hasVariableParameterCount, int maxArgumentCount, const std::vector<std::string>& aliases, const std::string& description ) override;
	std::string ToString() const override;

private:
	std::stringstream myUsage;
	size_t myIndent;

	std::string Indent() const;
	void FormatArgument( const std::string& primaryName, bool hasVariableParameterCount, int maxArgumentCount, const std::vector<std::string>& aliases, const std::string& description );

	SystemOutputUsageFormatter( const SystemOutputUsageFormatter& ) = delete;
	SystemOutputUsageFormatter& operator=( const SystemOutputUsageFormatter& ) = delete;
};

