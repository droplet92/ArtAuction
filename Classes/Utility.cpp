#include "Utility.h"

#include <Windows.h>


namespace lhs
{
    void Utility::Abort(std::string_view message) noexcept
    {
        (void)MessageBoxA(nullptr, message.data(), "Error!", MB_OK | MB_ICONERROR);
        std::abort();
    }

    std::string Utility::ConvertToAscii(std::u8string_view u8str) noexcept
	{
        return { reinterpret_cast<const char*>(u8str.data()) };
	}

    std::u8string Utility::ConvertToUtf8(std::string_view str) noexcept
    {
        return { reinterpret_cast<const char8_t*>(str.data()) };
    }
}