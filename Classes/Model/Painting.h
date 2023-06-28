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

		Painting& operator=(const Painting& copy)
		{
			path = copy.path;
			title = copy.title;
			painter = copy.painter;
			return *this;
		}

		bool operator==(const Painting& rhs)
		{
			if (path != rhs.path) return false;
			if (title != rhs.title) return false;
			return (painter == rhs.painter);
		}
	};
}