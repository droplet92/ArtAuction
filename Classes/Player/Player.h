#pragma once

#include <string>
#include <vector>
#include <Model/Painting.h>

namespace lhs
{
	class Player
	{
	public:
		Player(std::string name);

		virtual ~Player() = default;

		void SetPaintings(const std::vector<Model::Painting*>& paintings);

		void SetId(size_t id);

		std::vector<Model::Painting*> GetPaintings() const;

		size_t GetId() const;

		Model::Painting* GetPainting(size_t idx) const;

	protected:
		size_t id;

		std::string name;

		size_t money;

		std::vector<Model::Painting*> paintings;
	};
}