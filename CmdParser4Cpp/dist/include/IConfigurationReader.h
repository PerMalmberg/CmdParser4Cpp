// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.

#pragma once

namespace cmdparser4cpp {

class Argument;

class IConfigurationReader {
public:
	virtual bool FillFromConfiguration( std::shared_ptr<Argument> argument ) = 0;
};

}