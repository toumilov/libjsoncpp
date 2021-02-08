
#include "json.hpp"
#include "CppUTest/TestHarness.h"

using namespace jsoncpp;

TEST_GROUP(JsonGroup)
{
	Error e;
	void setup()
	{
		e.clear();
	}
	void teardown()
	{
	}
};

TEST(JsonGroup, EmptyStringTest)
{
	// Empty string is valid
	CHECK( Json::validate( "", e ) );
	CHECK( e.empty() );
	CHECK( Json::validate( " ", e ) );
	CHECK( e.empty() );
	CHECK( Json::validate( "\t", e ) );
	CHECK( e.empty() );
	CHECK( Json::validate( "\r", e ) );
	CHECK( e.empty() );
	CHECK( Json::validate( "\n", e ) );
	CHECK( e.empty() );
}

TEST(JsonGroup, LexemeTest)
{
	// "true" lexeme
	CHECK( Json::validate( "true", e ) );
	CHECK( e.empty() );

	// "false" lexeme
	CHECK( Json::validate( "false", e ) );
	CHECK( e.empty() );

	// "null" lexeme
	CHECK( Json::validate( "null", e ) );
	CHECK( e.empty() );

	// Whitespaces
	CHECK( Json::validate( " \ttrue\t ", e ) );
	CHECK( e.empty() );

	// Bad lexeme
	CHECK_FALSE( Json::validate( "None", e ) );
	CHECK( e.code() == Error::UnexpectedToken );
	e.clear();

	CHECK_FALSE( Json::validate( "bad", e ) );
	CHECK( e.code() == Error::UnexpectedToken );
	e.clear();
}

