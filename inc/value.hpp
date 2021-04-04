#pragma once

#include <cmath>
#include <cstdint>
#include <functional>
#include <vector>
#include <string>
#include <map>
#include "variant.hpp"


namespace jsoncpp
{

/**
 * @brief Polymorphic value container, which is able to handle JSON types.
 */
class Value
{
public:
	/**
	 * @brief Value type constants.
	 */
	enum class Type
	{
		None = 0,
		Int,
		Float,
		Bool,
		String,
		Array,
		Object
	};
	/**
	 * @brief Value types.
	 */
	typedef void* None;
	typedef int64_t Int;
	typedef double Float;
	typedef bool Bool;
	typedef std::string String;
	typedef std::vector<Value> Array;
	typedef std::map<std::string, Value> Object;

	typedef std::function<bool(const Value &element)> ElementPredicate;

	/**
	 * @brief Value visitor prototype.
	 */
	struct ValueVisitor
	{
		virtual ~ValueVisitor() {};
		virtual void operator()()                = 0;
		virtual void operator()( const Int& )    = 0;
		virtual void operator()( const Float& )  = 0;
		virtual void operator()( const Bool& )   = 0;
		virtual void operator()( const String& ) = 0;
		virtual void operator()( const Array& )  = 0;
		virtual void operator()( const Object& ) = 0;
	};

	/**
	 * @brief Constructors
	 */
	Value();
	Value( const Type t );
	Value( const char *s );
	Value( const Value &rhs );
	Value( Value&& rhs ) :
			type_( rhs.type_ ),
			data_( std::move( rhs.data_ ) )
	{}

	template<typename T,
			 typename std::enable_if <std::is_same<T, None>::value     ||
									  std::is_same<T, Int>::value      ||
									  std::is_same<T, Float>::value    ||
									  std::is_same<T, Bool>::value     ||
									  std::is_same<T, String>::value   ||
									  std::is_same<T, Array>::value    ||
									  std::is_same<T, Object>::value>::type* = nullptr>
	Value( const T &value )
	{
		data_ = value;
		type_ = type();
	}

	template<typename T,
			 typename std::enable_if <std::is_same<T, int32_t>::value  ||
									  std::is_same<T, uint32_t>::value ||
									  std::is_same<T, uint64_t>::value>::type* = nullptr>
	Value( const T &value )
	{
		data_ = (Int)value;
		type_ = type();
	}

	Value( const float &value )
	{
		data_ = (Float)value;
		type_ = type();
	}

	/**
	 * @brief Value swap
	 */
	void swap( Value &rhs );
	void swap( Value &&rhs );

	/**
	 * @brief Assignment operators
	 */
	Value& operator=( const Value &value );
	Value& operator=( const char *value );
	Value& operator=( const int32_t &value );
	Value& operator=( const uint32_t &value );
	Value& operator=( const uint64_t &value );
	Value& operator=( const float &value );

	template<typename T,
			 typename std::enable_if <std::is_same<T, None>::value     ||
									  std::is_same<T, Int>::value      ||
									  std::is_same<T, Float>::value    ||
									  std::is_same<T, Bool>::value     ||
									  std::is_same<T, String>::value   ||
									  std::is_same<T, Array>::value    ||
									  std::is_same<T, Object>::value>::type* = nullptr>
	Value& operator=( const T &value )
	{
		data_ = value;
		type_ = type();
		return *this;
	}

	/**
	 * @brief Comparison operators
	 */
	template<typename T,
			 typename std::enable_if<std::is_same<T, None>::value     ||
									 std::is_same<T, Int>::value      ||
									 std::is_same<T, Float>::value    ||
									 std::is_same<T, Bool>::value     ||
									 std::is_same<T, String>::value>::type* = nullptr>
	bool operator==( const T &value ) const
	{
		auto p_data = data_.get<T>();
		return is<T>() && p_data && value == *p_data;
	}

	bool operator==( const int32_t &value ) const;
	bool operator==( const uint32_t &value ) const;
	bool operator==( const uint64_t &value ) const;
	bool operator==( const float &value ) const;

	bool operator==( const char *value ) const;
	bool operator==( const Value &value ) const;

	template <typename T>
	bool operator!=( const T &value ) const
	{
		return !( *this == value );
	}

	/**
	 * @brief Operator bool. Checks if value is empty.
	 */
	operator bool() const;

	/**
	 * @brief Returns an element by specified index or Null object if no such element.
	 * @param index Element index.
	 * @return Object reference.
	 */
	Value& operator[]( unsigned index );
	Value& operator[]( int index );

