// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file. 

#pragma once

#include <string>
#include <utility>
#include <unordered_map>
#include <memory>
#include "IParseResult.h"
#include "Constructor.h"
#include "IUsageFormatter.h"

namespace cmdparser4cpp {

class BoolType;
class StringType;

typedef std::vector<std::string> VectorOfString;

class CmdParser4Cpp
{
public:
	explicit CmdParser4Cpp( IParseResult& parseResult );
	
	virtual ~CmdParser4Cpp();

	// Defines an argument with the provided argument name.
	// Use the returned Constructor object to further define the argument properties.
	const Constructor Accept( const std::string& argumentName );

	// Parses the command line arguments
	bool Parse( const std::vector<std::string>& arguments );

	void SetResult( const std::string& argumentName, const BoolType* type );
	void SetResult( const std::string& argumentName, const StringType* type );

	IParseResult& GetMessagerParser() const { return myParseResult; }
	int GetAvailableStringParameterCount( const std::string& argumentName ) const;
	int GetAvailableBoolParameterCount( const std::string& argumentName ) const;
	const char* GetString( const std::string& argumentName, int index = 0, const char* defaultValue = nullptr ) const;
	bool GetBool( const std::string& argumentName, int index = 0, bool defaultValue = false ) const;
	int GetAvailableBooleanParameterCount( const std::string& argumentName ) const;

	template<typename ArgumentType, typename ValueType>
	ValueType GetValue( const std::unordered_map<std::string, ArgumentType>& map, const std::string& argumentName, int index, ValueType defaultValue ) const;

	void GetUsage( IUsageFormatter& formatter ) const;

private:
	IParseResult& myParseResult;
	std::unordered_map<std::string, std::shared_ptr<Argument>> myArguments;
	std::unordered_map<std::string, const StringType*> myStringResults;
	std::unordered_map<std::string, const BoolType*> myBoolResults;

	bool CheckArgumentTypes() const;
	void RemoveEmptyArguments( std::vector<std::string>& arguments );
	template<typename ArgumentType>
	int GetAvailableParameterCount( const std::string& argumentName, std::unordered_map<std::string, const ArgumentType*> map ) const;
	bool CheckConstraints( const VectorOfString& args );
	bool CheckMandatory() const;
	bool CheckDependencies() const;
	bool CheckMutualExclusion() const;
	void GetIndexes( std::vector<std::pair<int, std::shared_ptr<Argument>>>& argumentIndexes, const std::vector<std::string>& arguments );

	class IndexSorter {
	public:
		bool operator()(const std::pair<int, std::shared_ptr<Argument>>& lhs, const std::pair<int, std::shared_ptr<Argument>>& rhs) {
			return lhs.first < rhs.first;

		}
	};

	// Prevent copying
	CmdParser4Cpp( const CmdParser4Cpp& ) = delete;
	CmdParser4Cpp& operator=( const CmdParser4Cpp& ) = delete;
};

} // END cmdparser4cpp