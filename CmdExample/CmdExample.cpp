// CmdExample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <vector>
#include <iostream>

#include "CmdParser4Cpp.h"
#include "DummyResult.h"

using namespace com::codezeal::commandline;
using namespace std;

int main(int argc, const char* argv[] )
{	
	DummyResult res;

	CmdParser4Cpp parser( res );
	parser.Accept( "opa" ).AsSingleBoolean().DescribedAs( "Operation A" ).BlockedBy( "opb" );
	parser.Accept( "opb" ).AsSingleBoolean().DescribedAs( "Operation B" );

	std::vector<std::string> args;
	// Skip first arg
	for( int i = 1; i < argc; ++i ) {
		args.push_back( std::string( argv[i] ) );
	}

	if( parser.Parse( args ) ) {
		if( parser.GetBool( "opa" ) ) {
			cout << "Operation A" << endl;
		}
		else if( parser.GetBool( "opb" ) ) {
			cout << "Operation B" << endl;
		}
		else {
			cout << "No operation" << endl;
		}
	}
	else {
		cout << "Could not parse"; // Here we should use the result parser provided to CmdParser4Cpp to print the problem.
	}

    return 0;
}

