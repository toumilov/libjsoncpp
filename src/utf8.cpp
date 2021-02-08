
#include "utf8.hpp"

namespace jsoncpp
{

std::string Utf8::to_ascii( const std::u32string &utf8, const char placeholder )
{
	size_t i = 0;
	std::string res( utf8.size(), '\0' );

	for( const auto &c : utf8 )
	{
		if ( c > 0x7f )
		{
			res[i] = placeholder;
		}
		else
		{
			res[i] = (char)c;
		}
		i++;
	}

	return res;
}

std::string Utf8::encode( const std::u32string &utf8, Error &e )
{
	std::string res;
	res.reserve( utf8.size() );

	for( const char32_t &c : utf8 )
	{
		if ( c <= 0x0000007f )
		{
			res.push_back( (unsigned char)( c & 0x0000007f ) );
		}
		else if ( c <= 0x000007ff )
		{
			res.push_back( 0xc0 | (unsigned char)( ( c >> 6 ) & 0x0000001f ) );
			res.push_back( 0x80 | (unsigned char)( c & 0x0000003f ) );
		}
		else if ( c <= 0x0000ffff )
		{
			res.push_back( 0xe0 | (unsigned char)( ( c >> 12 ) & 0x0000000f ) );
			res.push_back( 0x80 | (unsigned char)( ( c >> 6 ) & 0x0000003f ) );
			res.push_back( 0x80 | (unsigned char)( c & 0x0000003f ) );
		}
		else if ( c <= 0x0010ffff )
		{
			res.push_back( 0xf0 | (unsigned char)( ( c >> 18 ) & 0x00000007 ) );
			res.push_back( 0x80 | (unsigned char)( ( c >> 12 ) & 0x0000003f ) );
			res.push_back( 0x80 | (unsigned char)( ( c >> 6 ) & 0x0000003f ) );
			res.push_back( 0x80 | (unsigned char)( c & 0x0000003f ) );
		}
		else
		{
			e = Error::UnexpectedCharacter;
		}
	}

	return res;
}

std::u32string Utf8::decode( const std::string &utf8, Error &e )
{
	char32_t c;
	unsigned i = 0;
	std::u32string res;

	for( const char &byte : utf8 )
	{
		if ( i == 0 )
		{
			c = 0;

			if ( ( byte & 0x80 ) == 0 ) // 0xxx xxxx
			{
				c = byte;
			}
			else if ( ( byte & 0xe0 ) == 0xc0 ) // 110x xxxx
			{
				i = 1;
				c = byte & 0x1f;
			}
			else if ( ( byte & 0xf0 ) == 0xe0 ) // 1110 xxxx
			{
				i = 2;
				c = byte & 0x0f;
			}
			else if ( ( byte & 0xf1 ) == 0xf0 ) // 1111 0xxx
			{
				i = 3;
				c = byte & 0x07;
			}
			else
			{
				e = Error::UnexpectedCharacter;
			}
		}
		else
		{
			c = c << 6;
			c |= ( byte & 0x3f );
			i--;
		}

		if ( i == 0 )
		{
			res += c;
		}
	}
	if ( i )
	{
		e = Error::UnexpectedEnding;
	}

	return res;
}

} // namespace jsoncpp
