
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <limits>
#include "value.hpp"
#include "CppUTest/TestHarness.h"

using namespace jsoncpp;

TEST_GROUP(ValueGroup)
{
	void setup()
	{
	}
	void teardown()
	{
	}
};

TEST(ValueGroup, SwapTest)
{
	Value a( INT32_C( 123 ) );
	Value b( "test" );
	a.swap( b );
	UNSIGNED_LONGS_EQUAL( Value::Type::Int, b.type() );
	UNSIGNED_LONGS_EQUAL( Value::Type::String, a.type() );
	CHECK( b == INT32_C( 123 ) );
	STRCMP_EQUAL( "test", a.get_string().c_str() );

	Value c( std::move( a ) );
	UNSIGNED_LONGS_EQUAL( Value::Type::String, c.type() );
	STRCMP_EQUAL( "test", c.get_string().c_str() );
}

TEST(ValueGroup, NoneTest)
{
	Value v;
	CHECK( v.is_none() );
	CHECK( v.empty() );
}

TEST(ValueGroup, Int32Test)
{
	// Constructror
	Value v( Value::Type::Int );
	UNSIGNED_LONGS_EQUAL( Value::Type::Int, v.type() );
	CHECK( v.is_int() );
	CHECK_EQUAL( Value::default_value<Value::Int>(), v.get_int() );

	// Cleanup
	v.clear();
	CHECK( v.empty() );
	CHECK_FALSE( v.is_int() );
	CHECK_EQUAL( Value::default_value<Value::Int>(), v.get_int() );

	v = Value( INT32_C( 123 ) );
	UNSIGNED_LONGS_EQUAL( Value::Type::Int, v.type() );
	CHECK( v.is_int() );
	v.clear();

	// Assignment
	v = INT32_C( 123 );
	CHECK( v.is_int() );

	// Getting contents
	CHECK( v == INT32_C( 123 ) );
	CHECK( v != INT32_C( 124 ) );
	CHECK_EQUAL( INT32_C( 123 ), v.get_int() );

	// Size
	CHECK_EQUAL( sizeof( Value::Int ), v.size() );
}

TEST(ValueGroup, Int64Test)
{
	// Constructror
	Value v( INT64_C( 123 ) );
	UNSIGNED_LONGS_EQUAL( Value::Type::Int, v.type() );
	CHECK( v.is_int() );

	// Cleanup
	v.clear();
	CHECK( v.empty() );
	CHECK_FALSE( v.is_int() );
	CHECK_EQUAL( Value::default_value<Value::Int>(), v.get_int() );

	// Assignment
	v = INT64_C( 123 );
	CHECK( v.is_int() );

	// Getting contents
	CHECK( v == INT64_C( 123 ) );
	CHECK( v != INT64_C( 124 ) );
	CHECK_EQUAL( INT64_C( 123 ), v.get_int() );

	// Size
	CHECK_EQUAL( sizeof( Value::Int ), v.size() );
}

TEST(ValueGroup, Uint32Test)
{
	// Constructror
	Value v( UINT32_C( 123 ) );
	UNSIGNED_LONGS_EQUAL( Value::Type::Int, v.type() );
	CHECK( v.is_int() );

	// Cleanup
	v.clear();
	CHECK( v.empty() );
	CHECK_FALSE( v.is_int() );
	CHECK_EQUAL( UINT32_C( 0 ), v.as_uint32() );

	// Assignment
	v = UINT32_C( 123 );
	CHECK( v.is_int() );

	// Getting contents
	CHECK( v == UINT32_C( 123 ) );
	CHECK( v != UINT32_C( 124 ) );
	CHECK_EQUAL( UINT32_C( 123 ), v.as_uint32() );

	// Size
	CHECK_EQUAL( sizeof( Value::Int ), v.size() );
}