TEST(JsonGroup, StringTest)
{
	CHECK( Json::validate( "\"\"", e ) );
	CHECK( e.empty() );

	CHECK( Json::validate( "\" \"", e ) );
	CHECK( e.empty() );

	CHECK( Json::validate( "\"test\"", e ) );
	CHECK( e.empty() );

	CHECK( Json::validate( "\"test 123\"", e ) );
	CHECK( e.empty() );

	CHECK( Json::validate( "\"A_1!!!\"", e ) );
	CHECK( e.empty() );

	CHECK( Json::validate( "\"\\\"\"", e ) );
	CHECK( e.empty() );

	CHECK( Json::validate( "\"\\\\\"", e ) );
	CHECK( e.empty() );

	CHECK( Json::validate( "\"\\/\"", e ) );
	CHECK( e.empty() );

	CHECK( Json::validate( "\"\\b\"", e ) );
	CHECK( e.empty() );

	CHECK( Json::validate( "\"\\f\"", e ) );
	CHECK( e.empty() );

	CHECK( Json::validate( "\"\\n\"", e ) );
	CHECK( e.empty() );

	CHECK( Json::validate( "\"\\r\"", e ) );
	CHECK( e.empty() );

	CHECK( Json::validate( "\"\\t\"", e ) );
	CHECK( e.empty() );

	CHECK( Json::validate( "\"\\u0123\"", e ) );
	CHECK( e.empty() );

	CHECK( Json::validate( "\"\\uDead\"", e ) );
	CHECK( e.empty() );

	CHECK( Json::validate( "\"Feed 1 \\uDead\" ", e ) );
	CHECK( e.empty() );

	CHECK( Json::validate( R"_(" ' \\ \" \/ \b \f \n \r \t ,./*+-`~@!?<>_%$;:()#^[]{}|")_", e ) );
	CHECK( e.empty() );

	CHECK_FALSE( Json::validate( "\"\\ubad\"", e ) );
	CHECK( e.code() == Error::UnexpectedCharacter );
	e.clear();
}

TEST(JsonGroup, NumberTest)
{
	std::string valid_numbers[] = {
		"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "12", "123",
		"11", "22", "33", "44", "55", "66", "77", "88", "99",
		"-0", "-1", "-2", "-3", "-4", "-5", "-6", "-7", "-8", "-9", "-10",
		"-123", "-11", "-22", "-33", "-44", "-55", "-66", "-77", "-88", "-99",
		"0.0", "0.1", "0.2", "0.3", "0.4", "0.5", "0.6", "0.7", "0.8", "0.9", "1.2", "1.23",
		"1.1", "2.2", "3.3", "4.4", "5.5", "6.6", "7.7", "8.8", "9.9", "-0.0",
		"-1.1", "-2.2", "-3.3", "-4.4", "-5.5", "-6.6", "-7.7", "-8.8", "-9.9", "-10.01",
		"-12.3", "-11.1", "-22.2", "-33.3", "-44.4", "-55.5", "-66.6", "-77.7", "-88.8", "-99.9",
		"0e0", "1e1", "2e2", "3e3", "4e4", "5e5", "6e6", "7e7", "8e8", "9e9",
		"12e+12", "123e-123", "11e11", "22e22", "33e33", "44e44", "55e55", "66e66", "77e77",
		"88e88", "99e99", "-0e0", "-1e1", "-2e2", "-3e3", "-4e4", "-5e5", "-6e6", "-7e7",
		"-8e8", "-9e9", "-10e+2", "-123e-45", "-11e11", "-22e22", "-33e33", "-44e44", "-55e55",
		"-66e66", "-77e77", "-88e88", "-99e99", "0E0", "1E1", "2E2", "3E3", "4E4", "5E5",
		"6E6", "7E7", "8E8", "9E9", "12E+12", "123E-123", "11E11", "22E22", "33E33", "44E44",
		"55E55", "66E66", "77E77", "88E88", "99E99", "-0E0", "-1E1", "-2E2", "-3E3", "-4E4",
		"-5E5", "-6E6", "-7E7", "-8E8", "-9E9", "-10E+2", "-123E-45", "-11E11", "-22E22",
		"-33E33", "-44E44", "-55E55", "-66E66", "-77E77", "-88E88", "-99E99"
	};
	std::string invalid_numbers[] = { "-", "0-", "-e0", "-E0", "-E12", "e2E4", "e+", "E+", "0e+", "0E+" };

	for( const auto &i : valid_numbers )
	{
		CHECK( Json::validate( i, e ) );
		CHECK( e.empty() );
	}
	for( const auto &i : invalid_numbers )
	{
		CHECK_FALSE( Json::validate( i, e ) );
		CHECK( !e.empty() );
		e.clear();
	}
}

TEST(JsonGroup, ArrayTest)
{
	CHECK( Json::validate( "[]", e ) );
	CHECK( e.empty() );

	CHECK( Json::validate( "[ ]", e ) );
	CHECK( e.empty() );

	CHECK( Json::validate( "[\t]", e ) );
	CHECK( e.empty() );

	CHECK( Json::validate( "[\r\n]", e ) );
	CHECK( e.empty() );

	CHECK( Json::validate( "[\"test\"]", e ) );
	CHECK( e.empty() );

	CHECK( Json::validate( "[\"test\",5,-123,true]", e ) );
	CHECK( e.empty() );

	CHECK( Json::validate( "[ \"test\" , 5 , -123 , true ]", e ) );
	CHECK( e.empty() );

	// Missing comma
	CHECK_FALSE( Json::validate( "[ \"test\" , 5 true ]", e ) );
	CHECK( !e.empty() );
}

TEST(JsonGroup, ObjectTest)
{
	CHECK( Json::validate( "{}", e ) );
	CHECK( e.empty() );

	CHECK( Json::validate( "{ \t}", e ) );
	CHECK( e.empty() );

	CHECK( Json::validate( " { \"key\" : \"value\" } ", e ) );
	CHECK( e.empty() );

	CHECK( Json::validate( "{ \"key\" : \"value\", \"count\" : 5, \"price\" : 1.50 }", e ) );
	CHECK( e.empty() );

	CHECK( Json::validate( "{\"key\":\"value\",\"count\":5,\"price\":1.50,\r\n\"valid\":true,\"promo_code\":null}", e ) );
	CHECK( e.empty() );

	CHECK_FALSE( Json::validate( "{,}", e ) );
	CHECK( !e.empty() );
}

TEST(JsonGroup, CombinedTest)
{
	std::string str = R"_({
	"string": "test",
	"number": 123,
	"object": {
		"key": "value",
		"bool": true,
		"undef": null
	},
	"array": [
		123,
		false,
		"test"
	],
	"float": 1.5
	})_";
	CHECK( Json::validate( str, e ) );
	CHECK( e.empty() );
}

TEST(JsonGroup, ParseCombinedTest)
{
	std::string str = R"_({
	"string": "test",
	"number": 123,
	"object": {
		"key": "value",
		"bool": true,
		"undef": null
	},
	"array": [
		0.1,
		false,
		"text"
	],
	"double": 1.5,
	"empty_array": [{}]
	})_";
	auto v = Json::parse( str, e );
	CHECK( e.empty() );

	STRCMP_EQUAL( "test", v["string"].as_string().c_str() );
	CHECK_EQUAL( 123, v["number"].as_uint32() );
	STRCMP_EQUAL( "value", v["object"]["key"].as_string().c_str() );
	CHECK( v["object"]["bool"].as_bool() );
	CHECK( v["object"]["undef"].is_none() );
	CHECK_EQUAL( 3, v["array"].size() );
	DOUBLES_EQUAL( 0.1, v["array"][0].as_float(), std::numeric_limits<float>::epsilon() );
	CHECK_FALSE( v["array"][1].as_bool() );
	STRCMP_EQUAL( "text", v["array"][2].as_string().c_str() );
	STRCMP_EQUAL( "text", v["array"].back().as_string().c_str() );
	DOUBLES_EQUAL( 1.5, v["double"].as_double(), std::numeric_limits<double>::epsilon() );
	CHECK_EQUAL( 1, v["empty_array"].size() );
	CHECK( Value::Type::Object == v["empty_array"][0].type() );

	STRCMP_EQUAL( "null", v["bad_key"].as_string().c_str() );
}

