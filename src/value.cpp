
#include "value.hpp"
#include <cstdlib>
#include <cerrno>
#include <limits>
#include <inttypes.h>
#include <algorithm>

namespace jsoncpp
{

static Value none;

const Value::Int32 Value::default_int32_ = 0;
const Value::Int64 Value::default_int64_ = 0ll;
const Value::Uint32 Value::default_uint32_ = 0u;
const Value::Uint64 Value::default_uint64_ = 0ull;
const Value::Float Value::default_float_ = 0.0f;
const Value::Double Value::default_double_ = 0.0;
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
	case Value::Type::Int32:
		data_ = default_value<Int32>();
		break;
	case Value::Type::Int64:
		data_ = default_value<Int64>();
		break;
	case Value::Type::Uint32:
		data_ = default_value<Uint32>();
		break;
	case Value::Type::Uint64:
		data_ = default_value<Uint64>();
		break;
	case Value::Type::Float:
		data_ = default_value<Float>();
		break;
	case Value::Type::Double:
		data_ = default_value<Double>();
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
	static struct
	{
		bool res = true;
		const Value *v;
		void operator()() {}
		void operator()( const Int32  &data ) { res = data == v->get<Int32>();  }
		void operator()( const Int64  &data ) { res = data == v->get<Int64>();  }
		void operator()( const Uint32 &data ) { res = data == v->get<Uint32>(); }
		void operator()( const Uint64 &data ) { res = data == v->get<Uint64>(); }
		void operator()( const Float  &data ) { res = ( std::fabs( data - v->get<Float>() ) < std::numeric_limits<Float>::epsilon() ); }
		void operator()( const Double &data ) { res = ( std::fabs( data - v->get<Double>() ) < std::numeric_limits<Double>::epsilon() ); }
		void operator()( const Bool   &data ) { res = data == v->get<Bool>();   }
		void operator()( const String &data ) { res = data == v->get<String>(); }
		void operator()( const Array  &data )
		{
			const auto value = v->get<Array>();
			if ( data.size() != value.size() )
			{
				res = false;
			}
			else
			{
				for( unsigned i = 0; i < data.size(); ++i )
				{
					if ( data[i] != value[i] )
					{
						res = false;
						break;
					}
				}
			}
		}
		void operator()( const Object &data )
		{
			const auto value = v->get<Object>();
			if ( data.size() != value.size() )
			{
				res = false;
			}
			else
			{
				for( const auto &i : data )
				{
					auto element = value.find( i.first );
					if ( element == value.end() || element->second != i.second )
					{
						res = false;
						break;
					}
				}
			}
		}
	} visitor;

	visitor.v = &value;
	data_.accept( visitor );
	return visitor.res;
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
	static struct
	{
		Value::Type t = Value::Type::None;
		void operator()() { t = Type::None; }
		void operator()( const Int32& ) { t = Type::Int32; }
		void operator()( const Int64& ) { t = Type::Int64; }
		void operator()( const Uint32& ) { t = Type::Uint32; }
		void operator()( const Uint64& ) { t = Type::Uint64; }
		void operator()( const Float& ) { t = Type::Float; }
		void operator()( const Double& ) { t = Type::Double; }
		void operator()( const Bool& ) { t = Type::Bool; }
		void operator()( const String& ) { t = Type::String; }
		void operator()( const Array& ) { t = Type::Array; }
		void operator()( const Object& ) { t = Type::Object; }
	} visitor;

