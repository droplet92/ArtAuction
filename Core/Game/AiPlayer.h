#pragma once
#include <vector>

#include <Game/Game.h>
#include <Game/Random.h>
#include <Model/Bid.h>
#include <Model/Painting.h>
#include <Model/Player.h>
#include <Model/RoundType.h>


namespace lhs::game
{
	/**
	* A computer player
	*
	* It submits a painting and bids on a fixed schedule for each round type.
	* The schedules and the bids are those of the auction screen, for the players 1 to 3.
	*/
	class AiPlayer
	{
	public:
		explicit AiPlayer(model::PlayerId id);

		model::PlayerId GetId() const noexcept;

		// Return the painting to submit from the player's gallery.
		model::Painting ChooseSubmission(const model::Player& self) const;

		// Restart the schedule for a new auction of the given round type.
		void BeginBidding(model::RoundType type);

		// Let dt seconds of bidding pass, and bid on the game when the schedule says so.
		void Update(float dt, Game& game, Random& random);

	private:
		void TryBid(Game& game, Random& random) const;

	private:
		model::PlayerId id;

		model::RoundType roundType;

		std::vector<float> schedule;	// the seconds into the bidding at which to try to bid

		size_t next;

		float elapsed;
	};
}