	/**
	 * at Returns an element by specified index or Null object if no such element.
	 * @param index Element index.
	 * @return Object reference.
	 */
	Value& at( unsigned index );
	Value& at( int index );

	/**
	 * back Returns last element of array or Null if empty.
	 * @return Object reference.
	 */
	Value& back();

	/**
	 * insert Adds a new element into array.
	 * @param v Object.
	 * @return Object reference (this).
	 */
	Value& insert( Value &&v );

	/**
	 * erase Removes element from array by index.
	 * @param index Element index.
	 * @return Object reference (this).
	 */
	Value& erase( unsigned index );

	/**
	 * @brief Returns an element by key or Null object if no such element.
	 * @param key Element key.
	 * @return Object reference.
	 */
	Value& operator[]( const std::string &key );
	const Value& operator[]( const std::string &key ) const;
	Value& operator[]( const char *key );
	const Value& operator[]( const char *key ) const;

	/**
	 * at Returns an element by key or Null object if no such element.
	 * @param key Element key.
	 * @return Object reference.
	 */
	Value& at( const std::string &key );
	const Value& at( const std::string &key ) const;
	Value& at( const char *key );
	const Value& at( const char *key ) const;

	/**
	 * insert Adds a new element into object.
	 * @param key Object key.
	 * @param v Object itself.
	 * @return Object reference (this).
	 */
	Value& insert( const std::string &key, Value &&v );

	/**
	 * erase Removes element from object by key.
	 * @param key Element key.
	 * @return Object reference (this).
	 */
	Value& erase( const std::string &key );

	/**
	 * is Checks if value contain specific type.
	 * @return True if type matches.
	 */
	template<typename T,
			 typename std::enable_if <std::is_same<T, None>::value   ||
									  std::is_same<T, Int>::value    ||
									  std::is_same<T, Float>::value  ||
									  std::is_same<T, Bool>::value   ||
									  std::is_same<T, String>::value ||
									  std::is_same<T, Array>::value  ||
									  std::is_same<T, Object>::value>::type* = nullptr>
	bool is() const
	{
		const T* ptr = nullptr;
		return type( ptr ) == type_;
	}

	inline bool is_none() const
	{
		return data_.type_index() < 2;
	}
	inline bool is_bool() const
	{
		return data_.type_index() == 2;
	}
	inline bool is_int() const
	{
		return data_.type_index() == 3;
	}
	inline bool is_float() const
	{
		return data_.type_index() == 4;
	}
	inline bool is_string() const
	{
		return data_.type_index() == 5;
	}
	inline bool is_array() const
	{
		return data_.type_index() == 6;
	}
	inline bool is_object() const
	{
		return data_.type_index() == 7;
	}

	/**
	 * get Get value by specific type. Or empty value if type don't match.
	 * @return Value reference by type.
	 */
	template <typename T>
	T& get()
	{
		static T t = default_value<T>();
		auto v = data_.get<T>();
		return v ? *v : t;
	}

	inline Int& get_int()
	{
		return get<Int>();
	}
	inline Float& get_float()
	{
		return get<Float>();
	}
	inline Bool& get_bool()
	{
		return get<Bool>();
	}
	inline String& get_string()
	{
		return get<String>();
	}
	inline Array& get_array()
	{
		return get<Array>();
	}
	inline Object& get_object()
	{
		return get<Object>();
	}

	/**
	 * get Get value by specific type. Or empty value if type don't match.
	 * @return Constant value reference by type.
	 */
	template <typename T>
	const T& get() const
	{
		auto v = data_.get<T>();
		return v ? *v : default_value<T>();
	}

	inline const Int& get_int() const
	{
		return get<Int>();
	}
	inline const Float& get_float() const
	{
		return get<Float>();
	}
	inline const Bool& get_bool() const
	{
		return get<Bool>();
	}
	inline const String& get_string() const
	{
		return get<String>();
	}
	inline const Array& get_array() const
	{
		return get<Array>();
	}
	inline const Object& get_object() const
	{
		return get<Object>();
	}

	/**
	 * default_value Returns default object of specific type.
	 * @return Value reference.
	 */
	template<typename T,
			 typename std::enable_if <std::is_same<T, None>::value   ||
									  std::is_same<T, Int>::value    ||
									  std::is_same<T, Float>::value  ||
									  std::is_same<T, Bool>::value   ||
									  std::is_same<T, String>::value ||
									  std::is_same<T, Array>::value  ||
									  std::is_same<T, Object>::value>::type* = nullptr>
	static const T& default_value()
	{
		const T* ptr = nullptr;
		return default_value( ptr );
	}

