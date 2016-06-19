// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file. 

#pragma once

#include <typeinfo>
#include "BaseType.h"
#include "IConfigurationReader.h"

namespace cmdparser4cpp {

class Argument
{
public:
	Argument( const std::string& argumentName, IParseResult& parseResult );
	virtual ~Argument();

	void SetArgumentType( BaseType* type );
	bool HasArgumentType() const { return myType != nullptr; }
	bool Parse( VectorOfString& arguments );

	int FindArgument( const VectorOfString& arguments, int& hitCount );
	
	const std::string& GetPrimaryName() const;
	void AddAlias( const std::string& alias ) { myNames.push_back( alias ); }
	const VectorOfString GetAliases() const;

	bool HasVariableParameterCount() const { return myType->HasVariableParameterCount(); }
	int GetMaxParameterCount() const { return myType->GetMaxParameterCount(); }

	void SetMandatory() { myIsMandatory = true; }
	bool IsMandatory() const { return myIsMandatory; }
	bool IsSuccessFullyParsed() const { return myExistsOnCommandLine && myType->IsSucessfullyParsed(); }

	void SetDescription( const std::string& desc ) { myDescription = desc; }
	const std::string& GetDescription() const { return myDescription; }

	void AddDependency( const std::string& primaryName );
	bool CheckDependencies( std::unordered_map<std::string, std::shared_ptr<Argument>> arguments ) const;

	void AddBlockedBy( const std::string& blockedBy );
	bool CheckMutualExclusion( const std::unordered_map<std::string, std::shared_ptr<Argument>>& testAgainst, const std::unordered_map<std::string, std::shared_ptr<Argument>>& alreadyTested ) const;
	void SetHidden() { isHidden = true; }
	bool IsHidden() const { return isHidden; }
	void SetConfigPath( const std::string& configPath ) { myConfigPath = configPath; }
	std::string GetConfigPath() const { return myConfigPath; }

private:
	IParseResult& myParseResult;
	BaseType* myType = nullptr;
	bool myExistsOnCommandLine = false;
	VectorOfString myNames;
	bool myIsMandatory = false;
	std::string myDescription = "";
	VectorOfString myDependencies;
	VectorOfString myBlocks;
	bool isHidden = false;
	std::string myConfigPath = "";
	
	Argument( const Argument& ) = delete;
	Argument& operator=( const Argument& ) = delete;
};

} // END cmdparser4cpp