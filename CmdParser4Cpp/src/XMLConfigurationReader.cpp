// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#include <fstream>
#include <sys/stat.h>
#include "CmdParser4Cpp/XMLConfigurationReader.h"
#include "Argument.h"

namespace cmdparser4cpp {

using namespace pugi;

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
XMLConfigurationReader::XMLConfigurationReader()
		: myDoc(), myMatcher()
{
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
XMLConfigurationReader::XMLConfigurationReader(std::string xmlData)
		: XMLConfigurationReader()
{
	myDoc.load_string( xmlData.c_str() );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void
XMLConfigurationReader::SetMatcher(const std::string& primaryArgumentName, const NodeMatcher& matcher)
{
	myMatcher.emplace( primaryArgumentName, matcher );
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool
XMLConfigurationReader::FillFromConfiguration(std::shared_ptr<Argument> argument)
{
	bool res = true;
	auto it = myMatcher.find( argument->GetPrimaryName() );
	if( it != myMatcher.end() )
	{
		// The name of the argument must be the first item in array of items to parse
		std::vector<std::string> data{ argument->GetPrimaryName() };

		// Select the node in the XML tree
		auto& matcher = it->second;
		auto nodes = myDoc.select_nodes( matcher.GetPath().c_str() );

		// Loop each found node and let the matcher decide if it is a match.
		for( auto& n : nodes )
		{
			matcher.Match( n, data );
		}

		res = argument->Parse( data );
	}

	return res;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool
XMLConfigurationReader::LoadFromFile(const std::string& pathToFile)
{
	bool res = false;

	std::ifstream f;

	struct stat statBuff;
	if( stat( pathToFile.c_str(), &statBuff ) == 0 )
	{
		f.open( pathToFile, std::ifstream::binary );

		if( f.good() )
		{
			size_t size = static_cast<size_t>( statBuff.st_size );
			// Allocate one extra byte for terminating null character.
			std::unique_ptr<char[]> buff( new char[size + 1 ] );
			// Ensure last character in the buffer is 0.
			buff.get()[size] = 0;
			f.read( buff.get(), static_cast<std::streamsize>( size ) );

			if( !f.fail() )
			{
				res = myDoc.load_string( buff.get() );
			}

			f.close();
		}
	}

	return res;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void
XMLConfigurationReader::NodeMatcher::Match(const pugi::xpath_node& node, std::vector<std::string>& output)
{
	auto xmlNode = node.node();

	// Match using attribute name/value pair?
	if( !myMatchAttribute.empty() )
	{
		xml_attribute attr = xmlNode.attribute( myMatchAttribute.c_str() );
		if( myMatchAttribute == attr.name() && myMatchAttributeValue == attr.value() )
		{
			// We found a matching attribute/value pair
			if( myValueName.empty() )
			{
				ReadTextValue( xmlNode, output );
			}
			else
			{
				ReadAttributeValue( xmlNode, myValueName, output );
			}
		}
	}
	else if( myValueName.empty() )
	{
		// Read the child data, e.g. <Node>THE DATA</Node>
		ReadTextValue( xmlNode, output );
	}
	else
	{
		// We want to read the value of the attribute
		ReadAttributeValue( xmlNode, myValueName, output );
	}

}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void
XMLConfigurationReader::NodeMatcher::ReadAttributeValue(pugi::xml_node& node, const std::string& attributeName,
                                                        std::vector<std::string>& output)
{
	auto attr = node.attribute( attributeName.c_str() );
	std::string value = attr.as_string();
	if( !value.empty() )
	{
		output.push_back( value );
	}
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void
XMLConfigurationReader::NodeMatcher::ReadTextValue(pugi::xml_node& node, std::vector<std::string>& output)
{
	const auto& text = node.text();
	if( !text.empty() )
	{
		output.push_back( text.as_string() );
	}
}

}