	/**
	 * type Returns value type.
	 * @return Value type.
	 */
	template<typename T>
	Type type( const T *v ) const
	{
		static struct
		{
			Value::Type t = Value::Type::None;
			void operator()( const None* )   { t = Type::None;   }
			void operator()( const Int* )    { t = Type::Int;    }
			void operator()( const Float* )  { t = Type::Float;  }
			void operator()( const Bool* )   { t = Type::Bool;   }
			void operator()( const String* ) { t = Type::String; }
			void operator()( const Array* )  { t = Type::Array;  }
			void operator()( const Object* ) { t = Type::Object; }
		} visitor;

		data_.accept( v, visitor );
		return visitor.t;
	}

	/**
	 * type Returns current object type.
	 * @return Value type.
	 */
	Type type() const;

	/**
	 * accept Call corresponding method of specified visitor.
	 */
	void accept( ValueVisitor *visitor ) const;

	/**
	 * is Checks type.
	 * @return True if type matches.
	 */
	bool is( Type t ) const;

	/**
	 * as Converts value to specified type.
	 * @return Value instance.
	 */
	Value as( Type t ) const;

	/**
	 * empty Checks if object is empty (unset).
	 * @return True if object is none.
	 */
	bool empty() const;

	/**
	 * clear Turns object into none.
	 */
	void clear();

	/**
	 * size Returns object size in bytes for trivial types, or element count for strings, arrays and objects.
	 * @return Object size.
	 */
	unsigned size() const;

	/**
	 * has Checks if index exists in array.
	 * @return True if index exists.
	 */
	bool has( unsigned index ) const;

	/**
	 * index Checks if element exists in array using specified predicate
	 * @return True if element exists.
	 */
	bool find( ElementPredicate pred, unsigned &index ) const;

	/**
	 * has Checks if key exists in array.
	 * @return True if key exists.
	 */
	bool has( const std::string &key ) const;

	/**
	 * is_convertable Checks if value is convertable into specified type.
	 * @return True if value is convertable.
	 */
	bool is_convertable( const Type t ) const;

	/**
	 * as_int32 Returns value as 32-bit integer.
	 * @return int32_t value.
	 */
	int32_t as_int32() const;

	/**
	 * as_int64 Returns value as 64-bit integer.
	 * @return int64_t value.
	 */
	inline Int as_int64() const
	{
		return as( Value::Type::Int ).get_int();
	}

	inline Int as_int() const
	{
		return as( Value::Type::Int ).get_int();
	}

	/**
	 * as_uint32 Returns value as unsigned 32-bit integer.
	 * @return uint32_t value.
	 */
	uint32_t as_uint32() const;

	/**
	 * as_int64 Returns value as unsigned 64-bit integer.
	 * @return uint64_t value.
	 */
	uint64_t as_uint64() const;

	/**
	 * as_float Returns value as floating point number.
	 * @return Float value.
	 */
	float as_float() const;

	/**
	 * as_double Returns value as double precision floating point number.
	 * @return Double value.
	 */
	inline Float as_double() const
	{
		return as( Value::Type::Float ).get_float();
	}

	/**
	 * as_bool Returns value as boolean.
	 * @return Bool value.
	 */
	inline Bool as_bool() const
	{
		return as( Value::Type::Bool ).get_bool();
	}

	/**
	 * as_string Returns value as string.
	 * @return String value.
	 */
	inline String as_string() const
	{
		return as( Value::Type::String ).get_string();
	}

	/**
	 * as_array Returns value as array.
	 * @return Array value.
	 */
	inline Array as_array() const
	{
		return as( Value::Type::Array ).get_array();
	}

	/**
	 * as_object Returns value as object.
	 * @return Object value.
	 */
	inline Object as_object() const
	{
		return as( Value::Type::Object ).get_object();
	}

private:
	Type type_;
	Variant<
		None,
		Bool,
		Int,
		Float,
		String,
		Array,
		Object
	> data_;

	static const Int    default_int_;
	static const Float  default_float_;
	static const Bool   default_bool_;
	static const String default_string_;
	static const Array  default_array_;
	static const Object default_object_;

	static const Int&    default_value( const Int* );
	static const Float&  default_value( const Float* );
	static const Bool&   default_value( const Bool* );
	static const String& default_value( const String* );
	static const Array&  default_value( const Array* );
	static const Object& default_value( const Object* );
};


template<typename T>
bool operator==( const T &lhs, const Value &rhs )
{
	return rhs.operator==( lhs );
}

bool operator<( const Value::Type lhs, const Value::Type rhs );

const char* to_string( Value::Type type );

} // namespace jsoncpp