TEST(ValueGroup, Uint64Test)
{
	// Constructror
	Value v( UINT64_C( 123 ) );
	UNSIGNED_LONGS_EQUAL( Value::Type::Int, v.type() );
	CHECK( v.is_int() );

	// Cleanup
	v.clear();
	CHECK( v.empty() );
	CHECK_FALSE( v.is_int() );
	CHECK_EQUAL( UINT64_C( 0 ), v.as_uint64() );

	// Assignment
	v = UINT64_C( 123 );
	CHECK( v.is_int() );

	// Getting contents
	CHECK( v == UINT64_C( 123 ) );
	CHECK( v != UINT64_C( 124 ) );
	CHECK_EQUAL( UINT64_C( 123 ), v.as_uint64() );

	// Size
	CHECK_EQUAL( sizeof( Value::Int ), v.size() );
}

TEST(ValueGroup, FloatTest)
{
	// Constructror
	Value v( 1.23f );
	UNSIGNED_LONGS_EQUAL( Value::Type::Float, v.type() );
	CHECK( v.is_float() );

	// Cleanup
	v.clear();
	CHECK( v.empty() );
	CHECK_FALSE( v.is( Value::Type::Float ) );
	CHECK_FALSE( v.is_float() );
	CHECK_EQUAL( 0.0f, v.get_float() );

	// Assignment
	v = 1.23f;
	CHECK( v.is( Value::Type::Float ) );

	// Getting contents
	DOUBLES_EQUAL( v.get_float(), 1.23f, std::numeric_limits<Value::Float>::epsilon() );
	CHECK( v == 1.23f );
	CHECK( v != 1.24f );
	CHECK_EQUAL( 1.23f, v.get_float() );

	// Size
	CHECK_EQUAL( sizeof( Value::Float ), v.size() );
}

TEST(ValueGroup, DoubleTest)
{
	// Constructror
	Value v( Value::Type::Float );
	UNSIGNED_LONGS_EQUAL( Value::Type::Float, v.type() );
	CHECK( v.is_float() );

	// Cleanup
	v.clear();
	CHECK( v.empty() );
	CHECK_FALSE( v.is( Value::Type::Float ) );
	CHECK_FALSE( v.is_float() );
	CHECK_EQUAL( Value::default_value<Value::Float>(), v.get_float() );

	// Assignment
	v = 1.23;
	CHECK( v.is( Value::Type::Float ) );

	// Getting contents
	DOUBLES_EQUAL( v.get_float(), 1.23, std::numeric_limits<Value::Float>::epsilon() );
	CHECK( v == 1.23 );
	CHECK( v != 1.24 );
	CHECK_EQUAL( 1.23, v.get_float() );

	// Size
	CHECK_EQUAL( sizeof( Value::Float ), v.size() );
}

TEST(ValueGroup, BoolTest)
{
	// Constructror
	Value v( Value::Type::Bool );
	UNSIGNED_LONGS_EQUAL( Value::Type::Bool, v.type() );

	// Cleanup
	v.clear();
	CHECK( v.empty() );
	CHECK_FALSE( v.is( Value::Type::Bool ) );
	CHECK_FALSE( v.is_bool() );
	CHECK_EQUAL( Value::default_value<Value::Bool>(), v.get_bool() );

	// Assignment
	v = true;
	CHECK( v.is( Value::Type::Bool ) );

	// Getting contents
	CHECK( v == true );
	CHECK( v != false );
	CHECK_EQUAL( true, v.get_bool() );

	// Size
	CHECK_EQUAL( sizeof( Value::Bool ), v.size() );
}

TEST(ValueGroup, StringTest)
{
	// Constructror
	Value v( Value::Type::String );
	UNSIGNED_LONGS_EQUAL( Value::Type::String, v.type() );

	// Cleanup
	v.clear();
	CHECK( v.empty() );
	CHECK_FALSE( v.is( Value::Type::String ) );
	CHECK_FALSE( v.is_string() );
	CHECK_EQUAL( Value::default_value<Value::String>(), v.get_string() );

	// Assignment
	v = "test";
	CHECK( v.is( Value::Type::String ) );

	// Getting contents
	CHECK( v == Value::String( "test" ) );
	CHECK( v != "bad value" );
	STRCMP_EQUAL( "test", v.get_string().c_str() );

	// Size
	CHECK_EQUAL( v.get_string().size(), v.size() );
}

