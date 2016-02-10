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
	Argument* a = new Argument( argumentName );
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
		// Let each argument have a go until there are no more arguments available.
		// (we rely on that the Argument removes the parts it uses from the vector)

		auto currentArg = myArguments.begin();

		while( result && copy.size() > 0 && currentArg != myArguments.end() ) {
			result = (*currentArg).second->Parse( copy );
			++currentArg;
		}

		if( result && copy.size() > 0 ) {
			// Leftovers from command line
			myParseResult.UnknownArguments( copy );
			result = false;
		}

		
		result &= CheckMandatory();
		//result &= CheckDependencies();
		//result &= CheckMutualExclusion();
		

	}
	return result;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool
CmdParser4Cpp::CheckConstraints( VectorOfString& arguments )
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
CmdParser4Cpp::CheckMandatory()
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
CmdParser4Cpp::GetAvailableStringParameterCount( const std::string& argumentName ) const
{
	return GetAvailableParameterCount<StringType>( argumentName, myStringResults );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int
CmdParser4Cpp::GetAvailableBoolParameterCount( const std::string& argumentName ) const
{
	return GetAvailableParameterCount<BoolType>( argumentName, myBoolResults );
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
	return GetValue<const StringType*, const char*>( myStringResults, argumentName, index, nullptr );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool 
CmdParser4Cpp::GetBool( const std::string& argumentName, int index, bool defaultValue ) const
{
	return GetValue<const BoolType*, bool>( myBoolResults, argumentName, index, defaultValue );
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

} // END commandline
} // END codezeal
} // com