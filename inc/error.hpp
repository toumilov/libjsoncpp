#pragma once

#include <string>

namespace jsoncpp
{

class Error
{
protected:
	unsigned code_;
	std::string description_;

public:
	enum ErrorCodes
	{
		None = 0xFFFFFFFF,
		UnexpectedCharacter = 1,
		UnexpectedEnding,
		UnexpectedToken,
		BadKey,
		BadValue,
		UnexpectedType,
		NoSchema,
		OutOfRange,
		NoMatch
	};

	Error();
	Error( const ErrorCodes code );
	Error( ErrorCodes code, const char *description_fmt, ... );

	Error& operator=( const Error &e );
	unsigned code() const;
	const std::string& description() const;
	bool empty() const;
	void clear();
};

} // namespace jsoncpp
