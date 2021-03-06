// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include <typeinfo>
#include "BaseType.h"
#include "CmdParser4Cpp/IConfigurationReader.h"

namespace cmdparser4cpp {

class Argument
{
public:
	Argument( const std::string& argumentName, IParseResult& parseResult );
	virtual ~Argument();

	void SetArgumentType( ITypeParser* type );
	bool HasArgumentType() const { return myType != nullptr; }
	bool Parse( VectorOfString& arguments );

	int FindArgument( const VectorOfString& arguments, int& hitCount );
	
	const std::string& GetPrimaryName() const;
	void AddAlias( const std::string& alias ) { myNames.push_back( alias ); }
	const VectorOfString GetAliases() const;
	const VectorOfString& GetDependencies() const { return myDependencies; }

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
	void SetHidden() { myIsHidden = true; }
	bool IsHidden() const { return myIsHidden; }
	void SetHelpCommand() { myIsHelpCommand = true; }
	bool IsHelpCommand() const { return myIsHelpCommand; }

private:
	IParseResult& myParseResult;
	ITypeParser* myType = nullptr;
	bool myExistsOnCommandLine = false;
	VectorOfString myNames;
	bool myIsMandatory = false;
	std::string myDescription = "";
	VectorOfString myDependencies;
	VectorOfString myBlocks;
	bool myIsHidden = false;
	bool myIsHelpCommand = false;
	
	Argument( const Argument& ) = delete;
	Argument& operator=( const Argument& ) = delete;
};

} // END cmdparser4cpp