
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
	UNSIGNED_LONGS_EQUAL( Value::Type::Int32, b.type() );
	UNSIGNED_LONGS_EQUAL( Value::Type::String, a.type() );
	CHECK( b == INT32_C( 123 ) );
	STRCMP_EQUAL( "test", a.get<Value::String>().c_str() );

	Value c( std::move( a ) );
	UNSIGNED_LONGS_EQUAL( Value::Type::String, c.type() );
	STRCMP_EQUAL( "test", c.get<Value::String>().c_str() );
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
	Value v( Value::Type::Int32 );
	UNSIGNED_LONGS_EQUAL( Value::Type::Int32, v.type() );

	// Cleanup
	v.clear();
	CHECK( v.empty() );
	CHECK( !v.is<Value::Int32>() );
	CHECK_EQUAL( Value::default_value<Value::Int32>(), v.get<Value::Int32>() );

	// Assignment
	v = INT32_C( 123 );
	CHECK( v.is<Value::Int32>() );

	// Getting contents
	CHECK( v == INT32_C( 123 ) );
	CHECK( v != INT32_C( 124 ) );
	CHECK_EQUAL( INT32_C( 123 ), v.get<Value::Int32>() );

	// Size
	CHECK_EQUAL( sizeof( Value::Int32 ), v.size() );
}

TEST(ValueGroup, Int64Test)
{
	// Constructror
	Value v( Value::Type::Int64 );
	UNSIGNED_LONGS_EQUAL( Value::Type::Int64, v.type() );

	// Cleanup
	v.clear();
	CHECK( v.empty() );
	CHECK( !v.is<Value::Int64>() );
	CHECK_EQUAL( 0, v.get<Value::Int64>() );

	// Assignment
	v = INT64_C( 123 );
	CHECK( v.is<Value::Int64>() );

	// Getting contents
	CHECK( v == INT64_C( 123 ) );
	CHECK( v != INT64_C( 124 ) );
	CHECK_EQUAL( INT64_C( 123 ), v.get<Value::Int64>() );

	// Size
	CHECK_EQUAL( sizeof( Value::Int64 ), v.size() );
}

TEST(ValueGroup, Uint32Test)
{
	// Constructror
	Value v( Value::Type::Uint32 );
	CHECK_EQUAL( v.get<Value::Uint32>(), Value::default_value<Value::Uint32>() );
	UNSIGNED_LONGS_EQUAL( Value::Type::Uint32, v.type() );

	// Cleanup
	v.clear();
	CHECK( v.empty() );
	CHECK( !v.is<Value::Uint32>() );
	CHECK_EQUAL( UINT32_C( 0 ), v.get<Value::Uint32>() );

	// Assignment
	v = UINT32_C( 123 );
	CHECK( v.is<Value::Uint32>() );

	// Getting contents
	CHECK( v == UINT32_C( 123 ) );
	CHECK( v != UINT32_C( 124 ) );
	CHECK_EQUAL( UINT32_C( 123 ), v.get<Value::Uint32>() );

	// Size
	CHECK_EQUAL( sizeof( Value::Uint32 ), v.size() );
}

TEST(ValueGroup, Uint64Test)
{
	// Constructror
	Value v( Value::Type::Uint64 );
	UNSIGNED_LONGS_EQUAL( Value::Type::Uint64, v.type() );

	// Cleanup
	v.clear();
	CHECK( v.empty() );
	CHECK_FALSE( v.is( Value::Type::Uint64 ) );
	CHECK_FALSE( v.is<Value::Uint64>() );
	CHECK_EQUAL( Value::default_value<Value::Uint64>(), v.get<Value::Uint64>() );

	// Assignment
	v = UINT64_C( 123 );
	CHECK( v.is( Value::Type::Uint64 ) );

	// Getting contents
	CHECK( v == UINT64_C( 123 ) );
	CHECK( v != UINT64_C( 124 ) );
	CHECK_EQUAL( UINT64_C( 123 ), v.get<Value::Uint64>() );

	// Size
	CHECK_EQUAL( sizeof( Value::Uint64 ), v.size() );
}

TEST(ValueGroup, FloatTest)
{
	// Constructror
	Value v( Value::Type::Float );
	UNSIGNED_LONGS_EQUAL( Value::Type::Float, v.type() );

	// Cleanup
	v.clear();
	CHECK( v.empty() );
	CHECK_FALSE( v.is( Value::Type::Float ) );
	CHECK_FALSE( v.is<Value::Float>() );
	CHECK_EQUAL( Value::default_value<Value::Float>(), v.get<Value::Float>() );

	// Assignment
	v = 12.3f;
	CHECK( v.is( Value::Type::Float ) );

	// Getting contents
	DOUBLES_EQUAL( v.get<Value::Float>(), 12.3f, std::numeric_limits<Value::Float>::epsilon() );
	CHECK( v == 12.3f );
	CHECK( v != 12.4f );
	CHECK_EQUAL( 12.3f, v.get<Value::Float>() );

	// Size
	CHECK_EQUAL( sizeof( Value::Float ), v.size() );
}

