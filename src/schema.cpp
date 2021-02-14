
#include <string>
#include <regex>
#include "json.hpp"
#include "schema.hpp"

namespace jsoncpp
{

class ItemBase
{
public:
	std::string name;

	ItemBase() :
		parent_( nullptr )
	{
	}
	ItemBase( const std::string &name, ItemBase *parent ) :
		name( name ),
		parent_( parent )
	{
	}

	virtual ~ItemBase() {}
	virtual void parse( const Value &/*schema*/, Error &/*e*/ ) {}
	virtual bool validate( const Value &value, Error &e ) = 0;

protected:
	ItemBase *parent_;

	std::string path()
	{
		std::string p;
		if ( parent_ )
		{
			p = parent_->path();
		}
		p += "/";
		p += name;
		return p;
	}
};

class NoValidateItem : public ItemBase
{
public:
	NoValidateItem() :
		validation_( false )
	{}
	NoValidateItem( bool validation ) :
		validation_( validation )
	{}

	void parse( const Value &schema, Error &e ) override
	{
		if ( !schema.is( Value::Type::Bool ) )
		{
			e = Error( Error::UnexpectedType, "Unexpected type[%s]: %s", path().c_str(), to_string( schema.type() ) );
			return;
		}
		validation_ = schema.as_bool();
	}

	bool validate( const Value &value, Error &e ) override
	{
		if ( !validation_ )
		{
			e = Error( Error::BadValue, "Failed [%s]", path().c_str() );
		}
		return validation_;
	}

private:
	bool validation_;
};


class NullItem : public ItemBase
{
public:
	bool validate( const Value &value, Error &e ) override
	{
		if ( !value.is_none() )
		{
			e = Error( Error::UnexpectedType, "Unexpected type[%s]: %s", path().c_str(), to_string( value.type() ) );
			return false;
		}
		return true;
	}
};

class BoolItem : public ItemBase
{
public:
	bool validate( const Value &value, Error &e ) override
	{
		if ( !value.is( Value::Type::Bool ) )
		{
			e = Error( Error::UnexpectedType, "Unexpected type[%s]: %s", path().c_str(), to_string( value.type() ) );
			return false;
		}
		return true;
	}
};

class StringItem : public ItemBase
{
public:
	void parse( const Value &schema, Error &e ) override
	{
		for( auto &i : schema.as_object() )
		{
			if ( i.first == "minLength" )
			{
				min_length( i.second, e );
			}
			else if ( i.first == "maxLength" )
			{
				max_length( i.second, e );
			}
			else if ( i.first == "pattern" )
			{
				pattern( i.second, e );
			}
			else if ( i.first == "enum" )
			{
				enumeration( i.second, e );
			}
			// TODO: format
//			else
//			{
//				e = Error( Error::UnexpectedToken, "Unexpected element[%s]: %s", path().c_str(), i.first.c_str() );
//			}
		}
	}

	bool validate( const Value &value, Error &e ) override
	{
		if ( !value.is( Value::Type::String ) )
		{
			e = Error( Error::UnexpectedType, "Unexpected type[%s]: %s", path().c_str(), to_string( value.type() ) );
			return false;
		}
		auto s = value.get<Value::String>();
		if ( min_length_ && s.size() < *min_length_ )
		{
			e = Error( Error::OutOfRange, "minLength[%s]: %u", path().c_str(), *min_length_ );
			return false;
		}
		if ( max_length_ && s.size() > *max_length_ )
		{
			e = Error( Error::OutOfRange, "maxLength[%s]: %u", path().c_str(), *max_length_ );
			return false;
		}
		if ( pattern_ && !std::regex_match( s, std::regex( *pattern_ ) ) )
		{
			e = Error( Error::NoMatch, "pattern[%s]: %s", path().c_str(), pattern_->c_str() );
			return false;
		}
		if ( !enum_.empty() )
		{
			for( auto &i : enum_ )
			{
				if ( i == s )
				{
					return true;
				}
			}
			return false;
		}
		return true;
	}

private:
	std::unique_ptr<unsigned> min_length_;
	std::unique_ptr<unsigned> max_length_;
	std::unique_ptr<std::string> pattern_;
	std::vector<std::string> enum_;

