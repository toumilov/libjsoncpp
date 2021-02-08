
#include <cstring>
#include <cstdio>
#include <stack>
#include <utility>
#include <errno.h>
#include "json.hpp"


namespace jsoncpp
{

class JsonTokenizer
{
public:
	JsonTokenizer( const std::string &json_str ) :
		str_( json_str ),
		it_( json_str.begin() )
	{
	}

	bool is_end() const
	{
		return it_ == str_.end();
	}

	std::pair<unsigned, unsigned> get_last_token_position()
	{
		return std::make_pair( token_line_, token_offset_ );
	}

	std::string get_token( Error &e )
	{
		unsigned hex_esc = 0;
		std::string token;
		token_line_ = line_;
		token_offset_ = offset_;

		while(true)
		{
			if ( is_end() )
			{
				if ( !token.empty() )
				{
					state_ = State::Undefined;
				}
				return token;
			}
			char c = *it_;
			switch( state_ )
			{
			case State::Undefined:
				if ( isspace( c ) ) // Skip whitespaces
				{
					advance();
					continue;
				}
				token += c;
				if ( c == '{' || c == '}' || c == '[' || c == ']' || c == ':' || c == ',' )
				{
					advance();
					return token;
				}
				else if ( c== '\"' )
				{
					state_ = State::String;
				}
				else if ( c == '-' || isdigit( c ) )
				{
					state_ = State::Integer;
				}
				else if ( isalpha( c ) )
				{
					state_ = State::Lexeme;
				}
				else
				{
					e = Error( Error::UnexpectedCharacter, "Unexpected character (%d:%d)", line_, offset_ );
					it_ = str_.end();
					return "";
				}
				break;
			case State::String:
				token += c;
				if ( c == '\"' )
				{
					state_ = State::Undefined;
					advance();
					return token;
				}
				if ( c == '\\' )
				{
					state_ = State::Esc;
				}
				break;
			case State::Esc:
				token += c;
				if ( c == '\"' || c == '\\' || c == '/' || c == 'b' || c == 'f' || c == 'n' || c == 'r' || c == 't' )
				{
					state_ = State::String;
				}
				else if ( c == 'u' )
				{
					state_ = State::EscHex;
				}
				else
				{
					e = Error( Error::UnexpectedCharacter, "Unexpected character (%d:%d)", line_, offset_ );
					it_ = str_.end();
					return "";
				}
				break;
			case State::EscHex:
				token += c;
				if ( isdigit( c ) || ( c >= 'a' && c <= 'f' ) || ( c >= 'A' && c <= 'F' ) )
				{
					hex_esc++;
					if ( hex_esc == 4 )
					{
						hex_esc = 0;
						state_ = State::String;
					}
				}
				else
				{
					e = Error( Error::UnexpectedCharacter, "Unexpected character (%d:%d)", line_, offset_ );
					it_ = str_.end();
					return "";
				}
				break;
			case State::Integer:
				if ( isdigit( c ) || c == '+' || c == '-' || c == 'e' || c == 'E' || c == '.' )
				{
					token += c;
				}
				else
				{
					state_ = State::Undefined;
					return token;
				}
				break;
			case State::Lexeme:
				if ( isalpha( c ) )
				{
					token += c;
				}
				else
				{
					state_ = State::Undefined;
					return token;
				}
				break;
			}
			advance();
		}
	}

private:
	enum class State
	{
		Undefined = 0,
		String,
		Esc,
		EscHex,
		Integer,
		Lexeme
	} state_ = State::Undefined;
	const std::string &str_;
	std::string::const_iterator it_;
	unsigned line_ = 1;
	unsigned offset_ = 1;
	unsigned token_line_ = 1;
	unsigned token_offset_ = 1;

	void advance()
	{
		if ( is_end() )
		{
			return;
		}
		it_++;
		if ( *it_ == '\n' )
		{
			line_++;
			offset_ = 0;
		}
		else
		{
			offset_++;
		}
	}
};

class JsonImpl
{
	enum class State
	{
		Value,
		Key,
		KeyValueSeparator,
		ValueSeparator,
		End
	};
	enum class Levels
	{
		Object,
		Array
	};

	static bool is_lexeme( const std::string &token )
	{
		return token == "true" || token == "false" || token == "null";
	}

