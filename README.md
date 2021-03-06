# CmdParser4Cpp - Command line parser for C++

## Features
* Arguments that takes none, single, multiple and unlimited number of parameters.
* Mandatory and optional arguments.
* Dependencies between arguments, i.e. if entered, an argument may require another argument to be entered too.
* Mutually exclusive arguments.
* Hidden arguments
* Constraint validation
  * Same argument not allowed twice
  * Guard against unknown arguments.
* Custom "usage" formatting.
* Custom error messages.
* Configuration file integration
    * When arguments are not available on the
command line they can be read from a configuration file. An XML-reader is included,
supporting three different modes:
        * Matching only node name and reading a primary attribute.
        * Matching only node name and reading the 'text value' of the node.
        * Matching on node name and reading a primary attribute.
        * Matching on node name and attribute name/value, reading a second attribute value.
    * Can accept a string argument that specifies which configuration file to load.

## Ports
A Java port is available here: https://github.com/PerMalmberg/CmdParser4J

##Getting the source
The test project uses Catch (https://github.com/philsquared/Catch) for testing so you need that cloned too.

Use `git clone --recursive https://github.com/PerMalmberg/CmdParser4Cpp.git`


## Building
Standard CMakeLists.txt files included, directly usable by CLion etc.
To compile on Windows, do:
<pre>
cd &lt;path root of project&gt;
mkdir build
cd build
cmake -G "Visual Studio 14 2015" ..
</pre>
then open the generated solution.

## Usage
This example is taken from the example application. Please see the test cases for more detailed usage.

```C++

int main(int argc, const char* argv[])
{
	int result = 0;

	SystemOutputParseResult res;

	CmdParser4Cpp parser( res );
	parser.Accept( "opa" ).AsSingleBoolean().DescribedAs( "Operation A" ).BlockedBy( "opb" );
	parser.Accept( "opb" ).AsSingleBoolean().DescribedAs( "Operation B" );
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
			cout << usage.ToString();
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
```

#Class diagram
![Class diagram](https://github.com/PerMalmberg/CmdParser4Cpp/blob/master/cmdparser4cpp.png)