TEST(ValueGroup, ArrayTest)
{
	// Constructror
	Value v( Value::Type::Array );
	UNSIGNED_LONGS_EQUAL( Value::Type::Array, v.type() );

	// Cleanup
	v.clear();
	CHECK( v.empty() );
	CHECK_FALSE( v.is( Value::Type::Array ) );

	// Assignment
	v = Value::Array();
	CHECK( v.is( Value::Type::Array ) );
	CHECK( v.is_array() );

	// Getting contents
	v.insert( 123 )
	 .insert( "test" );
	CHECK( v.has( 0 ) );
	CHECK( v.has( 1 ) );
	CHECK_FALSE( v.has( 2 ) );
	CHECK( 123 == v[0] );
	CHECK( "test" == v.at( 1 ) );
	CHECK( v.at( 2 ).empty() );

	Value a;
	a.insert( 123 )
	 .insert( "test" );
	CHECK_EQUAL( 2, a.size() );
	CHECK( a.has( 0 ) );
	CHECK( a.has( 1 ) );
	CHECK_FALSE( a.has( 2 ) );
	CHECK( 123 == a[0] );
	CHECK( "test" == a.at( 1 ) );
	CHECK( a.at( 2 ).empty() );

	// Find element
	Value o( Value::Type::Object );
	o.insert( "user", "usr1" );
	o.insert( "type", 2 );
	a.insert( std::move( o ) );
	auto pred = []( const std::string &user, const Value &o ) -> bool {
		return o.has( "user" ) && o["user"].get_string() == user;
	};
	unsigned i;
	CHECK( a.find( std::bind( pred, "usr1", std::placeholders::_1 ), i ) );
	CHECK_EQUAL( 2, i );
	CHECK_FALSE( a.find( std::bind( pred, "usr2", std::placeholders::_1 ), i ) );

	// Size
	CHECK_EQUAL( 2, v.size() );

	// Erase
	v.erase( 0 );
	CHECK_EQUAL( 1, v.size() );
	CHECK( "test" == v.at( 0 ) );
}

TEST(ValueGroup, ObjectTest)
{
	// Constructror
	Value v( Value::Type::Object );
	UNSIGNED_LONGS_EQUAL( Value::Type::Object, v.type() );

	// Cleanup
	v.clear();
	CHECK( v.empty() );
	CHECK_FALSE( v.is( Value::Type::Object ) );

	// Assignment
	v = Value::Object();
	CHECK( v.is( Value::Type::Object ) );
	CHECK( ( v.is_object() ) );

	// Getting contents
	v.insert( "key1", 123 )
	 .insert( "key2", "test" );
	CHECK( v.has( "key1" ) );
	CHECK( v.has( "key2" ) );
	CHECK_FALSE( v.has( "key3" ) );
	CHECK( 123 == v["key1"] );
	CHECK( "test" == v.at( "key2" ) );
	CHECK_FALSE( v.has( "key3" ) );

	Value o;
	o["key1"] = 123;
	o.at( "key2" ) = "test";
	CHECK( o.has( "key1" ) );
	CHECK( o.has( "key2" ) );
	CHECK_FALSE( o.has( "key3" ) );
	CHECK( 123 == o["key1"] );
	CHECK( "test" == o.at( "key2" ) );
	CHECK_FALSE( o.has( "key3" ) );

	// Size
	CHECK_EQUAL( 2, v.size() );

	// Erase
	v.erase( "key2" );
	CHECK_EQUAL( 1, v.size() );
	CHECK( 123 == v.at( "key1" ) );
}

TEST(ValueGroup, DefaultValue)
{
	CHECK_EQUAL( 0, Value::default_value<Value::Int>() );
	CHECK_EQUAL( 0.0f, Value::default_value<Value::Float>() );
	CHECK_EQUAL( false, Value::default_value<Value::Bool>() );
	CHECK_EQUAL( std::string(), Value::default_value<Value::String>() );
	CHECK( Value::Array() == Value::default_value<Value::Array>() );
	CHECK( Value::Object() == Value::default_value<Value::Object>() );
}

