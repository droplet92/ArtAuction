#pragma once
#include <filesystem>
#include <string>


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