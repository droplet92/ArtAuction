#pragma once
#include <cstdint>
#include <utility>
#include <vector>

#include <Game/Random.h>
#include <Model/RoundType.h>


namespace lhs::game
{
	/**
	* The auction of a single painting
	*
	* It records the bids and decides the winner by the rules of the round type.
	* A bid is a pair of (player id, amount).
	*/
	class Auction
	{
	public:
		Auction(model::RoundType type, uint32_t nPlayers);

		// The lowest amount a new bid may offer.
		uint32_t GetMinimumBid() const noexcept;

		// Record a bid. A bid that repeats the previous one exactly is ignored and returns false.
		bool Place(const std::pair<int, int>& bid);

		// Return the bids from the latest to the earliest.
		std::vector<std::pair<int, int>> GetBids() const;

		std::pair<int, int> GetLastBid(Random& random) const;

		// Return the winning bid and clear the bids.
		std::pair<int, int> GetWinningBid(Random& random);

	private:
		std::pair<int, int> GetClosedWinningBid(Random& random);

	private:
		model::RoundType type;

		uint32_t nPlayers;

		uint32_t minimumBid;

		std::vector<std::pair<int, int>> bids;
	};
}
