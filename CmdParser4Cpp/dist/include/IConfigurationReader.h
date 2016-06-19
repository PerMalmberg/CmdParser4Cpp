// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.

#pragma once

namespace cmdparser4cpp {

class Argument;

class IConfigurationReader {
public:
	virtual void SetPathForArgument( const std::string& searchPath, const std::string& primaryArgumentName ) = 0;
	virtual void FillFromConfiguration( const std::string& primaryArgumentName, std::shared_ptr<Argument> argument ) = 0;
};

}