	void min_length( const Value &schema, Error &e )
	{
		if ( !schema.is( Value::Type::Uint32 ) )
		{
			e = Error( Error::UnexpectedType, "Unexpected type[%s]: %s", path().c_str(), to_string( schema.type() ) );
			return;
		}
		min_length_.reset( new unsigned( schema.as_uint32() ) );
	}

	void max_length( const Value &schema, Error &e )
	{
		if ( !schema.is( Value::Type::Uint32 ) )
		{
			e = Error( Error::UnexpectedType, "Unexpected type[%s]: %s", path().c_str(), to_string( schema.type() ) );
			return;
		}
		max_length_.reset( new unsigned( schema.as_uint32() ) );
	}

	void pattern( const Value &schema, Error &e )
	{
		if ( !schema.is( Value::Type::String ) )
		{
			e = Error( Error::UnexpectedType, "Unexpected type[%s]: %s", path().c_str(), to_string( schema.type() ) );
			return;
		}
		pattern_.reset( new std::string( schema.as_string() ) );
	}

	void enumeration( const Value &schema, Error &e )
	{
		if ( !schema.is( Value::Type::Array ) )
		{
			e = Error( Error::UnexpectedType, "Unexpected type[%s]: %s", path().c_str(), to_string( schema.type() ) );
			return;
		}
		for( auto &i : schema.get<Value::Array>() )
		{
			if ( !i.is( Value::Type::String ) )
			{
				e = Error( Error::UnexpectedType, "Unexpected type[%s]: %s", path().c_str(), to_string( schema.type() ) );
				break;
			}
			enum_.push_back( i.get<Value::String>() );
		}
	}
};

class NumberItem : public ItemBase
{
public:
	NumberItem() :
		min_exclusive_( false ),
		max_exclusive_( false )
	{}

	void parse( const Value &schema, Error &e ) override
	{
		for( auto &i : schema.as_object() )
		{
			if ( i.first == "multipleOf" )
			{
				multiple_of( i.second, e );
			}
			else if ( i.first == "minimum" )
			{
				range( i.second, min_, e );
			}
			else if ( i.first == "exclusiveMinimum" )
			{
				min_exclusive_ = true;
				range( i.second, min_, e );
			}
			else if ( i.first == "maximum" )
			{
				range( i.second, max_, e );
			}
			else if ( i.first == "exclusiveMaximum" )
			{
				max_exclusive_ = true;
				range( i.second, max_, e );
			}
			else
			{
				e = Error( Error::UnexpectedToken, "Unexpected element[%s]: %s", path().c_str(), i.first.c_str() );
			}
		}
	}

	bool validate( const Value &value, Error &e ) override
	{
		// TODO
		return false;
	}

private:
	std::unique_ptr<Variant<uint64_t, double> > multiple_of_;
	std::unique_ptr<Variant<uint64_t, double> > min_;
	std::unique_ptr<Variant<uint64_t, double> > max_;
	bool min_exclusive_;
	bool max_exclusive_;

	void multiple_of( const Value &schema, Error &e )
	{
		if ( schema.is( Value::Type::Uint32 ) || schema.is( Value::Type::Uint64 ) )
		{
			multiple_of_.reset( new Variant<uint64_t, double>( schema.as_uint64() ) );
		}
		else if ( schema.is( Value::Type::Float ) || schema.is( Value::Type::Double ) )
		{
			multiple_of_.reset( new Variant<uint64_t, double>( schema.as_double() ) );
		}
		else
		{
			e = Error( Error::UnexpectedType, "Unexpected type[%s]: %s", path().c_str(), to_string( schema.type() ) );
		}
	}

