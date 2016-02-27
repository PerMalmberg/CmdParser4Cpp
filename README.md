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

## Ports
A Java port is available here: https://github.com/PerMalmberg/CmdParser4J

## Usage
This example is taken from the test cases.

```C++

SystemOutputParseResult msg;

CmdParser4Cpp p( msg );
p.Accept( "-argument" ).AsBoolean( 1 ).SetMandatory().DescribedAs( "An argument that accept a single boolean parameter" );
p.Accept( "-multi" ).AsString( 1, 4 ).DescribedAs( "An optional argument that accept one to four argument." );
// The name of the argument, or any prefix characters, doesn't really matter, here we use double dash.
p.Accept( "--otherArgument" ).WithAlias( std::vector<std::string>( { "-o", "-O" } ) ).AsSingleBoolean().DescribedAs( "An optional argument that takes no parameters" );
// Arguments with variable parameters are only accepted as the last argument on the commandline.
Assert::IsTrue( p.Parse( std::vector<std::string>( { "-argument", "true", "-O", "-multi", "parameter1", "parameter2", "parameter3" } ) ) );
// Verify the number of parameters that can be read for the different arguments.
Assert::AreEqual( 1, p.GetAvailableBooleanParameterCount( "--otherArgument" ) );
Assert::AreEqual( 3, p.GetAvailableStringParameterCount( "-multi" ) );
Assert::AreEqual( 1, p.GetAvailableBooleanParameterCount( "-argument" ) );

// Read the values from the parser.
Assert::AreEqual( true, p.GetBool( "-argument", 0 ) );
Assert::AreEqual( "parameter1", p.GetString( "-multi", 0 ) );
Assert::AreEqual( "parameter2", p.GetString( "-multi", 1 ) );
Assert::AreEqual( "parameter3", p.GetString( "-multi", 2 ) );
Assert::AreEqual( nullptr, p.GetString( "-multi", 4 ) );
```
