// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.

#include "XMLConfigurationReader.h"
#include "Argument.h"


namespace cmdparser4cpp {

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
XMLConfigurationReader::XMLConfigurationReader( std::string xmlData )
		: myPath()
{
	myDoc.load_string( xmlData.c_str() );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void
XMLConfigurationReader::SetPathForArgument( const std::string& primaryArgumentName, const std::string& searchPath )
{
	myPath.emplace( primaryArgumentName, searchPath );
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool
XMLConfigurationReader::FillFromConfiguration( std::shared_ptr<Argument> argument )
{
	bool res = true;
	auto it = myPath.find( argument->GetPrimaryName() );
	if( it != myPath.end() )
	{
		// The name of the argument must be the first item in array of items to parse
		std::vector<std::string> data{ argument->GetPrimaryName() };

		const std::string& searchPath = it->second;
		auto query = myDoc.select_nodes( searchPath.c_str() );
		for( auto q : query )
		{
			std::string v( q.node().child_value() );
			data.push_back( v );
		}

		res = argument->Parse( data );
	}

	return res;
}

}