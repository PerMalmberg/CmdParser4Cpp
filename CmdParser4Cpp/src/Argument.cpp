// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file. 

#include "Argument.h"

namespace cmdparser4cpp {

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
Argument::Argument( const std::string& argumentName, IParseResult& parseResult )
	:
	myParseResult( parseResult ),
	myType( nullptr ),
	myExistsOnCommandLine( false ),
	myNames(),
	myIsMandatory( false ),
	myDescription(),
	myDependencies(),
	myBlocks(),
	isHidden( false )
{
	myNames.push_back( argumentName );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
Argument::~Argument()
{
	if( myType != 0 ) {
		delete myType;
	}
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void Argument::SetArgumentType( BaseType* type )
{
	myType = type;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool
Argument::Parse( VectorOfString& arguments )
{
	bool result = true;
	int hits = 0;

	int ix = FindArgument( arguments, hits );

	if( ix >= 0 ) {
		// Argument found, parse it
		myExistsOnCommandLine = true;
		result = myType->Parse( arguments, ix );
	}

	return result;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int
Argument::FindArgument( const VectorOfString& allArguments, int& hitCount )
{
	int foundIx = -1;

	hitCount = 0;

	// Loop all our names and test against all provided arguments.
	for( const auto& name : myNames ) {
		for( int ix = 0; ix < static_cast<int>( allArguments.size() ); ++ix ) {
			if( allArguments.at( static_cast<size_t>( ix ) ).compare( name ) == 0 ) {
				if( foundIx == -1 ) {
					// First hit
					foundIx = ix;
				}

				++hitCount;
			}
		}
	}

	return foundIx;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
const std::string&
Argument::GetPrimaryName() const
{
	return myNames.at( 0 );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
const VectorOfString
Argument::GetAliases() const
{
	VectorOfString aliasesOnly;

	if( myNames.size() > 0 ) {
		for( auto it = myNames.begin() + 1; it != myNames.end(); ++it ) {
			aliasesOnly.push_back( *it );
		}
	}

	return aliasesOnly;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void
Argument::AddDependency( const std::string& primaryName )
{
	myDependencies.push_back( primaryName );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool
Argument::CheckDependencies( std::unordered_map<std::string, std::shared_ptr<Argument>> arguments ) const
{
	bool result = true;

	// Only check if the current Argument has been parsed itself.
	if( IsSuccessFullyParsed() ) {
		for( const auto& dep : myDependencies ) {
			const auto& dependsOn = arguments.find( dep );
			if( dependsOn == arguments.end() ) {
				// Can't find the argument, this is a programming error
				myParseResult.NoSuchArgumentDefined( GetPrimaryName(), dep );
				result = false;
			}
			else if( !(*dependsOn).second->IsSuccessFullyParsed() ) {
				myParseResult.MissingDependentArgument( GetPrimaryName(), dep );
				result = false;
			}
		}
	}

	return result;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void
Argument::AddBlockedBy( const std::string& blockedBy )
{
	myBlocks.push_back( blockedBy );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool
Argument::CheckMutualExclusion( const std::unordered_map<std::string, std::shared_ptr<Argument>>& testAgainst, const std::unordered_map<std::string, std::shared_ptr<Argument>>& alreadyTested ) const
{
	bool result = true;

	// Only check if the current Argument has been parsed itself.
	if( IsSuccessFullyParsed() ) {
		for( const auto& blocker : myBlocks ) {
			auto isTested = alreadyTested.find( blocker );
			if( isTested == alreadyTested.end() ) {
				// Not yet tested
				const auto blockedBy = testAgainst.find( blocker );
				if( blockedBy == testAgainst.end() ) {
					// Can't find the argument, this is a programming error
					myParseResult.NoSuchMutuallyExclusiveArgumentDefined( GetPrimaryName(), blocker );
					result = false;
				}
				else if( (*blockedBy).second->IsSuccessFullyParsed() ) {
					myParseResult.ArgumentsAreMutuallyExclusive( GetPrimaryName(), (*blockedBy).second->GetPrimaryName() );
					result = false;
				}
			}
		}
	}

	return result;
}

} // END cmdparser4cpp