	static bool is_number( const std::string &token )
	{
		char *endptr = 0;
		errno = 0;
		auto ret __attribute__((unused)) = strtod( token.c_str(), &endptr );
		return ( errno == 0 && endptr && ( *endptr == '\0' ) );
	}

	static bool is_string( const std::string &token )
	{
		if ( token.size() < 2 || token.front() != '\"' || token.back() != '\"' )
		{
			return false;
		}
		for( const char *p = token.c_str() + 1; *p != '\0'; ++p )
		{
			if ( *p == '\"' && p < &token.back() )
			{
				return false;
			}
			if ( *p == '\\' )
			{
				p++;
				if ( *p == '\0' )
				{
					return false;
				}
				switch( *p )
				{
				case '\"':
				case '\\':
				case '/':
				case 'b':
				case 'f':
				case 'n':
				case 'r':
				case 't':
					break;
				case 'u':
					for( int i = 0; i < 4; i++ )
					{
						p++;
						if ( *p == '\0' || !( ( *p >= '0' && *p <= '9' ) || ( *p >= 'a' && *p <= 'f' ) || ( *p >= 'A' && *p <= 'F' ) ) )
						{
							return false;
						}
					}
					break;
				default:
					return false;
				}
			}
		}
		return true;
	}

	static std::string build_string( const std::string &token )
	{
		return unescape_string( token.substr( 1, token.size() - 2 ) );
	}

	static Value build_lexeme( const std::string &token )
	{
		if ( token == "true" )
		{
			return Value( true );
		}
		else if ( token == "false" )
		{
			return Value( false );
		}
		return Value();
	}

	static Value build_number( const std::string &token, Error &e )
	{
		Value v( token );
		if ( token.find_first_of( ".eE" ) == std::string::npos )
		{
			if ( token[0] == '-' )
			{
				if ( v.is_convertable( Value::Type::Int32 ) )
				{
					return v.as_int32();
				}
				else if ( v.is_convertable( Value::Type::Int64 ) )
				{
					return v.as_int64();
				}
			}
			else
			{
				if ( v.is_convertable( Value::Type::Uint32 ) )
				{
					return v.as_uint32();
				}
				else if ( v.is_convertable( Value::Type::Uint64 ) )
				{
					return v.as_uint64();
				}
			}
		}
		else
		{
			if ( v.is_convertable( Value::Type::Float ) )
			{
				return v.as_float();
			}
			else if ( v.is_convertable( Value::Type::Double ) )
			{
				return v.as_double();
			}
		}
		e = Error::BadValue;
		return v;
	}

public:
	static std::string escape_string( const std::string &s )
	{
		std::string res( s );
		auto replace_all = []( std::string &s, const char *from, const char *to ) {
			size_t start = 0;
			while( ( start = s.find( from, start ) ) != std::string::npos )
			{
					 s.replace( start, strlen( from ), to );
					 start += strlen( to );
			}
		};
		replace_all( res, "\\", "\\\\" );
		replace_all( res, "\"", "\\\"" );
		replace_all( res, "\b", "\\b" );
		replace_all( res, "\f", "\\f" );
		replace_all( res, "\n", "\\n" );
		replace_all( res, "\r", "\\r" );
		replace_all( res, "\t", "\\t" );
		return res;
	}

	static std::string unescape_string( const std::string &s )
	{
		std::string res( s );
		auto replace_all = []( std::string &s, const char *from, const char *to ) {
			size_t start = 0;
			while( ( start = s.find( from, start ) ) != std::string::npos )
			{
				s.replace( start, strlen( from ), to );
				start += strlen( to );
			}
		};
		replace_all( res, "\\\\", "\\" );
		replace_all( res, "\\\"", "\"" );
		replace_all( res, "\\b", "\b" );
		replace_all( res, "\\f", "\f" );
		replace_all( res, "\\n", "\n" );
		replace_all( res, "\\r", "\r" );
		replace_all( res, "\\t", "\t" );
		return res;
	}

	static bool validate( const std::string &json, Error &e )
	{
		parse( json, nullptr, e );
		return e.empty();
	}

	static Value parse( const std::string &json, Error &e )
	{
		Value v;
		parse( json, &v, e );
		return v;
	}

