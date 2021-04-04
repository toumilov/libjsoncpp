
#include "value.hpp"
#include <cstdlib>
#include <cerrno>
#include <limits>
#include <inttypes.h>
#include <algorithm>

namespace jsoncpp
{

static Value none;

const Value::Int Value::default_int_ = 0;
const Value::Float Value::default_float_ = 0.0f;
const Value::Bool Value::default_bool_ = false;
const Value::String Value::default_string_ = "";
const Value::Array Value::default_array_ = {};
const Value::Object Value::default_object_ = {};

Value::Value() :
	type_( Value::Type::None )
{}

Value::Value( const Value::Type t ) :
	type_( t )
{
	switch( t )
	{
	case Value::Type::None:
		break;
	case Value::Type::Int:
		data_ = default_value<Int>();
		break;
	case Value::Type::Float:
		data_ = default_value<Float>();
		break;
	case Value::Type::Bool:
		data_ = default_value<Bool>();
		break;
	case Value::Type::String:
		data_ = default_value<String>();
		break;
	case Value::Type::Array:
		data_ = default_value<Array>();
		break;
	case Value::Type::Object:
		data_ = default_value<Object>();
		break;
	}
}

Value::Value( const char *s )
{
	type_ = Type::String;
	data_ = String( s );
}

Value::Value( const Value &rhs ) :
	type_( rhs.type_ ),
	data_( rhs.data_ )
{
}

void Value::swap( Value &rhs )
{
	auto t = type_;
	auto d = std::move( data_ );
	type_ = rhs.type_;
	data_ = rhs.data_;
	rhs.type_ = t;
	rhs.data_ = std::move( d );
}

void Value::swap( Value &&rhs )
{
	auto t = type_;
	auto d = std::move( data_ );
	type_ = rhs.type_;
	data_ = rhs.data_;
	rhs.type_ = t;
	rhs.data_ = std::move( d );
}

Value& Value::operator=( const Value &value )
{
	data_ = value.data_;
	type_ = value.type_;
	return *this;
}

Value& Value::operator=( const char *value )
{
	return operator=( String( value ) );
}

Value& Value::operator=( const int32_t &value )
{
	return operator=( (Int)value );
}

Value& Value::operator=( const uint32_t &value )
{
	return operator=( (Int)value );
}

Value& Value::operator=( const uint64_t &value )
{
	if ( value > (uint64_t)std::numeric_limits<Int>::max() )
	{
		return operator=( (Int)0 );
	}
	return operator=( (Int)value );
}

Value& Value::operator=( const float &value )
{
	return operator=( (Float)value );
}

bool Value::operator==( const int32_t &value ) const
{
	auto i = get_int();
	return i == (Int)value;
}

bool Value::operator==( const uint32_t &value ) const
{
	auto i = get_int();
	if ( i < 0ll )
	{
		return false;
	}
	return i == (Int)value;
}

bool Value::operator==( const uint64_t &value ) const
{
	auto i = get_int();
	if ( i < 0ll || value > (uint64_t)std::numeric_limits<Int>::max() )
	{
		return false;
	}
	return i == (Int)value;
}

bool Value::operator==( const float &value ) const
{
	return get_float() == (Float)value;
}

bool Value::operator==( const char *value ) const
{
	auto v = Value( value );
	return operator==( v );
}

bool Value::operator==( const Value &value ) const
{
	if ( type_ != value.type() )
	{
		return false;
	}
	bool ret = true;
	switch( data_.type_index() )
	{
	case 1:
		break;
	case 2:
		ret = get_bool() == value.get_bool();
		break;
	case 3:
		ret = get_int() == value.get_int();
		break;
	case 4:
		ret = ( std::fabs( get_float() - value.get_float() ) < std::numeric_limits<Float>::epsilon() );
		break;
	case 5:
		ret = get_string() == value.get_string();
		break;
	case 6:
	{
		const auto that = get_array();
		const auto v = value.get_array();
		if ( that.size() != v.size() )
		{
			ret = false;
		}
		else
		{
			for( unsigned i = 0; i < that.size(); ++i )
			{
				if ( that[i] != v[i] )
				{
					ret = false;
					break;
				}
			}
		}
		break;
	}
	case 7:
	{
		const auto that = get_object();
		const auto v = value.get_object();
		if ( that.size() != v.size() )
		{
			ret = false;
		}
		else
		{
			for( const auto &i : that )
			{
				auto element = v.find( i.first );
				if ( element == v.end() || element->second != i.second )
				{
					ret = false;
					break;
				}
			}
		}
		break;
	}
	default:
		break;
	}
	return ret;
}

Value::operator bool() const
{
	return !empty();
}

Value& Value::operator[]( unsigned index )
{
	return at( index );
}

Value& Value::operator[]( int index )
{
	return at( index );
}

Value& Value::at( unsigned index )
{
	auto &arr = get<Array>();
	if ( type_ != Type::Array || arr.size() <= index )
	{
		none.clear();
		return none;
	}
	return arr.at( index );
}

Value& Value::at( int index )
{
	auto &arr = get<Array>();
	if ( type_ != Type::Array || arr.size() <= (size_t)index )
	{
		none.clear();
		return none;
	}
	return arr.at( (size_t)index );
}

Value& Value::back()
{
	auto &arr = get<Array>();
	if ( type_ != Type::Array || arr.size() == 0 )
	{
		none.clear();
		return none;
	}
	return arr.at( arr.size() - 1 );
}

Value& Value::operator[]( const std::string &key )
{
	return at( key );
}

const Value& Value::operator[]( const std::string &key ) const
{
	return at( key );
}

Value& Value::operator[]( const char *key )
{
	return at( key );
}

const Value& Value::operator[]( const char *key ) const
{
	return at( key );
}

Value& Value::at( const std::string &key )
{
	if ( type_ != Type::Object )
	{
		swap( Value( Type::Object ) );
	}
	auto &o = get<Object>();
	return o[key];
}

const Value& Value::at( const std::string &key ) const
{
	auto &o = get<Object>();
	if ( type_ != Type::Object || o.find( key ) == o.end() )
	{
		none.clear();
		return none;
	}
	return o.at( key );
}

Value& Value::at( const char *key )
{
	return at( std::string( key ) );
}

const Value& Value::at( const char *key ) const
{
	return at( std::string( key ) );
}

Value& Value::insert( Value &&v )
{
	if ( type_ != Type::Array )
	{
		swap( Value( Type::Array ) );
	}
	get<Array>().push_back( std::move( v ) );
	return *this;
}

Value& Value::erase( unsigned index )
{
	auto &arr = get<Array>();
	if ( type_ != Type::Array || arr.size() <= index )
	{
		none.clear();
		return none;
	}
	arr.erase( arr.begin() + index );
	return *this;
}

Value& Value::insert( const std::string &key, Value &&v )
{
	if ( type_ != Type::Object )
	{
		swap( Value( Type::Object ) );
	}
	get<Object>().insert( std::pair<std::string, Value>( key, std::move( v ) ) );
	return *this;
}

Value& Value::erase( const std::string &key )
{
	auto &o = get<Object>();
	if ( type_ != Type::Object || o.find( key ) == o.end() )
	{
		none.clear();
		return none;
	}
	o.erase( key );
	return *this;
}

Value::Type Value::type() const
{
	switch( data_.type_index() )
	{
	case 1:
		return Type::None;
	case 2:
		return Type::Bool;
	case 3:
		return Type::Int;
	case 4:
		return Type::Float;
	case 5:
		return Type::String;
	case 6:
		return Type::Array;
	case 7:
		return Type::Object;
	default:
		return Type::None;
	}
}

bool Value::has( unsigned index ) const
{
	return type_ == Type::Array && index < get<Value::Array>().size();
}

bool Value::find( ElementPredicate pred, unsigned &index ) const
{
	bool res = false;
	if ( type_ == Type::Array )
	{
		auto items = get<Value::Array>();
		for( index = 0; index < items.size(); index++ )
		{
			if ( pred( items[index] ) )
			{
				res = true;
				break;
			}
		}
	}
	return res;
}

bool Value::has( const std::string &key ) const
{
	auto o = get<Value::Object>();
	return type_ == Type::Object && o.find( key ) != o.end();
}

void Value::accept( ValueVisitor *visitor ) const
{
	data_.accept( *visitor );
}

bool Value::empty() const
{
	return is_none();
}

void Value::clear()
{
	data_.clear();
	type_ = Type::None;
}

unsigned Value::size() const
{
	switch( data_.type_index() )
	{
	case 1:
		return 0u;
	case 2:
		return sizeof( Bool );
	case 3:
		return sizeof( Int );
	case 4:
		return sizeof( Float );
	case 5:
		return get_string().size();
	case 6:
		return get_array().size();
	case 7:
		return get_object().size();
	default:
		return 0u;
	}
}

bool Value::is( Type t ) const
{
	return type() == t;
}

const Value::Int& Value::default_value( const Value::Int* )       { return default_int_;    }
const Value::Float& Value::default_value( const Value::Float* )   { return default_float_;  }
const Value::Bool& Value::default_value( const Value::Bool* )     { return default_bool_;   }
const Value::String& Value::default_value( const Value::String* ) { return default_string_; }
const Value::Array& Value::default_value( const Value::Array* )   { return default_array_;  }
const Value::Object& Value::default_value( const Value::Object* ) { return default_object_; }

Value Value::as( Type t ) const
{
	Value v( t );
	switch( type_ )
	{
	case Value::Type::None:
		switch( t )
		{
		case Value::Type::None:   break;
		case Value::Type::Bool:   break;
		case Value::Type::Int:    break;
		case Value::Type::Float:  break;
		case Value::Type::String: v = "null"; break;
		case Value::Type::Array:  break;
		case Value::Type::Object: break;
		}
		break;
	case Value::Type::Bool:
		if ( is_convertable( t ) )
		{
			switch( t )
			{
			case Value::Type::None:   break;
			case Value::Type::Bool:   v = (Bool)*data_.get<Bool>(); break;
			case Value::Type::Int:    v = (Int)*data_.get<Bool>(); break;
			case Value::Type::Float:  v = (Float)*data_.get<Bool>(); break;
			case Value::Type::String: v = *data_.get<Bool>() ? "true": "false"; break;
			case Value::Type::Array:  break;
			case Value::Type::Object: break;
			}
		}
		break;
	case Value::Type::Int:
		if ( is_convertable( t ) )
		{
			switch( t )
			{
			case Value::Type::None:   break;
			case Value::Type::Bool:   v = (Bool)*data_.get<Int>(); break;
			case Value::Type::Int:    v = (Int)*data_.get<Int>(); break;
			case Value::Type::Float:  v = (Float)*data_.get<Int>(); break;
			case Value::Type::String:
			{
				char buf[21];
				snprintf( buf, sizeof( buf ), "%" PRId64, *data_.get<Int>() );
				v = std::string( buf );
				break;
			}
			case Value::Type::Array:  break;
			case Value::Type::Object: break;
			}
		}
		break;
	case Value::Type::Float:
		if ( is_convertable( t ) )
		{
			switch( t )
			{
			case Value::Type::None:   break;
			case Value::Type::Bool:   v = (Bool)*data_.get<Float>(); break;
			case Value::Type::Int:    v = (Int)*data_.get<Float>(); break;
			case Value::Type::Float:  v = (Float)*data_.get<Float>(); break;
			case Value::Type::String:
			{
				char buf[50];
				snprintf( buf, sizeof( buf ), "%g", *data_.get<Float>() );
				v = std::string( buf );
				break;
			}
			case Value::Type::Array:  break;
			case Value::Type::Object: break;
			}
		}
		break;
	case Value::Type::String:
		if ( is_convertable( t ) )
		{
			auto s = get<String>();
			switch( t )
			{
			case Value::Type::None: break;
			case Value::Type::Bool:
			{
				std::transform( s.begin(), s.end(), s.begin(), tolower );
				v = ( s == "true" || s == "1" ) ? true : false;
				break;
			}
			case Value::Type::Int:
			{
				v = (Value::Int)strtol( s.c_str(), nullptr, 10 );
				break;
			}
			case Value::Type::Float:
			{
				v = (Value::Float)strtod( s.c_str(), nullptr );
				break;
			}
			case Value::Type::String:
			{
				v = s;
				break;
			}
			case Value::Type::Array:  break;
			case Value::Type::Object: break;
			}
		}
		break;
	case Value::Type::Array:
		if ( t == Value::Type::Array )
		{
			v = get<Array>();
		}
		break;
	case Value::Type::Object:
		if ( t == Value::Type::Object )
		{
			v = get<Object>();
		}
		break;
	}

	return v;
}


bool Value::is_convertable( const Value::Type t ) const
{
	switch( type_ )
	{
	case Value::Type::None:
		switch( t )
		{
		case Value::Type::None:   return true;
		case Value::Type::Bool:   return false;
		case Value::Type::Int:    return false;
		case Value::Type::Float:  return false;
		case Value::Type::String: return false;
		case Value::Type::Array:  return false;
		case Value::Type::Object: return false;
		}
		break;
	case Value::Type::Bool:
		switch( t )
		{
		case Value::Type::None:   return true;
		case Value::Type::Bool:   return true;
		case Value::Type::Int:    return true;
		case Value::Type::Float:  return true;
		case Value::Type::String: return true;
		case Value::Type::Array:  return false;
		case Value::Type::Object: return false;
		}
		break;
	case Value::Type::Int: //-9223372036854775808 : 9223372036854775807
		switch( t )
		{
		case Value::Type::None:   return true;
		case Value::Type::Bool:   return true;
		case Value::Type::Int:    return true;
		case Value::Type::Float:  return true;
		case Value::Type::String: return true;
		case Value::Type::Array:  return false;
		case Value::Type::Object: return false;
		}
		break;
	case Value::Type::Float: //2.22507e-308 : 1.79769e+308
		switch( t )
		{
		case Value::Type::None:   return true;
		case Value::Type::Bool:   return true;
		case Value::Type::Int:    return false;
		case Value::Type::Float:  return true;
		case Value::Type::String: return true;
		case Value::Type::Array:  return false;
		case Value::Type::Object: return false;
		}
		break;
	case Value::Type::String:
		switch( t )
		{
		case Value::Type::None:   return true;
		case Value::Type::Bool:
		{
			auto s = get<String>();
			std::transform( s.begin(), s.end(), s.begin(), tolower );
			return ( s == "true" || s == "1" || s == "false" || s == "0" );
		}
		case Value::Type::Int:
		{
			auto s = get<String>();
			char* end_ptr = nullptr;
			errno = 0;
			auto ret __attribute__((unused)) = strtoll( s.c_str(), &end_ptr, 10 );
			return ( errno == 0 && end_ptr == ( s.data() + s.length() ) );
		}
		case Value::Type::Float:
		{
			auto s = get<String>();
			char* end_ptr = nullptr;
			errno = 0;
			auto ret __attribute__((unused)) = strtod( s.c_str(), &end_ptr );
			return ( errno == 0 && end_ptr == ( s.data() + s.length() ) );
		}
		case Value::Type::String: return true;
		case Value::Type::Array:  return false;
		case Value::Type::Object: return false;
		}
		break;
	case Value::Type::Array:
		switch( t )
		{
		case Value::Type::None:   return true;
		case Value::Type::Bool:   return false;
		case Value::Type::Int:    return false;
		case Value::Type::Float:  return false;
		case Value::Type::String: return false;
		case Value::Type::Array:  return true;
		case Value::Type::Object: return false;
		}
		break;
	case Value::Type::Object:
		switch( t )
		{
		case Value::Type::None:   return true;
		case Value::Type::Bool:   return false;
		case Value::Type::Int:    return false;
		case Value::Type::Float:  return false;
		case Value::Type::String: return false;
		case Value::Type::Array:  return false;
		case Value::Type::Object: return true;
		}
		break;
	}
	return false;
}

int32_t Value::as_int32() const
{
	Int i = as( Value::Type::Int ).get_int();
	if ( i > (Int)std::numeric_limits<int32_t>::max() )
	{
		return 0;
	}
	return (int32_t)i;
}

uint32_t Value::as_uint32() const
{
	Int i = as( Value::Type::Int ).get_int();
	if ( i < 0ll )
	{
		return 0u;
	}
	return (uint32_t)i;
}

uint64_t Value::as_uint64() const
{
	Int i = as( Value::Type::Int ).get_int();
	if ( i < 0ll )
	{
		return 0ull;
	}
	return (uint64_t)i;
}

float Value::as_float() const
{
	Float f = as( Value::Type::Float ).get_float();
	if ( f < (Float)std::numeric_limits<float>::min() ||
		 f > (Float)std::numeric_limits<float>::max() )
	{
		return 0.0f;
	}
	return (float)f;
}

bool operator<( const Value::Type lhs, const Value::Type rhs )
{
	return (unsigned)lhs < (unsigned)rhs;
}

const char* to_string( Value::Type type )
{
	static const std::map<Value::Type, const char*> mapping =
	{
		{ Value::Type::None,   "null"   },
		{ Value::Type::Bool,   "bool"   },
		{ Value::Type::Int,    "int"    },
		{ Value::Type::Float,  "float"  },
		{ Value::Type::String, "string" },
		{ Value::Type::Array,  "array"  },
		{ Value::Type::Object, "object" }
	};
	auto i = mapping.find( type );
	if ( i != mapping.end() )
	{
		return i->second;
	}
	return mapping.at( Value::Type::None );
}

} // namespace jsoncpp
