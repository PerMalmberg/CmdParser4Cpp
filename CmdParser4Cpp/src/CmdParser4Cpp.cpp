// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file. 

#include <algorithm>
#include "CmdParser4Cpp.h"
#include "Argument.h"
#include "StringType.h"
#include "BoolType.h"


namespace com {
namespace codezeal {
namespace commandline {

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
CmdParser4Cpp::CmdParser4Cpp( const char* argumentPrefix, IParseResult& parseResult )
	: myArgumentPrefix( argumentPrefix ),
	myParseResult( parseResult ),
	myArguments(),
	myStringResults(),
	myBoolResults()
{
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
const Constructor
CmdParser4Cpp::Accept( const std::string& argumentName )
{
	Argument* a = new Argument( argumentName, myParseResult );
	myArguments.insert( { argumentName, a } );
	Constructor c( *a, *this );

	// Return a copy for further use.
	return c;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
CmdParser4Cpp::~CmdParser4Cpp()
{
	// Deallocate our arguments
	for( auto& pair : myArguments ) {
		delete pair.second;
	}
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool
CmdParser4Cpp::Parse( const std::vector<std::string>& arguments )
{
	std::vector<std::string> copy( arguments );
	RemoveEmptyArguments( copy );

	bool result = CheckConstraints( copy );

	if( result ) {
		// First, get indexes for the respective arguments
		std::vector<std::pair<int, Argument*>> argumentIndexes;
		GetIndexes( argumentIndexes, arguments );

		// Now let each argument parse any parameter until the next argument.
		// This ensures that an argument isn't considered as a parameter to another argument.
		for( auto curr = argumentIndexes.begin(); result && curr != argumentIndexes.end(); ++curr ) {
			int argumentPos = (*curr).first;
			int nextArgumentPos;

			// Are there more arguments left? If so, stop at that one. Otherwise take parameters until end.
			std::vector<std::pair<int, Argument*>>::iterator nextArgument;
			if( curr == --argumentIndexes.end() ) {
				nextArgumentPos = copy.size();
			}
			else {
				// Make a copy and 
				auto next = curr;
				++next;
				nextArgument = next;
				nextArgumentPos = (*nextArgument).first;
			}
						

			// Get a copy of the argument and the parameters after the argument.
			std::vector<std::string> parameters( copy.begin() + argumentPos, copy.begin() + nextArgumentPos );
			
			// Let the argument parse its parameters
			result = (*curr).second->Parse( parameters );

			if( result && parameters.size() > 0 ) {
				// Leftovers from command line
				myParseResult.UnknownArguments( parameters );
				result = false;
			}
		}
		
		result &= CheckMandatory();
		result &= CheckDependencies();
		result &= CheckMutualExclusion();
	}
	return result;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool
CmdParser4Cpp::CheckConstraints( const VectorOfString& arguments )
{
	bool res = true;

	// Find all arguments with unlimited parameters on the command line.
	std::vector<int> variable;
	std::vector<int> argumentIndexs;

	for( auto& a : myArguments ) {
		int hitCount = 0;
		int ix = a.second->FindArgument( arguments, hitCount );

		if( ix >= 0 ) {
			if( hitCount > 1 ) {
				// Same argument multiple times - that's bad
				res = false;
				myParseResult.ArgumentSpecifiedMultipleTimes( a.second->GetPrimaryName() );
			}
			else if( a.second->HasVariableParameterCount() ) {
				variable.push_back( ix );
			}
			else {
				argumentIndexs.push_back( ix );
			}
		}
	}

	if( res && variable.size() > 1 ) {
		res = false;
		myParseResult.MultipleMultiArgumentsSpecified();
	}

	if( res && variable.size() == 1 ) {
		// Check if the argument is last on the list
		int max = variable.at( 0 );
		for( auto curr : argumentIndexs ) {
			max = std::max( max, curr );
		}

		if( variable.at( 0 ) < max ) {
			res = false;
			myParseResult.MultiArgumentsMustBePalcedLast();
		}
	}

	return res;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool
CmdParser4Cpp::CheckMandatory() const
{
	bool res = true;

	for( auto& arg : myArguments ) {
		auto& a = *arg.second;
		if( a.IsMandatory() && !a.IsSuccessFullyParsed() ) {
			myParseResult.MissingMandatoryArgument( a.GetPrimaryName() );
			res = false;
		}
	}

	return res;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool
CmdParser4Cpp::CheckDependencies() const
{
	bool res = true;

	for( const auto& a : myArguments ) {
		res &= a.second->CheckDependencies( myArguments );
	}

	return res;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool
CmdParser4Cpp::CheckMutualExclusion() const
{
	bool result = true;
	// We don't want to check blockers 'a' -> 'b', then 'b' -> 'a' as that will give the same error message twice

	std::unordered_map<std::string, Argument*> testAgainst( myArguments );
	std::unordered_map<std::string, Argument*> alreadyTested;

	for( const auto& pair : myArguments ) {
		const Argument& arg = *pair.second;
		bool blocksFound = !arg.CheckMutualExclusion( testAgainst, alreadyTested );
		if( blocksFound ) {
			// Remove argument to prevent double checks
			alreadyTested.insert( pair );
			testAgainst.erase( pair.first );
		}
		result &= !blocksFound;
	}

	return result;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void
CmdParser4Cpp::RemoveEmptyArguments( std::vector<std::string>& arguments )
{
	for( size_t i = 0; i < arguments.size(); ) {
		if( arguments.at( i ).length() <= 0 ) {
			// Delete, don't increase index
			arguments.erase( arguments.begin() + i );
		}
		else {
			// Move to next
			++i;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void
CmdParser4Cpp::SetResult( const std::string& argumentName, const BoolType* result )
{
	myBoolResults.insert( { argumentName, result } );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void
CmdParser4Cpp::SetResult( const std::string& argumentName, const StringType* result )
{
	myStringResults.insert( { argumentName, result } );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int
CmdParser4Cpp::GetAvailableBoolParameterCount( const std::string& argumentName ) const
{
	return GetAvailableParameterCount( argumentName, myBoolResults );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int
CmdParser4Cpp::GetAvailableStringParameterCount( const std::string& argumentName ) const
{
	return GetAvailableParameterCount( argumentName, myStringResults );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
template<typename ArgumentType>
int
CmdParser4Cpp::GetAvailableParameterCount( const std::string& argumentName, std::unordered_map <std::string, const ArgumentType*> map ) const
{
	int res = 0;
	auto item = map.find( argumentName );
	if( item != map.end() ) {
		res = (*item).second->GetAvailableParameterCount();
	}

	return res;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
const char*
CmdParser4Cpp::GetString( const std::string& argumentName, int index, const char* defaultValue ) const
{
	return GetValue( myStringResults, argumentName, index, defaultValue );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool 
CmdParser4Cpp::GetBool( const std::string& argumentName, int index, bool defaultValue ) const
{
	return GetValue( myBoolResults, argumentName, index, defaultValue );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int
CmdParser4Cpp::GetAvailableBooleanParameterCount( const std::string& argumentName ) const
{
	return GetAvailableParameterCount( argumentName, myBoolResults );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
template<typename ArgumentType, typename ValueType>
ValueType
CmdParser4Cpp::GetValue( const std::unordered_map<std::string, ArgumentType>& map, const std::string& argumentName, int index, ValueType defaultValue ) const
{
	ValueType res = defaultValue;

	auto item = map.find( argumentName );

	if( item != map.end() ) {
		res = (*item).second->GetResult( index, defaultValue );
	}

	return res;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void
CmdParser4Cpp::GetUsage( IUsageFormatter& formatter ) const
{
	for( const auto& pair : myArguments ) {
		const auto& arg = *pair.second;
		if( arg.IsMandatory() && !arg.IsHidden() ) {
			formatter.PrepareMandatory( arg.GetPrimaryName(), arg.HasVariableParameterCount(), arg.GetMaxParameterCount(), arg.GetAliases(), arg.GetDescription() );
		}
	}

	for( const auto& pair : myArguments ) {
		const auto& arg = *pair.second;
		if( !arg.IsMandatory() && !arg.IsHidden() ) {
			formatter.PrepareMandatory( arg.GetPrimaryName(), arg.HasVariableParameterCount(), arg.GetMaxParameterCount(), arg.GetAliases(), arg.GetDescription() );
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void
CmdParser4Cpp::GetIndexes( std::vector<std::pair<int, Argument*>>& argumentIndexes, const std::vector<std::string>& arguments )
{
	for( auto& pair : myArguments ) {
		int hit = 0;
		int ix = pair.second->FindArgument( arguments, hit );
		if( ix != -1 ) {
			argumentIndexes.push_back( std::make_pair( ix, pair.second ) );
		}
	}

	IndexSorter sorter;

	std::sort( argumentIndexes.begin(), argumentIndexes.end(), sorter );
}

} // END commandline
} // END codezeal
} // com