	static void parse( const std::string &json, Value *v, Error &e )
	{
		std::string key;
		std::stack<Levels> levels;
		std::stack<Value*> values;

		auto tokenizer = JsonTokenizer( json );
		State state = State::Value;
		std::string token;
		while(true)
		{
			token = tokenizer.get_token( e );
			if ( token.empty() )
			{
				if ( !e.empty() )
				{
					if ( v ) { *v = Value(); }
					return;
				}
				if ( !levels.empty() )
				{
					auto pos = tokenizer.get_last_token_position();
					e = Error( Error::UnexpectedEnding, "Unexpected ending (%d:%d)", pos.first, pos.second );
					if ( v ) { *v = Value(); }
					return;
				}
				break;
			}
			switch( state )
			{
			case State::Value:
				if ( is_string( token ) )
				{
					if ( levels.empty() )
					{
						state = State::End;
						if ( v ) { *v = build_string( token ); }
					}
					else
					{
						state = State::ValueSeparator;
						if ( levels.top() == Levels::Object )
						{
							if ( v ) { values.top()->insert( key, Value( build_string( token ) ) ); }
						}
						else if ( v )
						{
							values.top()->insert( Value( build_string( token ) ) );
						}
					}
					continue;
				}
				if ( is_lexeme( token ) )
				{
					if ( levels.empty() )
					{
						state = State::End;
						if ( v ) { *v = build_lexeme( token ); }
					}
					else
					{
						state = State::ValueSeparator;
						if ( levels.top() == Levels::Object )
						{
							if ( v ) { values.top()->insert( key, build_lexeme( token ) ); }
						}
						else if ( v )
						{
							values.top()->insert( build_lexeme( token ) );
						}
					}
					continue;
				}
				if ( is_number( token ) )
				{
					if ( levels.empty() )
					{
						state = State::End;
						if ( v ) { *v = build_number( token, e ); }
					}
					else
					{
						state = State::ValueSeparator;
						if ( levels.top() == Levels::Object )
						{
							if ( v ) { values.top()->insert( key, build_number( token, e ) ); }
						}
						else if ( v )
						{
							values.top()->insert( build_number( token, e ) );
						}
					}
					if ( !e.empty() )
					{
						auto pos = tokenizer.get_last_token_position();
						e = Error( Error::BadValue, "Bad value (%d:%d)", pos.first, pos.second );
						if ( v ) { *v = Value(); }
						return;
					}
					continue;
				}
				else if ( token == "{" )
				{
					state = State::Key;
					if ( levels.empty() )
					{
						if ( v )
						{
							*v = Value( Value::Type::Object );
							values.push( v );
						}
					}
					else if ( levels.top() == Levels::Array && v )
					{
						values.top()->insert( Value( Value::Type::Object ) );
						values.push( &values.top()->back() );
					}
					else if ( v )
					{
						values.top()->insert( key, Value( Value::Type::Object ) );
						values.push( &values.top()->at( key ) );
					}
					levels.push( Levels::Object );
					continue;
				}
				else if ( token == "[" )
				{
					if ( levels.empty() )
					{
						if ( v )
						{
							*v = Value( Value::Type::Array );
							values.push( v );
						}
					}
					else if ( levels.top() == Levels::Array && v )
					{
						values.top()->insert( Value( Value::Type::Object ) );
						values.push( &values.top()->back() );
					}
					else if ( v )
					{
						values.top()->insert( key, Value( Value::Type::Array ) );
						values.push( &values.top()->at( key ) );
					}
					levels.push( Levels::Array );
					continue;
				}
				else if ( token == "]" && !levels.empty() && levels.top() == Levels::Array )
				{
					levels.pop();
					if ( v ) { values.pop(); }
					state = levels.empty() ? State::End : State::ValueSeparator;
					continue;
				}
				break;
			case State::Key:
				if ( is_string( token ) )
				{
					key = build_string( token );
					if ( key.empty() )
					{
						auto pos = tokenizer.get_last_token_position();
						e = Error( Error::BadKey, "Empty key (%d:%d)", pos.first, pos.second );
						if ( v ) { *v = Value(); }
						return;
					}
					state = State::KeyValueSeparator;
					continue;
				}
				else if ( token == "}" && !levels.empty() && levels.top() == Levels::Object )
				{
					levels.pop();
					if ( v ) { values.pop(); }
					state = State::ValueSeparator;
					continue;
				}
				break;
			case State::KeyValueSeparator:
				if ( token == ":" )
				{
					state = State::Value;
					continue;
				}
				break;
			case State::ValueSeparator:
				if ( !levels.empty() )
				{
					if ( token == "," )
					{
						state = levels.top() == Levels::Array ? State::Value : State::Key;
						continue;
					}
					else if ( token == "}" && levels.top() == Levels::Object )
					{
						levels.pop();
						if ( v ) { values.pop(); }
						continue;
					}
					else if ( token == "]" && levels.top() == Levels::Array )
					{
						levels.pop();
						if ( v ) { values.pop(); }
						continue;
					}
				}
				break;
			case State::End:
				if ( !token.empty() )
				{
					auto pos = tokenizer.get_last_token_position();
					e = Error( Error::UnexpectedEnding, "Unexpected ending (%d:%d)", pos.first, pos.second );
					if ( v ) { *v = Value(); }
					return;
				}
				continue;
			}
			auto pos = tokenizer.get_last_token_position();
			e = Error( Error::UnexpectedToken, "Unexpected token (%d:%d)", pos.first, pos.second );
			if ( v ) { *v = Value(); }
			return;
		}
	}

