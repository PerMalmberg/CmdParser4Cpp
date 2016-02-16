// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file. 

#include "SystemOutputParseResult.h"
#include <iostream>
#include <string>
#include <sstream>


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
SystemOutputParseResult::SystemOutputParseResult()
	: myLines()
{
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
SystemOutputParseResult::~SystemOutputParseResult()
{
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
std::string
SystemOutputParseResult::GetParseResult()
{
	std::string res;
	for( auto& s : myLines ) {
		res.append( s ).append( "\r\n" );
	}

	return res;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void
SystemOutputParseResult::UnknownArguments( const std::vector<std::string>& leftovers )
{
	std::string line = "Unknown arguments:";
	for( auto s : leftovers ) {
		line.append( " " ).append( s );
	}
	myLines.push_back( line );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void
SystemOutputParseResult::NotEnoughParameters( const std::string& argument, int minParameterCount )
{
	std::string line = "Not enough parameters, argument '";
	line.append( argument ).append( "' requires " ).append( std::to_string( minParameterCount ) );
	myLines.push_back( line );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void
SystemOutputParseResult::FailedToParseArgument( const std::string& argument )
{
	std::string line = "Failed to parse argument '";
	line.append( argument ).append( "'" );
	myLines.push_back( line );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void
SystemOutputParseResult::ArgumentSpecifiedMultipleTimes( const std::string& argument )
{
	std::string line = "The argument '";
	line.append( argument );
	line.append( "' is specified multiple times." );
	myLines.push_back( line );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void
SystemOutputParseResult::MultipleMultiArgumentsSpecified()
{
	myLines.push_back( "Multiple arguments which allows for variable parameter count are specified on the command line." );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void
SystemOutputParseResult::MultiArgumentsMustBePalcedLast()
{
	myLines.push_back( "An argument that allows an unlimited variable number of parameters must be placed last on the command line." );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void
SystemOutputParseResult::MissingMandatoryArgument( const std::string& argument )
{
	std::string line = "The mandatory argument '";
	line.append( argument );
	line.append( "' is missing" );
	myLines.push_back( line );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void
SystemOutputParseResult::NoSuchArgumentDefined( const std::string& argument, const std::string& dependsOn )
{
	std::stringstream line;
	line << "Argument '" << argument << "' depends on '" << dependsOn << "', but no such argument is defined - contact the author of the application";
	myLines.push_back( line.str() );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void
SystemOutputParseResult::MissingDependentArgument( const std::string& argument, const std::string& dependsOn )
{
	std::stringstream line;
	line << "Argument '" << argument << "' depends on '" << dependsOn << "', but the latter is missing";
	myLines.push_back( line.str() );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void
SystemOutputParseResult::NoSuchMutuallyExclusiveArgumentDefined( const std::string& argument, const std::string& missing )
{
	std::stringstream line;
	line << "Argument '" << argument << "' is mutually exclusive to '" << missing << "', but no such argument is defined - contact the author of the application";
	myLines.push_back( line.str() );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void
SystemOutputParseResult::ArgumentsAreMutuallyExclusive( const std::string& argument, const std::string& blocker )
{
	std::stringstream line;
	line << "Arguments '" << argument << "' and '" << blocker << "' are mutually exclusive.";
	myLines.push_back( line.str() );
}

