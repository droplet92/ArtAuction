#pragma once
#include <optional>
#include <string>
#include <vector>

#include <Game/Auction.h>
#include <Game/GameEvent.h>
#include <Game/GameView.h>
#include <Game/Random.h>
#include <Game/Reputation.h>
#include <Model/Painting.h>
#include <Model/RoundType.h>


namespace lhs::game
{
	// The paintings a game deals from.
	struct Catalog
	{
		std::vector<model::Painting> paintings;
		std::vector<std::string> painters;
	};

	// How long each phase lasts, in seconds. The defaults are the lengths of the current scenes.
	struct Timings
	{
		float roundIntro = 6.f;			// three lines of the round explanation, 2 seconds each
		float submission = 31.f;		// the gallery's 30 second countdown, which fires a second after reaching 0
		float submissionReveal = 3.f;	// the submitted painting is shown until the next 3 second check
		float presenting = 6.f;			// three lines of the dealer, 2 seconds each
		float bidding = 32.f;			// the 30 second countdown and 2 more seconds
		float reveal = 2.f;				// the dealer announces the winner
		float closedReveal = 9.f;		// opening the closed bids one by one takes 7 more seconds
		float roundSettlement = 11.f;	// the reputation of 5 painters, 2 seconds each, and 1 more second
	};

	enum class BidError
	{
		NotBidding,		// The auction is not taking bids now.
		BelowMinimum,
		NotEnoughGold,
	};

	/**
	* A game from dealing the galleries to the ranking
	*
	* It takes commands from the players and the time passed through Update,
	* and reports its progress as events.
	*/
	class Game
	{
	public:
		Game(Catalog catalog, std::vector<std::string> playerNames, Random random, Timings timings = {});

		// Deal the galleries and begin the first round. A finished game starts over.
		void Start();

		// Put up a painting from the player's gallery for this round's auctions.
		// Return false if the player has already submitted or does not own the painting.
		bool Submit(model::PlayerId player, const model::Painting& painting);

		// Bid for the painting on the easel. A bid that repeats the previous one is ignored.
		std::optional<BidError> PlaceBid(const model::Bid& bid);

		// Let the time pass by dt seconds.
		void Update(float dt);

		const GameView& View() const noexcept;

		// Return the events since the last call, in the order they happened.
		[[nodiscard]] std::vector<GameEvent> TakeEvents();

	private:
		void Enter(Phase phase, float duration = 0.f);

		void OnTimeOut();

		void BeginSubmission();

		bool HasAllSubmitted() const;

		void SubmitForTheLate();

		// Put a random painting among the submissions on the easel.
		void OpenNextAuction();

		void CloseAuction();

		void SettleRound();

		// Sell every gallery at the painters' reputation and rank the players.
		void EndGame();

	private:
		static constexpr int PAINTINGS_PER_PLAYER = 10;

		Catalog catalog;

		std::vector<std::string> playerNames;

		Random random;

		Timings timings;

		GameView view;

		std::vector<model::RoundType> rounds;	// the round types in the order they are played

		std::vector<model::Painting> selections;	// the submitted paintings not auctioned yet in this round

		std::optional<Auction> auction;	// the auction of the painting on the easel

		Reputation reputation;

		std::vector<GameEvent> events;	// the events not taken yet
	};
}