	static std::string build( const Value &value, Error &e, const Json::Format &f )
	{
		std::function<std::string(const Value&, const Json::Format&, unsigned)> build_value =
			[&]( const Value &value, const Json::Format &f, unsigned level ) -> std::string {
			std::string s;
			switch( value.type() )
			{
			case Value::Type::Array:
			{
				auto &arr = value.get<Value::Array>();
				auto size = arr.size();
				s += '[';
				if ( f.indent_size )
				{
					s += '\n';
					level++;
				}
				for( unsigned i = 0; i < size; i++ )
				{
					s += f.indent( level );
					s += build_value( arr[i], f, level );
					if ( size > 1 && i <= size - 2 )
					{
						s += ',';
					}
					if ( f.indent_size )
					{
						s += '\n';
					}
				}
				if ( f.indent_size )
				{
					level--;
					s += f.indent( level );
				}
				s += ']';
				return s;
			}
			case Value::Type::Object:
			{
				auto &obj = value.get<Value::Object>();
				auto size = obj.size();
				s += '{';
				if ( f.indent_size )
				{
					s += '\n';
					level++;
				}
				unsigned i = 0;
				for( auto &v : obj )
				{
					s += f.indent( level );
					s += '\"';
					s += v.first;
					s += '\"';
					s += ':';
					if ( f.indent_size )
					{
						s += ' ';
					}
					s += build_value( v.second, f, level );
					if ( size > 1 && i <= size - 2 )
					{
						s += ',';
					}
					if ( f.indent_size )
					{
						s += '\n';
					}
					i++;
				}
				if ( f.indent_size )
				{
					level--;
					s += f.indent( level );
				}
				s += '}';
				return s;
			}
			default:
				if ( value.type() == Value::Type::String )
				{
					s += '\"';
					s += escape_string( value.as_string() );
					s += '\"';
					return s;
				}
				return value.as_string();
			}
		};
		return build_value( value, f, 0 );
	}

	static std::string format( const std::string &json, Error &e, const Json::Format &formatter )
	{
		auto v = Json::parse( json, e );
		if (!e.empty())
		{
			return "";
		}
		return build(v, e, formatter);
	}
};


std::string Json::Format::indent( unsigned level ) const
{
	std::string res;
	for( unsigned i = level*indent_size; i > 0; i-- )
	{
		res += indent_char;
	}
	return res;
}

bool Json::validate( const std::string &json, Error &e )
{
	return JsonImpl::validate( json, e );
}

Value Json::parse( const std::string &json, Error &e )
{
	return JsonImpl::parse( json, e );
}


std::string Json::build( const Value &value, Error &e )
{
	return JsonImpl::build( value, e, Json::Format() );
}

std::string Json::build( const Value &value, Error &e, const Json::Format &formatter )
{
	return JsonImpl::build( value, e, formatter );
}

std::string Json::format( const std::string &json, Error &e, const Format &formatter )
{
	return JsonImpl::format( json, e, formatter );
}

std::string Json::minimize( const std::string &json, Error &e)
{
	return JsonImpl::format( json, e, Format() );
}

} // namespace jsoncpp