TEST(JsonGroup, ParseLexemeTest)
{
	auto v = Json::parse( "true", e );
	CHECK( e.empty() );
	CHECK( v.as_bool() );

	v = Json::parse( "false", e );
	CHECK( e.empty() );
	CHECK_FALSE( v.as_bool() );

	v = Json::parse( "null", e );
	CHECK( e.empty() );
	CHECK( v.empty() );
	CHECK( v.is_none() );
}

TEST(JsonGroup, ParseEmptyTest)
{
	auto v = Json::parse( "", e );
	CHECK( e.empty() );
	CHECK( v.empty() );
	CHECK( v.is_none() );
}

TEST(JsonGroup, ParseNumberTest)
{
	auto v = Json::parse( "0", e );
	CHECK( e.empty() );
	CHECK_EQUAL( 0, v.as_int32() );

	v = Json::parse( "0.5", e );
	CHECK( e.empty() );
	DOUBLES_EQUAL( 0.5, v.as_float(), std::numeric_limits<float>::epsilon() );

	v = Json::parse( "-1.2", e );
	CHECK( e.empty() );
	DOUBLES_EQUAL( -1.2, v.as_double(), std::numeric_limits<float>::epsilon() ); // We have float epsilon in this context
}

TEST(JsonGroup, ParseStringTest)
{
    auto v = Json::parse( "\"str\"", e );
    CHECK( e.empty() );
    STRCMP_EQUAL( "str", v.as_string().c_str() );
}

TEST(JsonGroup, ParseMiscTest)
{
	auto v = Json::parse( "{\"a\": [], \"b\":{},\"c\":null}", e );
	CHECK( e.empty() );

	v = Json::parse( "{}", e );
	CHECK( e.empty() );

	v = Json::parse( "[]", e );
	CHECK( e.empty() );

	v = Json::parse( "{\"\":1}", e );
	CHECK_FALSE( e.empty() );
	CHECK_EQUAL( Error::BadKey, e.code() );
}

TEST(JsonGroup, InvalidInputTest)
{
	std::string scenarios[] = { "123 {}", "123{}", "123 true", "true {}", "tru", "{", "}", "[", "]", "{\"\":1}" };
	for( const auto &i : scenarios )
	{
		CHECK_FALSE_TEXT( Json::validate( i, e ), i.c_str() );
		CHECK_TEXT( !e.empty(), i.c_str() );
		e.clear();
	}
}

