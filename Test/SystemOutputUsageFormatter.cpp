#include "SystemOutputUsageFormatter.h"
#include "Constructor.h"

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
SystemOutputUsageFormatter::SystemOutputUsageFormatter( const std::string& header )
	: myUsage(), myIndent( header.length() )
{
	myUsage << header;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
SystemOutputUsageFormatter::~SystemOutputUsageFormatter()
{
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void
SystemOutputUsageFormatter::PrepareMandatory( const std::string& primaryName, bool hasVariableParameterCount, int maxArgumentCount, const std::vector<std::string>& aliases, const std::string& description )
{
	FormatArgument( primaryName, hasVariableParameterCount, maxArgumentCount, aliases, description );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void
SystemOutputUsageFormatter::PrepareNonMandatory( const std::string& primaryName, bool hasVariableParameterCount, int maxArgumentCount, const std::vector<std::string>& aliases, const std::string& description )
{
	FormatArgument( primaryName, hasVariableParameterCount, maxArgumentCount, aliases, description );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void
SystemOutputUsageFormatter::FormatArgument( const std::string& primaryName, bool hasVariableParameterCount, int maxArgumentCount, const std::vector<std::string>& aliases, const std::string& description )
{
	myUsage << std::endl << Indent() << primaryName;

	bool firstAlias = true;
	if( aliases.size() > 0 ) {
		myUsage << " (";
		for( auto& alias : aliases ) {
			if( !firstAlias ) {
				myUsage << ", ";
			}
			firstAlias = false;
			myUsage << alias;
		}
		myUsage << ")";
	}

	if( maxArgumentCount == Constructor::NO_PARAMETER_LIMIT ) {
		myUsage << " <arg1> ... <argN>";
	}
	else if( hasVariableParameterCount ) {
		for( int i = 0; i < maxArgumentCount; ++i ) {
			myUsage << (i == 0) ? "" : " ";
			myUsage << " <arg" << (i + 1) << ">";
		}
	}

	myUsage << std::endl << Indent() << Indent() << description;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
std::string
SystemOutputUsageFormatter::Indent() const
{
	std::string s;
	s.insert( 0, myIndent, ' ' );
	return s;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
std::string 
SystemOutputUsageFormatter::ToString() const
{
	return myUsage.str();
};
