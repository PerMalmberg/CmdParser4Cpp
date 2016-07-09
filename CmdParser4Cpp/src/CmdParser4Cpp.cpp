// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file. 

#include <algorithm>
#include <memory>
#include "CmdParser4Cpp.h"
#include "Argument.h"
#include "StringType.h"
#include "BoolType.h"
#include "IntegerType.h"

namespace cmdparser4cpp {

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
CmdParser4Cpp::CmdParser4Cpp(IParseResult& parseResult)
		: myParseResult( parseResult ),
		  myArguments(),
		  myResults()
{
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
const TypeConstructor
CmdParser4Cpp::Accept(const std::string& argumentName)
{
	std::shared_ptr<Argument> a( std::make_shared<Argument>( argumentName, myParseResult ) );
	myArguments.insert( { argumentName, a } );
	TypeConstructor c( *a, *this );

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
	myArguments.erase( myArguments.begin(), myArguments.end() );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool
CmdParser4Cpp::Parse(const std::vector<std::string>& arguments, std::shared_ptr<IConfigurationReader> cfgReader,
                     const std::string& fileNameArgument)
{
	std::vector<std::string> copy( arguments );
	RemoveEmptyArguments( copy );

	bool result = CheckArgumentTypes() && CheckConstraints( copy );

	if( result )
	{
		// First, get indexes for the respective arguments
		std::vector<std::pair<int, std::shared_ptr<Argument>>> argumentIndexes;
		GetIndexes( argumentIndexes, copy );

		if( argumentIndexes.size() == 0 && arguments.size() > 0 )
		{
			// Arguments provided on the command line, but no matches found.
			myParseResult.UnknownArguments( arguments );
			result = false;
		}
		else if( argumentIndexes.size() > 0 && argumentIndexes.at( 0 ).first > 0 )
		{
			// Unknown arguments before first matching Argument.
			std::vector<std::string> unknown( copy.begin(), copy.begin() + (argumentIndexes.at( 0 ).first) );
			myParseResult.UnknownArguments( unknown );
			result = false;
		}
		else
		{
			// Now let each argument parse any parameter until the next argument.
			// This ensures that an argument isn't considered as a parameter to another argument.
			for( auto curr = argumentIndexes.begin(); result && curr != argumentIndexes.end(); ++curr )
			{
				int argumentPos = (*curr).first;
				int nextArgumentPos;

				// Are there more arguments left? If so, stop at that one. Otherwise take parameters until end.
				if( curr == --argumentIndexes.end() )
				{
					nextArgumentPos = static_cast<int>( copy.size() );
				}
				else
				{
					// Make a copy and increment
					auto next = curr;
					++next;
					nextArgumentPos = (*next).first;
				}

				// Get a copy of the argument and the parameters after the argument.
				std::vector<std::string> parameters( copy.begin() + argumentPos, copy.begin() + nextArgumentPos );

				// Let the argument parse its parameters
				result = (*curr).second->Parse( parameters );

				if( result && parameters.size() > 0 )
				{
					// Leftovers from command line
					myParseResult.UnknownArguments( parameters );
					result = false;
				}
			}

			result = result
			         && LoadConfigFile( fileNameArgument, cfgReader )
			         && FallbackToConfiguration( cfgReader )
			         && CheckDependencies()
			         && CheckMandatory()
			         && CheckMutualExclusion();
		}

	}
	return result;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool
CmdParser4Cpp::LoadConfigFile( const std::string& fileNameArgument, std::shared_ptr<IConfigurationReader> cfgReader )
{
	bool result = true;

	if( !fileNameArgument.empty() )
	{
		std::string fileName = GetString( fileNameArgument );
		if( fileName.empty() ) {
			myParseResult.FailedToLoadConfiguration( fileNameArgument );
		}
		else
		{
			result = cfgReader->LoadFromFile( fileName );

			if( !result )
			{
				myParseResult.FailedToLoadConfiguration( fileNameArgument );
			}
		}
	}

	return result;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool
CmdParser4Cpp::CheckArgumentTypes() const
{
	bool res = true;

	// Find any argument that has no type
	for( auto& a : myArguments )
	{
		if( !a.second->HasArgumentType() )
		{
			res = false;
			myParseResult.ArgumentMissingType( a.second->GetPrimaryName() );
		}
	}

	return res;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool
CmdParser4Cpp::CheckConstraints(const VectorOfString& arguments)
{
	bool res = true;

	// Find all arguments duplicates
	for( auto& a : myArguments )
	{
		int hitCount = 0;
		int ix = a.second->FindArgument( arguments, hitCount );

		if( ix >= 0 )
		{
			if( hitCount > 1 )
			{
				// Same argument multiple times - that's bad
				res = false;
				myParseResult.ArgumentSpecifiedMultipleTimes( a.second->GetPrimaryName() );
			}
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

	for( auto& arg : myArguments )
	{
		const Argument& a = *arg.second;
		if( a.IsMandatory() && !a.IsSuccessFullyParsed() )
		{
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

	for( const auto& a : myArguments )
	{
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

	std::unordered_map<std::string, std::shared_ptr<Argument>> testAgainst( myArguments );
	std::unordered_map<std::string, std::shared_ptr<Argument>> alreadyTested;

	for( const auto& pair : myArguments )
	{
		const Argument& arg = *pair.second;
		bool blocksFound = !arg.CheckMutualExclusion( testAgainst, alreadyTested );
		if( blocksFound )
		{
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
CmdParser4Cpp::RemoveEmptyArguments(std::vector<std::string>& arguments)
{
	for( size_t i = 0; i < arguments.size(); )
	{
		if( arguments.at( i ).length() <= 0 )
		{
			// Delete, don't increase index
			arguments.erase( arguments.begin() + static_cast<int>( i ) );
		}
		else
		{
			// Move to next
			++i;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int
CmdParser4Cpp::GetAvailableStringParameterCount(const std::string& argumentName) const
{
	return GetAvailableParameterCount<StringType>( argumentName );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
template<typename ArgumentType>
int
CmdParser4Cpp::GetAvailableParameterCount(const std::string& argumentName) const
{

	const std::unordered_map<std::string, const ArgumentType*>& map = myResults;

	int res = 0;
	auto item = map.find( argumentName );
	if( item != map.end() )
	{
		res = (*item).second->GetAvailableParameterCount();
	}

	return res;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
const char*
CmdParser4Cpp::GetString(const std::string& argumentName, int index, const char* defaultValue) const
{
	return GetValue<StringType>( argumentName, index, defaultValue );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool
CmdParser4Cpp::GetBool(const std::string& argumentName, int index, bool defaultValue) const
{
	return GetValue<BoolType>( argumentName, index, defaultValue );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int
CmdParser4Cpp::GetInteger(const std::string& argumentName, int index, int defaultValue) const
{
	return GetValue<IntegerType>( argumentName, index, defaultValue );
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int
CmdParser4Cpp::GetAvailableBooleanParameterCount(const std::string& argumentName) const
{
	return GetAvailableParameterCount<BoolType>( argumentName );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int
CmdParser4Cpp::GetAvailableIntegerParameterCount(const std::string& argumentName) const
{
	return GetAvailableParameterCount<IntegerType>( argumentName );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
template<typename ArgumentType, typename ValueType>
ValueType
CmdParser4Cpp::GetValue(const std::string& argumentName, int index, ValueType defaultValue) const
{
	const std::unordered_map<std::string, const ArgumentType*>& map = myResults;

	ValueType res = defaultValue;

	auto item = map.find( argumentName );

	if( item != map.end() )
	{
		res = (*item).second->GetResult( index, defaultValue );
	}

	return res;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void
CmdParser4Cpp::GetUsage(IUsageFormatter& formatter) const
{
	for( const auto& pair : myArguments )
	{
		const auto& arg = *pair.second;
		if( arg.IsMandatory() && !arg.IsHidden() )
		{
			formatter.PrepareMandatory( arg.GetPrimaryName(), arg.HasVariableParameterCount(),
			                            arg.GetMaxParameterCount(), arg.GetAliases(), arg.GetDescription() );
		}
	}

	for( const auto& pair : myArguments )
	{
		const auto& arg = *pair.second;
		if( !arg.IsMandatory() && !arg.IsHidden() )
		{
			formatter.PrepareMandatory( arg.GetPrimaryName(), arg.HasVariableParameterCount(),
			                            arg.GetMaxParameterCount(), arg.GetAliases(), arg.GetDescription() );
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void
CmdParser4Cpp::GetIndexes(std::vector<std::pair<int, std::shared_ptr<Argument>>>& argumentIndexes,
                          const std::vector<std::string>& arguments)
{
	for( auto& pair : myArguments )
	{
		int hit = 0;
		int ix = pair.second->FindArgument( arguments, hit );
		if( ix != -1 )
		{
			argumentIndexes.push_back( std::make_pair( ix, pair.second ) );
		}
	}

	IndexSorter sorter;

	std::sort( argumentIndexes.begin(), argumentIndexes.end(), sorter );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool
CmdParser4Cpp::FallbackToConfiguration(std::shared_ptr<IConfigurationReader> cfgReader)
{
	bool res = true;
	// Let each argument that has not already been successfully parsed based on the
	// command line attempt a fallback to the configuration
	if( cfgReader )
	{
		for( auto arg = myArguments.begin(); res && arg != myArguments.end(); ++arg )
		{
			if( !arg->second->IsSuccessFullyParsed() )
			{
				res &= cfgReader->FillFromConfiguration( arg->second );
			}
		}
	}

	return res;
}

} // END cmdparser4cpp
