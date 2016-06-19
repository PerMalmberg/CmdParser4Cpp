// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.

#pragma once

#include <string>
#include <memory>
#include "IConfigurationReader.h"

namespace cmdparser4cpp {

class XMLConfigurationReader : public IConfigurationReader {
public:
	void SetPathForArgument( const std::string& searchPath, const std::string& primaryArgumentName ) override;
	void FillFromConfiguration( const std::string& primaryArgumentName, std::shared_ptr<Argument> argument ) override;
};

}