TEST(JsonGroup, ToStringTest)
{
	Value v( Value::Type::Object );
	v.insert( "string", "test" )
	 .insert( "number", 123 )
	 .insert( "object", Value( Value::Type::Object ) )
	 .insert( "array", Value( Value::Type::Array ) )
	 .insert( "float", 1.5 )
	 .insert( "double", std::numeric_limits<double>::max() );

	v["object"].insert( "key", "value" )
			   .insert( "bool", true )
			   .insert( "undef", Value( Value::Type::None ) );
	v["array"].insert( 123 )
			  .insert( false )
			  .insert( "test" );

	auto s = Json::build( v, e );
	CHECK( e.empty() );
	STRCMP_CONTAINS( "{\"array\":[", s.c_str() );
	STRCMP_CONTAINS( "]", s.c_str() );
	STRCMP_CONTAINS( "\"float\":1.5", s.c_str() );
	STRCMP_CONTAINS( "\"double\":1.79769e+308", s.c_str() );
	STRCMP_CONTAINS( "\"number\":123", s.c_str() );
	STRCMP_CONTAINS( "\"object\":{", s.c_str() );
	STRCMP_CONTAINS( "\"bool\":true", s.c_str() );
	STRCMP_CONTAINS( "\"key\":\"value\"", s.c_str() );
	STRCMP_CONTAINS( "\"undef\":null", s.c_str() );
	STRCMP_CONTAINS( "\"string\":\"test\"", s.c_str() );

	s = Json::build( v, e, Json::Format( ' ', 4 ) );
	CHECK( e.empty() );
	STRCMP_CONTAINS( "    \"array\": [", s.c_str() );
	STRCMP_CONTAINS( "        123", s.c_str() );
	STRCMP_CONTAINS( "        false", s.c_str() );
	STRCMP_CONTAINS( "        \"test\"", s.c_str() );
	STRCMP_CONTAINS( "    ],", s.c_str() );
	STRCMP_CONTAINS( "    \"float\": 1.5", s.c_str() );
	STRCMP_CONTAINS( "    \"double\": 1.79769e+308", s.c_str() );
	STRCMP_CONTAINS( "    \"number\": 123", s.c_str() );
	STRCMP_CONTAINS( "    \"object\": {", s.c_str() );
	STRCMP_CONTAINS( "        \"bool\": true", s.c_str() );
	STRCMP_CONTAINS( "        \"key\": \"value\"", s.c_str() );
	STRCMP_CONTAINS( "        \"undef\": null", s.c_str() );
	STRCMP_CONTAINS( "    },", s.c_str() );
	STRCMP_CONTAINS( "    \"string\": \"test\"", s.c_str() );
	STRCMP_CONTAINS( "}", s.c_str() );

	s = Json::minimize( s, e );
	CHECK( e.empty() );
	STRCMP_EQUAL( "{\"array\":[123,false,\"test\"],\"double\":1.79769e+308,\"float\":1.5,\"number\":123,\"object\":{\"bool\":true,\"key\":\"value\",\"undef\":null},\"string\":\"test\"}", s.c_str() );

	s = Json::format( "{\"key\":\"value\",\"list\": [123]}", e, Json::Format( ' ', 2 ) );
	STRCMP_EQUAL( "{\n  \"key\": \"value\",\n  \"list\": [\n    123\n  ]\n}", s.c_str() );
}

TEST(JsonGroup, Utf8Test)
{
	std::string utf8_str = u8"{\"цена\":\"10€\",\"количество\": 5}";

	CHECK( Json::validate( utf8_str, e ) );
	CHECK( e.empty() );

	auto v = Json::parse( utf8_str, e );
	CHECK( e.empty() );

	auto s = Json::build( v, e );
	CHECK( e.empty() );
	STRCMP_CONTAINS( "{", s.c_str() );
	STRCMP_CONTAINS( u8"\"цена\":\"10€\"", s.c_str() );
	STRCMP_CONTAINS( u8"\"количество\":5", s.c_str() );
	STRCMP_CONTAINS( "}", s.c_str() );

	auto s2 = Json::build( v, e, Json::Format( '\t', 1 ) );
	CHECK( e.empty() );
	STRCMP_CONTAINS( "{", s2.c_str() );
	STRCMP_CONTAINS( u8"\t\"цена\": \"10€\"", s2.c_str() );
	STRCMP_CONTAINS( u8"\t\"количество\": 5", s2.c_str() );
	STRCMP_CONTAINS( "}", s2.c_str() );

	auto s3 = Json::format( s2, e, Json::Format( ' ', 2 ) );
	CHECK( e.empty() );
	STRCMP_CONTAINS( "{", s3.c_str() );
	STRCMP_CONTAINS( u8"  \"цена\": \"10€\"", s3.c_str() );
	STRCMP_CONTAINS( u8"  \"количество\": 5", s3.c_str() );
	STRCMP_CONTAINS( "}", s3.c_str() );

	auto s4 = Json::minimize( s3, e );
	CHECK( e.empty() );
	STRCMP_CONTAINS( "{", s4.c_str() );
	STRCMP_CONTAINS( u8"\"цена\":\"10€\"", s4.c_str() );
	STRCMP_CONTAINS( u8"\"количество\":5", s4.c_str() );
	STRCMP_CONTAINS( "}", s4.c_str() );
}

TEST(JsonGroup, SpecialCharactersTest)
{
	Value v( " \" \\ \b \f \n \r \t " );
	auto dump = Json::build( v, e );
	CHECK( e.empty() );
	STRCMP_EQUAL( "\" \\\" \\\\ \\b \\f \\n \\r \\t \"", dump.c_str() );

	v = Value( Value::Type::Object );
	v.insert( "string", " \" \\ \b \f \n \r \t " );

	dump = Json::build( v, e );
	STRCMP_EQUAL( "{\"string\":\" \\\" \\\\ \\b \\f \\n \\r \\t \"}", dump.c_str() );

	auto res = Json::parse( dump, e );
	CHECK( e.empty() );
	STRCMP_EQUAL( " \" \\ \b \f \n \r \t ", res["string"].as_string().c_str() );
}