	void range( const Value &schema, std::unique_ptr<Variant<uint64_t, double> > &v, Error &e )
	{
		if ( schema.is( Value::Type::Uint32 ) || schema.is( Value::Type::Uint64 ) )
		{
			v.reset( new Variant<uint64_t, double>( schema.as_uint64() ) );
		}
		else if ( schema.is( Value::Type::Float ) || schema.is( Value::Type::Double ) )
		{
			v.reset( new Variant<uint64_t, double>( schema.as_double() ) );
		}
		else
		{
			e = Error( Error::UnexpectedType, "Unexpected type[%s]: %s", path().c_str(), to_string( schema.type() ) );
		}
	}
};

class SchemaDefinition
{
public:
	bool validate( const Value &value, Error &e )
	{
		if ( !schema_ )
		{
			e = Error( Error::NoSchema, "No schema" );
			return false;
		}
		return schema_->validate( value, e );
	}

	void parse( const Value &schema, Error &e )
	{
		if ( schema.is( Value::Type::Bool ) )
		{
			schema_.reset( new NoValidateItem() );
			schema_->parse( schema, e );
		}
		else if ( schema.is( Value::Type::Object ) )
		{
			auto o = schema.as_object();
			if ( o.empty() )
			{
				schema_.reset( new NoValidateItem( true ) );
			}
			else
			{
				for( auto &i : o )
				{
					if ( i.first == "$id" )
					{
						if ( i.second.is( Value::Type::String ) )
						{
							id_ = i.second.as_string();
						}
						else
						{
							e = Error( Error::UnexpectedType, "Unexpected type[%s]: %s", "$id", to_string( i.second.type() ) );
						}
					}
					else if ( i.first == "$schema" )
					{
						if ( i.second.is( Value::Type::String ) )
						{
							name_ = i.second.as_string();
						}
						else
						{
							e = Error( Error::UnexpectedType, "Unexpected type[%s]: %s", "$schema", to_string( i.second.type() ) );
						}
					}
					else if ( i.first == "type" )
					{
						if ( !i.second.is( Value::Type::String ) )
						{
							e = Error( Error::UnexpectedType, "Unexpected type[%s]: %s", "FIXME: path", to_string( i.second.type() ) );
							return;
						}
						auto t = i.second.as_string();
						if ( t == "null" )
						{
							schema_.reset( new NullItem() );
						}
						else if ( t == "boolean" )
						{
							schema_.reset( new BoolItem() );
						}
						else if ( t == "string" )
						{
							schema_.reset( new StringItem() );
						}
						if ( schema_ )
						{
							schema_->parse( o, e );
						}
					}
				}
			}
		}
	}

private:
	std::string id_;
	std::string name_;
	std::unique_ptr<ItemBase> schema_;
};

class Schema::Impl
{
public:
	void parse( const Value &schema, Error &e )
	{
		s_.parse( schema, e );
	}

	bool validate( const Value &value, Error &e )
	{
		return s_.validate( value, e );
	}

private:
	SchemaDefinition s_;
};

Schema::Schema()
{
}

Schema::Schema( const std::string &schema, Error &e ) :
		impl_( new Impl )
{
	auto s = Json::parse( schema, e );
	if ( e.empty() )
	{
		impl_->parse( s, e );
	}
}

Schema::~Schema()
{
}

bool Schema::init( const std::string &schema, Error &e )
{
	impl_.reset( new Impl );
	auto s = Json::parse( schema, e );
	if ( e.empty() )
	{
		impl_->parse( s, e );
	}
	return e.empty();
}

bool Schema::validate( const std::string &json, Error &e ) const
{
	auto value = Json::parse( json, e );
	if ( !e.empty() )
	{
		return false;
	}
	return impl_->validate( value, e );
}

bool Schema::validate( const Value &value, Error &e ) const
{
	return impl_->validate( value, e );
}

} // namespace jsoncpp
