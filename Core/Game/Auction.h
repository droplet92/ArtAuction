#pragma once
#include <cstdint>
#include <vector>

#include <Game/Random.h>
#include <Model/Bid.h>
#include <Model/RoundType.h>


namespace lhs::game
{
	/**
	* The auction of a single painting
	*
	* It records the bids and decides the winner by the rules of the round type.
	*/
	class Auction
	{
	public:
		Auction(model::RoundType type, uint32_t nPlayers);

		// The lowest amount a new bid may offer.
		model::Gold GetMinimumBid() const noexcept;

		// Record a bid. A bid that repeats the previous one exactly is ignored and returns false.
		bool Place(const model::Bid& bid);

		// A fixed-price auction ends at its first bid.
		bool IsClosedEarly() const noexcept;

		// Return the bids from the latest to the earliest.
		std::vector<model::Bid> GetBids() const;

		model::Bid GetLastBid(Random& random) const;

		// Return the winning bid and clear the bids.
		model::Bid GetWinningBid(Random& random);

	private:
		model::Bid GetClosedWinningBid(Random& random);

	private:
		model::RoundType type;

		uint32_t nPlayers;

		model::Gold minimumBid;

		std::vector<model::Bid> bids;
	};

	// Return the price an NFT sells for right after its auction.
	model::Gold RollNftPrice(Random& random);
}
