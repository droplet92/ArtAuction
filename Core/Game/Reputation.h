#pragma once
#include <cstdint>
#include <map>
#include <string>
#include <string_view>
#include <utility>
#include <vector>


namespace lhs::game
{
	/**
	* The painters' reputation
	*
	* Each round ranks the painters by the sales of their paintings and adds points to the best sellers.
	* The points last for the whole game.
	*/
	class Reputation
	{
	public:
		explicit Reputation(const std::vector<std::string>& painters = {});

		// Add the price of a sold painting to its painter's sales in this round.
		void AddWinningBid(std::string_view painter, int winningBid);

		// Add points to the best sellers of this round, then start a new round.
		void Update();

		// Return the painter-(points) table.
		const std::map<std::string, uint32_t>& Get() const noexcept;

	private:
		std::map<std::string, uint32_t> reputation;

		// total price of the sold paintings by painter at a round
		// the list of the pair of (painter, price)
		std::vector<std::pair<std::string, int>> winningBids;
	};
}
