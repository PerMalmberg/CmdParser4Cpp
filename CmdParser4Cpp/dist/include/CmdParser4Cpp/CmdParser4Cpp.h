// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include <string>
#include <utility>
#include <unordered_map>
#include <memory>
#include "IParseResult.h"
#include <CmdParser4Cpp/TypeConstructor.h>
#include "IUsageFormatter.h"
#include "IConfigurationReader.h"

namespace cmdparser4cpp {

class BoolType;

class StringType;

class IntegerType;

typedef std::vector<std::string> VectorOfString;

class CmdParser4Cpp
{
public:
	explicit CmdParser4Cpp( IParseResult& parseResult );

	virtual ~CmdParser4Cpp();

	// Defines an argument with the provided argument name.
	// Use the returned Constructor object to further define the argument properties.
	const TypeConstructor Accept( const std::string& argumentName );

	// Parses the command line arguments
	bool Parse( const std::vector<std::string>& arguments, std::shared_ptr<IConfigurationReader> cfgReader = nullptr, const std::string& pathToConfigFile = "" );

	template<typename ArgumentType>
	void SetResult( const std::string& argumentName, const ArgumentType* result );

	IParseResult& GetMessagerParser() const { return myParseResult; }
	int GetAvailableStringParameterCount( const std::string& argumentName ) const;
	const char* GetString( const std::string& argumentName, int index = 0, const char* defaultValue = nullptr ) const;
	bool GetBool( const std::string& argumentName, int index = 0, bool defaultValue = false ) const;
	int GetInteger(const std::string& argumentName, int index = 0, int defaultValue = 0) const;
	int GetAvailableBooleanParameterCount( const std::string& argumentName ) const;
	int GetAvailableIntegerParameterCount( const std::string& argumentName ) const;

	template<typename ArgumentType, typename ValueType>
	ValueType GetValue(const std::string& argumentName, int index, ValueType defaultValue) const;

	void GetUsage( IUsageFormatter& formatter ) const;

private:
	IParseResult& myParseResult;
	std::unordered_map<std::string, std::shared_ptr<Argument>> myArguments;

	// Create an envelope class that - by multiple inheritance - provides a common access interface to all
	// the different argument types that we support. Using a struct instead of a class saves us from having to specify
	// a public inheritance for each base class.
	struct ResultEnvelope : std::unordered_map<std::string, const StringType*>,
	                        std::unordered_map<std::string, const BoolType*>,
	                        std::unordered_map<std::string, const IntegerType*>
	{
	};

	ResultEnvelope myResults;

	bool LoadConfigFile( const std::string& fileNameArgument, std::shared_ptr<IConfigurationReader> cfgReader );
	bool CheckArgumentTypes() const;
	void RemoveEmptyArguments( std::vector<std::string>& arguments );
	template<typename ArgumentType>
	int GetAvailableParameterCount(const std::string& argumentName) const;

	bool CheckConstraints(const VectorOfString& args);

	bool CheckMandatory() const;

	bool CheckDependencies() const;

	bool CheckMutualExclusion() const;

	bool FallbackToConfiguration( std::shared_ptr<IConfigurationReader> cfgReader );

	void GetIndexes(std::vector<std::pair<int, std::shared_ptr<Argument>>>& argumentIndexes,
	                const std::vector<std::string>& arguments);

	class IndexSorter
	{
	public:
		bool operator()(const std::pair<int, std::shared_ptr<Argument>>& lhs,
		                const std::pair<int, std::shared_ptr<Argument>>& rhs)
		{
			return lhs.first < rhs.first;
		}
	};

	// Prevent copying
	CmdParser4Cpp(const CmdParser4Cpp&) = delete;

	CmdParser4Cpp& operator=(const CmdParser4Cpp&) = delete;
};

template<typename ArgumentType>
void CmdParser4Cpp::SetResult( const std::string& argumentName, const ArgumentType* result )
{
	std::unordered_map<std::string, const ArgumentType*>& map = myResults;
	map.insert( { argumentName, result } );
}

} // END cmdparser4cpp