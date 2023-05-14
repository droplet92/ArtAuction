#pragma once

#include <string>
#include <filesystem>

namespace lhs::Model
{
	class Painting
	{
	public:
		std::filesystem::path path;
		std::string title;
		std::string painter;
	};
}