// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.

#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include "IConfigurationReader.h"
#include "pugixml/src/pugixml.hpp"

namespace cmdparser4cpp {

class XMLConfigurationReader : public IConfigurationReader
{
public:
	class NodeMatcher
	{
	public:
		explicit NodeMatcher( const std::string& searchPath )
				: mySearchPath( searchPath )
		{ }

		NodeMatcher( const std::string& searchPath, const std::string& valueAttributeName )
				: mySearchPath( searchPath ), myValueName( valueAttributeName )
		{ }

		NodeMatcher( const std::string& searchPath, const std::string& valueAttributeName,
					 const std::string& matchAttribute, const std::string& matchAttributeValue )
				: mySearchPath( searchPath ), myValueName( valueAttributeName ), myMatchAttribute( matchAttribute ),
				  myMatchAttributeValue( matchAttributeValue )
		{ }

		void Match( const pugi::xpath_node& node, std::vector<std::string>& output );

		const std::string& GetPath() const { return mySearchPath; }

	private:
		std::string mySearchPath;
		std::string myValueName = "";
		std::string myMatchAttribute = "";
		std::string myMatchAttributeValue = "";

		void ReadAttributeValue( pugi::xml_node& node, const std::string& attributeName, std::vector<std::string>& output );
	};

	explicit XMLConfigurationReader( std::string xmlData );

	void SetMatcher( const std::string& primaryArgumentName, const NodeMatcher& matcher );

	bool FillFromConfiguration( std::shared_ptr<Argument> argument ) override;

private:
	pugi::xml_document myDoc;
	std::unordered_map<std::string, NodeMatcher> myMatcher;
};

}