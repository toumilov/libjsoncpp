
#include "utf8.hpp"
#include "CppUTest/TestHarness.h"

using namespace jsoncpp;

TEST_GROUP(Utf8Group)
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

TEST(Utf8Group, ToAsciiTest)
{
	auto s = Utf8::to_ascii( U"test - текст 10 €", '*' );
	CHECK( e.empty() );
	STRCMP_EQUAL("test - ***** 10 *", s.c_str());

	s = Utf8::to_ascii( U"test - текст 10 €" );
	CHECK( e.empty() );
	STRCMP_EQUAL("test - ????? 10 ?", s.c_str());
}

TEST(Utf8Group, SerializationTest)
{
	std::string v = "\xd0\xa6\xd0\xb5\xd0\xbd\xd0\xb0\x3a\x20\x31\x30\xe2\x82\xac";
	auto buf = Utf8::encode( U"Цена: 10€", e );
	CHECK( e.empty() );
	CHECK( buf == v );

	auto s = Utf8::decode( v, e );
	CHECK( e.empty() );
	CHECK( s == U"Цена: 10€" );
}

TEST(Utf8Group, SerializationFailureTest)
{
	std::u32string s = U"?";
	s[0] = 0xFFFFFFFF;
	auto buf = Utf8::encode( s, e );
	CHECK_FALSE( e.empty() );
	CHECK( e.code() == Error::UnexpectedCharacter );

	e.clear();
	std::string v = "\xFF\xFF\xFF\xFF";
	s = Utf8::decode( v, e );
	CHECK_FALSE( e.empty() );
	CHECK( e.code() == Error::UnexpectedCharacter );

	e.clear();
	v = "\xE2\x82";
	s = Utf8::decode( v, e );
	CHECK_FALSE( e.empty() );
	CHECK( e.code() == Error::UnexpectedEnding );
}
