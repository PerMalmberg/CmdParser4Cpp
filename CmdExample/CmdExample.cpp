// CmdExample.cpp : Defines the entry point for the console application.
//
#include <vector>
#include <iostream>

#include "CmdParser4Cpp.h"
#include "SystemOutputParseResult.h"
#include "SystemOutputUsageFormatter.h"

using namespace cmdparser4cpp;
using namespace std;

int main(int argc, const char* argv[])
{
	int result = 0;

	SystemOutputParseResult res;

	CmdParser4Cpp parser( res );
	parser.Accept( "opa" ).AsSingleBoolean().DescribedAs( "Operation A" ).BlockedBy( "opb" ).DependsOn("--opc");
	parser.Accept( "opb" ).AsSingleBoolean().DescribedAs( "Operation B" );
	parser.Accept( "--opc" ).AsSingleBoolean().DescribedAs( "A depends on this" );
	parser.Accept( "--hidden" ).AsSingleBoolean().DescribedAs( "Very secret command" ).SetHidden();
	parser.Accept( "-?" ).AsSingleBoolean().WithAlias( vector<string>( { "/?", "-help", "--help" } ) ).DescribedAs(
			"Displays this help message" );

	std::vector<std::string> args;
	// Skip first arg (path to executable)
	for( int i = 1; i < argc; ++i )
	{
		args.push_back( std::string( argv[i] ) );
	}

	if( parser.Parse( args ) )
	{
		if( parser.GetBool( "-?" ) )
		{
			SystemOutputUsageFormatter usage( "CmdExample usage" );
			parser.GetUsage( usage );
			cout << usage.ToString() << std::endl;
		}
		else if( parser.GetBool( "opa" ) )
		{
			cout << "Operation A" << endl;
		}
		else if( parser.GetBool( "opb" ) )
		{
			cout << "Operation B" << endl;
		}
		else if( parser.GetBool("--hidden")) {
			cout << "You found the top secret command!" << endl;
		}
		else
		{
			cout << "No operation specified" << endl;
			result = 1;
		}
	}
	else
	{
		cout << res.GetParseResult();
		result = 1;
	}

	return result;
}