TEST(ValueGroup, IsConvertable)
{
	///////////
	// None
	Value none_value;
	// None -> None
	CHECK( none_value.is_convertable( Value::Type::None ) );
	// None -> Bool
	CHECK_FALSE( none_value.is_convertable( Value::Type::Bool ) );
	// None -> Int
	CHECK_FALSE( none_value.is_convertable( Value::Type::Int ) );
	// None -> Float
	CHECK_FALSE( none_value.is_convertable( Value::Type::Float ) );
	// None -> String
	CHECK_FALSE( none_value.is_convertable( Value::Type::String ) );
	// None -> Array
	CHECK_FALSE( none_value.is_convertable( Value::Type::Array ) );
	// None -> Object
	CHECK_FALSE( none_value.is_convertable( Value::Type::Object ) );

	///////////
	// Bool
	Value bool_value( Value::Type::Bool );
	// Bool -> None
	CHECK( bool_value.is_convertable( Value::Type::None ) );
	// Bool -> Bool
	CHECK( bool_value.is_convertable( Value::Type::Bool ) );
	// Bool -> Int
	CHECK( bool_value.is_convertable( Value::Type::Int ) );
	// Bool -> Float
	CHECK( bool_value.is_convertable( Value::Type::Float ) );
	// Bool -> String
	CHECK( bool_value.is_convertable( Value::Type::String ) );
	// Bool -> Array
	CHECK_FALSE( bool_value.is_convertable( Value::Type::Array ) );
	// Bool -> Object
	CHECK_FALSE( bool_value.is_convertable( Value::Type::Object ) );

	///////////
	// Int
	auto i_value = Value( Value::Type::Int );
	// Int -> None
	CHECK( i_value.is_convertable( Value::Type::None ) );
	// Int -> Bool
	CHECK( i_value.is_convertable( Value::Type::Bool ) );
	// Int -> Int
	CHECK( i_value.is_convertable( Value::Type::Int ) );
	// Int -> Float
	CHECK( i_value.is_convertable( Value::Type::Float ) );
	// Int -> String
	CHECK( i_value.is_convertable( Value::Type::String ) );
	// Int -> Array
	CHECK_FALSE( i_value.is_convertable( Value::Type::Array ) );
	// Int -> Object
	CHECK_FALSE( i_value.is_convertable( Value::Type::Object ) );

	///////////
	// Float
	auto d_value = Value( Value::Type::Float );
	// Float -> None
	CHECK( d_value.is_convertable( Value::Type::None ) );
	// Float -> Bool
	CHECK( d_value.is_convertable( Value::Type::Bool ) );
	// Float -> Int
	CHECK_FALSE( d_value.is_convertable( Value::Type::Int ) );
	// Float -> Float
	CHECK( d_value.is_convertable( Value::Type::Float ) );
	// Float -> String
	CHECK( d_value.is_convertable( Value::Type::String ) );
	// Float -> Array
	CHECK_FALSE( d_value.is_convertable( Value::Type::Array ) );
	// Float -> Object
	CHECK_FALSE( d_value.is_convertable( Value::Type::Object ) );

	///////////
	// String
	auto s_value = Value( Value::Type::String );
	// String -> None
	CHECK( s_value.is_convertable( Value::Type::None ) );
	// String -> Bool
	s_value = "0";
	CHECK( s_value.is_convertable( Value::Type::Bool ) );
	s_value = "1";
	CHECK( s_value.is_convertable( Value::Type::Bool ) );
	s_value = "False";
	CHECK( s_value.is_convertable( Value::Type::Bool ) );
	s_value = "True";
	CHECK( s_value.is_convertable( Value::Type::Bool ) );
	s_value = "none";
	CHECK_FALSE( s_value.is_convertable( Value::Type::Bool ) );
	// String -> Int64
	s_value = "100500";
	CHECK( s_value.is_convertable( Value::Type::Int ) );
	s_value = "-100500";
	CHECK( s_value.is_convertable( Value::Type::Int ) );
	s_value = "fail";
	CHECK_FALSE( s_value.is_convertable( Value::Type::Int ) );
	// String -> Float
	s_value = "-1.5";
	CHECK( s_value.is_convertable( Value::Type::Float ) );
	s_value = "fail";
	CHECK_FALSE( s_value.is_convertable( Value::Type::Float ) );
	// String -> String
	CHECK( s_value.is_convertable( Value::Type::String ) );
	// String -> Array
	CHECK_FALSE( s_value.is_convertable( Value::Type::Array ) );
	// String -> Object
	CHECK_FALSE( s_value.is_convertable( Value::Type::Object ) );

	///////////
	// Array
	auto a_value = Value( Value::Type::Array );
	// Array -> None
	CHECK( a_value.is_convertable( Value::Type::None ) );
	// Array -> Bool
	CHECK_FALSE( a_value.is_convertable( Value::Type::Bool ) );
	// Array -> Int
	CHECK_FALSE( a_value.is_convertable( Value::Type::Int ) );
	// Array -> Float
	CHECK_FALSE( a_value.is_convertable( Value::Type::Float ) );
	// Array -> String
	CHECK_FALSE( a_value.is_convertable( Value::Type::String ) );
	// Array -> Array
	CHECK( a_value.is_convertable( Value::Type::Array ) );
	// Array -> Object
	CHECK_FALSE( a_value.is_convertable( Value::Type::Object ) );

	///////////
	// Object
	auto o_value = Value( Value::Type::Object );
	// Object -> None
	CHECK( o_value.is_convertable( Value::Type::None ) );
	// Object -> Bool
	CHECK_FALSE( o_value.is_convertable( Value::Type::Bool ) );
	// Object -> Int
	CHECK_FALSE( o_value.is_convertable( Value::Type::Int ) );
	// Object -> Float
	CHECK_FALSE( o_value.is_convertable( Value::Type::Float ) );
	// Object -> String
	CHECK_FALSE( o_value.is_convertable( Value::Type::String ) );
	// Object -> Array
	CHECK_FALSE( o_value.is_convertable( Value::Type::Array ) );
	// Object -> Object
	CHECK( o_value.is_convertable( Value::Type::Object ) );
}

