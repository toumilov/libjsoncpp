
#include <cstdarg>
#include "error.hpp"


namespace jsoncpp
{

Error::Error() :
	code_( ErrorCodes::None )
{}

Error::Error( const ErrorCodes code ) :
	code_( code )
{}

Error::Error( ErrorCodes code, const char *description_fmt, ... ) :
	code_( code )
{
	va_list arguments;
	va_start( arguments, description_fmt );
	unsigned len = vsnprintf( nullptr, 0, description_fmt, arguments ) + 1;
	description_.resize( len );
	vsnprintf( (char*)description_.c_str(), len, description_fmt, arguments );
	va_end( arguments );
}

Error& Error::operator=( const Error &e )
{
	code_ = e.code_;
	description_ = e.description_;
	return *this;
}

unsigned Error::code() const
{
	return code_;
}

const std::string& Error::description() const
{
	return description_;
}

bool Error::empty() const
{
	return code_ == ErrorCodes::None;
}

void Error::clear()
{
	code_ = ErrorCodes::None;
	description_.clear();
}

} // namespace jsoncpp
