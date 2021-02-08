#pragma once

#include "error.hpp"
#include <string>

namespace jsoncpp
{

/**
 * @brief UTF8 text processing class.
 */
class Utf8
{
public:

    /**
     * @brief to_ascii Convert wide character string to ASCII string.
     * Wide characters are replaced with specified placeholder.
     * @param utf8 source string.
     * @param placeholder wide character replacement.
     * @return ASCII string.
     */
    static std::string to_ascii( const std::u32string &utf8, const char placeholder = '?' );

    /**
     * @brief encode Serialize UTF8 string to byte array.
     * @param utf8 source string.
     * @param e Result variable.
     * @return byte array.
     */
    static std::string encode( const std::u32string &utf8, Error &e );

    /**
     * @brief decode Deserialize UTF8 string from byte array.
     * @param utf8 byte array.
     * @param e Result variable.
     * @return UTF8 string.
     */
    static std::u32string decode( const std::string &utf8, Error &e );
};

} // namespace jsoncpp