TEST(ValueGroup, AsType)
{
	///////////
	// None
	Value none_value;
	// None -> None
	CHECK( none_value.as( Value::Type::None ).is( Value::Type::None ) );
	// None -> Bool
	CHECK( none_value.as( Value::Type::Bool ).is( Value::Type::Bool ) );
	CHECK_EQUAL( Value::default_value<Value::Bool>(), none_value.as_bool() );
	// None -> int32_t
	CHECK_EQUAL( Value::default_value<Value::Int>(), none_value.as_int32() );
	// None -> Int
	CHECK( none_value.as( Value::Type::Int ).is( Value::Type::Int ) );
	CHECK_EQUAL( Value::default_value<Value::Int>(), none_value.as_int() );
	CHECK_EQUAL( Value::default_value<Value::Int>(), none_value.as_int64() );
	// None -> Uint32
	CHECK_EQUAL( 0, none_value.as_uint32() );
	// None -> Uint64
	CHECK_EQUAL( Value::default_value<Value::Int>(), none_value.as_uint64() );
	// None -> Float
	CHECK( none_value.as( Value::Type::Float ).is( Value::Type::Float ) );
	CHECK_EQUAL( Value::default_value<Value::Float>(), none_value.as_float() );
	// None -> Double
	CHECK_EQUAL( Value::default_value<Value::Float>(), none_value.as_double() );
	// None -> String
	CHECK( none_value.as( Value::Type::String ).is( Value::Type::String ) );
	STRCMP_EQUAL( "null", none_value.as_string().c_str() );
	// None -> Array
	CHECK( none_value.as( Value::Type::Array ).is( Value::Type::Array ) );
	CHECK( none_value.as_array().empty() );
	// None -> Object
	CHECK( none_value.as( Value::Type::Object ).is( Value::Type::Object ) );
	CHECK( none_value.as_object().empty() );

	///////////
	// Bool
	Value bool_value( true );
	// Bool -> None
	CHECK( bool_value.as( Value::Type::None ).is( Value::Type::None ) );
	// Bool -> Bool
	auto b = bool_value.as( Value::Type::Bool );
	CHECK( b.is( Value::Type::Bool ) );
	CHECK_EQUAL( true, b.get_bool() );
	CHECK_EQUAL( true, bool_value.as_bool() );
	// Bool -> Int32
	CHECK_EQUAL( 1, bool_value.as_int32() );
	// Bool -> Int64
	auto i64 = bool_value.as( Value::Type::Int );
	CHECK( i64.is( Value::Type::Int ) );
	CHECK_EQUAL( 1, i64.get_int() );
	CHECK_EQUAL( 1, bool_value.as_int64() );
	// Bool -> Uint32
	CHECK_EQUAL( 1, bool_value.as_uint32() );
	// Bool -> Uint64
	CHECK_EQUAL( 1, bool_value.as_uint64() );
	// Bool -> Float
	CHECK_EQUAL( 1.0f, bool_value.as_float() );
	// Bool -> Double
	auto d = bool_value.as( Value::Type::Float );
	CHECK( d.is( Value::Type::Float ) );
	DOUBLES_EQUAL( 1.0, d.get_float(), std::numeric_limits<Value::Float>::epsilon() );
	CHECK_EQUAL( 1.0, bool_value.as_double() );
	// Bool -> String
	auto s = bool_value.as( Value::Type::String );
	CHECK( s.is( Value::Type::String ) );
	STRCMP_EQUAL( "true", s.get_string().c_str() );
	STRCMP_EQUAL( "true", bool_value.as_string().c_str() );
	// Bool -> Array
	CHECK( bool_value.as( Value::Type::Array ).is( Value::Type::Array ) );
	CHECK( bool_value.as_array().empty() );
	// Bool -> Object
	CHECK( bool_value.as( Value::Type::Object ).is( Value::Type::Object ) );
	CHECK( bool_value.as_object().empty() );

	///////////
	// Int
	auto i32_value = Value( 123 );
	// Int32 -> None
	CHECK( i32_value.as( Value::Type::None ).is( Value::Type::None ) );
	// Int -> Bool
	b = i32_value.as( Value::Type::Bool );
	CHECK( b.is( Value::Type::Bool ) );
	CHECK_EQUAL( true, b.get_bool() );
	CHECK_EQUAL( true, i32_value.as_bool() );
	// Int -> Int32
	CHECK_EQUAL( 123, i32_value.as_int32() );
	// Int -> Int64
	i64 = i32_value.as( Value::Type::Int );
	CHECK( i64.is( Value::Type::Int ) );
	CHECK_EQUAL( 123, i64.get_int() );
	CHECK_EQUAL( 123, i32_value.as_int64() );
	// Int -> Uint32
	i32_value = -1;
	CHECK_EQUAL( Value::default_value<Value::Int>(), i32_value.as_uint32() );
	i32_value = 123;
	CHECK_EQUAL( 123, i32_value.as_uint32() );
	// Int -> Uint64
	i32_value = -1;
	CHECK_EQUAL( Value::default_value<Value::Int>(), i32_value.as_uint64() );
	i32_value = 123;
	CHECK_EQUAL( 123, i32_value.as_uint64() );
	// Int -> Float
	DOUBLES_EQUAL( 123.0, i32_value.as_float(), std::numeric_limits<Value::Float>::epsilon() );
	// Int -> Double
	d = i32_value.as( Value::Type::Float );
	CHECK( d.is( Value::Type::Float ) );
	DOUBLES_EQUAL( 123.0, d.get_float(), std::numeric_limits<Value::Float>::epsilon() );
	DOUBLES_EQUAL( 123.0, i32_value.as_double(), std::numeric_limits<Value::Float>::epsilon() );
	// Int -> String
	s = i32_value.as( Value::Type::String );
	STRCMP_EQUAL( "123", s.get_string().c_str() );
	STRCMP_EQUAL( "123", i32_value.as_string().c_str() );
	// Int -> Array
	CHECK( i32_value.as( Value::Type::Array ).is( Value::Type::Array ) );
	CHECK( i32_value.as_array().empty() );
	// Int -> Object
	CHECK( i32_value.as( Value::Type::Object ).is( Value::Type::Object ) );
	CHECK( i32_value.as_object().empty() );

	///////////
	// Float
	auto double_value = Value( 1.23 );
	// Float -> None
	CHECK( double_value.as( Value::Type::None ).is( Value::Type::None ) );
	// Float -> Bool
	b = double_value.as( Value::Type::Bool );
	CHECK( b.is( Value::Type::Bool ) );
	CHECK_EQUAL( true, b.get_bool() );
	CHECK_EQUAL( true, double_value.as_bool() );
	// Float -> int32_t
	CHECK_EQUAL( 0, double_value.as_int32() );
	// Float -> Int
	i64 = double_value.as( Value::Type::Int );
	CHECK( i64.is( Value::Type::Int ) );
	CHECK_EQUAL( Value::default_value<Value::Int>(), i64.get_int() );
	CHECK_EQUAL( Value::default_value<Value::Int>(), double_value.as_int64() );
	// Float -> Uint32
	CHECK_EQUAL( 0u, double_value.as_uint32() );
	// Float -> Uint64
	CHECK_EQUAL( 0, double_value.as_uint64() );
	// Float -> Float
	DOUBLES_EQUAL( 1.23f, double_value.as_float(), std::numeric_limits<Value::Float>::epsilon() );
	// Float -> Double
	d = double_value.as( Value::Type::Float );
	CHECK( d.is( Value::Type::Float ) );
	DOUBLES_EQUAL( 1.23, d.get_float(), std::numeric_limits<Value::Float>::epsilon() );
	DOUBLES_EQUAL( 1.23, double_value.as_double(), std::numeric_limits<Value::Float>::epsilon() );
	// Float -> String
	s = double_value.as( Value::Type::String );
	STRCMP_CONTAINS( "1.23", s.get_string().c_str() );
	STRCMP_CONTAINS( "1.23", double_value.as_string().c_str() );
	// Float -> Array
	CHECK( double_value.as( Value::Type::Array ).is( Value::Type::Array ) );
	CHECK( double_value.as_array().empty() );
	// Float -> Object
	CHECK( double_value.as( Value::Type::Object ).is( Value::Type::Object ) );
	CHECK( double_value.as_object().empty() );

	///////////
	// String
	auto str_value = Value( Value::Type::String );
	// String -> None
	CHECK( str_value.as( Value::Type::None ).is( Value::Type::None ) );
	// String -> Bool
	str_value = Value( "false" );
	b = str_value.as( Value::Type::Bool );
	CHECK( b.is( Value::Type::Bool ) );
	CHECK_EQUAL( false, b.get_bool() );
	CHECK_EQUAL( false, str_value.as_bool() );
	str_value = Value( "true" );
	b = str_value.as( Value::Type::Bool );
	CHECK( b.is( Value::Type::Bool ) );
	CHECK_EQUAL( true, b.get_bool() );
	CHECK_EQUAL( true, str_value.as_bool() );
	// String -> int32_t
	str_value = Value( "-123" );
	CHECK_EQUAL( -123, str_value.as_int32() );
	// String -> Int
	i64 = str_value.as( Value::Type::Int );
	CHECK( i64.is( Value::Type::Int ) );
	CHECK_EQUAL( -123, i64.get_int() );
	CHECK_EQUAL( -123, str_value.as_int64() );
	// String -> uint32_t
	str_value = Value( "123" );
	CHECK_EQUAL( 123, str_value.as_uint32() );
	// String -> uint64_t
	CHECK_EQUAL( 123, str_value.as_uint64() );
	// String -> Float
	str_value = Value( "1.23" );
	DOUBLES_EQUAL( 1.23f, str_value.as_float(), std::numeric_limits<float>::epsilon() );
	// String -> Double
	d = str_value.as( Value::Type::Float );
	CHECK( d.is( Value::Type::Float ) );
	DOUBLES_EQUAL( 1.23, d.get_float(), std::numeric_limits<Value::Float>::epsilon() );
	DOUBLES_EQUAL( 1.23, str_value.as_double(), std::numeric_limits<Value::Float>::epsilon() );
	// String -> String
	str_value = Value( "test" );
	s = str_value.as( Value::Type::String );
	STRCMP_EQUAL( "test", s.get_string().c_str() );
	STRCMP_EQUAL( "test", str_value.as_string().c_str() );
	// String -> Array
	CHECK( str_value.as( Value::Type::Array ).is( Value::Type::Array ) );
	CHECK( str_value.as_array().empty() );
	// String -> Object
	CHECK( str_value.as( Value::Type::Object ).is( Value::Type::Object ) );
	CHECK( str_value.as_object().empty() );

	///////////
	// Array
	auto arr_value = Value( Value::Type::Array );
	// Array -> None
	CHECK( arr_value.as( Value::Type::None ).is( Value::Type::None ) );
	// Array -> Bool
	CHECK( arr_value.as( Value::Type::Bool ).is( Value::Type::Bool ) );
	CHECK_EQUAL( Value::default_value<Value::Bool>(), arr_value.as_bool() );
	// Array -> Int32
	CHECK_EQUAL( 0, arr_value.as_int32() );
	// Array -> Int64
	CHECK( arr_value.as( Value::Type::Int ).is( Value::Type::Int ) );
	CHECK_EQUAL( Value::default_value<Value::Int>(), arr_value.as_int64() );
	// Array -> Uint32
	CHECK_EQUAL( UINT32_C( 0 ), arr_value.as_uint32() );
	// Array -> Uint64
	CHECK_EQUAL( UINT64_C( 0 ), arr_value.as_uint64() );
	// Array -> Float
	CHECK_EQUAL( 0.0f, arr_value.as_float() );
	// Array -> Double
	CHECK( arr_value.as( Value::Type::Float ).is( Value::Type::Float ) );
	CHECK_EQUAL( Value::default_value<Value::Float>(), arr_value.as_double() );
	// Array -> String
	CHECK( arr_value.as( Value::Type::String ).is( Value::Type::String ) );
	STRCMP_EQUAL( Value::default_value<Value::String>().c_str(), arr_value.as_string().c_str() );
	// Array -> Array
	CHECK( arr_value.as( Value::Type::Array ).is( Value::Type::Array ) );
	CHECK( arr_value.as_array().empty() );
	// Array -> Object
	CHECK( arr_value.as( Value::Type::Object ).is( Value::Type::Object ) );
	CHECK( arr_value.as_object().empty() );

	///////////
	// Object
	auto obj_value = Value( Value::Type::Object );
	// Object -> None
	CHECK( obj_value.as( Value::Type::None ).is( Value::Type::None ) );
	// Object -> Bool
	CHECK( obj_value.as( Value::Type::Bool ).is( Value::Type::Bool ) );
	CHECK_EQUAL( Value::default_value<Value::Bool>(), obj_value.as_bool() );
	// Object -> Int32
	CHECK_EQUAL( INT32_C( 0 ), obj_value.as_int32() );
	// Object -> Int64
	CHECK( obj_value.as( Value::Type::Int ).is( Value::Type::Int ) );
	CHECK_EQUAL( Value::default_value<Value::Int>(), obj_value.as_int64() );
	// Object -> Uint32
	CHECK_EQUAL( UINT32_C( 0 ), obj_value.as_uint32() );
	// Object -> Uint64
	CHECK_EQUAL( UINT64_C( 0 ), obj_value.as_uint64() );
	// Object -> Float
	CHECK_EQUAL( 0.0f, obj_value.as_float() );
	// Object -> Double
	CHECK( obj_value.as( Value::Type::Float ).is( Value::Type::Float ) );
	CHECK_EQUAL( Value::default_value<Value::Float>(), obj_value.as_double() );
	// Object -> String
	CHECK( obj_value.as( Value::Type::String ).is( Value::Type::String ) );
	STRCMP_EQUAL( Value::default_value<Value::String>().c_str(), obj_value.as_string().c_str() );
	// Object -> Array
	CHECK( obj_value.as( Value::Type::Array ).is( Value::Type::Array ) );
	CHECK( obj_value.as_array().empty() );
	// Object -> Object
	CHECK( obj_value.as( Value::Type::Object ).is( Value::Type::Object ) );
	CHECK( obj_value.as_object().empty() );
}
