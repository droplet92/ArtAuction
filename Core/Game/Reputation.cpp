#include "Reputation.h"

#include <algorithm>
#include <array>
#include <functional>
#include <ranges>
#include <set>


namespace lhs::game
{
	Reputation::Reputation(const std::vector<std::string>& painters)
	{
		for (const auto& painter : painters)
		{
			reputation.emplace(painter, 0);
			winningBids.push_back({ painter, 0 });
		}
	}

	void Reputation::AddWinningBid(std::string_view painter, int winningBid)
	{
		// Find the painter of the sold painting.
		auto r = std::find_if(winningBids.begin(), winningBids.end(), [&](auto source)
			{
				return (source.first == painter.data());
			});
		if (r == winningBids.end())
			return;

		// Add the bidding price.
		r->second += winningBid;
	}

	void Reputation::Update()
	{
		// Sort the winning bids in descending order.
		auto goldView = winningBids
			| std::views::transform([](auto& bid) { return bid.second; });

		std::set<int, std::greater<>> s{ goldView.begin(), goldView.end() };

		// Painters with no sales gain no reputation.
		s.erase(0);

		// Update the painters' reputation.
		// Increase by 10, 7, and 3 in descending order of sales amount; lower ranks gain nothing.
		constexpr std::array bonuses = { 10, 7, 3 };

		for (size_t rank = 0; const auto& target : s | std::views::take(bonuses.size()))
		{
			auto view = winningBids
				| std::views::filter([&](const auto& src) { return (src.second == target); })
				| std::views::transform([](const auto& src) { return src.first; });

			for (const auto& painter : view)
				reputation.at(painter) += bonuses.at(rank);

			++rank;
		}

		// Reset winningBids
		for (auto& [_, value] : winningBids)
			value = 0;
	}

	const std::map<std::string, uint32_t>& Reputation::Get() const noexcept
	{
		return reputation;
	}
}
