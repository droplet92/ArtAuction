#include "Auction.h"

#include <algorithm>
#include <cassert>
#include <ranges>

using namespace lhs::model;


namespace lhs::game
{
	Auction::Auction(RoundType type, uint32_t nPlayers)
		: type(type)
		, nPlayers(nPlayers)
		, minimumBid((type == RoundType::FixedPrice) ? 10 : 1)
	{
	}

	uint32_t Auction::GetMinimumBid() const noexcept
	{
		return minimumBid;
	}

	bool Auction::Place(const std::pair<int, int>& bid)
	{
		// inhibit consecutive duplicated bids
		if (!bids.empty() && bid == bids.back())
			return false;

		bids.push_back(bid);

		// Bids are visible in every round but the closed one, so the next bid has to outbid this one.
		if (type != RoundType::ClosedBidding)
			minimumBid = bid.second + 1;

		return true;
	}

	std::vector<std::pair<int, int>> Auction::GetBids() const
	{
		auto view = bids | std::views::reverse;
		return { view.begin(), view.end() };
	}

	std::pair<int, int> Auction::GetLastBid(Random& random) const
	{
		// If not all players have placed bids, a random player will be selected.
		assert(nPlayers > 1 && "nPlayers must be greater than 1.");
		return bids.empty()
			? std::make_pair(RandomInt(random, 0, static_cast<int>(nPlayers) - 1), 0)
			: bids.back();
	}

	std::pair<int, int> Auction::GetWinningBid(Random& random)
	{
		if (type == RoundType::ClosedBidding)
			return GetClosedWinningBid(random);

		auto winningBid = GetLastBid(random);
		bids.clear();
		return winningBid;
	}

	std::pair<int, int> Auction::GetClosedWinningBid(Random& random)
	{
		// If not all players have placed bids, a random player will be selected.
		assert(nPlayers > 1 && "nPlayers must be greater than 1.");
		auto winningBid = std::make_pair(RandomInt(random, 0, static_cast<int>(nPlayers) - 1), 0);

		while (!bids.empty())
		{
			auto bid = bids.back();

			std::erase_if(bids, [=](auto pair)
				{
					return (pair.first == bid.first);
				});

			if (bid.second > winningBid.second)
				winningBid = bid;
		}
		return winningBid;
	}
}
