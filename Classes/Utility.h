#pragma once

#include <string>


namespace lhs
{
	using u8stringstream = std::basic_stringstream<char8_t>;


	class Utility
	{
	public:
		static std::string ConvertToAscii(const std::u8string& u8str);

		static std::u8string ConvertToUtf8(const std::string& str);
	};
}
