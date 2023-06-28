#pragma once
#include <map>

#include <Model/Painting.h>


namespace lhs::model
{
	class Player
	{
	public:
		Player(uint32_t id, std::string_view name);

		virtual ~Player() = default;

		uint32_t GetId() const noexcept;

		std::string GetName() const noexcept;

		uint32_t GetGold() const noexcept;

		std::vector<Painting> GetPaintings() const noexcept;

		// Return idx-th painting and erase it.
		[[nodiscard]] Painting GetPainting(size_t idx);

		// Return the player's painter-(number of paintings) table.
		std::map<std::string, uint32_t> GetInformations() const;

		void SetPaintings(const std::vector<Painting>& paintings) noexcept;

		void AddGold(uint32_t amount) noexcept;

		void AddPainting(const Painting& painting);

		void RemovePainting(const Painting& target);

		// Sell all paintings at reputation prices.
		void SellAll(const std::map<std::string, uint32_t>& reputation);

	protected:
		uint32_t id;
		std::string name;
		uint32_t gold;
		std::vector<Painting> paintings;
	};
}