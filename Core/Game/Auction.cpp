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

	Gold Auction::GetMinimumBid() const noexcept
	{
		return minimumBid;
	}

	bool Auction::Place(const Bid& bid)
	{
		// inhibit consecutive duplicated bids
		if (!bids.empty() && bid == bids.back())
			return false;

		bids.push_back(bid);

		// Bids are visible in every round but the closed one, so the next bid has to outbid this one.
		if (type != RoundType::ClosedBidding)
			minimumBid = bid.amount + 1;

		return true;
	}

	bool Auction::IsClosedEarly() const noexcept
	{
		return (type == RoundType::FixedPrice) && !bids.empty();
	}

	std::vector<Bid> Auction::GetBids() const
	{
		auto view = bids | std::views::reverse;
		return { view.begin(), view.end() };
	}

	Bid Auction::GetLastBid(Random& random) const
	{
		// If not all players have placed bids, a random player will be selected.
		// TODO: An unbid painting goes to a random player for nothing. Decide whether it should stay unsold.
		assert(nPlayers > 1 && "nPlayers must be greater than 1.");
		return bids.empty()
			? Bid{ static_cast<PlayerId>(RandomInt(random, 0, static_cast<int>(nPlayers) - 1)), 0 }
			: bids.back();
	}

	Bid Auction::GetWinningBid(Random& random)
	{
		if (type == RoundType::ClosedBidding)
			return GetClosedWinningBid(random);

		auto winningBid = GetLastBid(random);
		bids.clear();
		return winningBid;
	}

	Bid Auction::GetClosedWinningBid(Random& random)
	{
		// If not all players have placed bids, a random player will be selected.
		// TODO: Only each player's latest bid counts, so bidding again with less lowers the offer.
		// TODO: On a tie the player whose latest bid came later wins, while the round explanation
		// says the earliest one does.
		assert(nPlayers > 1 && "nPlayers must be greater than 1.");
		auto winningBid = Bid{ static_cast<PlayerId>(RandomInt(random, 0, static_cast<int>(nPlayers) - 1)), 0 };

		while (!bids.empty())
		{
			auto bid = bids.back();

			std::erase_if(bids, [=](auto other)
				{
					return (other.bidder == bid.bidder);
				});

			if (bid.amount > winningBid.amount)
				winningBid = bid;
		}
		return winningBid;
	}

	Gold RollNftPrice(Random& random)
	{
		return static_cast<Gold>(RandomInt(random, 0, 30));
	}
}