	data_.accept( visitor );
	return visitor.t;
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

bool Value::is_none() const
{
	return is( Type::None );
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
	struct
	{
		unsigned size = 0;
		void operator()() {}
		void operator()( const Int32& )	{ size = sizeof( Int32 );  }
		void operator()( const Int64& )	{ size = sizeof( Int64 );  }
		void operator()( const Uint32& )   { size = sizeof( Uint32 ); }
		void operator()( const Uint64& )   { size = sizeof( Uint64 ); }
		void operator()( const Float& )	{ size = sizeof( Float );  }
		void operator()( const Double& )   { size = sizeof( Double ); }
		void operator()( const Bool& )	 { size = sizeof( Bool );   }
		void operator()( const String &s ) { size = s.size(); }
		void operator()( const Array &a )  { size = a.size(); }
		void operator()( const Object &o ) { size = o.size(); }
	} visitor;

	data_.accept( visitor );
	return visitor.size;
}

bool Value::is( Type t ) const
{
	return type() == t;
}

const Value::Int32& Value::default_value( const Value::Int32* )   { return default_int32_;  }
const Value::Int64& Value::default_value( const Value::Int64* )   { return default_int64_;  }
const Value::Uint32& Value::default_value( const Value::Uint32* ) { return default_uint32_; }
const Value::Uint64& Value::default_value( const Value::Uint64* ) { return default_uint64_; }
const Value::Float& Value::default_value( const Value::Float* )   { return default_float_;  }
const Value::Double& Value::default_value( const Value::Double* ) { return default_double_; }
const Value::Bool& Value::default_value( const Value::Bool* )	 { return default_bool_;   }
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
		case Value::Type::Int32:  break;
		case Value::Type::Int64:  break;
		case Value::Type::Uint32: break;
		case Value::Type::Uint64: break;
		case Value::Type::Float:  break;
		case Value::Type::Double: break;
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
			case Value::Type::Int32:  v = (Int32)*data_.get<Bool>(); break;
			case Value::Type::Int64:  v = (Int64)*data_.get<Bool>(); break;
			case Value::Type::Uint32: v = (Uint32)*data_.get<Bool>(); break;
			case Value::Type::Uint64: v = (Uint64)*data_.get<Bool>(); break;
			case Value::Type::Float:  v = (Float)*data_.get<Bool>(); break;
			case Value::Type::Double: v = (Double)*data_.get<Bool>(); break;
			case Value::Type::String: v = *data_.get<Bool>() ? "true": "false"; break;
			case Value::Type::Array:  break;
			case Value::Type::Object: break;
			}
		}
		break;
	case Value::Type::Int32:
		if ( is_convertable( t ) )
		{
			switch( t )
			{
			case Value::Type::None:   break;
			case Value::Type::Bool:   v = (Bool)*data_.get<Int32>(); break;
			case Value::Type::Int32:  v = (Int32)*data_.get<Int32>(); break;
			case Value::Type::Int64:  v = (Int64)*data_.get<Int32>(); break;
			case Value::Type::Uint32: v = (Uint32)*data_.get<Int32>(); break;
			case Value::Type::Uint64: v = (Uint64)*data_.get<Int32>(); break;
			case Value::Type::Float:  v = (Float)*data_.get<Int32>(); break;
			case Value::Type::Double: v = (Double)*data_.get<Int32>(); break;
			case Value::Type::String:
			{
				char buf[12];
				snprintf( buf, sizeof( buf ), "%d", *data_.get<Int32>() );
				v = std::string( buf );
				break;
			}
			case Value::Type::Array:  break;
			case Value::Type::Object: break;
			}
		}
		break;
	case Value::Type::Int64:
		if ( is_convertable( t ) )
		{
			switch( t )
			{
			case Value::Type::None:   break;
			case Value::Type::Bool:   v = (Bool)*data_.get<Int64>(); break;
			case Value::Type::Int32:  v = (Int32)*data_.get<Int64>(); break;
			case Value::Type::Int64:  v = (Int64)*data_.get<Int64>(); break;
			case Value::Type::Uint32: v = (Uint32)*data_.get<Int64>(); break;
			case Value::Type::Uint64: v = (Uint64)*data_.get<Int64>(); break;
			case Value::Type::Float:  v = (Float)*data_.get<Int64>(); break;
			case Value::Type::Double: v = (Double)*data_.get<Int64>(); break;
			case Value::Type::String:
			{
				char buf[21];
				snprintf( buf, sizeof( buf ), "%" PRId64, *data_.get<Int64>() );
				v = std::string( buf );
				break;
			}
			case Value::Type::Array:  break;
			case Value::Type::Object: break;
			}
		}
		break;
	case Value::Type::Uint32:
		if ( is_convertable( t ) )
		{
			switch( t )
			{
			case Value::Type::None:   break;
			case Value::Type::Bool:   v = (Bool)*data_.get<Uint32>(); break;
			case Value::Type::Int32:  v = (Int32)*data_.get<Uint32>(); break;
			case Value::Type::Int64:  v = (Int64)*data_.get<Uint32>(); break;
			case Value::Type::Uint32: v = (Uint32)*data_.get<Uint32>(); break;
			case Value::Type::Uint64: v = (Uint64)*data_.get<Uint32>(); break;
			case Value::Type::Float:  v = (Float)*data_.get<Uint32>(); break;
			case Value::Type::Double: v = (Double)*data_.get<Uint32>(); break;
			case Value::Type::String:
			{
				char buf[12];
				snprintf( buf, sizeof( buf ), "%u", *data_.get<Uint32>() );
				v = std::string( buf );
				break;
			}
			case Value::Type::Array:  break;
			case Value::Type::Object: break;
			}
		}
		break;
	case Value::Type::Uint64:
		if ( is_convertable( t ) )
		{
			switch( t )
			{
			case Value::Type::None:   break;
			case Value::Type::Bool:   v = (Bool)*data_.get<Uint64>(); break;
			case Value::Type::Int32:  v = (Int32)*data_.get<Uint64>(); break;
			case Value::Type::Int64:  v = (Int64)*data_.get<Uint64>(); break;
			case Value::Type::Uint32: v = (Uint32)*data_.get<Uint64>(); break;
			case Value::Type::Uint64: v = (Uint64)*data_.get<Uint64>(); break;
			case Value::Type::Float:  v = (Float)*data_.get<Uint64>(); break;
			case Value::Type::Double: v = (Double)*data_.get<Uint64>(); break;
			case Value::Type::String:
			{
				char buf[21];
				snprintf( buf, sizeof( buf ), "%" PRIu64, *data_.get<Uint64>() );
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
			case Value::Type::Int32:  v = (Int32)*data_.get<Float>(); break;
			case Value::Type::Int64:  v = (Int64)*data_.get<Float>(); break;
			case Value::Type::Uint32: v = (Uint32)*data_.get<Float>(); break;
			case Value::Type::Uint64: v = (Uint64)*data_.get<Float>(); break;
			case Value::Type::Float:  v = (Float)*data_.get<Float>(); break;
			case Value::Type::Double: v = (Double)*data_.get<Float>(); break;
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
	case Value::Type::Double:
		if ( is_convertable( t ) )
		{
			switch( t )
			{
			case Value::Type::None:   break;
			case Value::Type::Bool:   v = (Bool)*data_.get<Double>(); break;
			case Value::Type::Int32:  v = (Int32)*data_.get<Double>(); break;
			case Value::Type::Int64:  v = (Int64)*data_.get<Double>(); break;
			case Value::Type::Uint32: v = (Uint32)*data_.get<Double>(); break;
			case Value::Type::Uint64: v = (Uint64)*data_.get<Double>(); break;
			case Value::Type::Float:  v = (Float)*data_.get<Double>(); break;
			case Value::Type::Double: v = (Double)*data_.get<Double>(); break;
			case Value::Type::String:
			{
				char buf[320];
				snprintf( buf, sizeof( buf ), "%g", *data_.get<Double>() );
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
			case Value::Type::Int32:
			{
				v = (Value::Int32)strtol( s.c_str(), nullptr, 10 );
				break;
			}
			case Value::Type::Int64:
			{
				v = (Value::Int64)strtoll( s.c_str(), nullptr, 10 );
				break;
			}
			case Value::Type::Uint32:
			{
				v = (Value::Uint32)strtoul( s.c_str(), nullptr, 10 );
				break;
			}
			case Value::Type::Uint64:
			{
				v = (Value::Uint64)strtoull( s.c_str(), nullptr, 10 );
				break;
			}
			case Value::Type::Float:
			{
				v = (Value::Float)strtof( s.c_str(), nullptr );
				break;
			}
			case Value::Type::Double:
			{
				v = (Value::Double)strtod( s.c_str(), nullptr );
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

Value::Int32 Value::as_int32() const
{
	return as( Value::Type::Int32 ).get<Value::Int32>();
}

Value::Int64 Value::as_int64() const
{
	return as( Value::Type::Int64 ).get<Value::Int64>();
}

Value::Uint32 Value::as_uint32() const
{
	return as( Value::Type::Uint32 ).get<Value::Uint32>();
}

Value::Uint64 Value::as_uint64() const
{
	return as( Value::Type::Uint64 ).get<Value::Uint64>();
}

Value::Float Value::as_float() const
{
	return as( Value::Type::Float ).get<Value::Float>();
}

Value::Double Value::as_double() const
{
	return as( Value::Type::Double ).get<Value::Double>();
}

Value::Bool Value::as_bool() const
{
	return as( Value::Type::Bool ).get<Value::Bool>();
}

Value::String Value::as_string() const
{
	return as( Value::Type::String ).get<Value::String>();
}

Value::Array Value::as_array() const
{
	return as( Value::Type::Array ).get<Value::Array>();
}

Value::Object Value::as_object() const
{
	return as( Value::Type::Object ).get<Value::Object>();
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
		case Value::Type::Int32:  return false;
		case Value::Type::Int64:  return false;
		case Value::Type::Uint32: return false;
		case Value::Type::Uint64: return false;
		case Value::Type::Float:  return false;
		case Value::Type::Double: return false;
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
		case Value::Type::Int32:  return true;
		case Value::Type::Int64:  return true;
		case Value::Type::Uint32: return true;
		case Value::Type::Uint64: return true;
		case Value::Type::Float:  return true;
		case Value::Type::Double: return true;
		case Value::Type::String: return true;
		case Value::Type::Array:  return false;
		case Value::Type::Object: return false;
		}
		break;
	case Value::Type::Int32://-2147483648 : 2147483647
		switch( t )
		{
		case Value::Type::None:   return true;
		case Value::Type::Bool:   return true;
		case Value::Type::Int32:  return true;
		case Value::Type::Int64:  return true;
		case Value::Type::Uint32: return get<Int32>() >= 0;
		case Value::Type::Uint64: return get<Int32>() >= 0;
		case Value::Type::Float:  return true;
		case Value::Type::Double: return true;
		case Value::Type::String: return true;
		case Value::Type::Array:  return false;
		case Value::Type::Object: return false;
		}
		break;
	case Value::Type::Int64: //-9223372036854775808 : 9223372036854775807
		switch( t )
		{
		case Value::Type::None:   return true;
		case Value::Type::Bool:   return true;
		case Value::Type::Int32:
			return ( get<Int64>() >= (Int64)std::numeric_limits<Int32>::min() &&
					 get<Int64>() <= (Int64)std::numeric_limits<Int32>::max() );
		case Value::Type::Int64:  return true;
		case Value::Type::Uint32:
		{
			return ( get<Int64>() >= 0 &&
					 get<Int64>() <= std::numeric_limits<Uint32>::max() );
		}
		case Value::Type::Uint64: return get<Int64>() >= 0;
		case Value::Type::Float:  return true;
		case Value::Type::Double: return true;
		case Value::Type::String: return true;
		case Value::Type::Array:  return false;
		case Value::Type::Object: return false;
		}
		break;
	case Value::Type::Uint32: //0 : 4294967295
		switch( t )
		{
		case Value::Type::None:   return true;
		case Value::Type::Bool:   return true;
		case Value::Type::Int32:  return get<Uint32>() <= (Uint32)std::numeric_limits<Int32>::max();
		case Value::Type::Int64:  return true;
		case Value::Type::Uint32: return true;
		case Value::Type::Uint64: return true;
		case Value::Type::Float:  return true;
		case Value::Type::Double: return true;
		case Value::Type::String: return true;
		case Value::Type::Array:  return false;
		case Value::Type::Object: return false;
		}
		break;
	case Value::Type::Uint64: //0 : 18446744073709551615
		switch( t )
		{
		case Value::Type::None:   return true;
		case Value::Type::Bool:   return true;
		case Value::Type::Int32:  return get<Uint64>() <= (Uint64)std::numeric_limits<Int32>::max();
		case Value::Type::Int64:  return get<Uint64>() <= (Uint64)std::numeric_limits<Int64>::max();
		case Value::Type::Uint32: return get<Uint64>() <= (Uint64)std::numeric_limits<Uint32>::max();
		case Value::Type::Uint64: return true;
		case Value::Type::Float:  return true;
		case Value::Type::Double: return true;
		case Value::Type::String: return true;
		case Value::Type::Array:  return false;
		case Value::Type::Object: return false;
		}
		break;
	case Value::Type::Float: //1.17549e-38 : 3.40282e+38
		switch( t )
		{
		case Value::Type::None:   return true;
		case Value::Type::Bool:   return true;
		case Value::Type::Int32:  return false;
		case Value::Type::Int64:  return false;
		case Value::Type::Uint32: return false;
		case Value::Type::Uint64: return false;
		case Value::Type::Float:  return true;
		case Value::Type::Double: return true;
		case Value::Type::String: return true;
		case Value::Type::Array:  return false;
		case Value::Type::Object: return false;
		}
		break;
	case Value::Type::Double: //2.22507e-308 : 1.79769e+308
		switch( t )
		{
		case Value::Type::None:   return true;
		case Value::Type::Bool:   return true;
		case Value::Type::Int32:  return false;
		case Value::Type::Int64:  return false;
		case Value::Type::Uint32: return false;
		case Value::Type::Uint64: return false;
		case Value::Type::Float:  return false;
		case Value::Type::Double: return true;
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
		case Value::Type::Int32:
		{
			auto s = get<String>();
			char* end_ptr = nullptr;
			errno = 0;
			auto ret __attribute__((unused)) = strtol( s.c_str(), &end_ptr, 10 );
			return ( errno == 0 && end_ptr == ( s.data() + s.length() ) );
		}
		case Value::Type::Int64:
		{
			auto s = get<String>();
			char* end_ptr = nullptr;
			errno = 0;
			auto ret __attribute__((unused)) = strtoll( s.c_str(), &end_ptr, 10 );
			return ( errno == 0 && end_ptr == ( s.data() + s.length() ) );
		}
		case Value::Type::Uint32:
		{
			auto s = get<String>();
			if ( s[0] == '-' ) return false;
			char* end_ptr = nullptr;
			errno = 0;
			auto ret __attribute__((unused)) = strtoul( s.c_str(), &end_ptr, 10 );
			return ( errno == 0 && end_ptr == ( (char*)s.data() + s.length() ) );
		}
		case Value::Type::Uint64:
		{
			auto s = get<String>();
			if ( s[0] == '-' ) return false;
			char* end_ptr = nullptr;
			errno = 0;
			auto ret __attribute__((unused)) = strtoull( s.c_str(), &end_ptr, 10 );
			return ( errno == 0 && end_ptr == ( s.data() + s.length() ) );
		}
		case Value::Type::Float:
		{
			auto s = get<String>();
			char* end_ptr = nullptr;
			errno = 0;
			auto ret __attribute__((unused)) = strtof( s.c_str(), &end_ptr );
			return ( errno == 0 && end_ptr == ( s.data() + s.length() ) );
		}
		case Value::Type::Double:
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
		case Value::Type::Int32:  return false;
		case Value::Type::Int64:  return false;
		case Value::Type::Uint32: return false;
		case Value::Type::Uint64: return false;
		case Value::Type::Float:  return false;
		case Value::Type::Double: return false;
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
		case Value::Type::Int32:  return false;
		case Value::Type::Int64:  return false;
		case Value::Type::Uint32: return false;
		case Value::Type::Uint64: return false;
		case Value::Type::Float:  return false;
		case Value::Type::Double: return false;
		case Value::Type::String: return false;
		case Value::Type::Array:  return false;
		case Value::Type::Object: return true;
		}
		break;
	}
	return false;
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
		{ Value::Type::Int32,  "int32"  },
		{ Value::Type::Int64,  "int64"  },
		{ Value::Type::Uint32, "uint32" },
		{ Value::Type::Uint64, "uint64" },
		{ Value::Type::Float,  "float"  },
		{ Value::Type::Double, "double" },
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
