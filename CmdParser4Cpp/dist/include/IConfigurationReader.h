// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.

#pragma once

namespace cmdparser4cpp {

class Argument;

class IConfigurationReader {
public:
	virtual ~IConfigurationReader() {}
	virtual bool FillFromConfiguration( std::shared_ptr<Argument> argument ) = 0;
	virtual bool LoadFromFile( const std::string& pathToFile ) = 0;
};

}