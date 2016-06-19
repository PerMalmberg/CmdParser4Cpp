// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.

#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include "IConfigurationReader.h"
#include "pugixml/src/pugixml.hpp"

namespace cmdparser4cpp {

class XMLConfigurationReader : public IConfigurationReader {
public:
	explicit XMLConfigurationReader(std::string xmlData );

	void SetPathForArgument( const std::string& primaryArgumentName, const std::string& searchPath ) override;
	bool FillFromConfiguration( std::shared_ptr<Argument> argument ) override;
private:
	pugi::xml_document myDoc;
	std::unordered_map<std::string, std::string> myPath;
};

}