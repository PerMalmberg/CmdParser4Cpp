// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file. 

#pragma once

#include "CmdParser4Cpp.h"
#include "ITypeParser.h"


namespace cmdparser4cpp {

template <typename LimitType>
class BaseType : public ITypeParser
{
public:
	virtual bool Parse( VectorOfString& allArguments, int argumentIx ) override ;

	virtual ~BaseType() {}

	int GetAvailableParameterCount() const { return static_cast<int>( myResults.size() ); }
	int GetMaxParameterCount() const override { return myMaxParameterCount; }
	bool HasVariableParameterCount() const override { return myMinParameterCount != myMaxParameterCount; }
	virtual bool IsSucessfullyParsed() const override ;

protected:
	BaseType( CmdParser4Cpp& parser, Argument& argument, int minParameterCount, int maxParameterCount, std::unique_ptr<Limit<LimitType>> limit );

	virtual bool DoTypeParse( const std::string& parameter ) = 0;
	virtual void RetrieveResult() = 0;
	virtual bool CheckLimits() = 0;

	CmdParser4Cpp& myParser;
	Argument& myArgument;
	int myMinParameterCount;
	int myMaxParameterCount;
	VectorOfString myResults;
	std::unique_ptr<Limit<LimitType>> myLimit;

private:
	bool HasEnoughParametersLeft( VectorOfString parameters );

	BaseType( const BaseType& ) = delete;
	BaseType& operator=( const BaseType& ) = delete;
};


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
template<typename LimitType>
BaseType<LimitType>::BaseType( CmdParser4Cpp& parser, Argument& argument, int minParameterCount, int maxParameterCount,
                    std::unique_ptr<Limit<LimitType>> limit )
		: myParser( parser ),
		  myArgument( argument ),
		  myMinParameterCount( minParameterCount ),
		  myMaxParameterCount( maxParameterCount ),
		  myResults(),
		  myLimit( std::move( limit ) )
{
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
template<typename LimitType>
bool
BaseType<LimitType>::Parse( VectorOfString& allArguments, int argumentIx )
{
	// Save and remove the argument name
	std::string argumentName = allArguments.at( static_cast<size_t>( argumentIx ) );
	allArguments.erase( allArguments.begin() + argumentIx );

	bool res = HasEnoughParametersLeft( allArguments );

	if( res )
	{
		// We only do this if the current type takes at least one parameter
		for( int currentParameter = 0;
		     res && currentParameter < myMaxParameterCount // Don't take too many parameters
		     && allArguments.size() > 0 // Still some left
		     && argumentIx < static_cast<int>( allArguments.size() ) // Not yet reached end
				;
             ++currentParameter
				)
		{

			// Get the next parameter from the front, i.e. where our parameters start.
			std::string parameter = allArguments.at( static_cast<size_t>( argumentIx ) );
			allArguments.erase( allArguments.begin() + argumentIx );
			res = DoTypeParse( parameter );
		}
	}
	else
	{
		myParser.GetMessagerParser().NotEnoughParameters( argumentName, myMinParameterCount );
	}

	res = res && CheckLimits() && IsSucessfullyParsed();

	if( res )
	{
		RetrieveResult();
	}
	else
	{
		myParser.GetMessagerParser().FailedToParseArgument( argumentName );
	}

	return res;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
template<typename LimitType>
bool
BaseType<LimitType>::IsSucessfullyParsed() const
{
	return GetAvailableParameterCount() >= myMinParameterCount && GetAvailableParameterCount() <= myMaxParameterCount;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
template<typename LimitType>
bool
BaseType<LimitType>::HasEnoughParametersLeft( VectorOfString parameters )
{
	return static_cast<int>( parameters.size() ) >= myMinParameterCount;
}


} // END cmdparser4cpp