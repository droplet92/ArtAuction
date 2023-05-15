#include "Utility.h"

#include <locale>
#include <codecvt>


namespace lhs
{
    std::string Utility::ConvertToAscii(const std::u8string& u8str)
	{
        return { reinterpret_cast<const char*>(u8str.c_str()) };
	}

    std::u8string Utility::ConvertToUtf8(const std::string& str)
    {
        return { reinterpret_cast<const char8_t*>(str.c_str()) };
    }
}