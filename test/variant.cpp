#include <cassert>
#include "variant.hpp"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>


using namespace jsoncpp;

TEST_GROUP(VariantGroup)
{
	void setup()
	{
	}
	void teardown()
	{
	}
};

TEST(VariantGroup, Constructors)
{
	{
		Variant<int, std::string> v;
		CHECK( v.empty() );
	}
	{
		Variant<int, std::string> v( 123 );
		CHECK_FALSE( v.empty() );
		CHECK( v.is<int>() );
		CHECK_EQUAL( 123, *v.get<int>() );
	}
	{
		Variant<int, std::string> v( std::string( "test" ) );
		CHECK_FALSE( v.empty() );
		CHECK( v.is<std::string>() );
		STRCMP_EQUAL( "test", v.get<std::string>()->c_str() );
	}
}

TEST(VariantGroup, GenericTypesTest)
{
	Variant<int, bool, char> v;

	// Uninitialized variable should be empty
	CHECK( v.empty() );
	CHECK_FALSE( v.is<int>() );
	CHECK( v.get<int>() == nullptr );
	CHECK_FALSE( v.is<bool>() );
	CHECK( v.get<bool>() == nullptr );
	CHECK_FALSE( v.is<char>() );
	CHECK( v.get<char>() == nullptr );

	// int
	v = 123;
	CHECK( v.is<int>() );
	CHECK_EQUAL( 123, *v.get<int>() );
	CHECK_FALSE( v.is<bool>() );
	CHECK( v.get<bool>() == nullptr );
	CHECK_FALSE( v.is<char>() );
	CHECK( v.get<char>() == nullptr );

	// bool
	v = true;
	CHECK_EQUAL( true, *v.get<bool>() );
	CHECK_FALSE( v.is<int>() );
	CHECK( v.get<int>() == nullptr );
	CHECK_FALSE( v.is<char>() );
	CHECK( v.get<char>() == nullptr );

	// char
	v = 'a';
	CHECK_EQUAL( 'a', *v.get<char>() );
	CHECK_FALSE( v.is<int>() );
	CHECK( v.get<int>() == nullptr );
	CHECK_FALSE( v.is<bool>() );
	CHECK( v.get<bool>() == nullptr );
}

TEST(VariantGroup, StructTest)
{
	struct S
	{
		unsigned &ctr;
		S( unsigned &ctr ) : ctr( ctr ) { this->ctr++; }
		S( const S &s ) : ctr( s.ctr ) { this->ctr++; }
		S( const S &&s ) : ctr( s.ctr ) {}
		~S() { ctr--; }
	};
	unsigned c = 0;
	{
		Variant<S> v;
		v = S( c );
		CHECK_EQUAL( 1, c );
	}
	CHECK_EQUAL( 0, c );
}

TEST(VariantGroup, MoveTest)
{
	Variant<int, bool> v1;
	Variant<int, bool> v2;
	Variant<int, bool> v3;

	v1 = true;
	v2 = std::move( v1 );
	CHECK_EQUAL( true, *v2.get<bool>() );

	v3 = 5;
	auto v4 = Variant<int, bool>( std::move( v3 ) );
	CHECK_EQUAL( 5, *v4.get<int>() );
}

TEST(VariantGroup, ConstVisitorTest)
{
	struct Visitor
	{
		unsigned i = 0;
		void operator()() { i = 1; }
		void operator()( const int& ) { i = 2; }
		void operator()( const bool& ) { i = 3; }
		void operator()( const std::string& ) { i = 4; }
	} visitor;
	Variant<int, bool, std::string> v;

	v.accept( visitor );
	CHECK_EQUAL( 1, visitor.i );

	v = 123;
	v.accept( visitor );
	CHECK_EQUAL( 2, visitor.i );

	v = true;
	v.accept( visitor );
	CHECK_EQUAL( 3, visitor.i );

	v = std::string( "test" );
	v.accept( visitor );
	CHECK_EQUAL( 4, visitor.i );
}
