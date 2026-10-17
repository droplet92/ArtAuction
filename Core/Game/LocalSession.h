#pragma once
#include <string>
#include <vector>

#include <Game/AiPlayer.h>
#include <Game/Game.h>
#include <Game/GameSession.h>
#include <Game/Random.h>


namespace lhs::game
{
	/**
	* A game against the computer on this device
	*
	* The first player is the local player, and the others are computer players.
	*/
	class LocalSession : public GameSession
	{
	public:
		LocalSession(Catalog catalog, std::vector<std::string> playerNames, Random random, Timings timings = {});

		model::PlayerId GetLocalPlayer() const noexcept final;

		void StartGame() final;

		bool Submit(const model::Painting& painting) final;

		std::optional<BidError> PlaceBid(model::Gold amount) final;

		void Update(float dt) final;

		const GameView& View() const noexcept final;

		[[nodiscard]] std::vector<GameEvent> TakeEvents() final;

	private:
		// Move the game's new events to the session's and note the phases that began.
		void CollectEvents();

	private:
		Game game;

		std::vector<AiPlayer> computers;

		Random random;	// for the computer players' decisions

		std::vector<GameEvent> events;	// the events not taken yet

		// The computer players act after the game has entered the new phase.
		bool submissionBegan;
		bool biddingBegan;
	};
}
