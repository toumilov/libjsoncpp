#pragma once

#include <memory>
#include "error.hpp"
#include "value.hpp"

namespace jsoncpp
{

class Schema
{
public:
	Schema();
	Schema( const std::string &schema, Error &e );
	~Schema();

	bool init( const std::string &schema, Error &e );
	bool validate( const std::string &json, Error &e ) const;
	bool validate( const Value &value, Error &e ) const;

private:
	class Impl;
	std::unique_ptr<Impl> impl_;
};

} // namespace jsoncpp
