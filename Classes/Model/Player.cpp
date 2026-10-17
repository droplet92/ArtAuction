#include "Player.h"

#include <algorithm>
#include <cassert>
#include <ranges>


namespace lhs::model
{
	Player::Player(uint32_t id, std::string_view name)
		: id(id)
		, name(name.data())
		, gold(100)
	{
	}

	uint32_t Player::GetId() const noexcept
	{
		return id;
	}

	std::string Player::GetName() const noexcept
	{
		return name;
	}

	uint32_t Player::GetGold() const noexcept
	{
		return gold;
	}

	std::vector<Painting> Player::GetPaintings() const noexcept
	{
		return paintings;
	}

	Painting Player::GetPainting(size_t idx)
	{
		auto painting = paintings.at(idx);

		paintings.erase(paintings.begin() + idx);
		return painting;
	}

	std::map<std::string, uint32_t> Player::GetInformations(const std::vector<std::string>& painters) const
	{
		// painter-(player's number of paintings)
		std::map<std::string, uint32_t> information;

		// initialize the information keys(painters)
		auto view = paintings
			| std::views::transform([](auto model)
				{
					return model.painter;
				});
		for (const auto& painter : painters)
			information.emplace(painter, 0);

		// count the paintings
		std::ranges::for_each(view, [&](auto author)
			{
				++information[author];
			});
		return information;
	}

	void Player::SetPaintings(const std::vector<Painting>& paintings) noexcept
	{
		this->paintings = paintings;
	}

	void Player::AddGold(uint32_t amount) noexcept
	{
		gold += amount;
	}

	void Player::SubtractGold(uint32_t amount) noexcept
	{
		assert(amount <= gold && "a player cannot pay more gold than they have.");
		gold -= amount;
	}

	void Player::AddPainting(const Painting& painting)
	{
		paintings.push_back(painting);
	}

	void Player::RemovePainting(const Painting& target)
	{
		std::erase_if(paintings, [=](auto source)
			{
				return (source.title == target.title);
			});
	}

	void Player::SellAll(const std::map<std::string, uint32_t>& reputation)
	{
		for (auto& painting : paintings)
			gold += reputation.at(painting.painter);

		paintings.clear();
	}
}
