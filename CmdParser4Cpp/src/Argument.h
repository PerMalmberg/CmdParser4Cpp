// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file. 

#pragma once

#include <typeinfo>
#include "BaseType.h"

namespace com {
namespace codezeal {
namespace commandline {

class Argument
{
public:
	Argument( const std::string& argumentName, IParseResult& parseResult );
	virtual ~Argument();

	void SetArgumentType( BaseType* type );
	bool Parse( VectorOfString& arguments );

	int FindArgument( VectorOfString& arguments, int& hitCount );
	
	const std::string& GetPrimaryName() const;
	void AddAlias( const std::string& alias ) { myNames.push_back( alias ); }
	const VectorOfString GetAliases() const;

	bool HasVariableParameterCount() const { return myType->HasVariableParameterCount(); }
	int GetMaxParameterCount() const { return myType->GetMaxParameterCount(); }
	int GetAvailableParameterCount() const { return myType->GetAvailableParameterCount(); }

	void SetMandatory() { myIsMandatory = true; }
	bool IsMandatory() const { return myIsMandatory; }
	bool IsSuccessFullyParsed() const { return myExistsOnCommandLine && myType->IsSucessfullyParsed(); }

	void SetDescription( const std::string& desc ) { myDescription = desc; }
	const std::string& GetDescription() const { return myDescription; }

	void AddDependency( const std::string& primaryName );
	bool CheckDependencies( std::unordered_map<std::string, Argument*> arguments ) const;

	void AddBlockedBy( const std::string& blockedBy );
	bool CheckMutualExclusion( const std::unordered_map<std::string, Argument*>& testAgainst, const std::unordered_map<std::string, Argument*>& alreadyTested ) const;
	void SetHidden() { isHidden = true; }
	bool IsHidden() const { return isHidden; }

private:
	IParseResult& myParseResult;
	BaseType* myType;
	bool myExistsOnCommandLine;
	VectorOfString myNames;
	bool myIsMandatory;
	std::string myDescription;
	VectorOfString myDependencies;
	VectorOfString myBlocks;
	bool isHidden;
	
	Argument( const Argument& ) = delete;
	Argument& operator=( const Argument& ) = delete;
};

} // END commandline
} // END codezeal
} // com