TEST(ValueGroup, DoubleTest)
{
	// Constructror
	Value v( Value::Type::Double );
	UNSIGNED_LONGS_EQUAL( Value::Type::Double, v.type() );

	// Cleanup
	v.clear();
	CHECK( v.empty() );
	CHECK_FALSE( v.is( Value::Type::Double ) );
	CHECK_FALSE( v.is<Value::Double>() );
	CHECK_EQUAL( Value::default_value<Value::Double>(), v.get<Value::Double>() );

	// Assignment
	v = 12.3;
	CHECK( v.is( Value::Type::Double ) );

	// Getting contents
	DOUBLES_EQUAL( v.get<Value::Double>(), 12.3, std::numeric_limits<Value::Double>::epsilon() );
	CHECK( v == 12.3 );
	CHECK( v != 12.4 );
	CHECK_EQUAL( 12.3, v.get<Value::Double>() );

	// Size
	CHECK_EQUAL( sizeof( Value::Double ), v.size() );
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
	CHECK_FALSE( v.is<Value::Bool>() );
	CHECK_EQUAL( Value::default_value<Value::Bool>(), v.get<Value::Bool>() );

	// Assignment
	v = true;
	CHECK( v.is( Value::Type::Bool ) );

	// Getting contents
	CHECK( v == true );
	CHECK( v != false );
	CHECK_EQUAL( true, v.get<Value::Bool>() );

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
	CHECK_FALSE( v.is<Value::String>() );
	CHECK_EQUAL( Value::default_value<Value::String>(), v.get<Value::String>() );

	// Assignment
	v = "test";
	CHECK( v.is( Value::Type::String ) );

	// Getting contents
	CHECK( v == Value::String( "test" ) );
	CHECK( v != "bad value" );
	STRCMP_EQUAL( "test", v.get<Value::String>().c_str() );

	// Size
	CHECK_EQUAL( v.get<Value::String>().size(), v.size() );
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
	CHECK( v.is<Value::Array>() );

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
		return o.has( "user" ) && o["user"].get<Value::String>() == user;
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
	CHECK( ( v.is<Value::Object>() ) );

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
	CHECK_EQUAL( 0, Value::default_value<Value::Int32>() );
	CHECK_EQUAL( INT64_C( 0 ), Value::default_value<Value::Int64>() );
	CHECK_EQUAL( UINT32_C( 0 ), Value::default_value<Value::Uint32>() );
	CHECK_EQUAL( UINT64_C( 0 ), Value::default_value<Value::Uint64>() );
	CHECK_EQUAL( 0.0f, Value::default_value<Value::Float>() );
	CHECK_EQUAL( 0.0, Value::default_value<Value::Double>() );
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
	// None -> Int32
	CHECK_FALSE( none_value.is_convertable( Value::Type::Int32 ) );
	// None -> Int64
	CHECK_FALSE( none_value.is_convertable( Value::Type::Int64 ) );
	// None -> Uint32
	CHECK_FALSE( none_value.is_convertable( Value::Type::Uint32 ) );
	// None -> Uint64
	CHECK_FALSE( none_value.is_convertable( Value::Type::Uint64 ) );
	// None -> Float
	CHECK_FALSE( none_value.is_convertable( Value::Type::Float ) );
	// None -> Double
	CHECK_FALSE( none_value.is_convertable( Value::Type::Double ) );
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
	// Bool -> Int32
	CHECK( bool_value.is_convertable( Value::Type::Int32 ) );
	// Bool -> Int64
	CHECK( bool_value.is_convertable( Value::Type::Int64 ) );
	// Bool -> Uint32
	CHECK( bool_value.is_convertable( Value::Type::Uint32 ) );
	// Bool -> Uint64
	CHECK( bool_value.is_convertable( Value::Type::Uint64 ) );
	// Bool -> Float
	CHECK( bool_value.is_convertable( Value::Type::Float ) );
	// Bool -> Double
	CHECK( bool_value.is_convertable( Value::Type::Double ) );
	// Bool -> String
	CHECK( bool_value.is_convertable( Value::Type::String ) );
	// Bool -> Array
	CHECK_FALSE( bool_value.is_convertable( Value::Type::Array ) );
	// Bool -> Object
	CHECK_FALSE( bool_value.is_convertable( Value::Type::Object ) );

	///////////
	// Int32
	auto i32_value = Value( Value::Type::Int32 );
	// Int32 -> None
	CHECK( i32_value.is_convertable( Value::Type::None ) );
	// Int32 -> Bool
	CHECK( i32_value.is_convertable( Value::Type::Bool ) );
	// Int32 -> Int32
	CHECK( i32_value.is_convertable( Value::Type::Int32 ) );
	// Int32 -> Int64
	CHECK( i32_value.is_convertable( Value::Type::Int64 ) );
	// Int32 -> Uint32
	i32_value = -1;
	CHECK_FALSE( i32_value.is_convertable( Value::Type::Uint32 ) );
	i32_value = 100;
	CHECK( i32_value.is_convertable( Value::Type::Uint32 ) );
	// Int32 -> Uint64
	i32_value = -1;
	CHECK_FALSE( i32_value.is_convertable( Value::Type::Uint64 ) );
	i32_value = 100;
	CHECK( i32_value.is_convertable( Value::Type::Uint64 ) );
	// Int32 -> Float
	CHECK( i32_value.is_convertable( Value::Type::Float ) );
	// Int32 -> Double
	CHECK( i32_value.is_convertable( Value::Type::Double ) );
	// Int32 -> String
	CHECK( i32_value.is_convertable( Value::Type::String ) );
	// Int32 -> Array
	CHECK_FALSE( i32_value.is_convertable( Value::Type::Array ) );
	// Int32 -> Object
	CHECK_FALSE( i32_value.is_convertable( Value::Type::Object ) );

	///////////
	// Int64
	auto i64_value = Value( Value::Type::Int64 );
	// Int64 -> None
	CHECK( i64_value.is_convertable( Value::Type::None ) );
	// Int64 -> Bool
	CHECK( i64_value.is_convertable( Value::Type::Bool ) );
	// Int64 -> Int32
	i64_value = (Value::Int64)std::numeric_limits<Value::Int32>::min() - 1;
	CHECK_FALSE( i64_value.is_convertable( Value::Type::Int32 ) );
	i64_value = (Value::Int64)std::numeric_limits<Value::Int32>::max() + 1;
	CHECK_FALSE( i64_value.is_convertable( Value::Type::Int32 ) );
	i64_value = INT64_C( 100 );
	CHECK( i64_value.is_convertable( Value::Type::Int32 ) );
	// Int64 -> Int64
	CHECK( i64_value.is_convertable( Value::Type::Int64 ) );
	// Int64 -> Uint32
	i64_value = INT64_C( -1 );
	CHECK_FALSE( i64_value.is_convertable( Value::Type::Uint32 ) );
	i64_value = (Value::Int64)std::numeric_limits<Value::Uint32>::max() + 1;
	CHECK_FALSE( i64_value.is_convertable( Value::Type::Uint32 ) );
	i64_value = INT64_C( 100 );
	CHECK( i64_value.is_convertable( Value::Type::Uint32 ) );
	// Int64 -> Uint64
	i64_value = INT64_C( -1 );
	CHECK_FALSE( i64_value.is_convertable( Value::Type::Uint64 ) );
	i64_value = std::numeric_limits<Value::Int64>::max();
	CHECK( i64_value.is_convertable( Value::Type::Uint64 ) );
	// Int64 -> Float
	CHECK( i64_value.is_convertable( Value::Type::Float ) );
	// Int64 -> Double
	CHECK( i64_value.is_convertable( Value::Type::Double ) );
	// Int64 -> String
	CHECK( i64_value.is_convertable( Value::Type::String ) );
	// Int64 -> Array
	CHECK_FALSE( i64_value.is_convertable( Value::Type::Array ) );
	// Int64 -> Object
	CHECK_FALSE( i64_value.is_convertable( Value::Type::Object ) );

	///////////
	// Uint32
	auto u32_value = Value( Value::Type::Uint32 );
	// Uint32 -> None
	CHECK( u32_value.is_convertable( Value::Type::None ) );
	// Uint32 -> Bool
	CHECK( u32_value.is_convertable( Value::Type::Bool ) );
	// Uint32 -> Int32
	u32_value = (Value::Uint32)std::numeric_limits<Value::Int32>::max() + 1;
	CHECK_FALSE( u32_value.is_convertable( Value::Type::Int32 ) );
	u32_value = 100U;
	CHECK( u32_value.is_convertable( Value::Type::Int32 ) );
	// Uint32 -> Int64
	CHECK( u32_value.is_convertable( Value::Type::Int64 ) );
	// Uint32 -> Uint32
	CHECK( u32_value.is_convertable( Value::Type::Uint32 ) );
	// Uint32 -> Uint64
	CHECK( u32_value.is_convertable( Value::Type::Uint64 ) );
	// Uint32 -> Float
	CHECK( u32_value.is_convertable( Value::Type::Float ) );
	// Uint32 -> Double
	CHECK( u32_value.is_convertable( Value::Type::Double ) );
	// Uint32 -> String
	CHECK( u32_value.is_convertable( Value::Type::String ) );
	// Uint32 -> Array
	CHECK_FALSE( u32_value.is_convertable( Value::Type::Array ) );
	// Uint32 -> Object
	CHECK_FALSE( u32_value.is_convertable( Value::Type::Object ) );

	///////////
	// Uint64
	auto u64_value = Value( Value::Type::Uint64 );
	// Uint64 -> None
	CHECK( u64_value.is_convertable( Value::Type::None ) );
	// Uint64 -> Bool
	CHECK( u64_value.is_convertable( Value::Type::Bool ) );
	// Uint64 -> Int32
	u64_value = (Value::Uint64)std::numeric_limits<Value::Int32>::max() + 1;
	CHECK_FALSE( u64_value.is_convertable( Value::Type::Int32 ) );
	u64_value = 100U;
	CHECK( u64_value.is_convertable( Value::Type::Int32 ) );
	// Uint64 -> Int64
	u64_value = (Value::Uint64)std::numeric_limits<Value::Int64>::max() + 1;
	CHECK_FALSE( u64_value.is_convertable( Value::Type::Int64 ) );
	u64_value = 100U;
	CHECK( u64_value.is_convertable( Value::Type::Int64 ) );
	// Uint64 -> Uint32
	u64_value = (Value::Uint64)std::numeric_limits<Value::Uint32>::max() + 1;
	CHECK_FALSE( u64_value.is_convertable( Value::Type::Uint32 ) );
	u64_value = 100U;
	CHECK( u64_value.is_convertable( Value::Type::Uint32 ) );
	// Uint64 -> Uint64
	CHECK( u64_value.is_convertable( Value::Type::Uint64 ) );
	// Uint64 -> Float
	CHECK( u64_value.is_convertable( Value::Type::Float ) );
	// Uint64 -> Double
	CHECK( u64_value.is_convertable( Value::Type::Double ) );
	// Uint64 -> String
	CHECK( u64_value.is_convertable( Value::Type::String ) );
	// Uint64 -> Array
	CHECK_FALSE( u64_value.is_convertable( Value::Type::Array ) );
	// Uint64 -> Object
	CHECK_FALSE( u64_value.is_convertable( Value::Type::Object ) );

	///////////
	// Float
	auto f_value = Value( Value::Type::Float );
	// Float -> None
	CHECK( f_value.is_convertable( Value::Type::None ) );
	// Float -> Bool
	CHECK( f_value.is_convertable( Value::Type::Bool ) );
	// Float -> Int32
	CHECK_FALSE( f_value.is_convertable( Value::Type::Int32 ) );
	// Float -> Int64
	CHECK_FALSE( f_value.is_convertable( Value::Type::Int64 ) );
	// Float -> Uint32
	CHECK_FALSE( f_value.is_convertable( Value::Type::Uint32 ) );
	// Float -> Uint64
	CHECK_FALSE( f_value.is_convertable( Value::Type::Uint64 ) );
	// Float -> Float
	CHECK( f_value.is_convertable( Value::Type::Float ) );
	// Float -> Double
	CHECK( f_value.is_convertable( Value::Type::Double ) );
	// Float -> String
	CHECK( f_value.is_convertable( Value::Type::String ) );
	// Float -> Array
	CHECK_FALSE( f_value.is_convertable( Value::Type::Array ) );
	// Float -> Object
	CHECK_FALSE( f_value.is_convertable( Value::Type::Object ) );

	///////////
	// Double
	auto d_value = Value( Value::Type::Double );
	// Double -> None
	CHECK( d_value.is_convertable( Value::Type::None ) );
	// Double -> Bool
	CHECK( d_value.is_convertable( Value::Type::Bool ) );
	// Double -> Int32
	CHECK_FALSE( d_value.is_convertable( Value::Type::Int32 ) );
	// Double -> Int64
	CHECK_FALSE( d_value.is_convertable( Value::Type::Int64 ) );
	// Double -> Uint32
	CHECK_FALSE( d_value.is_convertable( Value::Type::Uint32 ) );
	// Double -> Uint64
	CHECK_FALSE( d_value.is_convertable( Value::Type::Uint64 ) );
	// Double -> Float
	CHECK_FALSE( d_value.is_convertable( Value::Type::Float ) );
	// Double -> Double
	CHECK( d_value.is_convertable( Value::Type::Double ) );
	// Double -> String
	CHECK( d_value.is_convertable( Value::Type::String ) );
	// Double -> Array
	CHECK_FALSE( d_value.is_convertable( Value::Type::Array ) );
	// Double -> Object
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
	// String -> Int32
	s_value = "100500";
	CHECK( s_value.is_convertable( Value::Type::Int32 ) );
	s_value = "-100500";
	CHECK( s_value.is_convertable( Value::Type::Int32 ) );
	s_value = "fail";
	CHECK_FALSE( s_value.is_convertable( Value::Type::Int32 ) );
	// String -> Int64
	s_value = "100500";
	CHECK( s_value.is_convertable( Value::Type::Int64 ) );
	s_value = "-100500";
	CHECK( s_value.is_convertable( Value::Type::Int64 ) );
	s_value = "fail";
	CHECK_FALSE( s_value.is_convertable( Value::Type::Int64 ) );
	// String -> Uint32
	s_value = "100500";
	CHECK( s_value.is_convertable( Value::Type::Uint32 ) );
	s_value = "-1";
	CHECK_FALSE( s_value.is_convertable( Value::Type::Uint32 ) );
	// String -> Uint64
	s_value = "100500";
	CHECK( s_value.is_convertable( Value::Type::Uint64 ) );
	s_value = "-1";
	CHECK_FALSE( s_value.is_convertable( Value::Type::Uint64 ) );
	// String -> Float
	s_value = "-1.5";
	CHECK( s_value.is_convertable( Value::Type::Float ) );
	s_value = "fail";
	CHECK_FALSE( s_value.is_convertable( Value::Type::Float ) );
	// String -> Double
	s_value = "-1.5";
	CHECK( s_value.is_convertable( Value::Type::Double ) );
	s_value = "fail";
	CHECK_FALSE( s_value.is_convertable( Value::Type::Double ) );
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
	// Array -> Int32
	CHECK_FALSE( a_value.is_convertable( Value::Type::Int32 ) );
	// Array -> Int64
	CHECK_FALSE( a_value.is_convertable( Value::Type::Int64 ) );
	// Array -> Uint32
	CHECK_FALSE( a_value.is_convertable( Value::Type::Uint32 ) );
	// Array -> Uint64
	CHECK_FALSE( a_value.is_convertable( Value::Type::Uint64 ) );
	// Array -> Float
	CHECK_FALSE( a_value.is_convertable( Value::Type::Float ) );
	// Array -> Double
	CHECK_FALSE( a_value.is_convertable( Value::Type::Double ) );
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
	// Object -> Int32
	CHECK_FALSE( o_value.is_convertable( Value::Type::Int32 ) );
	// Object -> Int64
	CHECK_FALSE( o_value.is_convertable( Value::Type::Int64 ) );
	// Object -> Uint32
	CHECK_FALSE( o_value.is_convertable( Value::Type::Uint32 ) );
	// Object -> Uint64
	CHECK_FALSE( o_value.is_convertable( Value::Type::Uint64 ) );
	// Object -> Float
	CHECK_FALSE( o_value.is_convertable( Value::Type::Float ) );
	// Object -> Double
	CHECK_FALSE( o_value.is_convertable( Value::Type::Double ) );
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
	// None -> Int32
	CHECK( none_value.as( Value::Type::Int32 ).is( Value::Type::Int32 ) );
	CHECK_EQUAL( Value::default_value<Value::Int32>(), none_value.as_int32() );
	// None -> Int64
	CHECK( none_value.as( Value::Type::Int64 ).is( Value::Type::Int64 ) );
	CHECK_EQUAL( Value::default_value<Value::Int64>(), none_value.as_int64() );
	// None -> Uint32
	CHECK( none_value.as( Value::Type::Uint32 ).is( Value::Type::Uint32 ) );
	CHECK_EQUAL( Value::default_value<Value::Uint32>(), none_value.as_uint32() );
	// None -> Uint64
	CHECK( none_value.as( Value::Type::Uint64 ).is( Value::Type::Uint64 ) );
	CHECK_EQUAL( Value::default_value<Value::Uint64>(), none_value.as_uint64() );
	// None -> Float
	CHECK( none_value.as( Value::Type::Float ).is( Value::Type::Float ) );
	CHECK_EQUAL( Value::default_value<Value::Float>(), none_value.as_float() );
	// None -> Double
	CHECK( none_value.as( Value::Type::Double ).is( Value::Type::Double ) );
	CHECK_EQUAL( Value::default_value<Value::Double>(), none_value.as_double() );
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
	CHECK_EQUAL( true, b.get<Value::Bool>() );
	CHECK_EQUAL( true, bool_value.as_bool() );
	// Bool -> Int32
	auto i32 = bool_value.as( Value::Type::Int32 );
	CHECK( i32.is( Value::Type::Int32 ) );
	CHECK_EQUAL( 1, i32.get<Value::Int32>() );
	CHECK_EQUAL( 1, bool_value.as_int32() );
	// Bool -> Int64
	auto i64 = bool_value.as( Value::Type::Int64 );
	CHECK( i64.is( Value::Type::Int64 ) );
	CHECK_EQUAL( 1, i64.get<Value::Int64>() );
	CHECK_EQUAL( 1, bool_value.as_int64() );
	// Bool -> Uint32
	auto u32 = bool_value.as( Value::Type::Uint32 );
	CHECK( u32.is( Value::Type::Uint32 ) );
	CHECK_EQUAL( 1, u32.get<Value::Uint32>() );
	CHECK_EQUAL( 1, bool_value.as_uint32() );
	// Bool -> Uint64
	auto u64 = bool_value.as( Value::Type::Uint64 );
	CHECK( u64.is( Value::Type::Uint64 ) );
	CHECK_EQUAL( 1, u64.get<Value::Uint64>() );
	CHECK_EQUAL( 1, bool_value.as_uint64() );
	// Bool -> Float
	auto f = bool_value.as( Value::Type::Float );
	CHECK( f.is( Value::Type::Float ) );
	DOUBLES_EQUAL( 1.0, f.get<Value::Float>(), std::numeric_limits<Value::Float>::epsilon() );
	CHECK_EQUAL( 1.0, bool_value.as_float() );
	// Bool -> Double
	auto d = bool_value.as( Value::Type::Double );
	CHECK( d.is( Value::Type::Double ) );
	DOUBLES_EQUAL( 1.0, d.get<Value::Double>(), std::numeric_limits<Value::Double>::epsilon() );
	CHECK_EQUAL( 1.0, bool_value.as_double() );
	// Bool -> String
	auto s = bool_value.as( Value::Type::String );
	CHECK( s.is( Value::Type::String ) );
	STRCMP_EQUAL( "true", s.get<Value::String>().c_str() );
	STRCMP_EQUAL( "true", bool_value.as_string().c_str() );
	// Bool -> Array
	CHECK( bool_value.as( Value::Type::Array ).is( Value::Type::Array ) );
	CHECK( bool_value.as_array().empty() );
	// Bool -> Object
	CHECK( bool_value.as( Value::Type::Object ).is( Value::Type::Object ) );
	CHECK( bool_value.as_object().empty() );

	///////////
	// Int32
	auto i32_value = Value( 123 );
	// Int32 -> None
	CHECK( i32_value.as( Value::Type::None ).is( Value::Type::None ) );
	// Int32 -> Bool
	b = i32_value.as( Value::Type::Bool );
	CHECK( b.is( Value::Type::Bool ) );
	CHECK_EQUAL( true, b.get<Value::Bool>() );
	CHECK_EQUAL( true, i32_value.as_bool() );
	// Int32 -> Int32
	i32 = i32_value.as( Value::Type::Int32 );
	CHECK( i32.is( Value::Type::Int32 ) );
	CHECK_EQUAL( 123, i32.get<Value::Int32>() );
	CHECK_EQUAL( 123, i32_value.as_int32() );
	// Int32 -> Int64
	i64 = i32_value.as( Value::Type::Int64 );
	CHECK( i64.is( Value::Type::Int64 ) );
	CHECK_EQUAL( 123, i64.get<Value::Int64>() );
	CHECK_EQUAL( 123, i32_value.as_int64() );
	// Int32 -> Uint32
	i32_value = -1;
	u32 = i32_value.as( Value::Type::Uint32 );
	CHECK( u32.is( Value::Type::Uint32 ) );
	CHECK_EQUAL( Value::default_value<Value::Uint32>(), u32.get<Value::Uint32>() );
	CHECK_EQUAL( Value::default_value<Value::Uint32>(), i32_value.as_uint32() );
	i32_value = 123;
	u32 = i32_value.as( Value::Type::Uint32 );
	CHECK( u32.is( Value::Type::Uint32 ) );
	CHECK_EQUAL( 123, u32.get<Value::Uint32>() );
	CHECK_EQUAL( 123, i32_value.as_int32() );
	// Int32 -> Uint64
	i32_value = -1;
	u64 = i32_value.as( Value::Type::Uint64 );
	CHECK( u64.is( Value::Type::Uint64 ) );
	CHECK_EQUAL( Value::default_value<Value::Uint64>(), u64.get<Value::Uint64>() );
	CHECK_EQUAL( Value::default_value<Value::Uint64>(), i32_value.as_uint64() );
	i32_value = 123;
	u64 = i32_value.as( Value::Type::Uint64 );
	CHECK( u64.is( Value::Type::Uint64 ) );
	CHECK_EQUAL( 123, u64.get<Value::Uint64>() );
	CHECK_EQUAL( 123, i32_value.as_uint64() );
	// Int32 -> Float
	f = i32_value.as( Value::Type::Float );
	CHECK( f.is( Value::Type::Float ) );
	DOUBLES_EQUAL( 123.0, f.get<Value::Float>(), std::numeric_limits<Value::Float>::epsilon() );
	DOUBLES_EQUAL( 123.0, i32_value.as_float(), std::numeric_limits<Value::Float>::epsilon() );
	// Int32 -> Double
	d = i32_value.as( Value::Type::Double );
	CHECK( d.is( Value::Type::Double ) );
	DOUBLES_EQUAL( 123.0, d.get<Value::Double>(), std::numeric_limits<Value::Double>::epsilon() );
	DOUBLES_EQUAL( 123.0, i32_value.as_double(), std::numeric_limits<Value::Double>::epsilon() );
	// Int32 -> String
	s = i32_value.as( Value::Type::String );
	STRCMP_EQUAL( "123", s.get<Value::String>().c_str() );
	STRCMP_EQUAL( "123", i32_value.as_string().c_str() );
	// Int32 -> Array
	CHECK( i32_value.as( Value::Type::Array ).is( Value::Type::Array ) );
	CHECK( i32_value.as_array().empty() );
	// Int32 -> Object
	CHECK( i32_value.as( Value::Type::Object ).is( Value::Type::Object ) );
	CHECK( i32_value.as_object().empty() );

	///////////
	// Int64
	auto i64_value = Value( (Value::Int64)123 );
	// Int64 -> None
	CHECK( i64_value.as( Value::Type::None ).is( Value::Type::None ) );
	// Int64 -> Bool
	b = i64_value.as( Value::Type::Bool );
	CHECK( b.is( Value::Type::Bool ) );
	CHECK_EQUAL( true, b.get<Value::Bool>() );
	CHECK_EQUAL( true, i64_value.as_bool() );
	// Int64 -> Int32
	i64_value = (Value::Int64)std::numeric_limits<Value::Int32>::max() + 1;
	i32 = i64_value.as( Value::Type::Int32 );
	CHECK( i32.is( Value::Type::Int32 ) );
	CHECK_EQUAL( Value::default_value<Value::Int32>(), i32.get<Value::Int32>() );
	CHECK_EQUAL( Value::default_value<Value::Int32>(), i64_value.as_int32() );
	i64_value = (Value::Int64)123;
	i32 = i64_value.as( Value::Type::Int32 );
	CHECK( i32.is( Value::Type::Int32 ) );
	CHECK_EQUAL( 123, i32.get<Value::Int32>() );
	CHECK_EQUAL( 123, i32.as_int32() );
	// Int64 -> Int64
	i64 = i64_value.as( Value::Type::Int64 );
	CHECK( i64.is( Value::Type::Int64 ) );
	CHECK_EQUAL( 123, i64.get<Value::Int64>() );
	CHECK_EQUAL( 123, i64_value.as_int64() );
	// Int64 -> Uint32
	i64_value = (Value::Int64)-1;
	u32 = i64_value.as( Value::Type::Uint32 );
	CHECK( u32.is( Value::Type::Uint32 ) );
	CHECK_EQUAL( Value::default_value<Value::Uint32>(), u32.get<Value::Uint32>() );
	CHECK_EQUAL( Value::default_value<Value::Uint32>(), i64_value.as_uint32() );
	i64_value = (Value::Int64)123;
	u32 = i64_value.as( Value::Type::Uint32 );
	CHECK( u32.is( Value::Type::Uint32 ) );
	CHECK_EQUAL( 123, u32.get<Value::Uint32>() );
	CHECK_EQUAL( 123, i64_value.as_uint32() );
	// Int64 -> Uint64
	i64_value = (Value::Int64)-1;
	u64 = i64_value.as( Value::Type::Uint64 );
	CHECK( u64.is( Value::Type::Uint64 ) );
	CHECK_EQUAL( Value::default_value<Value::Uint64>(), u64.get<Value::Uint64>() );
	CHECK_EQUAL( Value::default_value<Value::Uint64>(), i64_value.as_uint64() );
	i64_value = (Value::Int64)123;
	u64 = i64_value.as( Value::Type::Uint64 );
	CHECK( u64.is( Value::Type::Uint64 ) );
	CHECK_EQUAL( 123, u64.get<Value::Uint64>() );
	CHECK_EQUAL( 123, i64_value.as_uint64() );
	// Int64 -> Float
	f = i64_value.as( Value::Type::Float );
	CHECK( f.is( Value::Type::Float ) );
	DOUBLES_EQUAL( 123.0, f.get<Value::Float>(), std::numeric_limits<Value::Float>::epsilon() );
	DOUBLES_EQUAL( 123.0, i64_value.as_float(), std::numeric_limits<Value::Float>::epsilon() );
	// Int64 -> Double
	d = i64_value.as( Value::Type::Double );
	CHECK( d.is( Value::Type::Double ) );
	DOUBLES_EQUAL( 123.0, d.get<Value::Double>(), std::numeric_limits<Value::Double>::epsilon() );
	DOUBLES_EQUAL( 123.0, i64_value.as_double(), std::numeric_limits<Value::Double>::epsilon() );
	// Int64 -> String
	s = i64_value.as( Value::Type::String );
	STRCMP_EQUAL( "123", s.get<Value::String>().c_str() );
	STRCMP_EQUAL( "123", i64_value.as_string().c_str() );
	// Int64 -> Array
	CHECK( i64_value.as( Value::Type::Array ).is( Value::Type::Array ) );
	CHECK( i64_value.as_array().empty() );
	// Int64 -> Object
	CHECK( i64_value.as( Value::Type::Object ).is( Value::Type::Object ) );
	CHECK( i64_value.as_object().empty() );

	///////////
	// Uint32
	auto u32_value = Value( (Value::Uint32)123 );
	// Uint32 -> None
	CHECK( u32_value.as( Value::Type::None ).is( Value::Type::None ) );
	// Uint32 -> Bool
	b = u32_value.as( Value::Type::Bool );
	CHECK( b.is( Value::Type::Bool ) );
	CHECK_EQUAL( true, b.get<Value::Bool>() );
	CHECK_EQUAL( true, u32_value.as_bool() );
	// Uint32 -> Int32
	u32_value = Value( (Value::Uint32)std::numeric_limits<Value::Int32>::max() + 1 );
	i32 = u32_value.as( Value::Type::Int32 );
	CHECK( i32.is( Value::Type::Int32 ) );
	CHECK_EQUAL( Value::default_value<Value::Int32>(), i32.get<Value::Int32>() );
	CHECK_EQUAL( Value::default_value<Value::Int32>(), u32_value.as_int32() );
	u32_value = Value( (Value::Uint32)123 );
	i32 = u32_value.as( Value::Type::Int32 );
	CHECK( i32.is( Value::Type::Int32 ) );
	CHECK_EQUAL( 123, i32.get<Value::Int32>() );
	CHECK_EQUAL( 123, u32_value.as_int32() );
	// Uint32 -> Int64
	i64 = u32_value.as( Value::Type::Int64 );
	CHECK( i64.is( Value::Type::Int64 ) );
	CHECK_EQUAL( 123, i64.get<Value::Int64>() );
	CHECK_EQUAL( 123, u32_value.as_int64() );
	// Uint32 -> Uint32
	u32 = u32_value.as( Value::Type::Uint32 );
	CHECK( u32.is( Value::Type::Uint32 ) );
	CHECK_EQUAL( 123, u32.get<Value::Uint32>() );
	CHECK_EQUAL( 123, u32_value.as_uint32() );
	// Uint32 -> Uint64
	u64 = u32_value.as( Value::Type::Uint64 );
	CHECK( u64.is( Value::Type::Uint64 ) );
	CHECK_EQUAL( 123, u64.get<Value::Uint64>() );
	CHECK_EQUAL( 123, u32_value.as_uint64() );
	// Uint32 -> Float
	f = u32_value.as( Value::Type::Float );
	CHECK( f.is( Value::Type::Float ) );
	DOUBLES_EQUAL( 123.0, f.get<Value::Float>(), std::numeric_limits<Value::Float>::epsilon() );
	DOUBLES_EQUAL( 123.0, u32_value.as_float(), std::numeric_limits<Value::Float>::epsilon() );
	// Uint32 -> Double
	d = u32_value.as( Value::Type::Double );
	CHECK( d.is( Value::Type::Double ) );
	DOUBLES_EQUAL( 123.0, d.get<Value::Double>(), std::numeric_limits<Value::Double>::epsilon() );
	DOUBLES_EQUAL( 123.0, u32_value.as_double(), std::numeric_limits<Value::Double>::epsilon() );
	// Uint32 -> String
	s = u32_value.as( Value::Type::String );
	STRCMP_EQUAL( "123", s.get<Value::String>().c_str() );
	STRCMP_EQUAL( "123", u32_value.as_string().c_str() );
	// Uint32 -> Array
	CHECK( u32_value.as( Value::Type::Array ).is( Value::Type::Array ) );
	CHECK( u32_value.as_array().empty() );
	// Uint32 -> Object
	CHECK( u32_value.as( Value::Type::Object ).is( Value::Type::Object ) );
	CHECK( u32_value.as_object().empty() );

	///////////
	// Uint64
	auto u64_value = Value( (Value::Uint64)123 );
	// Uint64 -> None
	CHECK( u64_value.as( Value::Type::None ).is( Value::Type::None ) );
	// Uint64 -> Bool
	b = u64_value.as( Value::Type::Bool );
	CHECK( b.is( Value::Type::Bool ) );
	CHECK_EQUAL( true, b.get<Value::Bool>() );
	CHECK_EQUAL( true, u64_value.as_bool() );
	// Uint64 -> Int32
	u64_value = Value( (Value::Uint64)-1 );
	i32 = u64_value.as( Value::Type::Int32 );
	CHECK( i32.is( Value::Type::Int32 ) );
	CHECK_EQUAL( Value::default_value<Value::Int32>(), i32.get<Value::Int32>() );
	CHECK_EQUAL( Value::default_value<Value::Int32>(), u64_value.as_int32() );
	u64_value = Value( (Value::Uint64)123 );
	i32 = u64_value.as( Value::Type::Int32 );
	CHECK( i32.is( Value::Type::Int32 ) );
	CHECK_EQUAL( 123, i32.get<Value::Int32>() );
	CHECK_EQUAL( 123, u64_value.as_int32() );
	// Uint64 -> Int64
	u64_value = Value( (Value::Uint64)-1 );
	i64 = u64_value.as( Value::Type::Int64 );
	CHECK( i64.is( Value::Type::Int64 ) );
	CHECK_EQUAL( Value::default_value<Value::Int64>(), i64.get<Value::Int64>() );
	CHECK_EQUAL( Value::default_value<Value::Int64>(), u64_value.as_int64() );
	u64_value = Value( (Value::Uint64)123 );
	i64 = u64_value.as( Value::Type::Int64 );
	CHECK( i64.is( Value::Type::Int64 ) );
	CHECK_EQUAL( 123, i64.get<Value::Int64>() );
	CHECK_EQUAL( 123, u64_value.as_int64() );
	// Uint64 -> Uint32
	u64_value = Value( (Value::Uint64)std::numeric_limits<Value::Uint32>::max() + 1 );
	u32 = u64_value.as( Value::Type::Uint32 );
	CHECK( u32.is( Value::Type::Uint32 ) );
	CHECK_EQUAL( Value::default_value<Value::Uint32>(), u32.get<Value::Uint32>() );
	CHECK_EQUAL( Value::default_value<Value::Uint32>(), u64_value.as_uint32() );
	u64_value = Value( (Value::Uint64)123 );
	u32 = u64_value.as( Value::Type::Uint32 );
	CHECK( u32.is( Value::Type::Uint32 ) );
	CHECK_EQUAL( 123, u32.get<Value::Uint32>() );
	CHECK_EQUAL( 123, u64_value.as_uint32() );
	// Uint64 -> Uint64
	u64 = u64_value.as( Value::Type::Uint64 );
	CHECK( u64.is( Value::Type::Uint64 ) );
	CHECK_EQUAL( 123, u64.get<Value::Uint64>() );
	CHECK_EQUAL( 123, u64_value.as_uint64() );
	// Uint64 -> Float
	f = u64_value.as( Value::Type::Float );
	CHECK( f.is( Value::Type::Float ) );
	DOUBLES_EQUAL( 123.0, f.get<Value::Float>(), std::numeric_limits<Value::Float>::epsilon() );
	DOUBLES_EQUAL( 123.0, u64.as_float(), std::numeric_limits<Value::Float>::epsilon() );
	// Uint64 -> Double
	d = u64_value.as( Value::Type::Double );
	CHECK( d.is( Value::Type::Double ) );
	DOUBLES_EQUAL( 123.0, d.get<Value::Double>(), std::numeric_limits<Value::Double>::epsilon() );
	DOUBLES_EQUAL( 123.0, u64_value.as_double(), std::numeric_limits<Value::Double>::epsilon() );
	// Uint64 -> String
	s = u64_value.as( Value::Type::String );
	STRCMP_EQUAL( "123", s.get<Value::String>().c_str() );
	STRCMP_EQUAL( "123", u64_value.as_string().c_str() );
	// Uint64 -> Array
	CHECK( u64_value.as( Value::Type::Array ).is( Value::Type::Array ) );
	CHECK( u64_value.as_array().empty() );
	// Uint64 -> Object
	CHECK( u64_value.as( Value::Type::Object ).is( Value::Type::Object ) );
	CHECK( u64_value.as_object().empty() );

	///////////
	// Float
	auto float_value = Value( (Value::Float)12.3f );
	// Float -> None
	CHECK( float_value.as( Value::Type::None ).is( Value::Type::None ) );
	// Float -> Bool
	b = float_value.as( Value::Type::Bool );
	CHECK( b.is( Value::Type::Bool ) );
	CHECK_EQUAL( true, b.get<Value::Bool>() );
	CHECK_EQUAL( true, float_value.as_bool() );
	// Float -> Int32
	i32 = float_value.as( Value::Type::Int32 );
	CHECK( i32.is( Value::Type::Int32 ) );
	CHECK_EQUAL( Value::default_value<Value::Int32>(), i32.get<Value::Int32>() );
	CHECK_EQUAL( Value::default_value<Value::Int32>(), float_value.as_int32() );
	// Float -> Int64
	i64 = float_value.as( Value::Type::Int64 );
	CHECK( i64.is( Value::Type::Int64 ) );
	CHECK_EQUAL( Value::default_value<Value::Int64>(), i64.get<Value::Int64>() );
	CHECK_EQUAL( Value::default_value<Value::Int64>(), float_value.as_int64() );
	// Float -> Uint32
	u32 = float_value.as( Value::Type::Uint32 );
	CHECK( u32.is( Value::Type::Uint32 ) );
	CHECK_EQUAL( Value::default_value<Value::Uint32>(), u32.get<Value::Uint32>() );
	CHECK_EQUAL( Value::default_value<Value::Uint32>(), float_value.as_uint32() );
	// Float -> Uint64
	u64 = float_value.as( Value::Type::Uint64 );
	CHECK( u64.is( Value::Type::Uint64 ) );
	CHECK_EQUAL( Value::default_value<Value::Uint64>(), u64.get<Value::Uint64>() );
	CHECK_EQUAL( Value::default_value<Value::Uint64>(), float_value.as_uint64() );
	// Float -> Float
	f = float_value.as( Value::Type::Float );
	CHECK( f.is( Value::Type::Float ) );
	DOUBLES_EQUAL( 12.3f, f.get<Value::Float>(), std::numeric_limits<Value::Float>::epsilon() );
	DOUBLES_EQUAL( 12.3f, float_value.as_float(), std::numeric_limits<Value::Float>::epsilon() );
	// Float -> Double
	d = float_value.as( Value::Type::Double );
	CHECK( d.is( Value::Type::Double ) );
	DOUBLES_EQUAL( 12.3f, d.get<Value::Double>(), std::numeric_limits<Value::Double>::epsilon() );
	DOUBLES_EQUAL( 12.3f, float_value.as_double(), std::numeric_limits<Value::Double>::epsilon() );
	// Float -> String
	s = float_value.as( Value::Type::String );
	STRCMP_CONTAINS( "12.3", s.get<Value::String>().c_str() );
	STRCMP_CONTAINS( "12.3", float_value.as_string().c_str() );
	// Float -> Array
	CHECK( float_value.as( Value::Type::Array ).is( Value::Type::Array ) );
	CHECK( float_value.as_array().empty() );
	// Float -> Object
	CHECK( float_value.as( Value::Type::Object ).is( Value::Type::Object ) );
	CHECK( float_value.as_object().empty() );

	///////////
	// Double
	auto double_value = Value( (Value::Double)12.3 );
	// Double -> None
	CHECK( double_value.as( Value::Type::None ).is( Value::Type::None ) );
	// Double -> Bool
	b = double_value.as( Value::Type::Bool );
	CHECK( b.is( Value::Type::Bool ) );
	CHECK_EQUAL( true, b.get<Value::Bool>() );
	CHECK_EQUAL( true, double_value.as_bool() );
	// Double -> Int32
	i32 = double_value.as( Value::Type::Int32 );
	CHECK( i32.is( Value::Type::Int32 ) );
	CHECK_EQUAL( Value::default_value<Value::Int32>(), i32.get<Value::Int32>() );
	CHECK_EQUAL( Value::default_value<Value::Int32>(), double_value.as_int32() );
	// Double -> Int64
	i64 = double_value.as( Value::Type::Int64 );
	CHECK( i64.is( Value::Type::Int64 ) );
	CHECK_EQUAL( Value::default_value<Value::Int64>(), i64.get<Value::Int64>() );
	CHECK_EQUAL( Value::default_value<Value::Int64>(), double_value.as_int64() );
	// Double -> Uint32
	u32 = double_value.as( Value::Type::Uint32 );
	CHECK( u32.is( Value::Type::Uint32 ) );
	CHECK_EQUAL( Value::default_value<Value::Uint32>(), u32.get<Value::Uint32>() );
	CHECK_EQUAL( Value::default_value<Value::Uint32>(), double_value.as_uint32() );
	// Double -> Uint64
	u64 = double_value.as( Value::Type::Uint64 );
	CHECK( u64.is( Value::Type::Uint64 ) );
	CHECK_EQUAL( Value::default_value<Value::Uint64>(), u64.get<Value::Uint64>() );
	CHECK_EQUAL( Value::default_value<Value::Uint64>(), double_value.as_uint64() );
	// Double -> Float
	f = double_value.as( Value::Type::Float );
	CHECK( f.is( Value::Type::Float ) );
	DOUBLES_EQUAL( Value::default_value<Value::Float>(), f.get<Value::Float>(), std::numeric_limits<Value::Float>::epsilon() );
	DOUBLES_EQUAL( Value::default_value<Value::Float>(), double_value.as_float(), std::numeric_limits<Value::Float>::epsilon() );
	// Double -> Double
	d = double_value.as( Value::Type::Double );
	CHECK( d.is( Value::Type::Double ) );
	DOUBLES_EQUAL( 12.3, d.get<Value::Double>(), std::numeric_limits<Value::Double>::epsilon() );
	DOUBLES_EQUAL( 12.3, double_value.as_double(), std::numeric_limits<Value::Double>::epsilon() );
	// Double -> String
	s = double_value.as( Value::Type::String );
	STRCMP_CONTAINS( "12.3", s.get<Value::String>().c_str() );
	STRCMP_CONTAINS( "12.3", double_value.as_string().c_str() );
	// Double -> Array
	CHECK( double_value.as( Value::Type::Array ).is( Value::Type::Array ) );
	CHECK( double_value.as_array().empty() );
	// Double -> Object
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
	CHECK_EQUAL( false, b.get<Value::Bool>() );
	CHECK_EQUAL( false, str_value.as_bool() );
	str_value = Value( "true" );
	b = str_value.as( Value::Type::Bool );
	CHECK( b.is( Value::Type::Bool ) );
	CHECK_EQUAL( true, b.get<Value::Bool>() );
	CHECK_EQUAL( true, str_value.as_bool() );
	// String -> Int32
	str_value = Value( "-123" );
	i32 = str_value.as( Value::Type::Int32 );
	CHECK( i32.is( Value::Type::Int32 ) );
	CHECK_EQUAL( -123, i32.get<Value::Int32>() );
	CHECK_EQUAL( -123, str_value.as_int32() );
	// String -> Int64
	i64 = str_value.as( Value::Type::Int64 );
	CHECK( i64.is( Value::Type::Int64 ) );
	CHECK_EQUAL( -123, i64.get<Value::Int64>() );
	CHECK_EQUAL( -123, str_value.as_int64() );
	// String -> Uint32
	str_value = Value( "123" );
	u32 = str_value.as( Value::Type::Uint32 );
	CHECK( u32.is( Value::Type::Uint32 ) );
	CHECK_EQUAL( 123, u32.get<Value::Uint32>() );
	CHECK_EQUAL( 123, str_value.as_uint32() );
	// String -> Uint64
	u64 = str_value.as( Value::Type::Uint64 );
	CHECK( u64.is( Value::Type::Uint64 ) );
	CHECK_EQUAL( 123, u64.get<Value::Uint64>() );
	CHECK_EQUAL( 123, str_value.as_uint64() );
	// String -> Float
	str_value = Value( "12.3" );
	f = str_value.as( Value::Type::Float );
	CHECK( f.is( Value::Type::Float ) );
	DOUBLES_EQUAL( 12.3f, f.get<Value::Float>(), std::numeric_limits<Value::Float>::epsilon() );
	DOUBLES_EQUAL( 12.3f, str_value.as_float(), std::numeric_limits<Value::Float>::epsilon() );
	// String -> Double
	d = str_value.as( Value::Type::Double );
	CHECK( d.is( Value::Type::Double ) );
	DOUBLES_EQUAL( 12.3, d.get<Value::Double>(), std::numeric_limits<Value::Double>::epsilon() );
	DOUBLES_EQUAL( 12.3, str_value.as_double(), std::numeric_limits<Value::Double>::epsilon() );
	// String -> String
	str_value = Value( "test" );
	s = str_value.as( Value::Type::String );
	STRCMP_EQUAL( "test", s.get<Value::String>().c_str() );
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
	CHECK( arr_value.as( Value::Type::Int32 ).is( Value::Type::Int32 ) );
	CHECK_EQUAL( Value::default_value<Value::Int32>(), arr_value.as_int32() );
	// Array -> Int64
	CHECK( arr_value.as( Value::Type::Int64 ).is( Value::Type::Int64 ) );
	CHECK_EQUAL( Value::default_value<Value::Int64>(), arr_value.as_int64() );
	// Array -> Uint32
	CHECK( arr_value.as( Value::Type::Uint32 ).is( Value::Type::Uint32 ) );
	CHECK_EQUAL( Value::default_value<Value::Uint32>(), arr_value.as_uint32() );
	// Array -> Uint64
	CHECK( arr_value.as( Value::Type::Uint64 ).is( Value::Type::Uint64 ) );
	CHECK_EQUAL( Value::default_value<Value::Uint64>(), arr_value.as_uint64() );
	// Array -> Float
	CHECK( arr_value.as( Value::Type::Float ).is( Value::Type::Float ) );
	CHECK_EQUAL( Value::default_value<Value::Float>(), arr_value.as_float() );
	// Array -> Double
	CHECK( arr_value.as( Value::Type::Double ).is( Value::Type::Double ) );
	CHECK_EQUAL( Value::default_value<Value::Double>(), arr_value.as_double() );
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
	CHECK( obj_value.as( Value::Type::Int32 ).is( Value::Type::Int32 ) );
	CHECK_EQUAL( Value::default_value<Value::Int32>(), obj_value.as_int32() );
	// Object -> Int64
	CHECK( obj_value.as( Value::Type::Int64 ).is( Value::Type::Int64 ) );
	CHECK_EQUAL( Value::default_value<Value::Int64>(), obj_value.as_int64() );
	// Object -> Uint32
	CHECK( obj_value.as( Value::Type::Uint32 ).is( Value::Type::Uint32 ) );
	CHECK_EQUAL( Value::default_value<Value::Uint32>(), obj_value.as_uint32() );
	// Object -> Uint64
	CHECK( obj_value.as( Value::Type::Uint64 ).is( Value::Type::Uint64 ) );
	CHECK_EQUAL( Value::default_value<Value::Uint64>(), obj_value.as_uint64() );
	// Object -> Float
	CHECK( obj_value.as( Value::Type::Float ).is( Value::Type::Float ) );
	CHECK_EQUAL( Value::default_value<Value::Float>(), obj_value.as_float() );
	// Object -> Double
	CHECK( obj_value.as( Value::Type::Double ).is( Value::Type::Double ) );
	CHECK_EQUAL( Value::default_value<Value::Double>(), obj_value.as_double() );
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
