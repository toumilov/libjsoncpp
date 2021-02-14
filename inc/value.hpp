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
		Int32,
		Int64,
		Uint32,
		Uint64,
		Float,
		Double,
		Bool,
		String,
		Array,
		Object
	};
	/**
	 * @brief Value types.
	 */
	typedef void* None;
	typedef int32_t Int32;
	typedef int64_t Int64;
	typedef uint32_t Uint32;
	typedef uint64_t Uint64;
	typedef float Float;
	typedef double Double;
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
		virtual void operator()()				= 0;
		virtual void operator()( const Int32& )  = 0;
		virtual void operator()( const Int64& )  = 0;
		virtual void operator()( const Uint32& ) = 0;
		virtual void operator()( const Uint64& ) = 0;
		virtual void operator()( const Float& )  = 0;
		virtual void operator()( const Double& ) = 0;
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

	template <typename T>
	Value( const T &value )
	{
		data_ = value;
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

	template<typename T,
			 typename std::enable_if <std::is_same<T, None>::value   ||
									  std::is_same<T, Int32>::value  ||
									  std::is_same<T, Int64>::value  ||
									  std::is_same<T, Uint32>::value ||
									  std::is_same<T, Uint64>::value ||
									  std::is_same<T, Float>::value  ||
									  std::is_same<T, Double>::value ||
									  std::is_same<T, Bool>::value   ||
									  std::is_same<T, String>::value ||
									  std::is_same<T, Array>::value  ||
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
			 typename std::enable_if<std::is_same<T, None>::value   ||
									 std::is_same<T, Int32>::value  ||
									 std::is_same<T, Int64>::value  ||
									 std::is_same<T, Uint32>::value ||
									 std::is_same<T, Uint64>::value ||
									 std::is_same<T, Float>::value  ||
									 std::is_same<T, Double>::value ||
									 std::is_same<T, Bool>::value   ||
									 std::is_same<T, String>::value>::type* = nullptr>
	bool operator==( const T &value ) const
	{
		auto p_data = data_.get<T>();
		return is<T>() && p_data && value == *p_data;
	}

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
									  std::is_same<T, Int32>::value  ||
									  std::is_same<T, Int64>::value  ||
									  std::is_same<T, Uint32>::value ||
									  std::is_same<T, Uint64>::value ||
									  std::is_same<T, Float>::value  ||
									  std::is_same<T, Double>::value ||
									  std::is_same<T, Bool>::value   ||
									  std::is_same<T, String>::value ||
									  std::is_same<T, Array>::value  ||
									  std::is_same<T, Object>::value>::type* = nullptr>
	bool is() const
	{
		const T* ptr = nullptr;
		return type( ptr ) == type_;
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

	/**
	 * default_value Returns default object of specific type.
	 * @return Value reference.
	 */
	template <typename T>
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
			void operator()( const Int32* )  { t = Type::Int32;  }
			void operator()( const Int64* )  { t = Type::Int64;  }
			void operator()( const Uint32* ) { t = Type::Uint32; }
			void operator()( const Uint64* ) { t = Type::Uint64; }
			void operator()( const Float* )  { t = Type::Float;  }
			void operator()( const Double* ) { t = Type::Double; }
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
	 * is_none Checks if object is empty (unset).
	 * @return True if object is none.
	 */
	bool is_none() const;

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
	 * @return Int32 value.
	 */
	Int32 as_int32() const;

	/**
	 * as_int64 Returns value as 64-bit integer.
	 * @return Int64 value.
	 */
	Int64 as_int64() const;

	/**
	 * as_uint32 Returns value as unsigned 32-bit integer.
	 * @return Int32 value.
	 */
	Uint32 as_uint32() const;

	/**
	 * as_int64 Returns value as unsigned 64-bit integer.
	 * @return Int64 value.
	 */
	Uint64 as_uint64() const;

	/**
	 * as_float Returns value as floating point number.
	 * @return Float value.
	 */
	Float as_float() const;

	/**
	 * as_double Returns value as double precision floating point number.
	 * @return Double value.
	 */
	Double as_double() const;

	/**
	 * as_bool Returns value as boolean.
	 * @return Bool value.
	 */
	Bool as_bool() const;

	/**
	 * as_string Returns value as string.
	 * @return String value.
	 */
	String as_string() const;

	/**
	 * as_array Returns value as array.
	 * @return Array value.
	 */
	Array as_array() const;

	/**
	 * as_object Returns value as object.
	 * @return Object value.
	 */
	Object as_object() const;

private:
	Type type_;
	Variant<
		None,
		Bool,
		Int32,
		Int64,
		Uint32,
		Uint64,
		Float,
		Double,
		String,
		Array,
		Object
	> data_;

	static const Int32 default_int32_;
	static const Int64 default_int64_;
	static const Uint32 default_uint32_;
	static const Uint64 default_uint64_;
	static const Float default_float_;
	static const Double default_double_;
	static const Bool default_bool_;
	static const String default_string_;
	static const Array default_array_;
	static const Object default_object_;

	static const Int32&  default_value( const Int32* );
	static const Int64&  default_value( const Int64* );
	static const Uint32& default_value( const Uint32* );
	static const Uint64& default_value( const Uint64* );
	static const Float&  default_value( const Float* );
	static const Double& default_value( const Double* );
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
