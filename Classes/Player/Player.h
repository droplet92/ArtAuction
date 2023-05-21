#pragma once
#include <map>
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

		std::map<std::string, size_t> GetInformations() const;

		void AddPainting(Model::Painting* painting);

		void RemovePainting(Model::Painting const* target);

		inline std::string GetName() const { return name; }

		inline size_t GetGold() const { return gold; }

		inline void UpdateGold(size_t amount) { gold += amount; }

	protected:
		size_t id;

		std::string name;

		size_t gold;

		std::vector<Model::Painting*> paintings;
	};
}