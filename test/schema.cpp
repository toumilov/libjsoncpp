#include <cassert>
#include <memory>
#include "schema.hpp"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>


using namespace jsoncpp;

TEST_GROUP(SchemaGroup)
{
	Error e;
	Schema s;

	void setup()
	{
	}
	void teardown()
	{
		e.clear();
	}
};

TEST(SchemaGroup, BasicSchema)
{
	// Accept any valid JSON
	CHECK( s.init( "{}", e ) );
	CHECK( e.empty() );
	CHECK( s.validate( std::string( "{\"text\":\"test\"}" ), e ) );
	CHECK( e.empty() );

	// Accept any valid JSON
	CHECK( s.init( "true", e ) );
	CHECK( e.empty() );
	CHECK( s.validate( std::string( "{\"text\":\"test\"}" ), e ) );
	CHECK( e.empty() );

	// Reject any object
	CHECK( s.init( "false", e ) );
	CHECK( e.empty() );
	CHECK_FALSE( s.validate( std::string( "{\"text\":\"test\"}" ), e ) );
	CHECK_FALSE( e.empty() );
}

TEST(SchemaGroup, NullElement)
{
	std::string null_schema = R"_({
			"type": "null"
		})_";
	CHECK( s.init( null_schema, e ) );
	CHECK( e.empty() );

	CHECK( s.validate( std::string( "null" ), e ) );
	CHECK( e.empty() );

	CHECK_FALSE( s.validate( std::string( "{\"text\":\"test\"}" ), e ) );
	CHECK_FALSE( e.empty() );
}

TEST(SchemaGroup, BoolElement)
{
	std::string bool_schema = R"_({
			"type": "boolean"
		})_";
	CHECK( s.init( bool_schema, e ) );
	CHECK( e.empty() );

	CHECK( s.validate( std::string( "true" ), e ) );
	CHECK( e.empty() );

	CHECK( s.validate( std::string( "false" ), e ) );
	CHECK( e.empty() );

	CHECK_FALSE( s.validate( std::string( "\"true\"" ), e ) );
	CHECK_FALSE( e.empty() );

	e.clear();
	CHECK_FALSE( s.validate( std::string( "0" ), e ) );
	CHECK_FALSE( e.empty() );

	e.clear();
	CHECK_FALSE( s.validate( std::string( "{\"text\":\"test\"}" ), e ) );
	CHECK_FALSE( e.empty() );
}

TEST(SchemaGroup, StringElement)
{
	std::string str_schema = R"_({
			"type": "string"
		})_";
	CHECK( s.init( str_schema, e ) );
	CHECK( e.empty() );

	CHECK( s.validate( std::string( "\"str\"" ), e ) );
	CHECK( e.empty() );

	CHECK( s.validate( std::string( "\"\"" ), e ) );
	CHECK( e.empty() );

	CHECK( s.validate( std::string( "\"42\"" ), e ) );
	CHECK( e.empty() );

	CHECK_FALSE( s.validate( std::string( "42" ), e ) );
	CHECK_FALSE( e.empty() );
}
