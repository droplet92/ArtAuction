#pragma once
#include <filesystem>
#include <string>


namespace lhs::model
{
	class Painting
	{
	public:
		std::filesystem::path path;	// image file path
		std::string title;
		std::string painter;

		bool operator==(const Painting& rhs) const = default;
	};
}