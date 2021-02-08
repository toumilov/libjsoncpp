#pragma once

#include "error.hpp"
#include "value.hpp"

namespace jsoncpp
{

/**
 * @brief JSON data format processing class.
 */
class Json
{
public:

	/**
	 * @brief JSON formatting.
	 */
	struct Format
	{
		char indent_char;
		unsigned indent_size;

		Format() :
			indent_char( ' ' ),
			indent_size( 0 )
		{}
		Format( char indent_char, unsigned indent_size ) :
			indent_char( indent_char ),
			indent_size( indent_size )
		{}
		std::string indent( unsigned level ) const;
	};

	/**
	 * @brief validate Validates string as JSON.
	 * @param json String that contains JSON data.
	 * @param e Result variable.
	 * @return True if validation successful.
	 */
	static bool validate( const std::string &json, Error &e );

	/**
	 * @brief parse Parse JSON string.
	 * @param json String that contains JSON data.
	 * @param e Result variable.
	 * @return Variadic value object.
	 */
	static Value parse( const std::string &json, Error &e );

	/**
	 * @brief build Build JSON string from value.
	 * @param value Data object to build JSON from.
	 * @param e Result variable.
	 * @return JSON string.
	 */
	static std::string build( const Value &value, Error &e );

	/**
	 * @brief build Build JSON string from value using specific formatting.
	 * @param value Data object to build JSON from.
	 * @param e Result variable.
	 * @param formatter Formatting information.
	 * @return JSON string.
	 */
	static std::string build( const Value &value, Error &e, const Format &formatter );

	/**
	 * @brief format Format JSON string using specific formatting settings.
	 * @param json Unformatted JSON data in string form.
	 * @param e Result variable.
	 * @param formatter Formatting information.
	 * @return formatted JSON string.
	 */
	static std::string format( const std::string &json, Error &e, const Format &formatter );

	/**
	 * @brief minimize Minimize JSON string size keeping element names.
	 * @param json Unformatted JSON data in string form.
	 * @param e Result variable.
	 * @return minimized JSON string.
	 */
	static std::string minimize( const std::string &json, Error &e);
};

} // namespace jsoncpp
